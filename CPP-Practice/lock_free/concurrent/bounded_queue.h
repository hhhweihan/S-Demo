#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <utility>

// 有界阻塞队列：满时 push 阻塞、空时 pop 阻塞，用两个条件变量分别表达"非满"和"非空"，
// 避免生产者/消费者互相误唤醒。容量为 0 无意义，构造即拒绝。
template <typename T>
class BoundedQueue {
 public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {
        if (capacity_ == 0) {
            throw std::invalid_argument("capacity must be > 0");
        }
    }

    void push(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this] { return shutdown_ || queue_.size() < capacity_; });

        if (shutdown_) {
            throw std::runtime_error("push on shutdown BoundedQueue");
        }

        queue_.push(std::move(value));
        // 先解锁再通知：被唤醒的消费者不会立刻撞上仍被持有的锁。
        lock.unlock();
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return shutdown_ || !queue_.empty(); });

        if (queue_.empty()) {
            throw std::runtime_error("pop from shutdown BoundedQueue");
        }

        T value = std::move(queue_.front());
        queue_.pop();
        lock.unlock();
        not_full_.notify_one();  // 腾出一个空位，唤醒一个等待的生产者
        return value;
    }

    bool try_push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (shutdown_ || queue_.size() >= capacity_) {
            return false;
        }

        queue_.push(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }

        out = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return true;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_ = true;
        }
        // 两个 cv 都要 notify_all：生产者和消费者可能同时在各自的等待里。
        not_empty_.notify_all();
        not_full_.notify_all();
    }

 private:
    const std::size_t capacity_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    std::queue<T> queue_;
    bool shutdown_ = false;
};
