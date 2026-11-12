#pragma once

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "hazard_pointer.h"

// 最简"内存安全"的无锁栈：push/pop 用 CAS 维护栈顶。pop 出来的节点不立即 delete，而是塞进
// retired_ 延迟到析构统一释放——这样任何还持有旧指针的并发读者都不可能解引用到已释放内存。
//
// 关键限制（教学取舍）：retired_ 只增不减，pop/retire_node 一路 push_back，唯一的释放点是析构里
// 的 clear_retired_nodes()。因此运行期间 retired_ 无界增长，长时间大量 pop 会持续吃内存。这是
// 刻意的简化：换来"绝不悬垂"的安全性，代价是不回收。生产级实现会用 hazard pointer 或 epoch-based
// reclamation 做"扫描并释放"，只回收确认无人引用的节点，使 retired_ 保持有界。本文件下方的
// ReclaimingLockFreeStack 用原子 to_be_deleted_ 链 + try_reclaim 部分解决了这个问题。
template <typename T>
class LockFreeStack {
 public:
    LockFreeStack() = default;

    // 析构不与并发访问同步：调用方须保证此时没有其他线程在用栈。
    ~LockFreeStack() {
        clear_live_nodes();
        clear_retired_nodes();
    }

    LockFreeStack(const LockFreeStack&) = delete;
    LockFreeStack& operator=(const LockFreeStack&) = delete;

    void push(T value) {
        Node* new_node = new Node(std::move(value));
        new_node->next = head_.load(std::memory_order_relaxed);
        // 成功用 release 发布：保证新节点的构造对随后 acquire 读到它的 pop 可见。
        while (!head_.compare_exchange_weak(new_node->next, new_node, std::memory_order_release,
                                            std::memory_order_relaxed)) {
            // CAS 失败会把 new_node->next 刷新为最新栈顶，直接重试即可。
        }
    }

    bool pop(T& out) {
        Node* old_head = head_.load(std::memory_order_acquire);
        while (old_head != nullptr &&
               !head_.compare_exchange_weak(old_head, old_head->next, std::memory_order_acq_rel,
                                            std::memory_order_acquire)) {
            // 失败时 old_head 被刷新为新栈顶，继续尝试。
        }

        if (old_head == nullptr) {
            return false;
        }

        out = *old_head->data;
        // 不能在此 delete old_head：别的线程可能仍持有该指针在做 CAS 比较（ABA/悬垂风险）。
        retire_node(old_head);
        return true;
    }

    bool empty() const { return head_.load(std::memory_order_acquire) == nullptr; }

 private:
    struct Node {
        explicit Node(T value) : data(std::make_shared<T>(std::move(value))) {}

        std::shared_ptr<T> data;
        Node* next = nullptr;
    };

    void retire_node(Node* node) {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        retired_.push_back(node);  // 见类注释：只增不减，直到析构
    }

    void clear_live_nodes() {
        Node* current = head_.exchange(nullptr, std::memory_order_acq_rel);
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void clear_retired_nodes() {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        for (Node* node : retired_) {
            delete node;
        }
        retired_.clear();
    }

    std::atomic<Node*> head_{nullptr};
    std::mutex retired_mutex_;
    std::vector<Node*> retired_;  // 延迟释放列表：无界增长，仅析构时清空
};

// 带安全回收的无锁栈：用 threads_in_pop_ 计数 + to_be_deleted_ 原子链实现"没人在 pop 时才回收"。
// 这是对上面 LockFreeStack 无界 retired_ 的部分改进——节点在运行期就能被释放，而非拖到析构。
// 局限：高并发下若 pop 区域始终有人，to_be_deleted_ 仍可能积压（退化回近似无界），真正有界需
// hazard pointer / epoch 方案。
template <typename T>
class ReclaimingLockFreeStack {
 public:
    ReclaimingLockFreeStack() = default;

    ~ReclaimingLockFreeStack() {
        clear_live_nodes();
        delete_nodes(to_be_deleted_.exchange(nullptr, std::memory_order_acq_rel));
    }

    ReclaimingLockFreeStack(const ReclaimingLockFreeStack&) = delete;
    ReclaimingLockFreeStack& operator=(const ReclaimingLockFreeStack&) = delete;

