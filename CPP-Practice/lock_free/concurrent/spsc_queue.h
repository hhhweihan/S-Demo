#pragma once

#include <atomic>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

template <typename T>
class SPSCQueue {
public:
    explicit SPSCQueue(std::size_t capacity)
        : capacity_(capacity + 1),
          buffer_(capacity_) {}

    bool push(T value) {
        const std::size_t tail = tail_.load(std::memory_order_relaxed);
        const std::size_t next = increment(tail);
        if (next == head_.load(std::memory_order_acquire)) {
            return false;
        }

        buffer_[tail] = std::move(value);
        tail_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& out) {
        const std::size_t head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        out = std::move(*buffer_[head]);
        buffer_[head].reset();
        head_.store(increment(head), std::memory_order_release);
        return true;
    }

    bool empty() const {
        return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_acquire);
    }

    std::size_t capacity() const {
        return capacity_ - 1;
    }

private:
    std::size_t increment(std::size_t index) const {
        return (index + 1) % capacity_;
    }

    const std::size_t capacity_;
    std::vector<std::optional<T>> buffer_;
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
};
