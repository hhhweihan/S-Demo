#pragma once

#include "concurrent/blocking_queue.h"
#include "concurrent/countdown_latch.h"
#include "concurrent/joining_thread.h"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

struct ThreadPoolOptions {
    std::size_t min_threads = 1;
    std::size_t max_threads = 1;
    std::size_t grow_threshold = 4;
    std::chrono::milliseconds monitor_interval{1000};
};

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

    explicit ThreadPool(ThreadPoolOptions options)
        : options_(options) {
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

        if (options_.max_threads > options_.min_threads) {
            monitor_thread_ = std::make_unique<JoiningThread>([this] { monitor_loop(); });
        }
    }

    ~ThreadPool() {
        shutdown();
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void submit(Task task) {
        if (!task) {
            throw std::invalid_argument("empty task is not allowed");
        }

        enqueue_task(std::move(task));
    }

    template <typename F, typename... Args>
    auto submit(F&& func, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>> {
        using RetType = std::invoke_result_t<F, Args...>;

        auto packaged_task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));
        std::future<RetType> future = packaged_task->get_future();

        enqueue_task([packaged_task]() { (*packaged_task)(); });
        return future;
    }

    void await_termination() {
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
        bool expected = true;
        if (!running_.compare_exchange_strong(expected, false, std::memory_order_acq_rel)) {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(state_mutex_);
            accepting_ = false;
            paused_ = false;
        }
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

        request_worker_exit(worker_slots);
        queue_.shutdown();

        std::lock_guard<std::mutex> lock(workers_mutex_);
        workers_.clear();
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
            pause_cv_.wait(lock, [this] {
                return !paused_ || !accepting_ || !running_.load(std::memory_order_acquire);
            });

            if (!accepting_ || !running_.load(std::memory_order_acquire)) {
                rejected_tasks_.fetch_add(1, std::memory_order_relaxed);
                throw std::runtime_error("submit on stopped ThreadPool");
            }

            submitted_tasks_.fetch_add(1, std::memory_order_relaxed);
            pending_tasks_.fetch_add(1, std::memory_order_relaxed);
            pending_latch_.count_up();
        }

        try {
            queue_.push(std::move(task));
        } catch (...) {
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

        worker_exit_tokens_.fetch_add(count, std::memory_order_release);
        for (std::size_t index = 0; index < count; ++index) {
            queue_.push(Task{});
        }
    }

    bool try_consume_exit_token() {
        std::size_t current = worker_exit_tokens_.load(std::memory_order_acquire);
        while (current > 0) {
            if (worker_exit_tokens_.compare_exchange_weak(current,
                                                          current - 1,
                                                          std::memory_order_acq_rel,
                                                          std::memory_order_acquire)) {
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
                break;
            }

            if (!task) {
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

            if (!running_.load(std::memory_order_acquire)) {
                break;
            }

            const std::size_t queue_size = queue_.size();
            const std::size_t active_workers = active_count_.load(std::memory_order_acquire);
            const std::size_t target_workers = target_worker_count_.load(std::memory_order_acquire);

            if (queue_size > options_.grow_threshold && target_workers < options_.max_threads) {
                target_worker_count_.fetch_add(1, std::memory_order_relaxed);
                spawn_worker();
                continue;
            }

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
    bool accepting_ = true;
    bool paused_ = false;
    std::atomic<std::size_t> active_count_{0};
    std::atomic<std::size_t> target_worker_count_{0};
    std::atomic<std::size_t> worker_exit_tokens_{0};
    std::atomic<std::size_t> submitted_tasks_{0};
    std::atomic<std::size_t> completed_tasks_{0};
    std::atomic<std::size_t> rejected_tasks_{0};
    std::atomic<std::size_t> pending_tasks_{0};
};