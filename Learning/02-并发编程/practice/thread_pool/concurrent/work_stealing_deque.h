#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

// 一条 cache line。不用 std::hardware_destructive_interference_size：GCC 会报 -Winterference-size，
// libc++ 历史上也不一定提供——固定 64 更可移植（见项目记忆 ci-portability-and-clang-format）。
// 定义在此头文件，thread_pool.h 包含后复用同一 inline 变量（C++17 内联变量单一定义）。
inline constexpr std::size_t kCacheLine = 64;

// Chase-Lev work-stealing deque（定长版）。work-stealing 线程池的每个 worker 私有一个：
//   - **owner** 在 bottom 端做 LIFO push/pop（常路无 CAS，只有摘最后一个元素与 thief 撞车时一次
//   CAS）。
//     LIFO 让刚提交的子任务优先执行，缓存局部性最好。
//   - **thief**（其它 worker）在 top 端做 FIFO steal（一次 CAS）。从最老的一端偷，尽量不碰 owner
//   的热点。
// owner 与 thief 各操作一端，常态互不争用同一个原子量，这是它比"单锁队列人人抢"吞吐高的根本原因。
//
// 内存序遵循 Lê et al. 2013《Correct and Efficient Work-Stealing for Weak Memory Models》：pop 用
// seq_cst fence 与 steal 的 top CAS 排出全序，保证"最后一个元素"绝不会被 owner 和某 thief
// 同时取走。
//
// **为什么槽位存 `atomic<T*>` 而非直接存 T**：steal 会在 CAS 认领 top *之前*就投机读取槽位；这与
// owner 的 push 写同一槽位在 C++ 内存模型下构成数据竞争（哪怕算法上"读了也可能作废"，TSan 会如实
// 报 UB）。修法是让槽位访问本身原子化。但 T 可能是 std::function 这类非平凡类型，塞不进 atomic<T>，
// 于是存**持有所有权的指针** `atomic<T*>`：投机读的是指针（原子、无竞态），真正解引用/删除只在赢得
// CAS 之后。代价是每个任务一次 new/delete——换来对任意 T 的无数据竞争正确性。
//
// 定长设计（不扩容）换掉了数组增长时的旧缓冲回收难题：环形缓冲一次分配、活到 deque 析构。push 满
// 返回 false，由调用方走全局 overflow 队列兜底。
template <typename T>
class WorkStealingDeque {
 public:
    // 容量向上取到 2 的幂，用掩码取模。至少 2 个槽。
    explicit WorkStealingDeque(std::size_t capacity = 1024) {
        std::size_t cap = round_up_pow2(capacity);
        if (cap < 2) {
            cap = 2;
        }
        capacity_ = cap;
        mask_ = cap - 1;
        buffer_ = std::vector<std::atomic<T*>>(cap);  // 各槽 value-init 为 nullptr
        top_.store(0, std::memory_order_relaxed);
        bottom_.store(0, std::memory_order_relaxed);
    }

    // 析构无并发：删掉仍在 [top, bottom) 区间未被取走的任务；区间外是已取走的悬垂指针，不能删。
    ~WorkStealingDeque() {
        const std::int64_t t = top_.load(std::memory_order_relaxed);
        const std::int64_t b = bottom_.load(std::memory_order_relaxed);
        for (std::int64_t i = t; i < b; ++i) {
            delete buffer_[static_cast<std::size_t>(i) & mask_].load(std::memory_order_relaxed);
        }
    }

    WorkStealingDeque(const WorkStealingDeque&) = delete;
    WorkStealingDeque& operator=(const WorkStealingDeque&) = delete;

    std::size_t capacity() const { return capacity_; }

