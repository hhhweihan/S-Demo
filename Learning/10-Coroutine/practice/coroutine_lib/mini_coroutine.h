#pragma once

#include <chrono>
#include <coroutine>
#include <cstddef>
#include <deque>
#include <functional>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace mini_coro {

// FIFO 协程调度器：只持有句柄不拥有协程，恢复前查 done() 以防重复恢复已完成协程（UB）。
class Scheduler {
 public:
    void schedule(std::coroutine_handle<> handle) { ready_.push(handle); }
    void run() {
        while (!ready_.empty()) {
            auto handle = ready_.front();
            ready_.pop();
            if (!handle.done()) handle.resume();  // resume 可能再次把自己 schedule 进来
        }
    }
    std::size_t pending() const { return ready_.size(); }

 private:
    std::queue<std::coroutine_handle<>> ready_;
};

// awaiter 无法拿到调度器参数，故用 thread_local 全局约定“当前线程的调度器”。
inline Scheduler*& current_scheduler() {
    static thread_local Scheduler* scheduler = nullptr;
    return scheduler;
}

// RAII 设置/还原 current_scheduler，支持嵌套：析构时恢复进入前的值而非置空。
class SchedulerScope {
 public:
    explicit SchedulerScope(Scheduler& scheduler) : previous_(current_scheduler()) {
        current_scheduler() = &scheduler;
    }
    ~SchedulerScope() { current_scheduler() = previous_; }

 private:
    Scheduler* previous_;
};

// yield：await_ready 恒 false 保证必挂起，await_suspend 把自己重新入队，实现协作式让出。
struct YieldAwaiter {
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) const {
        current_scheduler()->schedule(handle);
    }
    void await_resume() const noexcept {}
};

inline YieldAwaiter yield() {
    return {};
}

// 教学简化：用阻塞 sleep 冒充异步定时。真实实现应把句柄挂到定时器而非占住线程。
class SleepAwaiter {
 public:
    explicit SleepAwaiter(std::chrono::milliseconds duration) : duration_(duration) {}
    bool await_ready() const noexcept {
        return duration_.count() == 0;
    }  // 零时长无需挂起，省一次调度
    void await_suspend(std::coroutine_handle<> handle) const {
        std::this_thread::sleep_for(duration_);
        current_scheduler()->schedule(handle);  // “到期”后重新入队
    }
    void await_resume() const noexcept {}

 private:
    std::chrono::milliseconds duration_;
};

inline SleepAwaiter async_sleep(std::chrono::milliseconds duration) {
    return SleepAwaiter(duration);
}

// 拥有型协程句柄包装：move-only，析构负责 destroy 句柄，避免协程帧泄漏。
class Task {
 public:
    struct promise_type {
        Task get_return_object() {
            return Task(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        std::suspend_always initial_suspend() noexcept {
            return {};
        }  // 创建即挂起，交由 start() 显式投递
        std::suspend_always final_suspend() noexcept {
            return {};
        }  // 结束后不自动销毁，留给 Task 析构 destroy
        void return_void() noexcept {}
        void unhandled_exception() { throw; }  // 传播异常，不吞掉
    };

    explicit Task(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();  // 先释放自身持有的帧再接管，防泄漏
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }
    Task(const Task&) = delete;  // 句柄所有权唯一，禁止拷贝
    Task& operator=(const Task&) = delete;
    ~Task() {
        if (handle_) handle_.destroy();
    }
    void start(Scheduler& scheduler) {
        scheduler.schedule(handle_);
    }  // 因 initial_suspend 挂起，需显式投递才运行
    bool done() const { return !handle_ || handle_.done(); }

 private:
    std::coroutine_handle<promise_type> handle_;
};

template <typename T>
class Channel {
 public:
    explicit Channel(std::size_t capacity = 0) : capacity_(capacity) {}
    // 满则返回 false（非阻塞语义），由调用方决定重试或让出；不会阻塞协程。
    bool send(T value) {
        if (capacity_ == 0 && !queue_.empty()) return false;  // 容量 0：至多缓存一个待收值
        if (capacity_ > 0 && queue_.size() >= capacity_) return false;
        queue_.push_back(std::move(value));
        return true;
    }
    std::optional<T> recv() {
        if (queue_.empty()) return std::nullopt;  // 空返回 nullopt，交由 select_recv 试下一个通道
        T value = std::move(queue_.front());
        queue_.pop_front();
        return value;
    }
    bool empty() const { return queue_.empty(); }
    std::size_t size() const { return queue_.size(); }
    std::size_t capacity() const { return capacity_; }

 private:
    std::size_t capacity_;  // 0 表示只能暂存一个待收值
    std::deque<T> queue_;
};

// 非阻塞 select：按给定顺序返回首个有数据的通道值，暗含优先级。
template <typename T>
std::optional<T> select_recv(std::vector<Channel<T>*>& channels) {
    for (auto* channel : channels) {
        if (auto value = channel->recv()) return value;
    }
    return std::nullopt;
}

class AsyncHttpClientCore {
 public:
    // 协程体现挂起-恢复：async_sleep 处让出，恢复后再把响应写回通道。
    Task get(std::string url, Channel<std::string>& out) {
        co_await async_sleep(std::chrono::milliseconds(1));
        out.send("GET " + url + " -> 200 OK");
    }
};

}  // namespace mini_coro
