#include "concurrent/countdown_latch.h"  // 引入倒计时同步器
#include "concurrent/rw_mutex.h"  // 引入读写锁实现
#include "concurrent/thread_pool.h"  // 引入线程池实现

#include <algorithm>  // 引入排序算法
#include <atomic>  // 引入原子类型
#include <chrono>  // 引入计时工具
#include <cstddef>  // 引入 std::size_t
#include <functional>  // 引入 std::function
#include <iomanip>  // 引入输出格式控制
#include <iostream>  // 引入标准输入输出流
#include <mutex>  // 引入互斥锁
#include <queue>  // 引入标准队列
#include <stdexcept>  // 引入运行时异常
#include <string>  // 引入字符串类型
#include <thread>  // 引入线程支持
#include <utility>  // 引入移动工具
#include <vector>  // 引入动态数组

#define EXPECT_TRUE(cond) /* 定义真值断言宏 */                                             \
    do { /* 使用 do while 包裹宏体 */                                                      \
        if (!(cond)) { /* 条件失败时抛出异常 */                                            \
            throw std::runtime_error("EXPECT_TRUE failed: " #cond); /* 报告失败条件 */     \
        } /* 结束失败分支 */                                                               \
    } while (false)  // 结束真值断言宏

#define EXPECT_EQ(lhs, rhs) EXPECT_TRUE((lhs) == (rhs))  // 定义相等断言宏

namespace {  // 使用匿名命名空间限制测试符号

using Clock = std::chrono::steady_clock;  // 定义稳定时钟别名

constexpr int kBenchmarkTaskCount = 1'000'000;  // 线程池基准任务数量
constexpr int kQueueBenchmarkCount = 1'000'000;  // 队列基准元素数量

struct BenchmarkResult {  // 保存吞吐基准结果
    std::string name;  // 基准名称
    double elapsed_ms = 0.0;  // 耗时毫秒数
    double qps = 0.0;  // 每秒处理数量
};  // 结束 BenchmarkResult 定义

struct LatencyBenchmarkResult {  // 保存延迟基准结果
    double p50_us = 0.0;  // 50 分位延迟微秒数
    double p99_us = 0.0;  // 99 分位延迟微秒数
};  // 结束 LatencyBenchmarkResult 定义

template <typename Fn>  // 接受任意可调用对象
double measure_ms(Fn&& fn) {  // 测量函数耗时毫秒数
    const auto start = Clock::now();  // 记录开始时间
    fn();  // 执行被测函数
    const auto end = Clock::now();  // 记录结束时间
    return std::chrono::duration<double, std::milli>(end - start).count();  // 返回毫秒耗时
}  // 结束 measure_ms

double micros_between(Clock::time_point start, Clock::time_point end) {  // 计算两个时间点的微秒差
    return std::chrono::duration<double, std::micro>(end - start).count();  // 返回微秒差值
}  // 结束 micros_between

double percentile_us(const std::vector<double>& values, double percentile) {  // 读取已排序延迟分位值
    EXPECT_TRUE(!values.empty());  // 确保样本非空
    const double index = percentile * static_cast<double>(values.size() - 1);  // 计算分位下标
    return values[static_cast<std::size_t>(index)];  // 返回对应分位值
}  // 结束 percentile_us

void test_day50_rw_mutex() {  // 测试读写锁互斥语义
    RWMutex mutex;  // 创建读写锁
    int shared_value = 0;  // 定义共享数据
    std::atomic<int> active_readers{0};  // 统计活跃读者数
    std::atomic<int> active_writers{0};  // 统计活跃写者数
    std::atomic<bool> valid{true};  // 记录互斥校验是否通过

    std::thread writer([&] {  // 启动写线程
        for (int iteration = 0; iteration < 2000; ++iteration) {  // 多次写入共享值
            mutex.lock_write();  // 获取写锁
            const int previous_writers = active_writers.fetch_add(1, std::memory_order_acq_rel);  // 标记当前写者进入
            if (previous_writers != 0 || active_readers.load(std::memory_order_acquire) != 0) {  // 检查写者独占性
                valid.store(false, std::memory_order_relaxed);  // 标记校验失败
            }  // 结束独占性检查

            ++shared_value;  // 更新共享值

            active_writers.fetch_sub(1, std::memory_order_acq_rel);  // 标记写者离开
            mutex.unlock_write();  // 释放写锁
        }  // 结束写入循环
    });  // 结束写线程创建

    std::vector<std::thread> readers;  // 保存读线程集合
    readers.reserve(4);  // 预留读线程容量
    for (int index = 0; index < 4; ++index) {  // 创建多个读线程
        readers.emplace_back([&] {  // 启动一个读线程
            int last_seen = 0;  // 记录上次读到的值
            for (int iteration = 0; iteration < 4000; ++iteration) {  // 多次读取共享值
                mutex.lock_read();  // 获取读锁
                active_readers.fetch_add(1, std::memory_order_acq_rel);  // 标记读者进入
                if (active_writers.load(std::memory_order_acquire) != 0) {  // 检查读写不能并发
                    valid.store(false, std::memory_order_relaxed);  // 标记校验失败
                }  // 结束读写并发检查

                const int snapshot = shared_value;  // 读取共享值快照
                if (snapshot < last_seen) {  // 检查读到的值不应倒退
                    valid.store(false, std::memory_order_relaxed);  // 标记校验失败
                }  // 结束单调性检查
                last_seen = snapshot;  // 更新最近快照

                active_readers.fetch_sub(1, std::memory_order_acq_rel);  // 标记读者离开
                mutex.unlock_read();  // 释放读锁
            }  // 结束读取循环
        });  // 结束读线程创建
    }  // 结束读线程创建循环

    writer.join();  // 等待写线程结束
    for (auto& reader : readers) {  // 遍历所有读线程
        reader.join();  // 等待读线程结束
    }  // 结束读线程回收

    EXPECT_TRUE(valid.load(std::memory_order_relaxed));  // 断言读写锁互斥校验通过
    EXPECT_EQ(shared_value, 2000);  // 断言写入次数正确
}  // 结束 test_day50_rw_mutex

void test_day51_countdown_latch() {  // 测试倒计时同步器
    CountDownLatch latch(5);  // 创建初始计数为 5 的 latch
    std::atomic<int> finished{0};  // 统计完成 worker 数

    std::vector<std::thread> workers;  // 保存 worker 线程
    workers.reserve(5);  // 预留 worker 容量
    for (int index = 0; index < 5; ++index) {  // 创建五个 worker
        workers.emplace_back([&, index] {  // 启动一个延迟完成的 worker
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + index * 5));  // 模拟不同耗时
            finished.fetch_add(1, std::memory_order_relaxed);  // 增加完成计数
            latch.count_down();  // 通知 latch 一个任务完成
        });  // 结束 worker 创建
    }  // 结束 worker 创建循环

    EXPECT_TRUE(!latch.wait_for(std::chrono::milliseconds(5)));  // 断言短超时不会提前完成
    latch.wait();  // 等待所有 worker 完成

    for (auto& worker : workers) {  // 遍历 worker 线程
        worker.join();  // 等待 worker 退出
    }  // 结束 worker 回收

    EXPECT_EQ(finished.load(std::memory_order_relaxed), 5);  // 断言全部 worker 完成
    EXPECT_EQ(latch.count(), static_cast<std::size_t>(0));  // 断言 latch 计数归零
}  // 结束 test_day51_countdown_latch

