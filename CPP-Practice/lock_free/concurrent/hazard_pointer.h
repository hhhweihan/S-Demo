#pragma once

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <stdexcept>
#include <vector>

// Hazard pointers: safe memory reclamation (SMR) for lock-free structures.
//
// 无锁栈/队列的核心难题不是并发正确，而是"何时能安全 delete 一个刚摘下的节点"——别的线程可能
// 正持有该指针准备解引用。lock_free_stack.h 里三个教学栈的做法是"死后统一释放"（retired_ 只增
// 不减，直到析构），运行期无界吃内存。hazard pointer 是生产级答案：读者在解引用前把指针**公开
// 登记**到一个全局可见的槽位（hazard slot）；回收方 delete 前先扫描所有槽位，只回收"当前无人登记"
// 的节点，其余留到下次扫描。于是 retired 列表长度被扫描阈值 R 卡住，实现**有界回收**。
//
// 本实现：每线程一个 hazard 槽（够护住 Treiber 栈 pop 里的 old_head），全局定长槽数组 + 全局
// retire 链；retire 计数越过阈值即触发 scan。单头文件、无外部依赖、C++17。
namespace lockfree {

// 一条 cache line。不用 std::hardware_destructive_interference_size：GCC 会报 -Winterference-size，
// 且 libc++ 历史上不一定提供——用固定 64 更可移植（见项目记忆 ci-portability-and-clang-format）。
inline constexpr std::size_t kCacheLine = 64;

class HazardPointerDomain {
 public:
    // 槽位上限即"可能并发访问该结构的线程数"上限。128 对本仓库的压测足够；超出会在 acquire 抛出。
    static constexpr std::size_t kMaxThreads = 128;
    // retire 链越过该长度就扫描回收。阈值取 2*kMaxThreads：保证每次扫描能摊还掉至少一半的
    // retired 节点（最坏每个存活线程各占一个 hazard），均摊 O(1) 且 retired 长度恒有界。
    static constexpr std::size_t kScanThreshold = 2 * kMaxThreads;

    static HazardPointerDomain& instance() {
        static HazardPointerDomain domain;
        return domain;
    }

    // 发布保护：把 src 的当前值登记到本线程槽位，再重读校验 src 未变（load-verify 协议）。
    // 返回被稳定保护的指针——此后解引用它是安全的，直到 clear() 或再次 protect。
    template <typename T>
    T* protect(const std::atomic<T*>& src) {
        Slot* slot = acquire_slot();
        T* value = src.load(std::memory_order_seq_cst);
        while (true) {
            // 登记与校验必须同为 seq_cst，才能与 scan() 的 seq_cst 快照构成 Dekker 式全序。
            // 关键陷阱：若校验只用 acquire，ARM 等弱序机可把「重读 head」StoreLoad 重排到「发布
            // hp」之前——于是回收方 scan 尚未看见我的登记就判定无人引用、delete 掉节点，地址随即被
            // push 的 new Node 复用，造成 use-after-free（TSan 会在 old_head->next 处命中）。
            slot->pointer.store(static_cast<void*>(value), std::memory_order_seq_cst);
            T* again = src.load(std::memory_order_seq_cst);
            if (again == value) {
                return value;  // 登记后 src 未变，value 已被本槽位护住
            }
            value = again;  // src 变了（有人先摘走），重登记新值再校验
        }
    }

    // 撤销保护：本线程不再引用任何节点，清空槽位，让它们可被回收。
    void clear() {
        if (Slot* slot = tls_slot()) {
            slot->pointer.store(nullptr, std::memory_order_release);
        }
    }

    // 退役一个不再挂在结构上的节点：不立即 delete，挂进全局 retire 链；越过阈值触发扫描回收。
    template <typename T>
    void retire(T* pointer) {
        auto* record = new Retired{static_cast<void*>(pointer),
                                   [](void* raw) { delete static_cast<T*>(raw); }, nullptr};
        record->next = retired_head_.load(std::memory_order_relaxed);
        // seq_cst 入链：与 scan() 的 seq_cst 取链落在同一全序，是「摘除 <ₛ 取链」推理链的一环。
        while (!retired_head_.compare_exchange_weak(record->next, record, std::memory_order_seq_cst,
                                                    std::memory_order_relaxed)) {
        }
        if (retired_count_.fetch_add(1, std::memory_order_acq_rel) + 1 >= kScanThreshold) {
            scan();
        }
    }

    // 当前尚未回收的 retired 节点数（近似）。测试用它断言"回收有界"。
    std::size_t retired_count() const { return retired_count_.load(std::memory_order_acquire); }

