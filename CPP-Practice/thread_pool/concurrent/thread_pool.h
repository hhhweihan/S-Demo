#pragma once  // 保证头文件只被包含一次

#include "concurrent/blocking_queue.h"  // 引入阻塞任务队列
#include "concurrent/countdown_latch.h"  // 引入任务完成计数器
#include "concurrent/joining_thread.h"  // 引入自动 join 线程包装器

#include <atomic>  // 引入原子变量支持
#include <chrono>  // 引入时间间隔支持
#if defined(__cpp_concepts)  // 检查是否支持 concepts
#include <concepts>  // 引入 invocable 约束
#endif  // 结束 concepts 条件包含
#include <cstddef>  // 引入 std::size_t
#include <condition_variable>  // 引入条件变量支持
#include <exception>  // 引入异常基类
#include <functional>  // 引入 function 和 bind
#include <future>  // 引入 future 和 packaged_task
#include <memory>  // 引入智能指针
#include <mutex>  // 引入互斥锁支持
#include <stdexcept>  // 引入标准异常类型
#include <type_traits>  // 引入返回类型推导工具
#include <utility>  // 引入移动和转发工具
#include <vector>  // 引入 worker 容器

struct ThreadPoolOptions {  // 定义线程池配置项
    std::size_t min_threads = 1;  // 最少工作线程数
    std::size_t max_threads = 1;  // 最多工作线程数
    std::size_t grow_threshold = 4;  // 触发扩容的队列阈值
    std::chrono::milliseconds monitor_interval{1000};  // 监控线程检查间隔
};  // 结束 ThreadPoolOptions 定义

class ThreadPool {  // 声明动态线程池
public:  // 对外公开接口
    using Task = std::function<void()>;  // 定义无返回任务类型

    struct Stats {  // 定义线程池快照统计
        std::size_t submitted = 0;  // 已提交任务数
        std::size_t completed = 0;  // 已完成任务数
        std::size_t rejected = 0;  // 已拒绝任务数
        std::size_t pending = 0;  // 等待完成任务数
        std::size_t active_workers = 0;  // 当前活跃 worker 数
        std::size_t target_workers = 0;  // 目标 worker 数
        std::size_t queue_size = 0;  // 当前队列长度
        bool accepting = false;  // 是否仍接受提交
        bool paused = false;  // 是否处于暂停提交状态
    };  // 结束 Stats 定义

    explicit ThreadPool(std::size_t worker_count)  // 使用固定线程数构造
        : ThreadPool(ThreadPoolOptions{worker_count, worker_count}) {}  // 转发为固定配置

    explicit ThreadPool(ThreadPoolOptions options)  // 使用完整配置构造
        : options_(options) {  // 保存线程池配置
        if (options_.min_threads == 0) {  // 校验最小线程数
            throw std::invalid_argument("min_threads must be > 0");  // 抛出非法最小线程数
        }  // 结束最小线程数校验
        if (options_.max_threads < options_.min_threads) {  // 校验最大线程数不小于最小值
            throw std::invalid_argument("max_threads must be >= min_threads");  // 抛出非法最大线程数
        }  // 结束最大线程数校验

        target_worker_count_.store(options_.min_threads, std::memory_order_relaxed);  // 初始化目标 worker 数
        for (std::size_t index = 0; index < options_.min_threads; ++index) {  // 创建最小数量 worker
            spawn_worker();  // 启动一个 worker
        }  // 结束初始 worker 创建

        if (options_.max_threads > options_.min_threads) {  // 需要动态扩缩容时启动监控线程
            monitor_thread_ = std::make_unique<JoiningThread>([this] { monitor_loop(); });  // 创建监控线程
        }  // 结束监控线程创建
    }  // 结束 ThreadPool 构造

    ~ThreadPool() {  // 析构线程池
        shutdown();  // 确保线程池关闭
    }  // 结束析构

    ThreadPool(const ThreadPool&) = delete;  // 禁止复制线程池
    ThreadPool& operator=(const ThreadPool&) = delete;  // 禁止复制赋值线程池