void test_day36_basic_thread_pool() {  // 测试线程池基础提交
    ThreadPool pool(4);  // 创建四线程池
    std::atomic<int> counter{0};  // 统计任务执行次数

    for (int index = 0; index < 100; ++index) {  // 提交一百个任务
        pool.submit(std::function<void()>([&counter]() {  // 提交无返回任务
            counter.fetch_add(1, std::memory_order_relaxed);  // 增加执行计数
        }));  // 结束任务提交
    }  // 结束任务提交循环

    pool.await_termination();  // 等待线程池完成并关闭
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 100);  // 断言任务全部执行
}  // 结束 test_day36_basic_thread_pool

void test_day37_future_submit() {  // 测试带返回值提交
    ThreadPool pool(4);  // 创建四线程池
    std::vector<std::future<int>> futures;  // 保存任务 future

    for (int index = 0; index < 10; ++index) {  // 提交十个返回值任务
        futures.push_back(pool.submit([index]() { return index * 10; }));  // 保存任务返回 future
    }  // 结束任务提交循环

    for (int index = 0; index < 10; ++index) {  // 逐个读取 future
        EXPECT_EQ(futures[static_cast<std::size_t>(index)].get(), index * 10);  // 断言返回值正确
    }  // 结束 future 校验循环

    pool.await_termination();  // 等待线程池完成并关闭
}  // 结束 test_day37_future_submit

