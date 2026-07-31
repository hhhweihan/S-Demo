#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <utility>

// 无界 FIFO 阻塞队列。契约：shutdown() 后 push 抛异常；pop 在关闭且队列排空后抛异常，
// 否则一直阻塞，因此消费者只有拿到元素或队列已死两种结局。
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
        // 在锁外通知：避免被唤醒的消费者立刻又撞上仍被持有的锁而再次休眠。
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 谓词同时覆盖虚假唤醒和关闭：关闭时也要醒来去走下面的抛异常路径。
        cv_.wait(lock, [this] { return shutdown_ || !queue_.empty(); });

        // 仅当关闭且已排空才会到这里——正常有数据时 front() 一定有效。
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
        // notify_all 而非 notify_one：所有阻塞的消费者都必须醒来观察关闭状态。
        cv_.notify_all();
    }

 private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;
    bool shutdown_ = false;
};
