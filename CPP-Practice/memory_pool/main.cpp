#include "fixed_allocator.h"  // 引入固定块分配器测试对象。
#include "my_allocator.h"  // 引入 STL allocator 适配器。
#include "pool_allocator.h"  // 引入多 size class 池分配器。

#include <cassert>  // 保留断言相关标准头。
#include <atomic>  // 多线程测试和基准使用原子计数。
#include <chrono>  // 基准测试计时。
#include <cstddef>  // 使用 std::size_t。
#include <cstdlib>  // 使用 std::malloc 和 std::free。
#include <iomanip>  // 控制性能输出格式。
#include <iostream>  // 输出测试和基准结果。
#include <random>  // 随机分配释放测试。
#include <stdexcept>  // 测试宏抛出运行时异常。
#include <string>  // 保存基准名称。
#include <thread>  // 多线程分配测试。
#include <unordered_set>  // 检查活跃指针不重复。
#include <utility>  // 使用 std::move。
#include <vector>  // 保存指针和测试数据。

#define EXPECT_TRUE(cond) /* 检查条件为真，否则抛出异常。 */                               \
    do { /* 用 do-while 包装成单语句宏。 */                                                \
        if (!(cond)) { /* 条件不满足时进入失败路径。 */                                    \
            throw std::runtime_error("EXPECT_TRUE failed: " #cond); /* 抛出带表达式的错误。 */ \
        } /* 失败路径结束。 */                                                             \
    } while (false)  // EXPECT_TRUE 宏结束。

#define EXPECT_EQ(lhs, rhs) EXPECT_TRUE((lhs) == (rhs))  // 检查两个表达式相等。

constexpr int kBenchmarkIterations = 1'000'000;  // 单线程基准总操作次数。
constexpr int kDay12ThreadCount = 4;  // Day12 多线程测试线程数。
constexpr int kDay12IterationsPerThread = 100'000;  // Day12 每线程功能测试次数。
constexpr int kDay12BenchmarkIterationsPerThread = kBenchmarkIterations / kDay12ThreadCount;  // Day12 每线程基准次数。

struct BenchmarkResult {  // 保存一次基准测试结果。
    std::string name;  // 基准名称。
    double elapsed_ms;  // 耗时毫秒数。
};  // BenchmarkResult 定义结束。

template <typename Fn>  // 接收任意可调用对象作为基准主体。
BenchmarkResult run_benchmark(const std::string& name, Fn&& fn) {  // 执行并计时一个基准函数。
    const auto start = std::chrono::high_resolution_clock::now();  // 记录开始时间。
    fn();  // 执行待测逻辑。
    const auto end = std::chrono::high_resolution_clock::now();  // 记录结束时间。
    const auto elapsed = std::chrono::duration<double, std::milli>(end - start).count();  // 转换为毫秒耗时。
    return BenchmarkResult{name, elapsed};  // 返回名称和耗时。
}  // run_benchmark 结束。

void run_day05_benchmark() {  // 运行 Day05 固定块分配器性能基准。
    // 累加写入结果，避免编译器把整个循环优化掉。
    volatile int sink = 0;  // 保存副作用累加值。

    const BenchmarkResult new_delete = run_benchmark("new/delete", [&sink]() {  // 计时 new/delete 路径。
        for (int i = 0; i < kBenchmarkIterations; ++i) {  // 重复执行固定次数。
            int* p = new int;  // 动态分配一个 int。
            *p = 42;  // 写入测试值。
            sink += *p;  // 读取并累加形成副作用。
            delete p;  // 释放 int。
        }  // new/delete 循环结束。
    });  // new/delete 基准结束。

    FixedAllocator allocator;  // 创建固定块分配器。
    // 预留足够大的初始容量，避免 benchmark 测到扩容成本。
    allocator.init(sizeof(int), static_cast<std::size_t>(kBenchmarkIterations));  // 初始化为足够多的 int 块。

    const BenchmarkResult fixed_allocator = run_benchmark("FixedAllocator", [&allocator, &sink]() {  // 计时 FixedAllocator 路径。
        for (int i = 0; i < kBenchmarkIterations; ++i) {  // 重复执行固定次数。
            void* p = allocator.allocate();  // 从固定块池取一个块。
            *static_cast<int*>(p) = 42;  // 把块视为 int 并写入。
            sink += *static_cast<int*>(p);  // 读取并累加形成副作用。
            allocator.deallocate(p);  // 把块归还分配器。
        }  // FixedAllocator 循环结束。
    });  // FixedAllocator 基准结束。

    const BenchmarkResult malloc_free = run_benchmark("malloc/free", [&sink]() {  // 计时 malloc/free 路径。
        for (int i = 0; i < kBenchmarkIterations; ++i) {  // 重复执行固定次数。
            int* p = static_cast<int*>(std::malloc(sizeof(int)));  // 分配一个 int 大小的原始块。
            *p = 42;  // 写入测试值。
            sink += *p;  // 读取并累加形成副作用。
            std::free(p);  // 释放原始块。
        }  // malloc/free 循环结束。
    });  // malloc/free 基准结束。

    const double ratio_vs_new = new_delete.elapsed_ms / fixed_allocator.elapsed_ms;  // 计算相对 new/delete 加速比。
    const double ratio_vs_malloc = malloc_free.elapsed_ms / fixed_allocator.elapsed_ms;  // 计算相对 malloc/free 加速比。

    std::cout << std::fixed << std::setprecision(3);  // 固定输出三位小数。
    std::cout << "Day 005 benchmark (" << kBenchmarkIterations << " iterations)\n";  // 输出基准标题。
    std::cout << "  " << new_delete.name << ": " << new_delete.elapsed_ms << " ms\n";  // 输出 new/delete 耗时。
    std::cout << "  " << fixed_allocator.name << ": " << fixed_allocator.elapsed_ms << " ms\n";  // 输出 FixedAllocator 耗时。
    std::cout << "  " << malloc_free.name << ": " << malloc_free.elapsed_ms << " ms\n";  // 输出 malloc/free 耗时。
    std::cout << "  FixedAllocator speedup vs new/delete: " << ratio_vs_new << "x\n";  // 输出相对 new/delete 加速比。
    std::cout << "  FixedAllocator speedup vs malloc/free: " << ratio_vs_malloc << "x\n";  // 输出相对 malloc/free 加速比。

    if (sink == 0) {  // 防止极端情况下 sink 被认为无用。
        std::cout << "sink=" << sink << "\n";  // 输出 sink 值。
    }  // sink 输出检查结束。
}  // run_day05_benchmark 结束。

void run_day12_benchmark() {  // 运行 Day12 线程安全池分配器性能基准。
    std::atomic<long long> sink{0};  // 多线程共享副作用累加器。

    const BenchmarkResult new_delete = run_benchmark("new/delete", [&sink]() {  // 计时数组 new/delete 路径。
        for (int i = 0; i < kBenchmarkIterations; ++i) {  // 重复执行固定次数。
            char* p = new char[32];  // 分配 32 字节数组。
            p[0] = 42;  // 写入首字节测试值。
            sink.fetch_add(p[0], std::memory_order_relaxed);  // 累加首字节形成副作用。
            delete[] p;  // 释放数组。
        }  // new/delete 数组循环结束。
    });  // new/delete 数组基准结束。

    PoolAllocator single_thread_allocator;  // 创建单线程测试用池分配器。
    const BenchmarkResult pool_single_thread =  // 保存单线程池分配基准结果。
        run_benchmark("PoolAllocator single-thread", [&single_thread_allocator, &sink]() {  // 计时单线程池分配路径。
            for (int i = 0; i < kBenchmarkIterations; ++i) {  // 重复执行固定次数。
                char* p = static_cast<char*>(single_thread_allocator.allocate(32));  // 从池分配 32 字节。
                p[0] = 42;  // 写入首字节测试值。
                sink.fetch_add(p[0], std::memory_order_relaxed);  // 累加首字节形成副作用。
                single_thread_allocator.deallocate(p);  // 归还给池分配器。
            }  // 单线程池分配循环结束。
        });  // 单线程池基准结束。

    PoolAllocator shared_allocator;  // 创建多线程共享池分配器。
    const BenchmarkResult pool_four_threads =  // 保存四线程竞争基准结果。
        run_benchmark("PoolAllocator 4-thread contention", [&shared_allocator, &sink]() {  // 计时四线程竞争路径。
            std::vector<std::thread> threads;  // 保存工作线程对象。
            threads.reserve(kDay12ThreadCount);  // 预留线程数组容量。

            for (int thread_index = 0; thread_index < kDay12ThreadCount; ++thread_index) {  // 创建固定数量工作线程。
                threads.emplace_back([&shared_allocator, &sink, thread_index]() {  // 启动一个分配释放任务。
                    for (int i = 0; i < kDay12BenchmarkIterationsPerThread; ++i) {  // 当前线程执行分摊后的次数。
                        char* p = static_cast<char*>(shared_allocator.allocate(32));  // 从共享池分配 32 字节。
                        p[0] = static_cast<char>(thread_index + 1);  // 写入线程相关测试值。
                        sink.fetch_add(p[0], std::memory_order_relaxed);  // 累加首字节形成副作用。
                        shared_allocator.deallocate(p);  // 归还给共享池。
                    }  // 当前线程分配循环结束。
                });  // 工作线程创建结束。
            }  // 工作线程创建循环结束。

            for (std::thread& thread : threads) {  // 等待所有工作线程。
                thread.join();  // 汇合当前线程。
            }  // 线程汇合循环结束。
        });  // 四线程竞争基准结束。

    const double pool_single_speedup = new_delete.elapsed_ms / pool_single_thread.elapsed_ms;  // 计算池分配相对 new/delete 加速比。
    const double pool_contended_ratio = pool_four_threads.elapsed_ms / pool_single_thread.elapsed_ms;  // 计算竞争场景相对单线程耗时比。

    std::cout << "Day 012 benchmark (32-byte object, " << kBenchmarkIterations  // 输出 Day12 基准操作数量。
              << " alloc/free operations)\n";  // 输出 Day12 基准标题。
    std::cout << "  " << new_delete.name << ": " << new_delete.elapsed_ms << " ms\n";  // 输出 new/delete 耗时。
    std::cout << "  " << pool_single_thread.name << ": " << pool_single_thread.elapsed_ms  // 输出单线程池分配名称和数值。
              << " ms\n";  // 输出单线程池分配耗时。
    std::cout << "  " << pool_four_threads.name << ": " << pool_four_threads.elapsed_ms  // 输出四线程池分配名称和数值。
              << " ms\n";  // 输出四线程池分配耗时。
    std::cout << "  PoolAllocator single-thread speedup vs new/delete: " << pool_single_speedup  // 输出单线程加速比数值。
              << "x\n";  // 输出单线程池分配加速比。
    std::cout << "  4-thread contention / single-thread: " << pool_contended_ratio << "x\n";  // 输出四线程相对单线程耗时比。

    if (sink.load(std::memory_order_relaxed) == 0) {  // 防止极端情况下 sink 被认为无用。
        std::cout << "sink=" << sink.load(std::memory_order_relaxed) << "\n";  // 输出 sink 值。
    }  // sink 输出检查结束。
}  // run_day12_benchmark 结束。

void test_basic_allocate_deallocate() {  // 测试固定块分配和释放基础流程。
    FixedAllocator allocator;  // 创建固定块分配器。
    allocator.init(32, 128);  // 初始化 32 字节块和 128 个初始块。

    std::vector<void*> ptrs;  // 保存已分配指针。
    ptrs.reserve(1000);  // 预留指针数组容量。

    for (int i = 0; i < 1000; ++i) {  // 连续分配多个块触发必要扩容。
        void* p = allocator.allocate();  // 分配一个块。
        EXPECT_TRUE(p != nullptr);  // 确认分配成功。
        ptrs.push_back(p);  // 记录指针供后续释放。
    }  // 批量分配循环结束。

    for (void* p : ptrs) {  // 遍历所有已分配块。
        allocator.deallocate(p);  // 逐个归还块。
    }  // 批量释放循环结束。

    FixedAllocator::Stats s = allocator.stats();  // 获取释放后的统计信息。
    EXPECT_EQ(s.used, static_cast<std::size_t>(0));  // 确认没有仍在使用的块。
}  // test_basic_allocate_deallocate 结束。

void test_reuse_address() {  // 测试释放后的地址可以被复用。
    FixedAllocator allocator;  // 创建固定块分配器。
    allocator.init(32, 8);  // 初始化少量 32 字节块。

    void* a = allocator.allocate();  // 分配第一个块。
    EXPECT_TRUE(a != nullptr);  // 确认分配成功。
    allocator.deallocate(a);  // 立即归还第一个块。

    void* b = allocator.allocate();  // 再次分配一个块。
    EXPECT_EQ(a, b);  // 头插空闲链表应复用同一地址。

    allocator.deallocate(b);  // 归还第二次拿到的块。
    EXPECT_EQ(allocator.stats().used, static_cast<std::size_t>(0));  // 确认使用计数归零。
}  // test_reuse_address 结束。

void test_expand_chunks() {  // 测试固定块分配器按需扩容。
    FixedAllocator allocator;  // 创建固定块分配器。
    allocator.init(32, 4);  // 初始化很小容量以便触发扩容。

    std::vector<void*> ptrs;  // 保存分配出来的指针。
    ptrs.reserve(100);  // 预留指针数组容量。

    for (int i = 0; i < 100; ++i) {  // 分配超过初始容量的块数。
        void* p = allocator.allocate();  // 分配一个块。
        EXPECT_TRUE(p != nullptr);  // 确认扩容后仍可成功分配。
        ptrs.push_back(p);  // 记录指针供后续释放。
    }  // 扩容分配循环结束。

    FixedAllocator::Stats s = allocator.stats();  // 获取扩容后的统计信息。
    EXPECT_TRUE(s.chunks > 1);  // 确认产生了多个 chunk。

    for (void* p : ptrs) {  // 遍历所有已分配块。
        allocator.deallocate(p);  // 归还当前块。
    }  // 扩容测试释放循环结束。
    EXPECT_EQ(allocator.stats().used, static_cast<std::size_t>(0));  // 确认释放后使用计数归零。
}  // test_expand_chunks 结束。

void test_mixed_random_allocate_deallocate() {  // 测试随机交错分配和释放。
    FixedAllocator allocator;  // 创建固定块分配器。
    allocator.init(32, 16);  // 初始化 32 字节块和较小初始容量。

    std::mt19937 rng(20260413);  // 使用固定种子保证测试可复现。
    std::uniform_int_distribution<int> op_dist(0, 99);  // 生成操作选择概率。

    std::vector<void*> active;  // 保存当前仍活跃的指针。
    active.reserve(10000);  // 预留活跃指针容量。
    std::unordered_set<void*> allocated_set;  // 用集合检查地址不重复分配。

    for (int i = 0; i < 10000; ++i) {  // 执行大量随机操作。
        // 空集合时必须分配；否则按 60% 概率继续分配。
        bool do_allocate = active.empty() || op_dist(rng) < 60;  // 决定本轮分配还是释放。

        if (do_allocate) {  // 本轮执行分配。
            void* p = allocator.allocate();  // 分配一个块。
            EXPECT_TRUE(p != nullptr);  // 确认分配成功。
            EXPECT_TRUE(allocated_set.find(p) == allocated_set.end());  // 确认没有重复发放活跃地址。
            allocated_set.insert(p);  // 记录新活跃地址。
            active.push_back(p);  // 加入活跃数组。
        } else {  // 本轮执行释放。
            // 用 swap-pop 从活动集合中 O(1) 删除一个随机元素。
            std::uniform_int_distribution<std::size_t> idx_dist(0, active.size() - 1);  // 生成活跃数组随机下标。
            std::size_t idx = idx_dist(rng);  // 选中待释放元素下标。
            void* p = active[idx];  // 取出待释放指针。

            allocator.deallocate(p);  // 归还选中块。
            allocated_set.erase(p);  // 从活跃地址集合移除。

            active[idx] = active.back();  // 用末尾元素覆盖被删除位置。
            active.pop_back();  // 弹出末尾完成 O(1) 删除。
        }  // 本轮操作结束。
    }  // 随机操作循环结束。

    for (void* p : active) {  // 遍历剩余活跃指针。
        allocator.deallocate(p);  // 统一归还剩余块。
    }  // 剩余块释放循环结束。

    EXPECT_EQ(allocator.stats().used, static_cast<std::size_t>(0));  // 确认最终没有泄漏活跃块。
}  // test_mixed_random_allocate_deallocate 结束。

void test_day08_size_class_mapping() {  // 测试请求大小到 size class 的边界映射。
    EXPECT_EQ(PoolAllocator::size_to_class(0), 0);  // 零字节映射到最小 class。
    EXPECT_EQ(PoolAllocator::size_to_class(1), 0);  // 1 字节映射到 8 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(8), 0);  // 8 字节仍在第 0 类。
    EXPECT_EQ(PoolAllocator::size_to_class(9), 1);  // 9 字节进入 16 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(16), 1);  // 16 字节仍在第 1 类。
    EXPECT_EQ(PoolAllocator::size_to_class(17), 2);  // 17 字节进入 24 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(24), 2);  // 24 字节仍在第 2 类。
    EXPECT_EQ(PoolAllocator::size_to_class(25), 3);  // 25 字节进入 32 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(32), 3);  // 32 字节仍在第 3 类。
    EXPECT_EQ(PoolAllocator::size_to_class(33), 4);  // 33 字节进入 48 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(48), 4);  // 48 字节仍在第 4 类。
    EXPECT_EQ(PoolAllocator::size_to_class(49), 5);  // 49 字节进入 64 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(64), 5);  // 64 字节仍在第 5 类。
    EXPECT_EQ(PoolAllocator::size_to_class(65), 6);  // 65 字节进入 80 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(80), 6);  // 80 字节仍在第 6 类。
    EXPECT_EQ(PoolAllocator::size_to_class(81), 7);  // 81 字节进入 96 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(96), 7);  // 96 字节仍在第 7 类。
    EXPECT_EQ(PoolAllocator::size_to_class(97), 8);  // 97 字节进入 128 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(128), 8);  // 128 字节仍在第 8 类。
    EXPECT_EQ(PoolAllocator::size_to_class(129), 9);  // 129 字节进入 192 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(192), 9);  // 192 字节仍在第 9 类。
    EXPECT_EQ(PoolAllocator::size_to_class(193), 10);  // 193 字节进入 256 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(256), 10);  // 256 字节仍在第 10 类。
    EXPECT_EQ(PoolAllocator::size_to_class(257), 11);  // 257 字节进入 384 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(384), 11);  // 384 字节仍在第 11 类。
    EXPECT_EQ(PoolAllocator::size_to_class(385), 12);  // 385 字节进入 512 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(512), 12);  // 512 字节仍在第 12 类。
    EXPECT_EQ(PoolAllocator::size_to_class(513), 13);  // 513 字节进入 768 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(768), 13);  // 768 字节仍在第 13 类。
    EXPECT_EQ(PoolAllocator::size_to_class(769), 14);  // 769 字节进入 1024 字节 class。
    EXPECT_EQ(PoolAllocator::size_to_class(1024), 14);  // 1024 字节仍在小对象最大 class。
    EXPECT_EQ(PoolAllocator::size_to_class(1025), PoolAllocator::kLargeObjectClass);  // 超过上限进入大对象路径。

    for (std::size_t n = 1; n <= 1200; ++n) {  // 扫描一段连续大小验证单调边界。
        const int class_index = PoolAllocator::size_to_class(n);  // 计算当前大小所属 class。
        if (n <= PoolAllocator::kMaxSmallObjectSize) {  // 小对象范围内应命中非负 class。
            EXPECT_TRUE(class_index >= 0);  // 确认不是大对象标记。
            const std::size_t block_size = PoolAllocator::class_to_block_size(class_index);  // 查询 class 容量。
            EXPECT_TRUE(block_size >= n);  // 当前 class 必须能容纳请求大小。
            if (class_index > 0) {  // 非首个 class 需要验证前一类不足以容纳。
                EXPECT_TRUE(PoolAllocator::class_to_block_size(class_index - 1) < n);  // 确认映射到最小可容纳 class。
            }  // 前一类边界检查结束。
        } else {  // 超过小对象上限。
            EXPECT_EQ(class_index, PoolAllocator::kLargeObjectClass);  // 必须映射到大对象 class。
        }  // 大小范围分支结束。
    }  // 连续大小扫描结束。
}  // test_day08_size_class_mapping 结束。

void print_day08_size_class_mapping() {  // 打印 1 到 1200 字节的 size class 映射表。
    std::cout << "Day 008 size class mapping (1-1200 bytes)\n";  // 输出映射表标题。

    for (std::size_t n = 1; n <= 1200; ++n) {  // 遍历待展示的请求大小。
        const int class_index = PoolAllocator::size_to_class(n);  // 查询当前大小对应 class。

        std::cout << std::setw(4) << n << " -> ";  // 输出请求大小和箭头。
        if (class_index == PoolAllocator::kLargeObjectClass) {  // 大对象映射单独展示。
            std::cout << "large";  // 输出大对象标记。
        } else {  // 小对象映射展示 class 和块大小。
            std::cout << "class " << std::setw(2) << class_index  // 输出小对象 class 编号。
                      << " (" << std::setw(4)  // 输出容量字段左侧格式。
                      << PoolAllocator::class_to_block_size(class_index) << " bytes)";  // 输出 class 编号和容量。
        }  // 映射展示分支结束。

        if (n % 4 == 0 || n == 1200) {  // 每四项或最后一项换行。
            std::cout << '\n';  // 输出换行。
        } else {  // 当前行还可继续展示。
            std::cout << "    ";  // 输出列间空格。
        }  // 换行控制结束。
    }  // 映射表打印循环结束。
}  // print_day08_size_class_mapping 结束。

void test_day09_pool_allocator_core() {  // 测试 PoolAllocator 小对象核心分配释放。
    PoolAllocator allocator;  // 创建池分配器。
    std::vector<void*> small_ptrs;  // 保存小对象指针。
    std::vector<void*> medium_ptrs;  // 保存中等对象指针。

    small_ptrs.reserve(10);  // 预留小对象指针容量。
    medium_ptrs.reserve(10);  // 预留中等对象指针容量。

    for (int i = 0; i < 10; ++i) {  // 分配 10 个 20 字节对象。
        void* ptr = allocator.allocate(20);  // 从池分配小对象。
        EXPECT_TRUE(ptr != nullptr);  // 确认分配成功。
        small_ptrs.push_back(ptr);  // 保存小对象指针。
    }  // 小对象分配循环结束。

    for (int i = 0; i < 10; ++i) {  // 分配 10 个 200 字节对象。
        void* ptr = allocator.allocate(200);  // 从池分配中等对象。
        EXPECT_TRUE(ptr != nullptr);  // 确认分配成功。
        medium_ptrs.push_back(ptr);  // 保存中等对象指针。
    }  // 中等对象分配循环结束。

    const int class_20 = PoolAllocator::size_to_class(20);  // 获取 20 字节所属 class。
    const int class_200 = PoolAllocator::size_to_class(200);  // 获取 200 字节所属 class。
    const FixedAllocator::Stats small_stats = allocator.stats_for_class(class_20);  // 读取小对象 class 统计。
    const FixedAllocator::Stats medium_stats = allocator.stats_for_class(class_200);  // 读取中等对象 class 统计。

    EXPECT_EQ(small_stats.used, static_cast<std::size_t>(10));  // 确认小对象活跃数为 10。
    EXPECT_EQ(medium_stats.used, static_cast<std::size_t>(10));  // 确认中等对象活跃数为 10。
    EXPECT_EQ(small_stats.total, PoolAllocator::kDefaultBlockCount);  // 确认小对象 class 初始块数正确。
    EXPECT_EQ(medium_stats.total, PoolAllocator::kDefaultBlockCount);  // 确认中等对象 class 初始块数正确。

    for (void* ptr : small_ptrs) {  // 遍历小对象指针。
        allocator.deallocate(ptr);  // 归还小对象。
    }  // 小对象释放循环结束。

    for (void* ptr : medium_ptrs) {  // 遍历中等对象指针。
        allocator.deallocate(ptr);  // 归还中等对象。
    }  // 中等对象释放循环结束。

    EXPECT_EQ(allocator.stats_for_class(class_20).used, static_cast<std::size_t>(0));  // 确认小对象活跃数归零。
    EXPECT_EQ(allocator.stats_for_class(class_200).used, static_cast<std::size_t>(0));  // 确认中等对象活跃数归零。
}  // test_day09_pool_allocator_core 结束。

void test_day10_header_based_deallocate() {  // 测试基于 header 的无 size 释放。
    PoolAllocator allocator;  // 创建池分配器。
    std::vector<void*> ptrs;  // 保存不同大小的指针。

    ptrs.push_back(allocator.allocate(20));  // 分配小对象。
    ptrs.push_back(allocator.allocate(200));  // 分配中等小对象。
    ptrs.push_back(allocator.allocate(1024));  // 分配小对象上限大小。
    ptrs.push_back(allocator.allocate(1025));  // 分配刚超过上限的大对象。
    ptrs.push_back(allocator.allocate(4096));  // 分配更大的大对象。

    for (void* ptr : ptrs) {  // 遍历所有分配结果。
        EXPECT_TRUE(ptr != nullptr);  // 确认每个分配都成功。
    }  // 分配结果检查结束。

    EXPECT_EQ(allocator.stats_for_size(20).used, static_cast<std::size_t>(1));  // 确认 20 字节 class 活跃数为 1。
    EXPECT_EQ(allocator.stats_for_size(200).used, static_cast<std::size_t>(1));  // 确认 200 字节 class 活跃数为 1。
    EXPECT_EQ(allocator.stats_for_size(1024).used, static_cast<std::size_t>(1));  // 确认 1024 字节 class 活跃数为 1。

    for (void* ptr : ptrs) {  // 遍历所有指针。
        allocator.deallocate(ptr);  // 不传 size 直接释放。
    }  // header 释放循环结束。

    EXPECT_EQ(allocator.stats_for_size(20).used, static_cast<std::size_t>(0));  // 确认 20 字节 class 活跃数归零。
    EXPECT_EQ(allocator.stats_for_size(200).used, static_cast<std::size_t>(0));  // 确认 200 字节 class 活跃数归零。
    EXPECT_EQ(allocator.stats_for_size(1024).used, static_cast<std::size_t>(0));  // 确认 1024 字节 class 活跃数归零。
}  // test_day10_header_based_deallocate 结束。

void test_day11_std_allocator_adapter() {  // 测试 MyAllocator 适配标准容器。
    std::vector<int, MyAllocator<int>> values;  // 使用自定义 allocator 的 vector。

    for (int i = 0; i < 10000; ++i) {  // 连续插入大量元素。
        values.push_back(i);  // 写入当前整数。
    }  // vector 插入循环结束。

    EXPECT_EQ(values.size(), static_cast<std::size_t>(10000));  // 确认元素数量正确。
    EXPECT_EQ(values.front(), 0);  // 确认首元素正确。
    EXPECT_EQ(values.back(), 9999);  // 确认尾元素正确。

    for (int i = 0; i < 10000; i += 137) {  // 抽样检查中间元素。
        EXPECT_EQ(values[static_cast<std::size_t>(i)], i);  // 确认下标和值一致。
    }  // 抽样检查循环结束。

    std::vector<int, MyAllocator<int>> copied(values.begin(), values.end());  // 使用自定义 allocator 拷贝构造 vector。
    EXPECT_EQ(copied.size(), values.size());  // 确认拷贝后大小一致。
    EXPECT_EQ(copied[9999], 9999);  // 确认拷贝后尾部数据正确。

    std::vector<int, MyAllocator<int>> moved(std::move(values));  // 移动构造使用自定义 allocator 的 vector。
    EXPECT_EQ(moved.size(), static_cast<std::size_t>(10000));  // 确认移动后目标大小正确。
    EXPECT_EQ(moved[9999], 9999);  // 确认移动后尾部数据正确。
}  // test_day11_std_allocator_adapter 结束。

void test_day12_thread_safe_pool_allocator() {  // 测试 PoolAllocator 多线程安全性。
    PoolAllocator allocator;  // 创建共享池分配器。
    std::atomic<long long> checksum{0};  // 保存多线程写入校验和。
    std::vector<std::thread> threads;  // 保存工作线程。
    threads.reserve(kDay12ThreadCount);  // 预留线程数组容量。

    for (int thread_index = 0; thread_index < kDay12ThreadCount; ++thread_index) {  // 创建多个并发线程。
        threads.emplace_back([&allocator, &checksum, thread_index]() {  // 启动当前线程的分配释放任务。
            for (int i = 0; i < kDay12IterationsPerThread; ++i) {  // 当前线程循环执行固定次数。
                int* value = static_cast<int*>(allocator.allocate(32));  // 从共享池分配 32 字节。
                EXPECT_TRUE(value != nullptr);  // 确认分配成功。
                *value = thread_index * kDay12IterationsPerThread + i;  // 写入唯一测试值。
                checksum.fetch_add(*value, std::memory_order_relaxed);  // 累加到校验和。
                allocator.deallocate(value);  // 归还共享池。
            }  // 当前线程循环结束。
        });  // 当前线程创建结束。
    }  // 多线程创建循环结束。

    for (std::thread& thread : threads) {  // 等待所有工作线程结束。
        thread.join();  // 汇合当前线程。
    }  // 线程汇合循环结束。

    EXPECT_EQ(allocator.stats_for_size(32).used, static_cast<std::size_t>(0));  // 确认所有 32 字节块都已释放。
    EXPECT_TRUE(checksum.load(std::memory_order_relaxed) > 0);  // 确认线程确实执行了写入。
}  // test_day12_thread_safe_pool_allocator 结束。

void test_day14_tls_cache_prototype() {  // 测试线程本地缓存原型行为。
    PoolAllocator allocator;  // 创建池分配器。

    void* first = allocator.allocate(32);  // 首次分配 32 字节块。
    EXPECT_TRUE(first != nullptr);  // 确认首次分配成功。
    allocator.deallocate(first);  // 归还到线程本地缓存。

    // 同一线程再次申请同一 class 时，应优先命中线程本地缓存。
    void* second = allocator.allocate(32);  // 再次分配同规格块。
    EXPECT_EQ(first, second);  // 确认命中缓存并复用地址。
    allocator.deallocate(second);  // 归还第二次分配的块。

    EXPECT_EQ(allocator.stats_for_size(32).used, static_cast<std::size_t>(0));  // 确认活跃分配数归零。
    allocator.flush_current_thread_cache();  // 主动清空当前线程缓存。
    EXPECT_EQ(allocator.stats_for_size(32).used, static_cast<std::size_t>(0));  // 确认刷缓存后活跃数仍为零。
}  // test_day14_tls_cache_prototype 结束。

int main() {  // 程序入口，运行所有测试和基准。
    try {  // 捕获测试或基准抛出的异常。
        // 先跑 Day04 的功能正确性测试，再跑 Day05 的性能基准。
        test_basic_allocate_deallocate();  // 运行基础分配释放测试。
        test_reuse_address();  // 运行地址复用测试。
        test_expand_chunks();  // 运行扩容测试。
        test_mixed_random_allocate_deallocate();  // 运行随机交错分配释放测试。
        test_day08_size_class_mapping();  // 运行 size class 映射测试。
        test_day09_pool_allocator_core();  // 运行 PoolAllocator 核心测试。
        test_day10_header_based_deallocate();  // 运行 header 释放测试。
        test_day11_std_allocator_adapter();  // 运行 STL allocator 适配测试。
        test_day12_thread_safe_pool_allocator();  // 运行多线程安全测试。
        test_day14_tls_cache_prototype();  // 运行 TLS cache 原型测试。
        print_day08_size_class_mapping();  // 打印 size class 映射表。
        run_day05_benchmark();  // 运行固定块分配器基准。
        run_day12_benchmark();  // 运行池分配器多线程基准。
    } catch (const std::exception& ex) {  // 处理测试或基准失败。
        std::cerr << "Tests or benchmark failed: " << ex.what() << "\n";  // 输出失败原因。
        return 1;  // 返回非零表示失败。
    }  // 异常处理结束。

    std::cout << "Day 004 tests passed\n";  // 输出 Day04 测试通过信息。
    std::cout << "Day 008 mapping tests passed\n";  // 输出 Day08 测试通过信息。
    std::cout << "Day 009 pool allocator tests passed\n";  // 输出 Day09 测试通过信息。
    std::cout << "Day 010 header deallocate tests passed\n";  // 输出 Day10 测试通过信息。
    std::cout << "Day 011 allocator adapter tests passed\n";  // 输出 Day11 测试通过信息。
    std::cout << "Day 012 thread safety tests passed\n";  // 输出 Day12 测试通过信息。
    std::cout << "Day 014 TLS cache prototype tests passed\n";  // 输出 Day14 测试通过信息。
    return 0;  // 返回零表示全部成功。
}  // main 结束。