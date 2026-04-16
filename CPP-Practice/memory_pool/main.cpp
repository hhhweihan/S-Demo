#include "fixed_allocator.h"
#include "my_allocator.h"
#include "pool_allocator.h"

#include <cassert>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

#define EXPECT_TRUE(cond)                                                                  \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            throw std::runtime_error("EXPECT_TRUE failed: " #cond);                      \
        }                                                                                  \
    } while (false)

#define EXPECT_EQ(lhs, rhs) EXPECT_TRUE((lhs) == (rhs))

constexpr int kBenchmarkIterations = 1'000'000;
constexpr int kDay12ThreadCount = 4;
constexpr int kDay12IterationsPerThread = 100'000;
constexpr int kDay12BenchmarkIterationsPerThread = kBenchmarkIterations / kDay12ThreadCount;

struct BenchmarkResult {
    std::string name;
    double elapsed_ms;
};

template <typename Fn>
BenchmarkResult run_benchmark(const std::string& name, Fn&& fn) {
    const auto start = std::chrono::high_resolution_clock::now();
    fn();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    return BenchmarkResult{name, elapsed};
}

void run_day05_benchmark() {
    // 累加写入结果，避免编译器把整个循环优化掉。
    volatile int sink = 0;

    const BenchmarkResult new_delete = run_benchmark("new/delete", [&sink]() {
        for (int i = 0; i < kBenchmarkIterations; ++i) {
            int* p = new int;
            *p = 42;
            sink += *p;
            delete p;
        }
    });

    FixedAllocator allocator;
    // 预留足够大的初始容量，避免 benchmark 测到扩容成本。
    allocator.init(sizeof(int), static_cast<std::size_t>(kBenchmarkIterations));

    const BenchmarkResult fixed_allocator = run_benchmark("FixedAllocator", [&allocator, &sink]() {
        for (int i = 0; i < kBenchmarkIterations; ++i) {
            void* p = allocator.allocate();
            *static_cast<int*>(p) = 42;
            sink += *static_cast<int*>(p);
            allocator.deallocate(p);
        }
    });

    const BenchmarkResult malloc_free = run_benchmark("malloc/free", [&sink]() {
        for (int i = 0; i < kBenchmarkIterations; ++i) {
            int* p = static_cast<int*>(std::malloc(sizeof(int)));
            *p = 42;
            sink += *p;
            std::free(p);
        }
    });

    const double ratio_vs_new = new_delete.elapsed_ms / fixed_allocator.elapsed_ms;
    const double ratio_vs_malloc = malloc_free.elapsed_ms / fixed_allocator.elapsed_ms;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Day 005 benchmark (" << kBenchmarkIterations << " iterations)\n";
    std::cout << "  " << new_delete.name << ": " << new_delete.elapsed_ms << " ms\n";
    std::cout << "  " << fixed_allocator.name << ": " << fixed_allocator.elapsed_ms << " ms\n";
    std::cout << "  " << malloc_free.name << ": " << malloc_free.elapsed_ms << " ms\n";
    std::cout << "  FixedAllocator speedup vs new/delete: " << ratio_vs_new << "x\n";
    std::cout << "  FixedAllocator speedup vs malloc/free: " << ratio_vs_malloc << "x\n";

    if (sink == 0) {
        std::cout << "sink=" << sink << "\n";
    }
}