    // 供测试在单线程收尾时强制清空 retired 链（此刻无并发读者，全部可删）。
    void reclaim_all() {
        Retired* list = retired_head_.exchange(nullptr, std::memory_order_acq_rel);
        std::size_t freed = 0;
        while (list != nullptr) {
            Retired* next = list->next;
            list->deleter(list->payload);
            delete list;
            ++freed;
            list = next;
        }
        retired_count_.fetch_sub(freed, std::memory_order_acq_rel);
    }

 private:
    struct Slot {
        alignas(kCacheLine) std::atomic<bool> active{false};
        std::atomic<void*> pointer{nullptr};
    };

    struct Retired {
        void* payload;
        void (*deleter)(void*);
        Retired* next;
    };

    HazardPointerDomain() = default;

    ~HazardPointerDomain() { reclaim_all(); }

    HazardPointerDomain(const HazardPointerDomain&) = delete;
    HazardPointerDomain& operator=(const HazardPointerDomain&) = delete;

    // 本线程的槽位指针，首次 protect 时懒惰获取，线程退出时由 Releaser 归还。
    Slot*& tls_slot() {
        thread_local Slot* slot = nullptr;
        return slot;
    }

    Slot* acquire_slot() {
        Slot*& cached = tls_slot();
        if (cached != nullptr) {
            return cached;
        }
        for (std::size_t i = 0; i < kMaxThreads; ++i) {
            bool expected = false;
            if (slots_[i].active.compare_exchange_strong(expected, true, std::memory_order_acq_rel,
                                                         std::memory_order_relaxed)) {
                cached = &slots_[i];
                // 线程退出时自动归还槽位：thread_local 对象析构即触发。
                static thread_local Releaser releaser;
                releaser.slot = &slots_[i];
                return cached;
            }
        }
        throw std::runtime_error("hazard pointer slots exhausted (raise kMaxThreads)");
    }

    // 归还槽位：清指针、置 active=false，使其可被其他线程复用。
    void release_slot(Slot* slot) {
        if (slot == nullptr) {
            return;
        }
        slot->pointer.store(nullptr, std::memory_order_release);
        slot->active.store(false, std::memory_order_release);
    }

    struct Releaser {
        Slot* slot = nullptr;
        ~Releaser() { HazardPointerDomain::instance().release_slot(slot); }
    };

    // 扫描回收：**必须先取走 retire 链，再快照 hazard 槽**（顺序不可颠倒）。
    // seq_cst 全序论证：reader 在 protect 里「发布 hp → 校验 head==N（读到 N 可达 ⇒ 此读 <ₛ 摘除
    // N）」；摘除 <ₛ retire 入链 <ₛ 本函数的取链（读到了 N）<ₛ 下面的槽位快照。于是
    // 「hp 发布 <ₛ 摘除 <ₛ 取链 <ₛ 快照」，快照必然看见 reader 的 hp，绝不会误删仍被引用的节点。
    // 若颠倒成「先快照再取链」，快照可能发生在 reader 发布 hp 之前而漏看，酿成 use-after-free。
    void scan() {
        Retired* list = retired_head_.exchange(nullptr, std::memory_order_seq_cst);

        std::vector<void*> hazards;
        hazards.reserve(kMaxThreads);
        for (std::size_t i = 0; i < kMaxThreads; ++i) {
            void* p = slots_[i].pointer.load(std::memory_order_seq_cst);
            if (p != nullptr) {
                hazards.push_back(p);
            }
        }

        std::size_t removed = 0;
        while (list != nullptr) {
            Retired* next = list->next;
            const bool hazardous =
                std::find(hazards.begin(), hazards.end(), list->payload) != hazards.end();
            if (hazardous) {
                // 仍有线程护着它：重新挂回 retire 链，下次再判。
                list->next = retired_head_.load(std::memory_order_relaxed);
                while (!retired_head_.compare_exchange_weak(
                    list->next, list, std::memory_order_seq_cst, std::memory_order_relaxed)) {
                }
            } else {
                list->deleter(list->payload);
                delete list;
                ++removed;
            }
            list = next;
        }
        retired_count_.fetch_sub(removed, std::memory_order_acq_rel);
    }

    Slot slots_[kMaxThreads];
    alignas(kCacheLine) std::atomic<Retired*> retired_head_{nullptr};
    alignas(kCacheLine) std::atomic<std::size_t> retired_count_{0};
};

}  // namespace lockfree
