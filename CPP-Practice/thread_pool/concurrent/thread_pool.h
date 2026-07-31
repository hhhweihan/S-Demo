#pragma once

#include "concurrent/blocking_queue.h"
#include "concurrent/countdown_latch.h"
#include "concurrent/joining_thread.h"
#include "concurrent/work_stealing_deque.h"

#include <atomic>
#include <chrono>
#if defined(__cpp_concepts)
#include <concepts>
#endif
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <future>
#include <limits>
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

// 支持动态扩缩容 + work-stealing 的线程池。worker 数在 [min_threads, max_threads] 间浮动，由一条
// 后台监控线程按队列积压情况增减。运行标志用 atomic，其余提交/暂停状态受 state_mutex_ 保护。
//
// 任务分发两级：每个 worker 私有一个 Chase-Lev deque（work_stealing_deque.h），承接**从 worker
// 内部提交**的子任务（LIFO，缓存最热）；外部线程提交、以及 deque 满溢出的任务进全局 BlockingQueue
// `queue_`——它同时是 worker 无活可干时的**阻塞停泊点**与优雅关闭的哨兵通道。worker 取活顺序：
// 自己的 deque(LIFO) → 随机 victim steal(FIFO) → 阻塞在全局队列上。既有的哨兵关闭、退出令牌 CAS、
// pending_latch_、completion_cv_ 语义原样保留。
class ThreadPool {
 public:
    using Task = std::function<void()>;

    // work-stealing 槽位上限。worker 启动时认领一个空闲槽发布自己的 deque，退出时归还；thief 遍历
    // 全部槽位偷活。取值远高于常见 max_threads，覆盖动态扩缩容瞬态下存活 worker 短暂超过 target 的
    // 情况；认领失败的 worker 只是没有本地 deque（内部提交回退到全局队列），正确性不受影响。
    static constexpr std::size_t kMaxWorkers = 128;

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

