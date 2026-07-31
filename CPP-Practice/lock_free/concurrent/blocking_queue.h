#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <utility>

// 无界 FIFO 阻塞队列（作为无锁队列的对照基线）。契约同 thread_pool 版：shutdown() 后 push
// 抛异常；pop 在关闭且排空后抛异常，否则阻塞到有数据。
template <typename T>
class BlockingQueue {
 public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (shutdown_) {
                throw std::runtime_error("push on shutdown BlockingQueue");
            }
            queue_.push(std::move(value));
        }
        // 锁外通知，减少被唤醒者立即再次阻塞的概率。
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 谓词兼顾虚假唤醒与关闭：关闭时也要醒来走抛异常路径。
        cv_.wait(lock, [this] { return shutdown_ || !queue_.empty(); });

        if (queue_.empty()) {
            throw std::runtime_error("pop from shutdown BlockingQueue");
        }

        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }

        out = std::move(queue_.front());
        queue_.pop();
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
        cv_.notify_all();  // 全部唤醒，令每个 waiter 观察到关闭
    }

 private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
    bool shutdown_ = false;
};
