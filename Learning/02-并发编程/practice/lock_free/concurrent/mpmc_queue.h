#pragma once

#include <atomic>
#include <cstddef>
#include <stdexcept>
#include <utility>

// 真无锁有界 MPMC 队列（Dmitry Vyukov 算法）。多生产者多消费者，push/pop 各只需一次 CAS，
// 无锁、无 ABA、无节点分配（元素原地存在环形缓冲里）。
//
// 与 bounded_queue.h 的对比：那个名在 lock_free/ 目录却是 mutex + 双条件变量的**带锁**队列，
// 已迁到 thread_pool/。本类才是本模块真正的无锁有界队列。
//
// 核心机制——每个槽位一个"序列号"（sequence）当作轻量的一次性令牌：
//   - 槽位 i 初始 seq=i。生产者要写槽位时期望 seq==pos（轮到它写），写完把 seq 设为 pos+1
//     （通知消费者"可读"）。消费者要读时期望 seq==pos+1，读完把 seq 设为 pos+capacity
//     （通知下一轮生产者"可写"）。
//   - seq 与期望值的差（dif）同时区分了三种情况：==0 该我操作、<0 队列满/空、>0 别人抢先了要重读。
// 这样生产者只在 enqueue_pos_ 上 CAS、消费者只在 dequeue_pos_ 上 CAS，两端互不争用同一个原子量。
template <typename T>
class MPMCQueue {
 public:
    // 容量向上取到 2 的幂（用掩码取模，比 % 快且无分支）。要求 T 可默认构造（槽位数组预建）。
    explicit MPMCQueue(std::size_t capacity) : buffer_(nullptr) {
        std::size_t cap = round_up_pow2(capacity);
        if (cap < 2) {
            cap = 2;  // 至少两个槽，否则 push/pop 无法交替推进
        }
        buffer_ = new Cell[cap];
        capacity_ = cap;
        mask_ = cap - 1;
        for (std::size_t i = 0; i < cap; ++i) {
            buffer_[i].sequence.store(i, std::memory_order_relaxed);
        }
        enqueue_pos_.store(0, std::memory_order_relaxed);
        dequeue_pos_.store(0, std::memory_order_relaxed);
    }

    ~MPMCQueue() { delete[] buffer_; }

    MPMCQueue(const MPMCQueue&) = delete;
    MPMCQueue& operator=(const MPMCQueue&) = delete;

    // 入队。队列满返回 false（不阻塞）。
    bool push(T value) {
        Cell* cell;
        std::size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
        for (;;) {
            cell = &buffer_[pos & mask_];
            const std::size_t seq = cell->sequence.load(std::memory_order_acquire);
            const std::intptr_t dif =
                static_cast<std::intptr_t>(seq) - static_cast<std::intptr_t>(pos);
            if (dif == 0) {
                // 轮到写这个槽：抢占 pos（把 enqueue_pos_ 推进一格）。成功则跳出去写数据。
                if (enqueue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    break;
                }
            } else if (dif < 0) {
                return false;  // 该槽还没被消费者腾空（seq 落后 pos 一圈）→ 队列满
            } else {
                pos = enqueue_pos_.load(std::memory_order_relaxed);  // 别的生产者抢先，重读 pos
            }
        }
        cell->data = std::move(value);
        // release 发布 seq=pos+1：保证上面的数据写对随后 acquire 读到该 seq 的消费者可见。
        cell->sequence.store(pos + 1, std::memory_order_release);
        return true;
    }

    // 出队。队列空返回 false（不阻塞）。
    bool pop(T& out) {
        Cell* cell;
        std::size_t pos = dequeue_pos_.load(std::memory_order_relaxed);
        for (;;) {
            cell = &buffer_[pos & mask_];
            const std::size_t seq = cell->sequence.load(std::memory_order_acquire);
            const std::intptr_t dif =
                static_cast<std::intptr_t>(seq) - static_cast<std::intptr_t>(pos + 1);
            if (dif == 0) {
                if (dequeue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    break;
                }
            } else if (dif < 0) {
                return false;  // 该槽尚无生产者写入（seq 还等于 pos）→ 队列空
            } else {
                pos = dequeue_pos_.load(std::memory_order_relaxed);
            }
        }
        out = std::move(cell->data);
        // 把槽位标记为"下一圈可写"：seq = pos + capacity，正好等于生产者下次访问该槽时的期望 pos。
        cell->sequence.store(pos + mask_ + 1, std::memory_order_release);
        return true;
    }

    std::size_t capacity() const { return capacity_; }

    // 近似大小（并发下仅供观测）：入队位与出队位之差。
    bool empty() const {
        return enqueue_pos_.load(std::memory_order_acquire) ==
               dequeue_pos_.load(std::memory_order_acquire);
    }

 private:
    // 本地定义 cache line 常量，理由同 hazard_pointer.h：避开
    // std::hardware_destructive_interference_size。
    static constexpr std::size_t kCacheLineMPMC = 64;

    // 每个 Cell 独占一条 cache line（对齐 + 填充），避免相邻槽位的 sequence/data 落在同一 line 上
    // 造成 false sharing——Vyukov 原版即如此隔离。理由同上，用本地常量避开
    // std::hardware_destructive_interference_size。
    struct alignas(kCacheLineMPMC) Cell {
        std::atomic<std::size_t> sequence;
        T data;
    };

    static std::size_t round_up_pow2(std::size_t v) {
        if (v == 0) {
            return 1;
        }
        --v;
        for (std::size_t shift = 1; shift < sizeof(std::size_t) * 8; shift <<= 1) {
            v |= v >> shift;
        }
        return v + 1;
    }

    Cell* buffer_;
    std::size_t capacity_ = 0;
    std::size_t mask_ = 0;
    // 入队位与出队位各占一条 cache line：两端在不同原子量上 CAS，隔离后互不 false sharing。
    alignas(kCacheLineMPMC) std::atomic<std::size_t> enqueue_pos_{0};
    alignas(kCacheLineMPMC) std::atomic<std::size_t> dequeue_pos_{0};
};