void run_day12_benchmark() {
    std::atomic<long long> sink{0};

    const BenchmarkResult new_delete = run_benchmark("new/delete", [&sink]() {
        for (int i = 0; i < kBenchmarkIterations; ++i) {
            char* p = new char[32];
            p[0] = 42;
            sink.fetch_add(p[0], std::memory_order_relaxed);
            delete[] p;
        }
    });

    PoolAllocator single_thread_allocator;
    const BenchmarkResult pool_single_thread =
        run_benchmark("PoolAllocator single-thread", [&single_thread_allocator, &sink]() {
            for (int i = 0; i < kBenchmarkIterations; ++i) {
                char* p = static_cast<char*>(single_thread_allocator.allocate(32));
                p[0] = 42;
                sink.fetch_add(p[0], std::memory_order_relaxed);
                single_thread_allocator.deallocate(p);
            }
        });

    PoolAllocator shared_allocator;
    const BenchmarkResult pool_four_threads =
        run_benchmark("PoolAllocator 4-thread contention", [&shared_allocator, &sink]() {
            std::vector<std::thread> threads;
            threads.reserve(kDay12ThreadCount);

            for (int thread_index = 0; thread_index < kDay12ThreadCount; ++thread_index) {
                threads.emplace_back([&shared_allocator, &sink, thread_index]() {
                    for (int i = 0; i < kDay12BenchmarkIterationsPerThread; ++i) {
                        char* p = static_cast<char*>(shared_allocator.allocate(32));
                        p[0] = static_cast<char>(thread_index + 1);
                        sink.fetch_add(p[0], std::memory_order_relaxed);
                        shared_allocator.deallocate(p);
                    }
                });
            }

            for (std::thread& thread : threads) {
                thread.join();
            }
        });

    const double pool_single_speedup = new_delete.elapsed_ms / pool_single_thread.elapsed_ms;
    const double pool_contended_ratio = pool_four_threads.elapsed_ms / pool_single_thread.elapsed_ms;

    std::cout << "Day 012 benchmark (32-byte object, " << kBenchmarkIterations
              << " alloc/free operations)\n";
    std::cout << "  " << new_delete.name << ": " << new_delete.elapsed_ms << " ms\n";
    std::cout << "  " << pool_single_thread.name << ": " << pool_single_thread.elapsed_ms
              << " ms\n";
    std::cout << "  " << pool_four_threads.name << ": " << pool_four_threads.elapsed_ms
              << " ms\n";
    std::cout << "  PoolAllocator single-thread speedup vs new/delete: " << pool_single_speedup
              << "x\n";
    std::cout << "  4-thread contention / single-thread: " << pool_contended_ratio << "x\n";

    if (sink.load(std::memory_order_relaxed) == 0) {
        std::cout << "sink=" << sink.load(std::memory_order_relaxed) << "\n";
    }
}

void test_basic_allocate_deallocate() {
    FixedAllocator allocator;
    allocator.init(32, 128);

    std::vector<void*> ptrs;
    ptrs.reserve(1000);

    for (int i = 0; i < 1000; ++i) {
        void* p = allocator.allocate();
        EXPECT_TRUE(p != nullptr);
        ptrs.push_back(p);
    }

    for (void* p : ptrs) {
        allocator.deallocate(p);
    }

    FixedAllocator::Stats s = allocator.stats();
    EXPECT_EQ(s.used, static_cast<std::size_t>(0));
}

void test_reuse_address() {
    FixedAllocator allocator;
    allocator.init(32, 8);

    void* a = allocator.allocate();
    EXPECT_TRUE(a != nullptr);
    allocator.deallocate(a);

    void* b = allocator.allocate();
    EXPECT_EQ(a, b);

    allocator.deallocate(b);
    EXPECT_EQ(allocator.stats().used, static_cast<std::size_t>(0));
}

void test_expand_chunks() {
    FixedAllocator allocator;
    allocator.init(32, 4);

    std::vector<void*> ptrs;
    ptrs.reserve(100);

    for (int i = 0; i < 100; ++i) {
        void* p = allocator.allocate();
        EXPECT_TRUE(p != nullptr);
        ptrs.push_back(p);
    }

    FixedAllocator::Stats s = allocator.stats();
    EXPECT_TRUE(s.chunks > 1);

    for (void* p : ptrs) {
        allocator.deallocate(p);
    }
    EXPECT_EQ(allocator.stats().used, static_cast<std::size_t>(0));
}