    void push(T value) {
        Node* new_node = new Node(std::move(value));
        // next 是原子字段：并发 pop 里 CAS 重试会读它、try_reclaim 挂待删链会写它，二者若非原子
        // 就是同址读写的数据竞争（标准 UB，TSan 会报）。这里用局部 current 承接 CAS 的 expected，
        // 再把 next 原子发布，避免把 atomic 成员直接当 CAS 引用参数。
        Node* current = head_.load(std::memory_order_relaxed);
        new_node->next.store(current, std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(current, new_node, std::memory_order_release,
                                            std::memory_order_relaxed)) {
            new_node->next.store(current, std::memory_order_relaxed);
        }
    }

    std::shared_ptr<T> pop() {
        // 进入 pop 前先登记：try_reclaim 靠这个计数判断"是否只有我一个人在 pop"。
        threads_in_pop_.fetch_add(1, std::memory_order_acq_rel);

        Node* old_head = head_.load(std::memory_order_acquire);
        while (old_head != nullptr &&
               !head_.compare_exchange_weak(old_head, old_head->next.load(std::memory_order_acquire),
                                            std::memory_order_acq_rel, std::memory_order_acquire)) {
        }

        std::shared_ptr<T> result;
        if (old_head != nullptr) {
            // swap 而非拷贝：把数据从节点转移出来，节点本身留待 try_reclaim 决定何时删。
            result.swap(old_head->data);
        }

        try_reclaim(old_head);
        return result;
    }

    bool empty() const { return head_.load(std::memory_order_acquire) == nullptr; }

 private:
    struct Node {
        explicit Node(T value) : data(std::make_shared<T>(std::move(value))) {}

        std::shared_ptr<T> data;
        std::atomic<Node*> next{nullptr};  // 原子：并发 pop 的 CAS 重试与挂待删链会同址读写它
    };

    static void delete_nodes(Node* nodes) {
        while (nodes != nullptr) {
            Node* next = nodes->next.load(std::memory_order_relaxed);
            delete nodes;
            nodes = next;
        }
    }

    void chain_pending_nodes(Node* first, Node* last) {
        Node* old = to_be_deleted_.load(std::memory_order_relaxed);
        last->next.store(old, std::memory_order_relaxed);
        // 把 [first..last] 整段无锁地接到 to_be_deleted_ 头部；release 发布链表内容。
        while (!to_be_deleted_.compare_exchange_weak(old, first, std::memory_order_release,
                                                     std::memory_order_relaxed)) {
            last->next.store(old, std::memory_order_relaxed);
        }
    }

    void chain_pending_nodes(Node* nodes) {
        Node* last = nodes;
        while (last != nullptr && last->next.load(std::memory_order_relaxed) != nullptr) {
            last = last->next.load(std::memory_order_relaxed);
        }
        if (nodes != nullptr && last != nullptr) {
            chain_pending_nodes(nodes, last);
        }
    }

    void chain_pending_node(Node* node) { chain_pending_nodes(node, node); }

    void try_reclaim(Node* old_head) {
        if (old_head == nullptr) {
            threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel);
            return;
        }

        // 只有当前是唯一在 pop 的线程时，回收待删链才安全——否则别人可能正持有其中的指针。
        if (threads_in_pop_.load(std::memory_order_acquire) == 1) {
            Node* nodes_to_delete = to_be_deleted_.exchange(nullptr, std::memory_order_acq_rel);
            // 取走待删链后再次确认仍只有自己：fetch_sub 返回 1 说明期间无人进入，可放心删。
            if (threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete_nodes(nodes_to_delete);
            } else if (nodes_to_delete != nullptr) {
                // 有并发 pop 插进来，别人可能引用这些节点，重新挂回待删链推迟回收。
                chain_pending_nodes(nodes_to_delete);
            }
            delete old_head;  // 当前节点无人再引用（此刻只有自己在 pop），可直接删
        } else {
            // 有其他线程在 pop，本节点先挂起，等到某次"独占 pop"时再统一回收。
            chain_pending_node(old_head);
            threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel);
        }
    }

    void clear_live_nodes() {
        Node* current = head_.exchange(nullptr, std::memory_order_acq_rel);
        delete_nodes(current);
    }

    std::atomic<Node*> head_{nullptr};
    std::atomic<unsigned> threads_in_pop_{0};
    std::atomic<Node*> to_be_deleted_{nullptr};
};