void test_day38_await_termination() {  // 测试等待终止逻辑
    ThreadPool pool(4);  // 创建四线程池
    std::atomic<int> counter{0};  // 统计任务执行次数

    for (int index = 0; index < 1000; ++index) {  // 提交一千个任务
        pool.submit(std::function<void()>([&counter]() {  // 提交计数任务
            counter.fetch_add(1, std::memory_order_relaxed);  // 增加执行计数
        }));  // 结束任务提交
    }  // 结束任务提交循环

    pool.await_termination();  // 等待所有任务完成
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 1000);  // 断言任务全部执行
}  // 结束 test_day38_await_termination

void test_day39_dynamic_scaling() {  // 测试线程池动态扩容
    ThreadPoolOptions options;  // 创建线程池配置
    options.min_threads = 2;  // 设置最小线程数
    options.max_threads = 4;  // 设置最大线程数
    options.grow_threshold = 2;  // 设置扩容队列阈值
    options.monitor_interval = std::chrono::milliseconds(100);  // 设置监控间隔

    ThreadPool pool(options);  // 使用动态配置创建线程池
    std::atomic<int> counter{0};  // 统计任务完成数量

    for (int index = 0; index < 100; ++index) {  // 提交足够触发扩容的任务
        pool.submit(std::function<void()>([&counter]() {  // 提交慢任务
            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 模拟任务耗时
            counter.fetch_add(1, std::memory_order_relaxed);  // 增加完成计数
        }));  // 结束任务提交
    }  // 结束任务提交循环

    std::this_thread::sleep_for(std::chrono::milliseconds(350));  // 等待监控线程完成扩容判断
    const ThreadPool::Stats stats = pool.snapshot_stats();  // 获取扩容后的统计快照

    pool.await_termination();  // 等待任务完成并关闭线程池

    EXPECT_TRUE(stats.active_workers >= 3);  // 断言活跃 worker 已扩容
    EXPECT_TRUE(stats.target_workers >= 3);  // 断言目标 worker 已扩容
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 100);  // 断言任务全部完成
}  // 结束 test_day39_dynamic_scaling

BenchmarkResult run_thread_pool_benchmark(std::size_t worker_count) {  // 运行线程池吞吐基准
    ThreadPool pool(worker_count);  // 创建指定线程数线程池
    std::atomic<int> counter{0};  // 统计执行任务数

    const double elapsed_ms = measure_ms([&]() {  // 测量提交和执行总耗时
        for (int index = 0; index < kBenchmarkTaskCount; ++index) {  // 提交大量轻任务
            pool.submit(std::function<void()>([&counter]() {  // 提交计数任务
                counter.fetch_add(1, std::memory_order_relaxed);  // 增加完成计数
            }));  // 结束任务提交
        }  // 结束基准任务提交
        pool.await_termination();  // 等待基准任务全部完成
    });  // 结束耗时测量

    EXPECT_EQ(counter.load(std::memory_order_relaxed), kBenchmarkTaskCount);  // 断言基准任务全部执行
    return BenchmarkResult{"thread_pool_" + std::to_string(worker_count) + "_threads",  // 返回线程池基准名称
                           elapsed_ms,  // 返回基准耗时
                           kBenchmarkTaskCount / (elapsed_ms / 1000.0)};  // 计算并返回 QPS
}  // 结束 run_thread_pool_benchmark

BenchmarkResult run_single_thread_benchmark() {  // 运行单线程基线基准
    std::atomic<int> counter{0};  // 统计执行次数

    const double elapsed_ms = measure_ms([&]() {  // 测量单线程计数耗时
        for (int index = 0; index < kBenchmarkTaskCount; ++index) {  // 执行相同数量轻操作
            counter.fetch_add(1, std::memory_order_relaxed);  // 增加计数
        }  // 结束单线程循环
    });  // 结束耗时测量

    EXPECT_EQ(counter.load(std::memory_order_relaxed), kBenchmarkTaskCount);  // 断言基线计数正确
    return BenchmarkResult{"single_thread_baseline",  // 返回单线程基准名称
                           elapsed_ms,  // 返回基准耗时
                           kBenchmarkTaskCount / (elapsed_ms / 1000.0)};  // 计算并返回 QPS
}  // 结束 run_single_thread_benchmark

