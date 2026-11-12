#pragma once

#include <atomic>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

// 单生产者单消费者无锁环形队列。正确性前提：只有一个线程调用 push、一个线程调用 pop。
// 生产者独占 tail_、消费者独占 head_，跨线程只通过这两个索引的 acquire/release 同步，
// 因此无需 CAS 或锁。多生产者或多消费者会破坏该假设，行为未定义。
template <typename T>
class SPSCQueue {
 public:
    explicit SPSCQueue(std::size_t capacity)
        // 多留一个空槽：满(tail 追到 head 前一格)与空(tail==head)才能区分，否则两者都是
        // head==tail。
        : capacity_(capacity + 1), buffer_(capacity_) {}

    bool push(T value) {
        const std::size_t tail = tail_.load(std::memory_order_relaxed);  // 本线程独占，relaxed 足够
        const std::size_t next = increment(tail);
        // acquire 读 head_：与消费者对 head_ 的 release 配对，确保看到它已腾空的槽位。
        if (next == head_.load(std::memory_order_acquire)) {
            return false;  // 满
        }

        buffer_[tail] = std::move(value);
        // release 发布 tail_：保证上面的槽位写入对消费者的 acquire 可见（先写数据后发布索引）。
        tail_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& out) {
        const std::size_t head = head_.load(std::memory_order_relaxed);  // 本线程独占
        // acquire 读 tail_：与生产者 release 配对，读到 tail_ 的新值即意味着对应槽位数据已就绪。
        if (head == tail_.load(std::memory_order_acquire)) {
            return false;  // 空
        }

        out = std::move(*buffer_[head]);
        buffer_[head].reset();
        // release 发布 head_：让生产者的 acquire 看到该槽位已释放，可安全复用。
        head_.store(increment(head), std::memory_order_release);
        return true;
    }

    bool empty() const {
        return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_acquire);
    }

    std::size_t capacity() const {
        return capacity_ - 1;  // 扣掉那个哨兵空槽
    }

 private:
    std::size_t increment(std::size_t index) const { return (index + 1) % capacity_; }

    const std::size_t capacity_;
    std::vector<std::optional<T>> buffer_;
    // head_ 与 tail_ 各占一条 cache line：否则两个高频写的索引落在同一行会 false sharing，
    // 生产者和消费者的写互相使对方 cache 失效，吞吐骤降。
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
};
