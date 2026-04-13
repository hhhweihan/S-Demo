#include "fixed_allocator.h"

#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#define EXPECT_TRUE(cond)                                                                  \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            throw std::runtime_error("EXPECT_TRUE failed: " #cond);                      \
        }                                                                                  \
    } while (false)

#define EXPECT_EQ(lhs, rhs) EXPECT_TRUE((lhs) == (rhs))

constexpr int kBenchmarkIterations = 1'000'000;

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
        bool do_allocate = active.empty() || op_dist(rng) < 60;

        if (do_allocate) {
            void* p = allocator.allocate();
            EXPECT_TRUE(p != nullptr);
            EXPECT_TRUE(allocated_set.find(p) == allocated_set.end());
            allocated_set.insert(p);
            active.push_back(p);
        } else {
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

int main() {
    try {
        test_basic_allocate_deallocate();
        test_reuse_address();
        test_expand_chunks();
        test_mixed_random_allocate_deallocate();
        run_day05_benchmark();
    } catch (const std::exception& ex) {
        std::cerr << "Tests or benchmark failed: " << ex.what() << "\n";
        return 1;
    }

    std::cout << "Day 004 unit tests passed\n";
    return 0;
}