    void submit(Task task) {  // 提交无返回任务
        if (!task) {  // 检查任务是否为空
            throw std::invalid_argument("empty task is not allowed");  // 拒绝空任务
        }  // 结束空任务检查

        enqueue_task(std::move(task));  // 入队任务
    }  // 结束 submit

#if defined(__cpp_concepts)  // concepts 可用时启用约束
    template <typename F, typename... Args>  // 定义带返回值任务模板
        requires std::invocable<F, Args...>  // 约束可调用对象
#else  // concepts 不可用时使用普通模板
    template <typename F, typename... Args>  // 定义带返回值任务模板
#endif  // 结束 submit 模板条件编译
    auto submit(F&& func, Args&&... args)  // 提交可调用对象和参数
        -> std::future<std::invoke_result_t<F, Args...>> {  // 返回任务 future
        using RetType = std::invoke_result_t<F, Args...>;  // 推导任务返回类型

        auto packaged_task = std::make_shared<std::packaged_task<RetType()>>(  // 创建共享 packaged_task
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));  // 绑定函数和参数
        std::future<RetType> future = packaged_task->get_future();  // 取得返回值 future

        enqueue_task([packaged_task]() { (*packaged_task)(); });  // 将 packaged_task 包装为无参任务
        return future;  // 返回 future 给调用方
    }  // 结束带返回值 submit

    void await_termination() {  // 等待所有已提交任务完成并关闭
        pending_latch_.wait();  // 等待待完成任务归零
        shutdown();  // 关闭线程池
    }  // 结束 await_termination

    void pause() {  // 暂停新任务提交入队
        std::lock_guard<std::mutex> lock(state_mutex_);  // 加锁修改状态
        if (accepting_) {  // 仅在仍接受任务时暂停
            paused_ = true;  // 设置暂停标志
        }  // 结束接受状态检查
    }  // 结束 pause

    void resume() {  // 恢复新任务提交入队
        {  // 限定状态锁作用域
            std::lock_guard<std::mutex> lock(state_mutex_);  // 加锁修改暂停状态
            paused_ = false;  // 清除暂停标志
        }  // 释放状态锁
        pause_cv_.notify_all();  // 唤醒等待提交的线程
    }  // 结束 resume

    void shutdown() {  // 关闭线程池
        bool expected = true;  // 准备从运行态切换
        if (!running_.compare_exchange_strong(expected, false, std::memory_order_acq_rel)) {  // 确保只关闭一次
            return;  // 已关闭则直接返回
        }  // 结束关闭一次性检查

        {  // 限定状态锁作用域
            std::lock_guard<std::mutex> lock(state_mutex_);  // 加锁修改提交状态
            accepting_ = false;  // 停止接受新任务
            paused_ = false;  // 清除暂停状态
        }  // 释放状态锁
        pause_cv_.notify_all();  // 唤醒所有暂停提交者

        if (monitor_thread_ != nullptr) {  // 存在监控线程时回收
            monitor_thread_->join();  // 等待监控线程退出
            monitor_thread_.reset();  // 释放监控线程对象
        }  // 结束监控线程回收

        std::size_t worker_slots = 0;  // 记录需要退出的 worker 数
        {  // 限定 worker 锁作用域
            std::lock_guard<std::mutex> lock(workers_mutex_);  // 加锁读取 worker 容器
            worker_slots = workers_.size();  // 保存当前 worker 数
        }  // 释放 worker 锁

        request_worker_exit(worker_slots);  // 投递 worker 退出令牌
        queue_.shutdown();  // 关闭任务队列唤醒阻塞 worker

        std::lock_guard<std::mutex> lock(workers_mutex_);  // 加锁清理 worker 容器
        workers_.clear();  // 销毁并 join 所有 worker
    }  // 结束 shutdown

    Stats snapshot_stats() const {  // 获取线程池统计快照
        std::lock_guard<std::mutex> lock(state_mutex_);  // 加锁读取状态标志
        return Stats{submitted_tasks_.load(std::memory_order_relaxed),  // 填充提交数量
                     completed_tasks_.load(std::memory_order_relaxed),  // 填充完成数量
                     rejected_tasks_.load(std::memory_order_relaxed),  // 填充拒绝数量
                     pending_tasks_.load(std::memory_order_relaxed),  // 填充待完成数量
                     active_count_.load(std::memory_order_relaxed),  // 填充活跃 worker 数
                     target_worker_count_.load(std::memory_order_relaxed),  // 填充目标 worker 数
                     queue_.size(),  // 填充队列长度
                     accepting_,  // 填充接受状态
                     paused_};  // 填充暂停状态
    }  // 结束 snapshot_stats