    // 仅 owner 调用：压入 bottom 端。满则返回 false（调用方走全局 overflow）。
    bool push(const T& value) {
        const std::int64_t b = bottom_.load(std::memory_order_relaxed);
        const std::int64_t t = top_.load(std::memory_order_acquire);
        if (b - t >= static_cast<std::int64_t>(capacity_)) {
            return false;  // 满：让调用方兜底到全局队列
        }
        T* node = new T(value);
        buffer_[static_cast<std::size_t>(b) & mask_].store(node, std::memory_order_relaxed);
        // release：把"槽位指针已写好"发布给随后 acquire 读到新 bottom 的 thief。
        bottom_.store(b + 1, std::memory_order_release);
        return true;
    }

    // 仅 owner 调用：从 bottom 端弹出（LIFO）。空返回 false。
    bool pop(T& out) {
        const std::int64_t b = bottom_.load(std::memory_order_relaxed) - 1;
        bottom_.store(b, std::memory_order_relaxed);  // 先占位，挡住新的 steal
        // seq_cst 全栅：与 steal() 里 top 的 seq_cst 加载/ CAS 排成同一全序，
        // 这样"只剩一个元素时 owner 与 thief 谁能拿"有唯一裁决，不会两取或漏取。
        std::atomic_thread_fence(std::memory_order_seq_cst);
        std::int64_t t = top_.load(std::memory_order_relaxed);

        if (t > b) {
            // 空：把 bottom 复原。
            bottom_.store(b + 1, std::memory_order_relaxed);
            return false;
        }

        T* node = buffer_[static_cast<std::size_t>(b) & mask_].load(std::memory_order_relaxed);
        if (t == b) {
            // 只剩最后一个：与可能同时 steal 的 thief 抢这一个，靠 top 的 CAS 定胜负。
            if (!top_.compare_exchange_strong(t, t + 1, std::memory_order_seq_cst,
                                              std::memory_order_relaxed)) {
                // 被 thief 抢走了：本次 pop 失败（不删 node，所有权已归 thief）。
                bottom_.store(b + 1, std::memory_order_relaxed);
                return false;
            }
            bottom_.store(b + 1, std::memory_order_relaxed);
        }
        // 至此本线程独占该 node：t<b 时 owner 独占 bottom 端无需 CAS；t==b 时已赢下 CAS。
        out = std::move(*node);
        delete node;
        return true;
    }

    // 任意 thief 调用：从 top 端偷（FIFO）。空或抢输返回 false。
    bool steal(T& out) {
        std::int64_t t = top_.load(std::memory_order_acquire);
        // seq_cst 全栅：与 pop() 的全栅排出全序，确保对 bottom 的读发生在 owner 占位之后可见。
        std::atomic_thread_fence(std::memory_order_seq_cst);
        const std::int64_t b = bottom_.load(std::memory_order_acquire);

        if (t >= b) {
            return false;  // 空
        }

        // 投机读的是**指针**（原子加载，无数据竞争）；CAS 赢下前不解引用、不删除。
        T* node = buffer_[static_cast<std::size_t>(t) & mask_].load(std::memory_order_relaxed);
        if (!top_.compare_exchange_strong(t, t + 1, std::memory_order_seq_cst,
                                          std::memory_order_relaxed)) {
            return false;  // 被 owner 或别的 thief 抢先，丢弃（不删，所有权归赢家）
        }
        out = std::move(*node);
        delete node;
        return true;
    }

    // 近似判空（供 owner 自查）。并发下仅供参考。
    bool empty() const {
        return bottom_.load(std::memory_order_relaxed) <= top_.load(std::memory_order_relaxed);
    }

 private:
    static std::size_t round_up_pow2(std::size_t value) {
        std::size_t result = 1;
        while (result < value) {
            result <<= 1;
        }
        return result;
    }

    std::size_t capacity_ = 0;
    std::size_t mask_ = 0;
    std::vector<std::atomic<T*>> buffer_;
    // owner 写 bottom、thief 写 top：各据一条 cache line，杜绝两端热点的 false sharing。
    alignas(kCacheLine) std::atomic<std::int64_t> top_{0};
    alignas(kCacheLine) std::atomic<std::int64_t> bottom_{0};
};
