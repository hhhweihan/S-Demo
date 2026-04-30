#pragma once

#include <thread>
#include <utility>

class JoiningThread {
public:
    JoiningThread() noexcept = default;

    template <typename F, typename... Args>
    explicit JoiningThread(F&& func, Args&&... args)
        : thread_(std::forward<F>(func), std::forward<Args>(args)...) {}

    JoiningThread(JoiningThread&& other) noexcept
        : thread_(std::move(other.thread_)) {}

    JoiningThread& operator=(JoiningThread&& other) noexcept {
        if (this != &other) {
            join_if_needed();
            thread_ = std::move(other.thread_);
        }
        return *this;
    }

    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;

    ~JoiningThread() {
        join_if_needed();
    }

    bool joinable() const noexcept {
        return thread_.joinable();
    }

    void join() {
        thread_.join();
    }

    std::thread::id get_id() const noexcept {
        return thread_.get_id();
    }

private:
    void join_if_needed() noexcept {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    std::thread thread_;
};