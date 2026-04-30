#include "concurrent/thread_pool.h"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#define EXPECT_TRUE(cond)                                                                  \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            throw std::runtime_error("EXPECT_TRUE failed: " #cond);                      \
        }                                                                                  \
    } while (false)

#define EXPECT_EQ(lhs, rhs) EXPECT_TRUE((lhs) == (rhs))

namespace {

using Clock = std::chrono::steady_clock;

constexpr int kBenchmarkTaskCount = 1'000'000;

struct BenchmarkResult {
    std::string name;
    double elapsed_ms = 0.0;
    double qps = 0.0;
};

template <typename Fn>
double measure_ms(Fn&& fn) {
    const auto start = Clock::now();
    fn();
    const auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

void test_day36_basic_thread_pool() {
    ThreadPool pool(4);
    std::atomic<int> counter{0};

    for (int index = 0; index < 100; ++index) {
        pool.submit(std::function<void()>([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    pool.await_termination();
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 100);
}

void test_day37_future_submit() {
    ThreadPool pool(4);
    std::vector<std::future<int>> futures;

    for (int index = 0; index < 10; ++index) {
        futures.push_back(pool.submit([index]() { return index * 10; }));
    }

    for (int index = 0; index < 10; ++index) {
        EXPECT_EQ(futures[static_cast<std::size_t>(index)].get(), index * 10);
    }

    pool.await_termination();
}

void test_day38_await_termination() {
    ThreadPool pool(4);
    std::atomic<int> counter{0};

    for (int index = 0; index < 1000; ++index) {
        pool.submit(std::function<void()>([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    pool.await_termination();
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 1000);
}

void test_day39_dynamic_scaling() {
    ThreadPoolOptions options;
    options.min_threads = 2;
    options.max_threads = 4;
    options.grow_threshold = 2;
    options.monitor_interval = std::chrono::milliseconds(100);

    ThreadPool pool(options);
    std::atomic<int> counter{0};

    for (int index = 0; index < 100; ++index) {
        pool.submit(std::function<void()>([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(350));
    const ThreadPool::Stats stats = pool.snapshot_stats();

    pool.await_termination();

    EXPECT_TRUE(stats.active_workers >= 3);
    EXPECT_TRUE(stats.target_workers >= 3);
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 100);
}

BenchmarkResult run_thread_pool_benchmark(std::size_t worker_count) {
    ThreadPool pool(worker_count);
    std::atomic<int> counter{0};

    const double elapsed_ms = measure_ms([&]() {
        for (int index = 0; index < kBenchmarkTaskCount; ++index) {
            pool.submit(std::function<void()>([&counter]() {
                counter.fetch_add(1, std::memory_order_relaxed);
            }));
        }
        pool.await_termination();
    });

    EXPECT_EQ(counter.load(std::memory_order_relaxed), kBenchmarkTaskCount);
    return BenchmarkResult{"thread_pool_" + std::to_string(worker_count) + "_threads",
                           elapsed_ms,
                           kBenchmarkTaskCount / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_single_thread_benchmark() {
    std::atomic<int> counter{0};

    const double elapsed_ms = measure_ms([&]() {
        for (int index = 0; index < kBenchmarkTaskCount; ++index) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    });

    EXPECT_EQ(counter.load(std::memory_order_relaxed), kBenchmarkTaskCount);
    return BenchmarkResult{"single_thread_baseline",
                           elapsed_ms,
                           kBenchmarkTaskCount / (elapsed_ms / 1000.0)};
}

void test_day41_stats_improvement() {
    ThreadPool pool(4);
    std::vector<std::future<int>> futures;

    for (int index = 0; index < 20; ++index) {
        futures.push_back(pool.submit([index]() { return index + 1; }));
    }

    int sum = 0;
    for (auto& future : futures) {
        sum += future.get();
    }

    pool.await_termination();
    const ThreadPool::Stats stats = pool.snapshot_stats();

    EXPECT_EQ(sum, 210);
    EXPECT_EQ(stats.submitted, static_cast<std::size_t>(20));
    EXPECT_EQ(stats.completed, static_cast<std::size_t>(20));
    EXPECT_EQ(stats.pending, static_cast<std::size_t>(0));
}

}  // namespace

int main() {
    try {
        test_day36_basic_thread_pool();
        test_day37_future_submit();
        test_day38_await_termination();
        test_day39_dynamic_scaling();
        test_day41_stats_improvement();

        const BenchmarkResult single_thread = run_single_thread_benchmark();
        const BenchmarkResult pool4 = run_thread_pool_benchmark(4);
        const BenchmarkResult pool8 = run_thread_pool_benchmark(8);

        std::cout << std::fixed << std::setprecision(3);
        std::cout << "All Week 06 thread pool tests passed.\n";
        std::cout << "Day 40 benchmark (" << kBenchmarkTaskCount << " light tasks)\n";
        std::cout << "  " << single_thread.name << ": " << single_thread.elapsed_ms
                  << " ms, QPS=" << single_thread.qps << "\n";
        std::cout << "  " << pool4.name << ": " << pool4.elapsed_ms << " ms, QPS="
                  << pool4.qps << "\n";
        std::cout << "  " << pool8.name << ": " << pool8.elapsed_ms << " ms, QPS="
                  << pool8.qps << "\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Week 06 validation failed: " << ex.what() << "\n";
        return 1;
    }
}