private:  // 内部实现细节
    void enqueue_task(Task task) {  // 将任务安全加入队列
        {  // 限定状态锁作用域
            std::unique_lock<std::mutex> lock(state_mutex_);  // 获取可等待状态锁
            pause_cv_.wait(lock, [this] {  // 等待暂停状态解除或线程池停止
                return !paused_ || !accepting_ || !running_.load(std::memory_order_acquire);  // 提交可继续的条件
            });  // 结束暂停等待

            if (!accepting_ || !running_.load(std::memory_order_acquire)) {  // 停止状态拒绝提交
                rejected_tasks_.fetch_add(1, std::memory_order_relaxed);  // 统计拒绝任务
                throw std::runtime_error("submit on stopped ThreadPool");  // 抛出停止提交异常
            }  // 结束停止状态检查

            submitted_tasks_.fetch_add(1, std::memory_order_relaxed);  // 增加提交计数
            pending_tasks_.fetch_add(1, std::memory_order_relaxed);  // 增加待完成计数
            pending_latch_.count_up();  // 增加完成等待计数
        }  // 释放状态锁

        try {  // 尝试把任务写入队列
            queue_.push(std::move(task));  // 移动任务进入阻塞队列
        } catch (...) {  // 入队失败时回滚计数
            pending_tasks_.fetch_sub(1, std::memory_order_relaxed);  // 回退待完成计数
            pending_latch_.count_down();  // 回退 latch 计数
            rejected_tasks_.fetch_add(1, std::memory_order_relaxed);  // 统计拒绝任务
            throw;  // 保留原异常继续抛出
        }  // 结束入队异常处理
    }  // 结束 enqueue_task

    void spawn_worker() {  // 创建一个 worker 线程
        std::lock_guard<std::mutex> lock(workers_mutex_);  // 加锁修改 worker 容器
        workers_.emplace_back([this] { worker_loop(); });  // 启动执行 worker_loop 的线程
    }  // 结束 spawn_worker

    void request_worker_exit(std::size_t count) {  // 请求指定数量 worker 退出
        if (count == 0) {  // 无需退出任何 worker
            return;  // 直接返回
        }  // 结束数量检查

        worker_exit_tokens_.fetch_add(count, std::memory_order_release);  // 增加退出令牌数
        for (std::size_t index = 0; index < count; ++index) {  // 为每个退出请求投递哨兵任务
            queue_.push(Task{});  // 空任务作为 worker 唤醒信号
        }  // 结束退出哨兵投递
    }  // 结束 request_worker_exit

    bool try_consume_exit_token() {  // 尝试消费一个退出令牌
        std::size_t current = worker_exit_tokens_.load(std::memory_order_acquire);  // 读取当前令牌数
        while (current > 0) {  // 有令牌时尝试扣减
            if (worker_exit_tokens_.compare_exchange_weak(current,  // 比较并更新令牌数
                                                          current - 1,  // 目标令牌数减一
                                                          std::memory_order_acq_rel,  // 成功时同步退出请求
                                                          std::memory_order_acquire)) {  // 失败时重新读取
                return true;  // 成功消费退出令牌
            }  // 结束 CAS 成功检查
        }  // 结束令牌消费循环
        return false;  // 没有可消费令牌
    }  // 结束 try_consume_exit_token

    void worker_loop() {  // worker 主循环
        active_count_.fetch_add(1, std::memory_order_relaxed);  // 记录一个活跃 worker

        while (true) {  // 持续处理任务直到退出
            Task task;  // 保存本轮取出的任务
            try {  // 尝试从队列获取任务
                task = queue_.pop();  // 阻塞等待任务
            } catch (const std::exception&) {  // 队列关闭时退出循环
                break;  // 结束 worker 主循环
            }  // 结束取任务异常处理

            if (!task) {  // 空任务表示可能需要退出
                if (!running_.load(std::memory_order_acquire) || try_consume_exit_token()) {  // 关闭或拿到退出令牌时退出
                    break;  // 结束 worker 主循环
                }  // 结束退出条件检查
                continue;  // 否则忽略哨兵继续取任务
            }  // 结束空任务处理

            try {  // 执行用户任务
                task();  // 调用任务函数
            } catch (...) {  // 捕获用户任务异常
                // 用户任务异常不终止 worker；packaged_task 会自行持有异常。
            }  // 结束任务异常处理

            const std::size_t remaining =  // 计算执行后剩余待完成任务
                pending_tasks_.fetch_sub(1, std::memory_order_acq_rel) - 1;  // 扣减待完成计数
            completed_tasks_.fetch_add(1, std::memory_order_relaxed);  // 增加完成计数
            pending_latch_.count_down();  // 通知一个任务完成
            if (remaining == 0) {  // 所有任务完成时通知等待者
                std::lock_guard<std::mutex> lock(completion_mutex_);  // 加锁配合完成条件变量
                completion_cv_.notify_all();  // 唤醒完成等待者
            }  // 结束完成通知
        }  // 结束 worker 主循环

        active_count_.fetch_sub(1, std::memory_order_relaxed);  // 记录 worker 退出
    }  // 结束 worker_loop

    void monitor_loop() {  // 动态扩缩容监控循环
        while (running_.load(std::memory_order_acquire)) {  // 在线程池运行时循环
            std::this_thread::sleep_for(options_.monitor_interval);  // 按配置间隔休眠

            if (!running_.load(std::memory_order_acquire)) {  // 休眠后再次检查运行状态
                break;  // 关闭时退出监控循环
            }  // 结束运行状态检查

            const std::size_t queue_size = queue_.size();  // 获取当前队列长度
            const std::size_t active_workers = active_count_.load(std::memory_order_acquire);  // 获取活跃 worker 数
            const std::size_t target_workers = target_worker_count_.load(std::memory_order_acquire);  // 获取目标 worker 数

            if (queue_size > options_.grow_threshold && target_workers < options_.max_threads) {  // 队列积压且未达上限时扩容
                target_worker_count_.fetch_add(1, std::memory_order_relaxed);  // 增加目标 worker 数
                spawn_worker();  // 新建一个 worker
                continue;  // 进入下一轮监控
            }  // 结束扩容判断

            if (queue_size == 0 && active_workers > options_.min_threads &&  // 队列为空且活跃线程超过下限
                target_workers > options_.min_threads) {  // 目标线程数也超过下限
                target_worker_count_.fetch_sub(1, std::memory_order_relaxed);  // 降低目标 worker 数
                request_worker_exit(1);  // 请求一个 worker 退出
            }  // 结束缩容判断
        }  // 结束监控循环
    }  // 结束 monitor_loop

    mutable std::mutex state_mutex_;  // 保护提交和暂停状态
    mutable std::mutex workers_mutex_;  // 保护 worker 容器
    mutable std::mutex completion_mutex_;  // 配合完成通知使用
    std::condition_variable completion_cv_;  // 所有任务完成的通知条件变量
    std::condition_variable pause_cv_;  // 暂停和恢复提交的条件变量

    ThreadPoolOptions options_;  // 保存线程池配置
    std::vector<JoiningThread> workers_;  // 保存 worker 线程集合
    std::unique_ptr<JoiningThread> monitor_thread_;  // 保存动态监控线程
    BlockingQueue<Task> queue_;  // 保存待执行任务队列
    CountDownLatch pending_latch_{0};  // 跟踪待完成任务数量

    std::atomic<bool> running_{true};  // 标记线程池是否运行
    bool accepting_ = true;  // 标记是否接受新任务
    bool paused_ = false;  // 标记是否暂停提交
    std::atomic<std::size_t> active_count_{0};  // 当前活跃 worker 数
    std::atomic<std::size_t> target_worker_count_{0};  // 当前目标 worker 数
    std::atomic<std::size_t> worker_exit_tokens_{0};  // 待消费的 worker 退出令牌
    std::atomic<std::size_t> submitted_tasks_{0};  // 累计提交任务数
    std::atomic<std::size_t> completed_tasks_{0};  // 累计完成任务数
    std::atomic<std::size_t> rejected_tasks_{0};  // 累计拒绝任务数
    std::atomic<std::size_t> pending_tasks_{0};  // 当前待完成任务数
};  // 结束 ThreadPool 定义