void test_mixed_random_allocate_deallocate() {
    FixedAllocator allocator;
    allocator.init(32, 16);

    std::mt19937 rng(20260413);
    std::uniform_int_distribution<int> op_dist(0, 99);

    std::vector<void*> active;
    active.reserve(10000);
    std::unordered_set<void*> allocated_set;

    for (int i = 0; i < 10000; ++i) {
        // 空集合时必须分配；否则按 60% 概率继续分配。
        bool do_allocate = active.empty() || op_dist(rng) < 60;

        if (do_allocate) {
            void* p = allocator.allocate();
            EXPECT_TRUE(p != nullptr);
            EXPECT_TRUE(allocated_set.find(p) == allocated_set.end());
            allocated_set.insert(p);
            active.push_back(p);
        } else {
            // 用 swap-pop 从活动集合中 O(1) 删除一个随机元素。
            std::uniform_int_distribution<std::size_t> idx_dist(0, active.size() - 1);
            std::size_t idx = idx_dist(rng);
            void* p = active[idx];

            allocator.deallocate(p);
            allocated_set.erase(p);

            active[idx] = active.back();
            active.pop_back();
        }
    }

    for (void* p : active) {
        allocator.deallocate(p);
    }

    EXPECT_EQ(allocator.stats().used, static_cast<std::size_t>(0));
}

void test_day08_size_class_mapping() {
    EXPECT_EQ(PoolAllocator::size_to_class(0), 0);
    EXPECT_EQ(PoolAllocator::size_to_class(1), 0);
    EXPECT_EQ(PoolAllocator::size_to_class(8), 0);
    EXPECT_EQ(PoolAllocator::size_to_class(9), 1);
    EXPECT_EQ(PoolAllocator::size_to_class(16), 1);
    EXPECT_EQ(PoolAllocator::size_to_class(17), 2);
    EXPECT_EQ(PoolAllocator::size_to_class(24), 2);
    EXPECT_EQ(PoolAllocator::size_to_class(25), 3);
    EXPECT_EQ(PoolAllocator::size_to_class(32), 3);
    EXPECT_EQ(PoolAllocator::size_to_class(33), 4);
    EXPECT_EQ(PoolAllocator::size_to_class(48), 4);
    EXPECT_EQ(PoolAllocator::size_to_class(49), 5);
    EXPECT_EQ(PoolAllocator::size_to_class(64), 5);
    EXPECT_EQ(PoolAllocator::size_to_class(65), 6);
    EXPECT_EQ(PoolAllocator::size_to_class(80), 6);
    EXPECT_EQ(PoolAllocator::size_to_class(81), 7);
    EXPECT_EQ(PoolAllocator::size_to_class(96), 7);
    EXPECT_EQ(PoolAllocator::size_to_class(97), 8);
    EXPECT_EQ(PoolAllocator::size_to_class(128), 8);
    EXPECT_EQ(PoolAllocator::size_to_class(129), 9);
    EXPECT_EQ(PoolAllocator::size_to_class(192), 9);
    EXPECT_EQ(PoolAllocator::size_to_class(193), 10);
    EXPECT_EQ(PoolAllocator::size_to_class(256), 10);
    EXPECT_EQ(PoolAllocator::size_to_class(257), 11);
    EXPECT_EQ(PoolAllocator::size_to_class(384), 11);
    EXPECT_EQ(PoolAllocator::size_to_class(385), 12);
    EXPECT_EQ(PoolAllocator::size_to_class(512), 12);
    EXPECT_EQ(PoolAllocator::size_to_class(513), 13);
    EXPECT_EQ(PoolAllocator::size_to_class(768), 13);
    EXPECT_EQ(PoolAllocator::size_to_class(769), 14);
    EXPECT_EQ(PoolAllocator::size_to_class(1024), 14);
    EXPECT_EQ(PoolAllocator::size_to_class(1025), PoolAllocator::kLargeObjectClass);

    for (std::size_t n = 1; n <= 1200; ++n) {
        const int class_index = PoolAllocator::size_to_class(n);
        if (n <= PoolAllocator::kMaxSmallObjectSize) {
            EXPECT_TRUE(class_index >= 0);
            const std::size_t block_size = PoolAllocator::class_to_block_size(class_index);
            EXPECT_TRUE(block_size >= n);
            if (class_index > 0) {
                EXPECT_TRUE(PoolAllocator::class_to_block_size(class_index - 1) < n);
            }
        } else {
            EXPECT_EQ(class_index, PoolAllocator::kLargeObjectClass);
        }
    }
}

