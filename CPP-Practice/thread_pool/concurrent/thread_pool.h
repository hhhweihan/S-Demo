#pragma once

#include "concurrent/blocking_queue.h"
#include "concurrent/countdown_latch.h"
#include "concurrent/joining_thread.h"

#include <atomic>
#include <chrono>
#if defined(__cpp_concepts)
#include <concepts>
#endif
#include <condition_variable>
#include <cstddef>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

struct ThreadPoolOptions {
    std::size_t min_threads = 1;
    std::size_t max_threads = 1;
    std::size_t grow_threshold = 4;  // 队列长度超过此值才考虑扩容
    std::chrono::milliseconds monitor_interval{1000};
};

// 支持动态扩缩容的线程池。worker 数在 [min_threads, max_threads] 间浮动，由一条后台监控
// 线程按队列积压情况增减。运行标志用 atomic，其余提交/暂停状态受 state_mutex_ 保护。
class ThreadPool {
 public:
    using Task = std::function<void()>;

    struct Stats {
        std::size_t submitted = 0;
        std::size_t completed = 0;
        std::size_t rejected = 0;
        std::size_t pending = 0;
        std::size_t active_workers = 0;
        std::size_t target_workers = 0;
        std::size_t queue_size = 0;
        bool accepting = false;
        bool paused = false;
    };

    explicit ThreadPool(std::size_t worker_count)
        : ThreadPool(ThreadPoolOptions{worker_count, worker_count}) {}

    explicit ThreadPool(ThreadPoolOptions options) : options_(options) {
        if (options_.min_threads == 0) {
            throw std::invalid_argument("min_threads must be > 0");
        }
        if (options_.max_threads < options_.min_threads) {
            throw std::invalid_argument("max_threads must be >= min_threads");
        }

        target_worker_count_.store(options_.min_threads, std::memory_order_relaxed);
        for (std::size_t index = 0; index < options_.min_threads; ++index) {
            spawn_worker();
        }

        // 固定大小的池（max==min）无需监控线程，省掉一条常驻线程。
        if (options_.max_threads > options_.min_threads) {
            monitor_thread_ = std::make_unique<JoiningThread>([this] { monitor_loop(); });
        }
    }