        // 一次性建好固定的 deque 池与认领标志：deque 对象活到线程池析构，thief 读到的槽位内存
        // 永远有效，无需回收协议（对照 hazard pointer 那套）。归还的槽位此刻必为空，可被复用。
        deques_ = std::make_unique<WorkStealingDeque<Task>[]>(kMaxWorkers);
        slot_taken_ = std::make_unique<std::atomic<bool>[]>(kMaxWorkers);
        for (std::size_t i = 0; i < kMaxWorkers; ++i) {
            slot_taken_[i].store(false, std::memory_order_relaxed);
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
            // 若由本池的 worker 内部提交，优先塞进它自己的 deque（LIFO、缓存最热）；deque
            // 满或调用者 非本池 worker，则回退到全局队列——全局 push 还能唤醒停泊在 pop 上的
            // worker。
            WorkerContext& ctx = tls_ctx();
            if (ctx.pool == this && ctx.deque != nullptr && ctx.deque->push(task)) {
                return;
            }
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

    // 每个 worker 的线程本地上下文：所属线程池 + 认领到的 deque 及其槽位 + 偷取用的 RNG 状态。
    // 用它在 enqueue_task 里判断"是不是本池 worker 在内部提交"，并给 worker 循环提供本地 deque。
    struct WorkerContext {
        ThreadPool* pool = nullptr;
        WorkStealingDeque<Task>* deque = nullptr;
        std::size_t slot = kNoSlot;
        std::uint64_t rng = 0;
    };

    static WorkerContext& tls_ctx() {
        thread_local WorkerContext ctx;
        return ctx;
    }

    // 认领一个空闲 deque 槽位。全满（存活 worker 超过 kMaxWorkers）时返回 kNoSlot：该 worker 退化为
    // 无本地 deque，内部提交回退全局队列，正确性不变。
    std::size_t claim_slot() {
        for (std::size_t i = 0; i < kMaxWorkers; ++i) {
            bool expected = false;
            if (slot_taken_[i].compare_exchange_strong(expected, true, std::memory_order_acq_rel,
                                                       std::memory_order_relaxed)) {
                // 抬高水位线，让 thief 只需遍历到"曾被认领的最大槽号"，而非全部 kMaxWorkers。
                std::size_t hw = slot_watermark_.load(std::memory_order_relaxed);
                while (i + 1 > hw && !slot_watermark_.compare_exchange_weak(
                                         hw, i + 1, std::memory_order_relaxed)) {
                }
                return i;
            }
        }
        return kNoSlot;
    }

    void release_slot(std::size_t slot) {
        if (slot != kNoSlot) {
            // 归还前 deque 必为空（worker 只在自己 deque 空时才走到退出路径），可安全复用。
            slot_taken_[slot].store(false, std::memory_order_release);
        }
    }

    // 从随机起点遍历其它 worker 的 deque 偷一个任务。命中返回 true。
    bool try_steal(WorkerContext& ctx, Task& out) {
        const std::size_t n = slot_watermark_.load(std::memory_order_acquire);
        if (n <= 1) {
            return false;  // 只有自己（或无人），无处可偷
        }
        // xorshift64 生成随机起点，分散 thief 的争用（不用 rand()/random_device，保持 header-only
        // 无副作用）。
        ctx.rng ^= ctx.rng << 13;
        ctx.rng ^= ctx.rng >> 7;
        ctx.rng ^= ctx.rng << 17;
        const std::size_t start = static_cast<std::size_t>(ctx.rng % n);
        for (std::size_t k = 0; k < n; ++k) {
            const std::size_t i = (start + k) % n;
            if (i == ctx.slot) {
                continue;
            }
            if (deques_[i].steal(out)) {
                return true;
            }
        }
        return false;
    }

    // 执行一个任务并做完成记账（与旧 worker_loop 尾部一致，抽出来供三个取活来源共用）。
    void run_task(Task& task) {
        try {
            task();
        } catch (...) {
            // 用户任务异常不终止 worker；packaged_task 会自行持有异常。
        }

        // fetch_sub 用 acq_rel 与 enqueue 的计数发布同步；用返回的旧值算出剩余量，
        // 剩余为 0 时才发完成通知，避免每个任务都抢 completion 锁。
        const std::size_t remaining = pending_tasks_.fetch_sub(1, std::memory_order_acq_rel) - 1;
        completed_tasks_.fetch_add(1, std::memory_order_relaxed);
        pending_latch_.count_down();
        if (remaining == 0) {
            std::lock_guard<std::mutex> lock(completion_mutex_);
            completion_cv_.notify_all();
        }
    }

    void worker_loop() {
        WorkerContext& ctx = tls_ctx();
        ctx.pool = this;
        ctx.slot = claim_slot();
        ctx.deque = (ctx.slot != kNoSlot) ? &deques_[ctx.slot] : nullptr;
        // 用槽号播种 RNG，避免所有 worker 同起点撞在一起；不含时间/随机源，可复现。
        ctx.rng = 0x9e3779b97f4a7c15ULL ^ ((ctx.slot + 1) * 0x100000001b3ULL);

        active_count_.fetch_add(1, std::memory_order_relaxed);

        while (true) {
            Task task;

            // 1) 先掏自己的 deque（LIFO，缓存最热）。
            if (ctx.deque != nullptr && ctx.deque->pop(task)) {
                run_task(task);
                continue;
            }

            // 2) 自己空了，从别的 worker 偷（FIFO）。
            if (try_steal(ctx, task)) {
                run_task(task);
                continue;
            }

            // 3) 无处可偷：阻塞停泊在全局队列上，顺带接收关闭哨兵。
            try {
                task = queue_.pop();
            } catch (const std::exception&) {
                break;  // 队列已 shutdown，无更多任务
            }

            if (!task) {
                // 收到哨兵：整池关闭则直接退；否则只在抢到退出令牌时退（缩容），
                // 抢不到说明这个哨兵不是给自己的，继续干活。走到这里本地 deque
                // 必空，退出不会遗留任务。
                if (!running_.load(std::memory_order_acquire) || try_consume_exit_token()) {
                    break;
                }
                continue;
            }

            run_task(task);
        }

        active_count_.fetch_sub(1, std::memory_order_relaxed);
        release_slot(ctx.slot);
        ctx.pool = nullptr;
        ctx.deque = nullptr;
        ctx.slot = kNoSlot;
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

    static constexpr std::size_t kNoSlot = std::numeric_limits<std::size_t>::max();

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

    // work-stealing：固定 deque 池 + 槽位认领标志 + 已用槽位水位线（thief 只遍历到此）。
    std::unique_ptr<WorkStealingDeque<Task>[]> deques_;
    std::unique_ptr<std::atomic<bool>[]> slot_taken_;
    std::atomic<std::size_t> slot_watermark_{0};

    std::atomic<bool> running_{true};
    bool accepting_ = true;  // 受 state_mutex_ 保护
    bool paused_ = false;    // 受 state_mutex_ 保护
    // 热点计数器各占一条 cache line：worker 高频更新 active/pending/completed，若挤在同一行会
    // 互相打脏对方缓存（false sharing）。alignas 隔离后各自独享一行。target/exit_tokens/submitted/
    // rejected 更新频率低，不单独隔离。
    alignas(kCacheLine) std::atomic<std::size_t> active_count_{0};
    alignas(kCacheLine) std::atomic<std::size_t> pending_tasks_{0};
    alignas(kCacheLine) std::atomic<std::size_t> completed_tasks_{0};
    std::atomic<std::size_t> target_worker_count_{0};
    std::atomic<std::size_t> worker_exit_tokens_{0};
    std::atomic<std::size_t> submitted_tasks_{0};
    std::atomic<std::size_t> rejected_tasks_{0};
};