void print_day08_size_class_mapping() {
    std::cout << "Day 008 size class mapping (1-1200 bytes)\n";

    for (std::size_t n = 1; n <= 1200; ++n) {
        const int class_index = PoolAllocator::size_to_class(n);

        std::cout << std::setw(4) << n << " -> ";
        if (class_index == PoolAllocator::kLargeObjectClass) {
            std::cout << "large";
        } else {
            std::cout << "class " << std::setw(2) << class_index
                      << " (" << std::setw(4)
                      << PoolAllocator::class_to_block_size(class_index) << " bytes)";
        }

        if (n % 4 == 0 || n == 1200) {
            std::cout << '\n';
        } else {
            std::cout << "    ";
        }
    }
}

void test_day09_pool_allocator_core() {
    PoolAllocator allocator;
    std::vector<void*> small_ptrs;
    std::vector<void*> medium_ptrs;

    small_ptrs.reserve(10);
    medium_ptrs.reserve(10);

    for (int i = 0; i < 10; ++i) {
        void* ptr = allocator.allocate(20);
        EXPECT_TRUE(ptr != nullptr);
        small_ptrs.push_back(ptr);
    }

    for (int i = 0; i < 10; ++i) {
        void* ptr = allocator.allocate(200);
        EXPECT_TRUE(ptr != nullptr);
        medium_ptrs.push_back(ptr);
    }

    const int class_20 = PoolAllocator::size_to_class(20);
    const int class_200 = PoolAllocator::size_to_class(200);
    const FixedAllocator::Stats small_stats = allocator.stats_for_class(class_20);
    const FixedAllocator::Stats medium_stats = allocator.stats_for_class(class_200);

    EXPECT_EQ(small_stats.used, static_cast<std::size_t>(10));
    EXPECT_EQ(medium_stats.used, static_cast<std::size_t>(10));
    EXPECT_EQ(small_stats.total, PoolAllocator::kDefaultBlockCount);
    EXPECT_EQ(medium_stats.total, PoolAllocator::kDefaultBlockCount);

    for (void* ptr : small_ptrs) {
        allocator.deallocate(ptr);
    }

    for (void* ptr : medium_ptrs) {
        allocator.deallocate(ptr);
    }

    EXPECT_EQ(allocator.stats_for_class(class_20).used, static_cast<std::size_t>(0));
    EXPECT_EQ(allocator.stats_for_class(class_200).used, static_cast<std::size_t>(0));
}

void test_day10_header_based_deallocate() {
    PoolAllocator allocator;
    std::vector<void*> ptrs;

    ptrs.push_back(allocator.allocate(20));
    ptrs.push_back(allocator.allocate(200));
    ptrs.push_back(allocator.allocate(1024));
    ptrs.push_back(allocator.allocate(1025));
    ptrs.push_back(allocator.allocate(4096));

    for (void* ptr : ptrs) {
        EXPECT_TRUE(ptr != nullptr);
    }

    EXPECT_EQ(allocator.stats_for_size(20).used, static_cast<std::size_t>(1));
    EXPECT_EQ(allocator.stats_for_size(200).used, static_cast<std::size_t>(1));
    EXPECT_EQ(allocator.stats_for_size(1024).used, static_cast<std::size_t>(1));

    for (void* ptr : ptrs) {
        allocator.deallocate(ptr);
    }

    EXPECT_EQ(allocator.stats_for_size(20).used, static_cast<std::size_t>(0));
    EXPECT_EQ(allocator.stats_for_size(200).used, static_cast<std::size_t>(0));
    EXPECT_EQ(allocator.stats_for_size(1024).used, static_cast<std::size_t>(0));
}

