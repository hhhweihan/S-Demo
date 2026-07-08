#include "concurrent/aba_demo.h"  // 引入 ABA 演示实验
#include "concurrent/blocking_queue.h"  // 引入无界阻塞队列
#include "concurrent/bounded_queue.h"  // 引入有界阻塞队列
#include "concurrent/lock_free_stack.h"  // 引入无锁栈实现
#include "concurrent/memory_order_experiments.h"  // 引入内存序实验
#include "concurrent/spsc_queue.h"  // 引入单生产者单消费者队列

#include <atomic>  // 使用原子变量
#include <chrono>  // 使用计时工具
#include <cstddef>  // 使用 std::size_t
#include <cstdint>  // 使用固定宽度整数
#include <exception>  // 捕获标准异常
#include <iomanip>  // 格式化输出宽度和精度
#include <iostream>  // 使用标准输入输出流
#include <queue>  // 使用标准队列作为基线
#include <sstream>  // 构造摘要字符串
#include <stdexcept>  // 抛出运行时异常
#include <string>  // 使用字符串
#include <thread>  // 使用线程压测
#include <vector>  // 使用动态数组

#define EXPECT_TRUE(cond) /* 断言条件为真 */                                               \
    do { /* 创建单次执行作用域 */                                                          \
        if (!(cond)) { /* 检查断言是否失败 */                                              \
            throw std::runtime_error("EXPECT_TRUE failed: " #cond); /* 抛出断言失败异常 */ \
        } /* 结束失败分支 */                                                               \
    } while (false)  // 让宏表现为单条语句

#define EXPECT_EQ(lhs, rhs) EXPECT_TRUE((lhs) == (rhs))  // 断言两个表达式相等

namespace {  // 限定本文件内部链接

using Clock = std::chrono::steady_clock;  // 定义单调计时时钟别名
constexpr int kExchangeCount = 1'000'000;  // 队列交接测试次数
constexpr std::size_t kPublicationIterations = 2'000;  // 内存序实验迭代次数
constexpr int kStackStressProducers = 4;  // 栈压测生产者数量
constexpr int kStackStressConsumers = 4;  // 栈压测消费者数量
constexpr int kStackStressValuesPerProducer = 50'000;  // 每个生产者压测写入数量
constexpr int kStackBenchmarkValuesPerProducer = 100'000;  // 每个生产者 benchmark 写入数量

struct BenchmarkResult {  // benchmark 输出结果
    std::string name;  // benchmark 名称
    double elapsed_ms = 0.0;  // 总耗时毫秒
    double throughput = 0.0;  // 每秒操作数
};  // 结束 BenchmarkResult

template <typename Fn>  // 可调用对象类型模板
double measure_ms(Fn&& fn) {  // 计量函数执行耗时
    const auto begin = Clock::now();  // 记录开始时间
    fn();  // 执行被测逻辑
    const auto end = Clock::now();  // 记录结束时间
    return std::chrono::duration<double, std::milli>(end - begin).count();  // 返回毫秒耗时
}  // 结束 measure_ms

void test_day43_memory_order(PublicationExperimentResult& result) {  // 验证发布内存序实验
    result = memory_order_experiments::run_publication_experiment(kPublicationIterations);  // 运行发布实验
    EXPECT_EQ(result.release_acquire_mismatches, static_cast<std::size_t>(0));  // release/acquire 不应错配
}  // 结束 test_day43_memory_order

void test_day44_lock_free_stack() {  // 验证基础无锁栈单线程行为
    LockFreeStack<int> stack;  // 创建整型无锁栈
    stack.push(1);  // 压入第一个值
    stack.push(2);  // 压入第二个值
    stack.push(3);  // 压入第三个值

    int value = 0;  // 接收弹出值
    EXPECT_TRUE(stack.pop(value));  // 弹出栈顶值
    EXPECT_EQ(value, 3);  // 验证后进先出顺序
    EXPECT_TRUE(stack.pop(value));  // 弹出第二个值
    EXPECT_EQ(value, 2);  // 验证第二个弹出值
    EXPECT_TRUE(stack.pop(value));  // 弹出第三个值
    EXPECT_EQ(value, 1);  // 验证最后一个弹出值
    EXPECT_TRUE(!stack.pop(value));  // 空栈应弹出失败
}  // 结束 test_day44_lock_free_stack

void test_day44_lock_free_stack_parallel_push() {  // 验证多生产者并发压栈
    LockFreeStack<int> stack;  // 创建共享无锁栈
    const int total_values = kStackStressProducers * kStackStressValuesPerProducer;  // 计算总写入数量

    std::vector<std::thread> producers;  // 保存生产者线程
    producers.reserve(kStackStressProducers);  // 预留线程数组容量

    for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {  // 创建每个生产者
        producers.emplace_back([&, producer_id] {  // 启动生产者线程
            const int begin = producer_id * kStackStressValuesPerProducer;  // 计算当前线程起始值
            const int end = begin + kStackStressValuesPerProducer;  // 计算当前线程结束值
            for (int value = begin; value < end; ++value) {  // 写入分配给该线程的值
                stack.push(value);  // 并发压入栈
            }  // 结束当前线程写入循环
        });  // 结束生产者线程创建
    }  // 结束生产者创建循环

    for (auto& producer : producers) {  // 遍历生产者线程
        producer.join();  // 等待生产者结束
    }  // 结束生产者等待

    std::vector<unsigned char> seen(static_cast<std::size_t>(total_values), 0);  // 标记每个值是否出现
    int popped_count = 0;  // 已弹出数量
    while (popped_count < total_values) {  // 弹出直到取完所有值
        int value = -1;  // 接收弹出值
        EXPECT_TRUE(stack.pop(value));  // 弹出一个值
        EXPECT_TRUE(value >= 0 && value < total_values);  // 验证值范围合法
        EXPECT_TRUE(seen[static_cast<std::size_t>(value)] == 0);  // 验证没有重复弹出
        seen[static_cast<std::size_t>(value)] = 1;  // 标记该值已出现
        ++popped_count;  // 累加弹出数量
    }  // 结束弹出验证循环

    EXPECT_TRUE(stack.empty());  // 所有值弹出后栈应为空
}  // 结束 test_day44_lock_free_stack_parallel_push

void test_day48_reclaiming_lock_free_stack() {  // 验证带回收无锁栈单线程行为
    ReclaimingLockFreeStack<int> stack;  // 创建带回收无锁栈
    stack.push(10);  // 压入第一个值
    stack.push(20);  // 压入第二个值
    stack.push(30);  // 压入第三个值

    std::shared_ptr<int> value = stack.pop();  // 弹出栈顶共享数据
    EXPECT_TRUE(static_cast<bool>(value));  // 验证弹出结果非空
    EXPECT_EQ(*value, 30);  // 验证第一个弹出值

    value = stack.pop();  // 继续弹出第二个值
    EXPECT_TRUE(static_cast<bool>(value));  // 验证弹出结果非空
    EXPECT_EQ(*value, 20);  // 验证第二个弹出值

    value = stack.pop();  // 继续弹出第三个值
    EXPECT_TRUE(static_cast<bool>(value));  // 验证弹出结果非空
    EXPECT_EQ(*value, 10);  // 验证第三个弹出值

    value = stack.pop();  // 尝试从空栈弹出
    EXPECT_TRUE(!value);  // 空栈应返回空指针
    EXPECT_TRUE(stack.empty());  // 栈应保持为空
}  // 结束 test_day48_reclaiming_lock_free_stack

void test_day46_tagged_pointer_stack() {  // 验证带标签指针栈
    TaggedPointerStack<int> stack;  // 创建带标签栈
    stack.push(7);  // 压入第一个值
    stack.push(8);  // 压入第二个值
    stack.push(9);  // 压入第三个值

    int value = 0;  // 接收弹出值
    EXPECT_TRUE(stack.pop(value));  // 弹出栈顶值
    EXPECT_EQ(value, 9);  // 验证第一个弹出值
    EXPECT_TRUE(stack.pop(value));  // 弹出第二个值
    EXPECT_EQ(value, 8);  // 验证第二个弹出值
    EXPECT_TRUE(stack.pop(value));  // 弹出第三个值
    EXPECT_EQ(value, 7);  // 验证第三个弹出值
    EXPECT_TRUE(!stack.pop(value));  // 空栈应弹出失败
    EXPECT_TRUE(stack.empty());  // 栈应为空
}  // 结束 test_day46_tagged_pointer_stack

void test_day48_reclaiming_lock_free_stack_parallel() {  // 验证带回收栈多生产多消费
    ReclaimingLockFreeStack<int> stack;  // 创建共享带回收栈
    const int total_values = kStackStressProducers * kStackStressValuesPerProducer;  // 计算总数据量
    std::vector<std::atomic<int>> seen(static_cast<std::size_t>(total_values));  // 记录每个值出现次数
    for (auto& slot : seen) {  // 初始化标记数组
        slot.store(0, std::memory_order_relaxed);  // 清零每个槽位
    }  // 结束标记初始化

    std::atomic<int> producers_done{0};  // 已完成生产者数量
    std::atomic<int> consumed{0};  // 已消费元素数量
    std::atomic<bool> valid{true};  // 并发校验是否仍有效

    std::vector<std::thread> producers;  // 保存生产者线程
    producers.reserve(kStackStressProducers);  // 预留生产者容量
    for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {  // 创建生产者
        producers.emplace_back([&, producer_id] {  // 启动生产者线程
            const int begin = producer_id * kStackStressValuesPerProducer;  // 计算起始值
            const int end = begin + kStackStressValuesPerProducer;  // 计算结束值
            for (int value = begin; value < end; ++value) {  // 遍历该线程负责的值
                stack.push(value);  // 压入共享栈
            }  // 结束写入循环
            producers_done.fetch_add(1, std::memory_order_release);  // 标记该生产者完成
        });  // 结束生产者线程创建
    }  // 结束生产者创建循环

    std::vector<std::thread> consumers;  // 保存消费者线程
    consumers.reserve(kStackStressConsumers);  // 预留消费者容量
    for (int consumer_id = 0; consumer_id < kStackStressConsumers; ++consumer_id) {  // 创建消费者
        consumers.emplace_back([&] {  // 启动消费者线程
            while (true) {  // 持续消费直到完成
                if (consumed.load(std::memory_order_acquire) >= total_values) {  // 检查是否已消费足量
                    break;  // 满足总量后退出
                }  // 结束总量检查

                std::shared_ptr<int> value = stack.pop();  // 尝试弹出一个值
                if (!value) {  // 当前栈为空时处理
                    if (producers_done.load(std::memory_order_acquire) == kStackStressProducers &&  // 所有生产者已结束
                        consumed.load(std::memory_order_acquire) >= total_values) {  // 且消费量已达标
                        break;  // 可以退出消费循环
                    }  // 结束完成条件检查
                    std::this_thread::yield();  // 让出时间片等待生产
                    continue;  // 继续尝试弹出
                }  // 结束空弹出处理

                const int index = *value;  // 读取弹出的值作为索引
                if (index < 0 || index >= total_values) {  // 验证索引范围
                    valid.store(false, std::memory_order_relaxed);  // 标记结果非法
                    continue;  // 跳过非法值
                }  // 结束范围检查

                if (seen[static_cast<std::size_t>(index)].fetch_add(1, std::memory_order_relaxed) != 0) {  // 检查重复消费
                    valid.store(false, std::memory_order_relaxed);  // 标记出现重复值
                }  // 结束重复检查
                consumed.fetch_add(1, std::memory_order_release);  // 累加消费数量
            }  // 结束消费循环
        });  // 结束消费者线程创建
    }  // 结束消费者创建循环

    for (auto& producer : producers) {  // 遍历生产者线程
        producer.join();  // 等待生产者完成
    }  // 结束生产者等待
    for (auto& consumer : consumers) {  // 遍历消费者线程
        consumer.join();  // 等待消费者完成
    }  // 结束消费者等待

    EXPECT_TRUE(valid.load(std::memory_order_relaxed));  // 验证没有范围或重复错误
    EXPECT_EQ(consumed.load(std::memory_order_acquire), total_values);  // 验证消费总数正确
    EXPECT_TRUE(stack.empty());  // 验证最终栈为空
}  // 结束 test_day48_reclaiming_lock_free_stack_parallel

void test_day45_spsc_queue() {  // 验证 SPSC 队列顺序
    SPSCQueue<int> queue(1024);  // 创建容量为 1024 的队列
    std::atomic<bool> order_ok{true};  // 记录顺序是否正确

    std::thread producer([&] {  // 创建单生产者线程
        for (int value = 0; value < kExchangeCount; ++value) {  // 按序生产整数
            while (!queue.push(value)) {  // 队列满时持续重试
                std::this_thread::yield();  // 让出时间片
            }  // 结束入队重试
        }  // 结束生产循环
    });  // 结束生产者创建

    std::thread consumer([&] {  // 创建单消费者线程
        for (int expected = 0; expected < kExchangeCount; ++expected) {  // 按序检查消费值
            int actual = -1;  // 接收实际弹出值
            while (!queue.pop(actual)) {  // 队列空时持续重试
                std::this_thread::yield();  // 让出时间片
            }  // 结束出队重试
            if (actual != expected) {  // 检查顺序是否匹配
                order_ok.store(false, std::memory_order_relaxed);  // 标记顺序错误
            }  // 结束顺序检查
        }  // 结束消费循环
    });  // 结束消费者创建

    producer.join();  // 等待生产者结束
    consumer.join();  // 等待消费者结束

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));  // 验证顺序始终正确
    EXPECT_TRUE(queue.empty());  // 验证队列最终为空
}  // 结束 test_day45_spsc_queue

