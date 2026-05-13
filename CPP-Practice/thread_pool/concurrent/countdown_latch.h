#pragma once

#include <chrono>
#include <cstddef>
#include <condition_variable>
#include <mutex>
#include <stdexcept>

class CountDownLatch {
public:
    explicit CountDownLatch(std::size_t count)
        : count_(count) {}

    void count_up(std::size_t delta = 1) {
        if (delta == 0) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        count_ += delta;
    }

    void count_down(std::size_t delta = 1) {
        if (delta == 0) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        if (delta > count_) {
            throw std::runtime_error("CountDownLatch underflow");
        }

        count_ -= delta;
        if (count_ == 0) {
            cv_.notify_all();
        }
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return count_ == 0; });
    }

    template <typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        return cv_.wait_for(lock, timeout, [this] { return count_ == 0; });
    }

    std::size_t count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::size_t count_ = 0;
};