void test_day11_std_allocator_adapter() {
    std::vector<int, MyAllocator<int>> values;

    for (int i = 0; i < 10000; ++i) {
        values.push_back(i);
    }

    EXPECT_EQ(values.size(), static_cast<std::size_t>(10000));
    EXPECT_EQ(values.front(), 0);
    EXPECT_EQ(values.back(), 9999);

    for (int i = 0; i < 10000; i += 137) {
        EXPECT_EQ(values[static_cast<std::size_t>(i)], i);
    }

    std::vector<int, MyAllocator<int>> copied(values.begin(), values.end());
    EXPECT_EQ(copied.size(), values.size());
    EXPECT_EQ(copied[9999], 9999);

    std::vector<int, MyAllocator<int>> moved(std::move(values));
    EXPECT_EQ(moved.size(), static_cast<std::size_t>(10000));
    EXPECT_EQ(moved[9999], 9999);
}

void test_day12_thread_safe_pool_allocator() {
    PoolAllocator allocator;
    std::atomic<long long> checksum{0};
    std::vector<std::thread> threads;
    threads.reserve(kDay12ThreadCount);

    for (int thread_index = 0; thread_index < kDay12ThreadCount; ++thread_index) {
        threads.emplace_back([&allocator, &checksum, thread_index]() {
            for (int i = 0; i < kDay12IterationsPerThread; ++i) {
                int* value = static_cast<int*>(allocator.allocate(32));
                EXPECT_TRUE(value != nullptr);
                *value = thread_index * kDay12IterationsPerThread + i;
                checksum.fetch_add(*value, std::memory_order_relaxed);
                allocator.deallocate(value);
            }
        });
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(allocator.stats_for_size(32).used, static_cast<std::size_t>(0));
    EXPECT_TRUE(checksum.load(std::memory_order_relaxed) > 0);
}

void test_day14_tls_cache_prototype() {
    PoolAllocator allocator;

    void* first = allocator.allocate(32);
    EXPECT_TRUE(first != nullptr);
    allocator.deallocate(first);

    // 同一线程再次申请同一 class 时，应优先命中线程本地缓存。
    void* second = allocator.allocate(32);
    EXPECT_EQ(first, second);
    allocator.deallocate(second);

    EXPECT_EQ(allocator.stats_for_size(32).used, static_cast<std::size_t>(0));
    allocator.flush_current_thread_cache();
    EXPECT_EQ(allocator.stats_for_size(32).used, static_cast<std::size_t>(0));
}

int main() {
    try {
        // 先跑 Day04 的功能正确性测试，再跑 Day05 的性能基准。
        test_basic_allocate_deallocate();
        test_reuse_address();
        test_expand_chunks();
        test_mixed_random_allocate_deallocate();
        test_day08_size_class_mapping();
        test_day09_pool_allocator_core();
        test_day10_header_based_deallocate();
        test_day11_std_allocator_adapter();
        test_day12_thread_safe_pool_allocator();
        test_day14_tls_cache_prototype();
        print_day08_size_class_mapping();
        run_day05_benchmark();
        run_day12_benchmark();
    } catch (const std::exception& ex) {
        std::cerr << "Tests or benchmark failed: " << ex.what() << "\n";
        return 1;
    }

    std::cout << "Day 004 tests passed\n";
    std::cout << "Day 008 mapping tests passed\n";
    std::cout << "Day 009 pool allocator tests passed\n";
    std::cout << "Day 010 header deallocate tests passed\n";
    std::cout << "Day 011 allocator adapter tests passed\n";
    std::cout << "Day 012 thread safety tests passed\n";
    std::cout << "Day 014 TLS cache prototype tests passed\n";
    return 0;
}