void test_day46_aba_defense() {  // 验证带标签 CAS 防 ABA 效果
    const ABAExperimentResult result = aba_demo::run_aba_compare_demo();  // 运行 ABA 对比实验
    EXPECT_TRUE(result.plain_head_cas_succeeded);  // 普通 CAS 会被 ABA 欺骗
    EXPECT_TRUE(!result.tagged_head_cas_succeeded);  // 带标签 CAS 应检测到变化
    EXPECT_EQ(result.final_plain_value, static_cast<std::uint32_t>(3));  // 验证普通头最终被改写
    EXPECT_EQ(result.final_tagged_index, static_cast<std::uint32_t>(1));  // 验证带标签索引保持原值
    EXPECT_EQ(result.final_tagged_tag, static_cast<std::uint32_t>(2));  // 验证带标签版本已递增
}  // 结束 test_day46_aba_defense

BenchmarkResult run_std_queue_baseline() {  // 运行 std::queue 单线程基线
    long long checksum = 0;  // 累加校验和防止循环被优化
    const double elapsed_ms = measure_ms([&] {  // 计量基线耗时
        std::queue<int> queue;  // 创建标准队列
        for (int value = 0; value < kExchangeCount; ++value) {  // 循环执行入队出队
            queue.push(value);  // 将值入队
            checksum += queue.front();  // 读取队首并累加
            queue.pop();  // 移除队首
        }  // 结束基线循环
    });  // 结束耗时计量

    const long long expected =  // 计算等差数列期望和
        static_cast<long long>(kExchangeCount - 1) * static_cast<long long>(kExchangeCount) / 2;  // 求 0 到 n-1 之和
    EXPECT_EQ(checksum, expected);  // 验证校验和正确
    return BenchmarkResult{"std::queue_baseline",  // 返回基线 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           kExchangeCount / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_std_queue_baseline

BenchmarkResult run_blocking_queue_benchmark() {  // 运行无界阻塞队列 benchmark
    BlockingQueue<int> queue;  // 创建阻塞队列
    std::atomic<bool> order_ok{true};  // 记录消费顺序是否正确

    const double elapsed_ms = measure_ms([&] {  // 计量生产消费耗时
        std::thread producer([&] {  // 创建生产者线程
            for (int value = 0; value < kExchangeCount; ++value) {  // 按序生产数据
                queue.push(value);  // 写入阻塞队列
            }  // 结束生产循环
        });  // 结束生产者创建

        std::thread consumer([&] {  // 创建消费者线程
            for (int expected = 0; expected < kExchangeCount; ++expected) {  // 按序消费数据
                const int actual = queue.pop();  // 阻塞弹出一个值
                if (actual != expected) {  // 检查顺序是否匹配
                    order_ok.store(false, std::memory_order_relaxed);  // 标记顺序错误
                }  // 结束顺序检查
            }  // 结束消费循环
        });  // 结束消费者创建

        producer.join();  // 等待生产者结束
        consumer.join();  // 等待消费者结束
    });  // 结束耗时计量

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));  // 验证消费顺序正确
    return BenchmarkResult{"BlockingQueue_1P1C",  // 返回 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           kExchangeCount / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_blocking_queue_benchmark

BenchmarkResult run_bounded_queue_benchmark() {  // 运行有界阻塞队列 benchmark
    BoundedQueue<int> queue(1024);  // 创建容量为 1024 的有界队列
    std::atomic<bool> order_ok{true};  // 记录消费顺序是否正确

    const double elapsed_ms = measure_ms([&] {  // 计量生产消费耗时
        std::thread producer([&] {  // 创建生产者线程
            for (int value = 0; value < kExchangeCount; ++value) {  // 按序生产数据
                queue.push(value);  // 阻塞写入队列
            }  // 结束生产循环
        });  // 结束生产者创建

        std::thread consumer([&] {  // 创建消费者线程
            for (int expected = 0; expected < kExchangeCount; ++expected) {  // 按序消费数据
                const int actual = queue.pop();  // 阻塞弹出一个值
                if (actual != expected) {  // 检查顺序是否匹配
                    order_ok.store(false, std::memory_order_relaxed);  // 标记顺序错误
                }  // 结束顺序检查
            }  // 结束消费循环
        });  // 结束消费者创建

        producer.join();  // 等待生产者结束
        consumer.join();  // 等待消费者结束
    });  // 结束耗时计量

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));  // 验证消费顺序正确
    return BenchmarkResult{"BoundedQueue_1P1C_cap1024",  // 返回 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           kExchangeCount / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_bounded_queue_benchmark

BenchmarkResult run_spsc_benchmark() {  // 运行 SPSC 队列 benchmark
    SPSCQueue<int> queue(1024);  // 创建容量为 1024 的 SPSC 队列
    std::atomic<bool> order_ok{true};  // 记录消费顺序是否正确

    const double elapsed_ms = measure_ms([&] {  // 计量生产消费耗时
        std::thread producer([&] {  // 创建生产者线程
            for (int value = 0; value < kExchangeCount; ++value) {  // 按序生产数据
                while (!queue.push(value)) {  // 满队列时重试
                    std::this_thread::yield();  // 让出时间片
                }  // 结束入队重试
            }  // 结束生产循环
        });  // 结束生产者创建

        std::thread consumer([&] {  // 创建消费者线程
            for (int expected = 0; expected < kExchangeCount; ++expected) {  // 按序消费数据
                int actual = -1;  // 接收实际值
                while (!queue.pop(actual)) {  // 空队列时重试
                    std::this_thread::yield();  // 让出时间片
                }  // 结束出队重试
                if (actual != expected) {  // 检查顺序是否匹配
                    order_ok.store(false, std::memory_order_relaxed);  // 标记顺序错误
                }  // 结束顺序检查
            }  // 结束消费循环
        });  // 结束消费者创建

        producer.join();  // 等待生产者结束
        consumer.join();  // 等待消费者结束
    });  // 结束耗时计量

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));  // 验证消费顺序正确
    EXPECT_TRUE(queue.empty());  // 验证队列最终为空
    return BenchmarkResult{"SPSCQueue_1P1C_cap1024",  // 返回 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           kExchangeCount / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_spsc_benchmark

BenchmarkResult run_lock_free_stack_push_drain_benchmark() {  // 运行基础无锁栈压入后排空 benchmark
    LockFreeStack<int> stack;  // 创建基础无锁栈
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;  // 计算总元素数

    const double elapsed_ms = measure_ms([&] {  // 计量压入和排空耗时
        std::vector<std::thread> producers;  // 保存生产者线程
        producers.reserve(kStackStressProducers);  // 预留生产者容量

        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {  // 创建生产者
            producers.emplace_back([&, producer_id] {  // 启动生产者线程
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;  // 计算起始值
                const int end = begin + kStackBenchmarkValuesPerProducer;  // 计算结束值
                for (int value = begin; value < end; ++value) {  // 遍历当前线程数据
                    stack.push(value);  // 压入共享栈
                }  // 结束压入循环
            });  // 结束生产者线程创建
        }  // 结束生产者创建循环

        for (auto& producer : producers) {  // 遍历生产者线程
            producer.join();  // 等待生产者结束
        }  // 结束生产者等待

        int popped = 0;  // 已弹出计数
        int value = 0;  // 接收弹出值
        while (popped < total_values) {  // 排空所有元素
            EXPECT_TRUE(stack.pop(value));  // 弹出一个元素
            ++popped;  // 累加弹出数量
        }  // 结束排空循环
    });  // 结束耗时计量

    return BenchmarkResult{"LockFreeStack_4P1C_push_drain",  // 返回 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           total_values / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_lock_free_stack_push_drain_benchmark

BenchmarkResult run_reclaiming_stack_push_drain_benchmark() {  // 运行带回收栈压入后排空 benchmark
    ReclaimingLockFreeStack<int> stack;  // 创建带回收无锁栈
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;  // 计算总元素数

    const double elapsed_ms = measure_ms([&] {  // 计量压入和排空耗时
        std::vector<std::thread> producers;  // 保存生产者线程
        producers.reserve(kStackStressProducers);  // 预留生产者容量

        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {  // 创建生产者
            producers.emplace_back([&, producer_id] {  // 启动生产者线程
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;  // 计算起始值
                const int end = begin + kStackBenchmarkValuesPerProducer;  // 计算结束值
                for (int value = begin; value < end; ++value) {  // 遍历当前线程数据
                    stack.push(value);  // 压入共享栈
                }  // 结束压入循环
            });  // 结束生产者线程创建
        }  // 结束生产者创建循环

        for (auto& producer : producers) {  // 遍历生产者线程
            producer.join();  // 等待生产者结束
        }  // 结束生产者等待

        int popped = 0;  // 已弹出计数
        while (popped < total_values) {  // 排空所有元素
            std::shared_ptr<int> value = stack.pop();  // 弹出共享数据
            EXPECT_TRUE(static_cast<bool>(value));  // 验证弹出成功
            ++popped;  // 累加弹出数量
        }  // 结束排空循环
    });  // 结束耗时计量

    return BenchmarkResult{"ReclaimStack_4P1C_push_drain",  // 返回 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           total_values / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_reclaiming_stack_push_drain_benchmark

BenchmarkResult run_tagged_pointer_stack_push_drain_benchmark() {  // 运行带标签栈压入后排空 benchmark
    TaggedPointerStack<int> stack;  // 创建带标签指针栈
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;  // 计算总元素数

    const double elapsed_ms = measure_ms([&] {  // 计量压入和排空耗时
        std::vector<std::thread> producers;  // 保存生产者线程
        producers.reserve(kStackStressProducers);  // 预留生产者容量

        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {  // 创建生产者
            producers.emplace_back([&, producer_id] {  // 启动生产者线程
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;  // 计算起始值
                const int end = begin + kStackBenchmarkValuesPerProducer;  // 计算结束值
                for (int value = begin; value < end; ++value) {  // 遍历当前线程数据
                    stack.push(value);  // 压入共享栈
                }  // 结束压入循环
            });  // 结束生产者线程创建
        }  // 结束生产者创建循环

        for (auto& producer : producers) {  // 遍历生产者线程
            producer.join();  // 等待生产者结束
        }  // 结束生产者等待

        int popped = 0;  // 已弹出计数
        int value = 0;  // 接收弹出值
        while (popped < total_values) {  // 排空所有元素
            EXPECT_TRUE(stack.pop(value));  // 弹出一个元素
            ++popped;  // 累加弹出数量
        }  // 结束排空循环
    });  // 结束耗时计量

    return BenchmarkResult{"TaggedStack_4P1C_push_drain",  // 返回 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           total_values / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_tagged_pointer_stack_push_drain_benchmark

BenchmarkResult run_reclaiming_stack_mpmc_benchmark() {  // 运行带回收栈多生产多消费 benchmark
    ReclaimingLockFreeStack<int> stack;  // 创建共享带回收栈
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;  // 计算总元素数
    std::atomic<int> producers_done{0};  // 已完成生产者数量
    std::atomic<int> consumed{0};  // 已消费元素数量

    const double elapsed_ms = measure_ms([&] {  // 计量并发压入弹出耗时
        std::vector<std::thread> producers;  // 保存生产者线程
        producers.reserve(kStackStressProducers);  // 预留生产者容量
        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {  // 创建生产者
            producers.emplace_back([&, producer_id] {  // 启动生产者线程
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;  // 计算起始值
                const int end = begin + kStackBenchmarkValuesPerProducer;  // 计算结束值
                for (int value = begin; value < end; ++value) {  // 遍历当前线程数据
                    stack.push(value);  // 压入共享栈
                }  // 结束压入循环
                producers_done.fetch_add(1, std::memory_order_release);  // 标记生产者完成
            });  // 结束生产者线程创建
        }  // 结束生产者创建循环

        std::vector<std::thread> consumers;  // 保存消费者线程
        consumers.reserve(kStackStressConsumers);  // 预留消费者容量
        for (int consumer_id = 0; consumer_id < kStackStressConsumers; ++consumer_id) {  // 创建消费者
            consumers.emplace_back([&] {  // 启动消费者线程
                while (consumed.load(std::memory_order_acquire) < total_values) {  // 未消费足量时循环
                    std::shared_ptr<int> value = stack.pop();  // 尝试弹出共享数据
                    if (value) {  // 成功弹出时处理
                        consumed.fetch_add(1, std::memory_order_release);  // 累加消费数量
                        continue;  // 继续下一次消费
                    }  // 结束成功弹出处理

                    if (producers_done.load(std::memory_order_acquire) == kStackStressProducers &&  // 所有生产者完成
                        consumed.load(std::memory_order_acquire) >= total_values) {  // 且消费数量达标
                        break;  // 退出消费循环
                    }  // 结束完成条件检查
                    std::this_thread::yield();  // 当前无数据时让出时间片
                }  // 结束消费者循环
            });  // 结束消费者线程创建
        }  // 结束消费者创建循环

        for (auto& producer : producers) {  // 遍历生产者线程
            producer.join();  // 等待生产者结束
        }  // 结束生产者等待
        for (auto& consumer : consumers) {  // 遍历消费者线程
            consumer.join();  // 等待消费者结束
        }  // 结束消费者等待
    });  // 结束耗时计量

    EXPECT_EQ(consumed.load(std::memory_order_acquire), total_values);  // 验证消费总量正确
    return BenchmarkResult{"ReclaimStack_4P4C_push_pop",  // 返回 benchmark 名称
                           elapsed_ms,  // 返回耗时
                           total_values / (elapsed_ms / 1000.0)};  // 计算吞吐量
}  // 结束 run_reclaiming_stack_mpmc_benchmark

std::string format_publication_summary(const PublicationExperimentResult& result) {  // 格式化内存序实验摘要
    std::ostringstream out;  // 创建字符串输出流
    out << "Day 43 publication experiment (iterations=" << result.iterations << ")\n";  // 输出实验次数
    out << "  relaxed mismatches: " << result.relaxed_mismatches << "\n";  // 输出 relaxed 错配次数
    out << "  release/acquire mismatches: " << result.release_acquire_mismatches << "\n";  // 输出同步错配次数
    out << "  note: relaxed mismatch may remain 0 on x86, but the ordering is still not guaranteed by the model.\n";  // 输出模型说明
    return out.str();  // 返回摘要字符串
}  // 结束 format_publication_summary

void print_benchmark(const BenchmarkResult& result) {  // 打印单条 benchmark 结果
    std::cout << "  " << std::setw(28) << std::left << result.name << " "  // 输出名称并左对齐
              << std::setw(10) << std::right << result.elapsed_ms << " ms"  // 输出耗时并右对齐
              << "  throughput=" << result.throughput << " ops/s\n";  // 输出吞吐量
}  // 结束 print_benchmark

}  // 结束匿名命名空间

int main() {  // 程序入口
    try {  // 捕获验证过程中的异常
        PublicationExperimentResult publication_result;  // 保存发布实验结果
        test_day43_memory_order(publication_result);  // 运行内存序测试
        test_day44_lock_free_stack();  // 运行基础无锁栈测试
        test_day44_lock_free_stack_parallel_push();  // 运行并发压栈测试
        test_day45_spsc_queue();  // 运行 SPSC 队列测试
        test_day46_aba_defense();  // 运行 ABA 防护测试
        test_day46_tagged_pointer_stack();  // 运行带标签栈测试
        test_day48_reclaiming_lock_free_stack();  // 运行带回收栈单线程测试
        test_day48_reclaiming_lock_free_stack_parallel();  // 运行带回收栈并发测试

        const BenchmarkResult baseline = run_std_queue_baseline();  // 运行标准队列基线
        const BenchmarkResult blocking = run_blocking_queue_benchmark();  // 运行阻塞队列 benchmark
        const BenchmarkResult bounded = run_bounded_queue_benchmark();  // 运行有界队列 benchmark
        const BenchmarkResult spsc = run_spsc_benchmark();  // 运行 SPSC benchmark
        const BenchmarkResult lock_free_stack_push_drain = run_lock_free_stack_push_drain_benchmark();  // 运行基础无锁栈 benchmark
        const BenchmarkResult tagged_stack_push_drain = run_tagged_pointer_stack_push_drain_benchmark();  // 运行带标签栈 benchmark
        const BenchmarkResult reclaim_stack_push_drain = run_reclaiming_stack_push_drain_benchmark();  // 运行带回收栈排空 benchmark
        const BenchmarkResult reclaim_stack_mpmc = run_reclaiming_stack_mpmc_benchmark();  // 运行带回收栈 MPMC benchmark

        std::cout << std::fixed << std::setprecision(3);  // 设置浮点输出格式
        std::cout << "Week 07 lock-free practice validation passed.\n";  // 输出验证通过提示
        std::cout << format_publication_summary(publication_result);  // 输出内存序实验摘要
        std::cout << "Stack stress tests\n";  // 输出栈压测标题
        std::cout << "  LockFreeStack skeleton: concurrent push + single-thread pop passed\n";  // 输出基础栈测试结果
        std::cout << "  TaggedPointerStack prototype: single-thread push/pop passed\n";  // 输出带标签栈测试结果
        std::cout << "  ReclaimingLockFreeStack: concurrent push/pop passed\n";  // 输出带回收栈测试结果
        std::cout << "Stack benchmark (" << (kStackStressProducers * kStackBenchmarkValuesPerProducer)  // 输出栈 benchmark 标题前半
              << " values)\n";  // 输出栈 benchmark 标题后半
        print_benchmark(lock_free_stack_push_drain);  // 打印基础无锁栈 benchmark
        print_benchmark(tagged_stack_push_drain);  // 打印带标签栈 benchmark
        print_benchmark(reclaim_stack_push_drain);  // 打印带回收栈排空 benchmark
        print_benchmark(reclaim_stack_mpmc);  // 打印带回收栈 MPMC benchmark
        std::cout << "Day 47 / Day 49 benchmark (" << kExchangeCount << " int handoffs, 1P1C)\n";  // 输出队列 benchmark 标题
        print_benchmark(baseline);  // 打印标准队列基线
        print_benchmark(blocking);  // 打印阻塞队列 benchmark
        print_benchmark(bounded);  // 打印有界队列 benchmark
        print_benchmark(spsc);  // 打印 SPSC benchmark
        return 0;  // 正常退出
    } catch (const std::exception& ex) {  // 捕获标准异常
        std::cerr << "Week 07 validation failed: " << ex.what() << "\n";  // 输出失败原因
        return 1;  // 异常退出
    }  // 结束异常处理
}  // 结束 main