// 用 (指针, tag) 打包成可原子操作的 Head 来防 ABA：每次改栈顶都递增 tag，即便指针兜圈回到旧值，
// tag 已变，CAS 仍会失败，从而防止 ABA 误判。
//
// 关键限制（同 LockFreeStack）：retired_ 只增不减，唯一释放点是析构里的 clear_retired_nodes()，
// 运行期间无界增长。tag 解决的是 ABA 正确性，并不解决内存回收——回收仍走"死后统一释放"的简化路径。
// 生产级实现需搭配 hazard pointer / epoch-based reclamation 做有界扫描回收。
template <typename T>
class TaggedPointerStack {
 public:
    TaggedPointerStack() = default;

    ~TaggedPointerStack() {
        clear_live_nodes();
        clear_retired_nodes();
    }

    TaggedPointerStack(const TaggedPointerStack&) = delete;
    TaggedPointerStack& operator=(const TaggedPointerStack&) = delete;

    void push(T value) {
        Node* new_node = new Node(std::move(value));
        Head old_head = head_.load(std::memory_order_relaxed);
        Head new_head{};

        do {
            new_node->next = old_head.ptr;
            new_head.ptr = new_node;
            new_head.tag = old_head.tag + 1;  // 每次发布都递增 tag，构成 ABA 防护
        } while (!head_.compare_exchange_weak(old_head, new_head, std::memory_order_release,
                                              std::memory_order_relaxed));
    }

    bool pop(T& out) {
        Head old_head = head_.load(std::memory_order_acquire);
        Head new_head{};

        while (old_head.ptr != nullptr) {
            new_head.ptr = old_head.ptr->next;
            new_head.tag = old_head.tag + 1;  // 递增 tag：即使 ptr 与某旧值相同也不会误配
            if (head_.compare_exchange_weak(old_head, new_head, std::memory_order_acq_rel,
                                            std::memory_order_acquire)) {
                out = *old_head.ptr->data;
                retire_node(old_head.ptr);  // 延迟释放，见类注释的无界增长限制
                return true;
            }
        }

        return false;
    }

    bool empty() const { return head_.load(std::memory_order_acquire).ptr == nullptr; }

 private:
    struct Node {
        explicit Node(T value) : data(std::make_shared<T>(std::move(value))) {}

        std::shared_ptr<T> data;
        Node* next = nullptr;
    };

    struct Head {
        Node* ptr = nullptr;
        std::size_t tag = 0;  // ABA 版本号
    };

    // Head 必须可平凡拷贝，std::atomic<Head> 才能对其做无锁 CAS（否则退化为带锁）。
    static_assert(std::is_trivially_copyable<Head>::value, "Head must be trivially copyable");

    // “无锁”声明的编译期强制：Head = {Node*, size_t} 共 16 字节，其 CAS 依赖硬件双字 CAS
    // （x86-64 上是 CMPXCHG16B，CMakeLists 已加 -mcx16 + libatomic）。若无此支持，std::atomic<Head>
    // 会静默退化为 libatomic 的互斥锁实现，“无锁栈”名不副实——此处编译失败以杜绝静默降级。
    //
    // 为何不能只写 static_assert(is_always_lock_free)：GCC 对 16B 原子的 is_always_lock_free /
    // is_lock_free() **恒为 false**（本环境已验证），这是它对“无锁 load 需 CMPXCHG16B 写回、在只读
    // 页上不安全”的保守处理，并非真的退化为带锁——开了 -mcx16 后 CAS 实际走 libatomic 的 cx16
    // ifunc（cmpxchg16b，无互斥）。GCC 用 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 精确表达“目标具备硬件
    // 双字 CAS”，该宏仅在 -mcx16 时定义，正是我们要的“未退化为锁”的可靠信号。故：is_always_lock_free
    // 为真（如 clang/MSVC）直接通过；GCC 则认这个硬件宏。两者皆无 ⇒ 确为带锁降级 ⇒ 编译失败。
    static constexpr bool kHeadIsLockFree =
        std::atomic<Head>::is_always_lock_free
#if defined(__GNUC__) && !defined(__clang__) && defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16)
        || true  // GCC + 硬件 cmpxchg16b：真无锁，只是 is_always_lock_free 误报为 false
#endif
        ;
    static_assert(kHeadIsLockFree,
                  "TaggedPointerStack requires a lock-free double-word CAS for std::atomic<Head>; "
                  "enable -mcx16 (x86-64), otherwise it degrades to a libatomic lock and the "
                  "\"lock-free\" claim is false");

