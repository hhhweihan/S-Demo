#pragma once

#include <thread>
#include <utility>

// RAII 线程包装（相当于 std::jthread 的简化版）：析构自动 join，避免可 join 的线程被销毁
// 时触发 std::terminate。只可移动不可拷贝——线程所有权是独占的。
class JoiningThread {
 public:
    JoiningThread() noexcept = default;

    template <typename F, typename... Args>
    explicit JoiningThread(F&& func, Args&&... args)
        : thread_(std::forward<F>(func), std::forward<Args>(args)...) {}

    JoiningThread(JoiningThread&& other) noexcept : thread_(std::move(other.thread_)) {}

    JoiningThread& operator=(JoiningThread&& other) noexcept {
        if (this != &other) {
            // 覆盖前必须先 join 现有线程，否则丢弃一个可 join 的 thread 会 terminate。
            join_if_needed();
            thread_ = std::move(other.thread_);
        }
        return *this;
    }

    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;

    ~JoiningThread() { join_if_needed(); }

    bool joinable() const noexcept { return thread_.joinable(); }

    void join() { thread_.join(); }

    std::thread::id get_id() const noexcept { return thread_.get_id(); }

 private:
    void join_if_needed() noexcept {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    std::thread thread_;
};