void test_day41_stats_improvement() {  // 测试线程池统计信息
    ThreadPool pool(4);  // 创建四线程池
    std::vector<std::future<int>> futures;  // 保存返回值 future

    for (int index = 0; index < 20; ++index) {  // 提交二十个求和任务
        futures.push_back(pool.submit([index]() { return index + 1; }));  // 保存每个任务 future
    }  // 结束任务提交循环

    int sum = 0;  // 汇总任务返回值
    for (auto& future : futures) {  // 遍历所有 future
        sum += future.get();  // 累加任务结果
    }  // 结束结果汇总

    pool.await_termination();  // 等待线程池完成并关闭
    const ThreadPool::Stats stats = pool.snapshot_stats();  // 获取关闭后的统计快照

    EXPECT_EQ(sum, 210);  // 断言求和结果正确
    EXPECT_EQ(stats.submitted, static_cast<std::size_t>(20));  // 断言提交数正确
    EXPECT_EQ(stats.completed, static_cast<std::size_t>(20));  // 断言完成数正确
    EXPECT_EQ(stats.pending, static_cast<std::size_t>(0));  // 断言无待完成任务
}  // 结束 test_day41_stats_improvement

void test_day52_pause_resume_and_await_termination() {  // 测试暂停恢复和等待终止
    ThreadPool pool(4);  // 创建四线程池
    std::atomic<int> counter{0};  // 统计执行任务数

    for (int index = 0; index < 100; ++index) {  // 提交第一批任务
        pool.submit([&counter] {  // 提交短耗时任务
            std::this_thread::sleep_for(std::chrono::milliseconds(2));  // 模拟任务耗时
            counter.fetch_add(1, std::memory_order_relaxed);  // 增加完成计数
        });  // 结束任务提交
    }  // 结束第一批任务提交

    pool.pause();  // 暂停后续提交入队

    CountDownLatch submitter_started(1);  // 用于确认提交线程已启动
    std::atomic<int> second_batch_submitted{0};  // 统计第二批成功提交数量
    std::thread submitter([&] {  // 启动被暂停阻塞的提交线程
        submitter_started.count_down();  // 通知提交线程已开始
        for (int index = 0; index < 100; ++index) {  // 尝试提交第二批任务
            pool.submit([&counter] {  // 提交短耗时任务
                std::this_thread::sleep_for(std::chrono::milliseconds(2));  // 模拟任务耗时
                counter.fetch_add(1, std::memory_order_relaxed);  // 增加完成计数
            });  // 结束任务提交
            second_batch_submitted.fetch_add(1, std::memory_order_relaxed);  // 记录提交成功
        }  // 结束第二批提交循环
    });  // 结束提交线程创建

    submitter_started.wait();  // 等待提交线程进入提交路径
    std::this_thread::sleep_for(std::chrono::milliseconds(30));  // 留出暂停阻塞观察时间
    EXPECT_EQ(second_batch_submitted.load(std::memory_order_relaxed), 0);  // 断言暂停期间第二批未入队

    const ThreadPool::Stats paused_stats = pool.snapshot_stats();  // 获取暂停状态快照
    EXPECT_TRUE(paused_stats.paused);  // 断言线程池处于暂停状态
    EXPECT_TRUE(paused_stats.accepting);  // 断言暂停时仍接受恢复后的提交

    pool.resume();  // 恢复提交
    submitter.join();  // 等待提交线程结束
    pool.await_termination();  // 等待所有任务完成并关闭

    EXPECT_EQ(second_batch_submitted.load(std::memory_order_relaxed), 100);  // 断言第二批全部提交
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 200);  // 断言两批任务全部执行
}  // 结束 test_day52_pause_resume_and_await_termination

BenchmarkResult run_blocking_queue_benchmark() {  // 运行阻塞队列吞吐基准
    BlockingQueue<int> queue;  // 创建整数阻塞队列

    const double elapsed_ms = measure_ms([&] {  // 测量单生产者单消费者耗时
        std::thread producer([&] {  // 启动生产者线程
            for (int value = 0; value < kQueueBenchmarkCount; ++value) {  // 产生连续整数
                queue.push(value);  // 写入队列
            }  // 结束生产循环
        });  // 结束生产者创建

        std::thread consumer([&] {  // 启动消费者线程
            for (int expected = 0; expected < kQueueBenchmarkCount; ++expected) {  // 消费所有整数
                const int value = queue.pop();  // 从队列取出一个值
                EXPECT_EQ(value, expected);  // 断言 FIFO 顺序正确
            }  // 结束消费循环
        });  // 结束消费者创建

        producer.join();  // 等待生产者结束
        consumer.join();  // 等待消费者结束
    });  // 结束耗时测量

    return BenchmarkResult{"blocking_queue_1p1c",  // 返回队列基准名称
                           elapsed_ms,  // 返回基准耗时
                           kQueueBenchmarkCount / (elapsed_ms / 1000.0)};  // 计算并返回 QPS
}  // 结束 run_blocking_queue_benchmark