    void retire_node(Node* node) {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        retired_.push_back(node);  // 只增不减，直到析构
    }

    void clear_live_nodes() {
        Head current = head_.exchange(Head{}, std::memory_order_acq_rel);
        Node* node = current.ptr;
        while (node != nullptr) {
            Node* next = node->next;
            delete node;
            node = next;
        }
    }

    void clear_retired_nodes() {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        for (Node* node : retired_) {
            delete node;
        }
        retired_.clear();
    }

    std::atomic<Head> head_{};
    std::mutex retired_mutex_;
    std::vector<Node*> retired_;  // 延迟释放列表：无界增长，仅析构时清空
};

// 用 hazard pointer 做**有界回收**的无锁栈——这才是上面三个教学栈"死后统一释放"简化的生产级答案。
// pop 前先把 old_head 登记到 hazard 槽（发布+校验），解引用完再 retire；回收方 delete 前扫描槽位，
// 只回收"当前无人登记"的节点。于是 retired 列表长度被 HazardPointerDomain 的扫描阈值卡住，运行期
// 内存有界——对照 LockFreeStack 的 retired_ 无界增长。正确性关键：hazard pointer 的 load-verify
// 协议要求"reader 发布 hp"与"reclaimer 摘除该节点"这两个操作落在同一个 seq_cst 全序里，否则弱序机
// 上二者可能互相看不见对方（ARM 上用 acq_rel 摘链在 TSan 下会暴露 use-after-free）。因此这里对
// head_ 的两处 CAS 都用 seq_cst，与 protect() 内部的 seq_cst 发布/校验配对，经典实现（如
// 《C++ Concurrency in Action》的 hazard pointer 栈）同样对这组操作全用默认的 seq_cst。
template <typename T>
class HazardStack {
 public:
    HazardStack() = default;

    // 析构前无并发访问：先摘净存活链，再让 domain 清空全局 retired（此刻无人登记，全部可删）。
    ~HazardStack() {
        Node* current = head_.exchange(nullptr, std::memory_order_seq_cst);
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        lockfree::HazardPointerDomain::instance().reclaim_all();
    }

    HazardStack(const HazardStack&) = delete;
    HazardStack& operator=(const HazardStack&) = delete;

    void push(T value) {
        Node* new_node = new Node(std::move(value));
        new_node->next = head_.load(std::memory_order_seq_cst);
        while (!head_.compare_exchange_weak(new_node->next, new_node, std::memory_order_seq_cst,
                                            std::memory_order_seq_cst)) {
        }
    }

    bool pop(T& out) {
        auto& domain = lockfree::HazardPointerDomain::instance();
        Node* old_head;
        for (;;) {
            // protect 内部 load-verify：返回时 old_head 已被本线程 hazard 槽护住，不会被并发回收。
            old_head = domain.protect(head_);
            if (old_head == nullptr) {
                domain.clear();
                return false;
            }
            Node* next = old_head->next;  // 安全解引用：old_head 受保护
            // seq_cst：与 protect() 的发布/校验落在同一全序，否则本次摘除可能被弱序机重排到
            // reader 发布 hp 之前，让 scan() 误判"无人保护"而回收仍被引用的节点。
            if (head_.compare_exchange_weak(old_head, next, std::memory_order_seq_cst,
                                            std::memory_order_seq_cst)) {
                break;  // 摘下成功
            }
            // CAS 失败说明栈顶已变，循环回去对新栈顶重新 protect。
        }
        out = old_head->value;  // 仍在保护期内，读取安全
        domain.clear();         // 先撤保护，再 retire——否则会永远护着自己刚退役的节点
        domain.retire(old_head);
        return true;
    }

    bool empty() const { return head_.load(std::memory_order_acquire) == nullptr; }

 private:
    struct Node {
        explicit Node(T value) : value(std::move(value)), next(nullptr) {}
        T value;
        Node* next;
    };

    std::atomic<Node*> head_{nullptr};
};