    ~ThreadPool() { shutdown(); }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void submit(Task task) {
        // 空 std::function 会被 worker 当作退出哨兵，故禁止用户提交空任务以免误触发退出。
        if (!task) {
            throw std::invalid_argument("empty task is not allowed");
        }

        enqueue_task(std::move(task));
    }

#if defined(__cpp_concepts)
    template <typename F, typename... Args>
        requires std::invocable<F, Args...>
#else
    template <typename F, typename... Args>
#endif
    auto submit(F&& func, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using RetType = std::invoke_result_t<F, Args...>;

        // packaged_task 放进 shared_ptr：Task 是 std::function 要求可拷贝，而 packaged_task
        // 只可移动，用 shared_ptr 包一层才能塞进队列。
        auto packaged_task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));
        std::future<RetType> future = packaged_task->get_future();

        enqueue_task([packaged_task]() { (*packaged_task)(); });
        return future;
    }

    void await_termination() {
        // 先等在途任务全部完成，再关闭——保证已提交的任务不被丢弃。
        pending_latch_.wait();
        shutdown();
    }

    void pause() {
        std::lock_guard<std::mutex> lock(state_mutex_);
        if (accepting_) {
            paused_ = true;
        }
    }

    void resume() {
        {
            std::lock_guard<std::mutex> lock(state_mutex_);
            paused_ = false;
        }
        pause_cv_.notify_all();
    }

    void shutdown() {
        // CAS 保证关闭逻辑只执行一次（析构和 await_termination 都可能调用）。
        bool expected = true;
        if (!running_.compare_exchange_strong(expected, false, std::memory_order_acq_rel)) {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(state_mutex_);
            accepting_ = false;
            paused_ = false;
        }
        // 唤醒卡在 pause 等待里的提交者，让它们观察到 running_=false 后抛出而非死等。
        pause_cv_.notify_all();

        if (monitor_thread_ != nullptr) {
            monitor_thread_->join();
            monitor_thread_.reset();
        }

        std::size_t worker_slots = 0;
        {
            std::lock_guard<std::mutex> lock(workers_mutex_);
            worker_slots = workers_.size();
        }

        // 给每个 worker 投一个退出哨兵，再关闭队列唤醒阻塞在 pop 上的 worker。
        request_worker_exit(worker_slots);
        queue_.shutdown();

        std::lock_guard<std::mutex> lock(workers_mutex_);
        workers_.clear();  // JoiningThread 析构会 join 每个 worker
    }

    Stats snapshot_stats() const {
        std::lock_guard<std::mutex> lock(state_mutex_);
        return Stats{submitted_tasks_.load(std::memory_order_relaxed),
                     completed_tasks_.load(std::memory_order_relaxed),
                     rejected_tasks_.load(std::memory_order_relaxed),
                     pending_tasks_.load(std::memory_order_relaxed),
                     active_count_.load(std::memory_order_relaxed),
                     target_worker_count_.load(std::memory_order_relaxed),
                     queue_.size(),
                     accepting_,
                     paused_};
    }

 private:
    void enqueue_task(Task task) {
        {
            std::unique_lock<std::mutex> lock(state_mutex_);
            // 暂停时阻塞在这里；关闭或停止接受时也醒来，走下面的拒绝路径而非永久挂起。
            pause_cv_.wait(lock, [this] {
                return !paused_ || !accepting_ || !running_.load(std::memory_order_acquire);
            });

            if (!accepting_ || !running_.load(std::memory_order_acquire)) {
                rejected_tasks_.fetch_add(1, std::memory_order_relaxed);
                throw std::runtime_error("submit on stopped ThreadPool");
            }

            // 计数必须在锁内、入队前就加：否则任务可能先被 worker 执行完再来减，导致下溢。
            submitted_tasks_.fetch_add(1, std::memory_order_relaxed);
            pending_tasks_.fetch_add(1, std::memory_order_relaxed);
            pending_latch_.count_up();
        }

        try {
            queue_.push(std::move(task));
        } catch (...) {
            // 入队失败要回滚上面预加的计数，否则 pending 永远不归零、await 会死等。
            pending_tasks_.fetch_sub(1, std::memory_order_relaxed);
            pending_latch_.count_down();
            rejected_tasks_.fetch_add(1, std::memory_order_relaxed);
            throw;
        }
    }

    void spawn_worker() {
        std::lock_guard<std::mutex> lock(workers_mutex_);
        workers_.emplace_back([this] { worker_loop(); });
    }

    void request_worker_exit(std::size_t count) {
        if (count == 0) {
            return;
        }

        // 令牌数与哨兵一一对应：worker 拿到空任务后需消费一个令牌才真正退出，
        // 这样缩容只退指定数量，避免哨兵被"路过"的 worker 误吞导致退多退少。
        worker_exit_tokens_.fetch_add(count, std::memory_order_release);
        for (std::size_t index = 0; index < count; ++index) {
            queue_.push(Task{});  // 空任务即退出哨兵
        }
    }

    bool try_consume_exit_token() {
        std::size_t current = worker_exit_tokens_.load(std::memory_order_acquire);
        while (current > 0) {
            // CAS 循环保证同一个令牌只被一个 worker 领走。
            if (worker_exit_tokens_.compare_exchange_weak(
                    current, current - 1, std::memory_order_acq_rel, std::memory_order_acquire)) {
                return true;
            }
        }
        return false;
    }

    void worker_loop() {
        active_count_.fetch_add(1, std::memory_order_relaxed);

        while (true) {
            Task task;
            try {
                task = queue_.pop();
            } catch (const std::exception&) {
                break;  // 队列已 shutdown，无更多任务
            }

            if (!task) {
                // 收到哨兵：整池关闭则直接退；否则只在抢到退出令牌时退（缩容），
                // 抢不到说明这个哨兵不是给自己的，继续干活。
                if (!running_.load(std::memory_order_acquire) || try_consume_exit_token()) {
                    break;
                }
                continue;
            }

            try {
                task();
            } catch (...) {
                // 用户任务异常不终止 worker；packaged_task 会自行持有异常。
            }

            // fetch_sub 用 acq_rel 与 enqueue 的计数发布同步；用返回的旧值算出剩余量，
            // 剩余为 0 时才发完成通知，避免每个任务都抢 completion 锁。
            const std::size_t remaining =
                pending_tasks_.fetch_sub(1, std::memory_order_acq_rel) - 1;
            completed_tasks_.fetch_add(1, std::memory_order_relaxed);
            pending_latch_.count_down();
            if (remaining == 0) {
                std::lock_guard<std::mutex> lock(completion_mutex_);
                completion_cv_.notify_all();
            }
        }

        active_count_.fetch_sub(1, std::memory_order_relaxed);
    }

    void monitor_loop() {
        while (running_.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(options_.monitor_interval);

            // 睡醒后复查：关闭可能发生在休眠期间。
            if (!running_.load(std::memory_order_acquire)) {
                break;
            }

            const std::size_t queue_size = queue_.size();
            const std::size_t active_workers = active_count_.load(std::memory_order_acquire);
            const std::size_t target_workers = target_worker_count_.load(std::memory_order_acquire);

            // 扩容：积压超阈值且未达上限。每轮只加一个，平滑响应而非一次性暴涨。
            if (queue_size > options_.grow_threshold && target_workers < options_.max_threads) {
                target_worker_count_.fetch_add(1, std::memory_order_relaxed);
                spawn_worker();
                continue;
            }

            // 缩容：队列空且活跃/目标都高于下限时退掉一个，回收空闲线程。
            if (queue_size == 0 && active_workers > options_.min_threads &&
                target_workers > options_.min_threads) {
                target_worker_count_.fetch_sub(1, std::memory_order_relaxed);
                request_worker_exit(1);
            }
        }
    }

    mutable std::mutex state_mutex_;
    mutable std::mutex workers_mutex_;
    mutable std::mutex completion_mutex_;
    std::condition_variable completion_cv_;
    std::condition_variable pause_cv_;

    ThreadPoolOptions options_;
    std::vector<JoiningThread> workers_;
    std::unique_ptr<JoiningThread> monitor_thread_;
    BlockingQueue<Task> queue_;
    CountDownLatch pending_latch_{0};

    std::atomic<bool> running_{true};
    bool accepting_ = true;  // 受 state_mutex_ 保护
    bool paused_ = false;    // 受 state_mutex_ 保护
    std::atomic<std::size_t> active_count_{0};
    std::atomic<std::size_t> target_worker_count_{0};
    std::atomic<std::size_t> worker_exit_tokens_{0};
    std::atomic<std::size_t> submitted_tasks_{0};
    std::atomic<std::size_t> completed_tasks_{0};
    std::atomic<std::size_t> rejected_tasks_{0};
    std::atomic<std::size_t> pending_tasks_{0};
};
