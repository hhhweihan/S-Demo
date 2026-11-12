#pragma once

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <stdexcept>

// 可增可减的倒计时闩：count_down 到 0 时永久放行所有 waiter。线程池用它跟踪在途任务数，
// 因此提供了标准 latch 没有的 count_up（提交任务时先加计数，任务完成后再减）。
class CountDownLatch {
 public:
    explicit CountDownLatch(std::size_t count) : count_(count) {}

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
        // count_ 是无符号：减多了会回绕成巨大值而永远等不到 0，故显式挡下溢。
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
        // 返回谓词结果：true 表示归零，false 表示超时——调用方据此区分两种返回原因。
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