LatencyBenchmarkResult run_thread_pool_latency_benchmark(std::size_t worker_count,  // 运行线程池提交延迟基准
                                                         int sample_count) {  // 接收采样数量
    ThreadPool pool(worker_count);  // 创建指定线程数线程池
    std::vector<double> latencies;  // 保存每个任务启动延迟
    latencies.reserve(static_cast<std::size_t>(sample_count));  // 预留延迟样本容量
    std::mutex latency_mutex;  // 保护延迟样本容器

    for (int index = 0; index < sample_count; ++index) {  // 提交延迟采样任务
        const auto submitted_at = Clock::now();  // 记录提交时间
        pool.submit([submitted_at, &latencies, &latency_mutex] {  // 提交记录启动延迟的任务
            const auto started_at = Clock::now();  // 记录实际开始执行时间
            const double latency_us = micros_between(submitted_at, started_at);  // 计算提交到执行延迟
            std::lock_guard<std::mutex> lock(latency_mutex);  // 加锁保护样本写入
            latencies.push_back(latency_us);  // 保存延迟样本
        });  // 结束延迟采样任务提交
    }  // 结束延迟任务提交循环

    pool.await_termination();  // 等待所有采样任务完成
    std::sort(latencies.begin(), latencies.end());  // 排序样本以计算分位数

    return LatencyBenchmarkResult{percentile_us(latencies, 0.50),  // 返回 p50 延迟
                                  percentile_us(latencies, 0.99)};  // 返回 p99 延迟
}  // 结束 run_thread_pool_latency_benchmark

}  // 结束匿名命名空间

int main() {  // 程序入口
    try {  // 捕获测试失败异常
        test_day50_rw_mutex();  // 运行读写锁测试
        test_day51_countdown_latch();  // 运行倒计时同步器测试
        test_day36_basic_thread_pool();  // 运行基础线程池测试
        test_day37_future_submit();  // 运行 future 提交测试
        test_day38_await_termination();  // 运行等待终止测试
        test_day39_dynamic_scaling();  // 运行动态扩容测试
        test_day41_stats_improvement();  // 运行统计信息测试
        test_day52_pause_resume_and_await_termination();  // 运行暂停恢复测试

        const BenchmarkResult blocking_queue = run_blocking_queue_benchmark();  // 执行阻塞队列基准
        const BenchmarkResult single_thread = run_single_thread_benchmark();  // 执行单线程基线基准
        const BenchmarkResult pool4 = run_thread_pool_benchmark(4);  // 执行四线程线程池基准
        const BenchmarkResult pool8 = run_thread_pool_benchmark(8);  // 执行八线程线程池基准
        const LatencyBenchmarkResult latency4 = run_thread_pool_latency_benchmark(4, 2000);  // 执行四线程延迟基准

        std::cout << std::fixed << std::setprecision(3);  // 设置小数输出格式
        std::cout << "All Week 06-08 thread pool tests passed.\n";  // 输出测试通过信息
        std::cout << "Day 54 benchmark summary\n";  // 输出 Day 54 基准标题
        std::cout << "  " << blocking_queue.name << ": " << blocking_queue.elapsed_ms  // 输出队列基准耗时
                  << " ms, QPS=" << blocking_queue.qps << "\n";  // 输出队列基准吞吐
        std::cout << "  single_task_submit_latency_4_threads: p50=" << latency4.p50_us  // 输出 p50 延迟
                  << " us, p99=" << latency4.p99_us << " us\n";  // 输出 p99 延迟
        std::cout << "Day 40 benchmark (" << kBenchmarkTaskCount << " light tasks)\n";  // 输出 Day 40 基准标题
        std::cout << "  " << single_thread.name << ": " << single_thread.elapsed_ms  // 输出单线程基准耗时
                  << " ms, QPS=" << single_thread.qps << "\n";  // 输出单线程基准吞吐
        std::cout << "  " << pool4.name << ": " << pool4.elapsed_ms << " ms, QPS="  // 输出四线程基准耗时
                  << pool4.qps << "\n";  // 输出四线程基准吞吐
        std::cout << "  " << pool8.name << ": " << pool8.elapsed_ms << " ms, QPS="  // 输出八线程基准耗时
                  << pool8.qps << "\n";  // 输出八线程基准吞吐
        return 0;  // 返回成功退出码
    } catch (const std::exception& ex) {  // 捕获测试或基准异常
        std::cerr << "Week 06 validation failed: " << ex.what() << "\n";  // 输出失败原因
        return 1;  // 返回失败退出码
    }  // 结束异常处理
}  // 结束 main