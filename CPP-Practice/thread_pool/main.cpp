#include "concurrent/countdown_latch.h"
#include "concurrent/rw_mutex.h"
#include "concurrent/thread_pool.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
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
constexpr int kQueueBenchmarkCount = 1'000'000;

struct BenchmarkResult {
    std::string name;
    double elapsed_ms = 0.0;
    double qps = 0.0;
};

struct LatencyBenchmarkResult {
    double p50_us = 0.0;
    double p99_us = 0.0;
};

template <typename Fn>
double measure_ms(Fn&& fn) {
    const auto start = Clock::now();
    fn();
    const auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

double micros_between(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration<double, std::micro>(end - start).count();
}

double percentile_us(const std::vector<double>& values, double percentile) {
    EXPECT_TRUE(!values.empty());
    const double index = percentile * static_cast<double>(values.size() - 1);
    return values[static_cast<std::size_t>(index)];
}

void test_day50_rw_mutex() {
    RWMutex mutex;
    int shared_value = 0;
    std::atomic<int> active_readers{0};
    std::atomic<int> active_writers{0};
    std::atomic<bool> valid{true};

    std::thread writer([&] {
        for (int iteration = 0; iteration < 2000; ++iteration) {
            mutex.lock_write();
            const int previous_writers = active_writers.fetch_add(1, std::memory_order_acq_rel);
            if (previous_writers != 0 || active_readers.load(std::memory_order_acquire) != 0) {
                valid.store(false, std::memory_order_relaxed);
            }

            ++shared_value;

            active_writers.fetch_sub(1, std::memory_order_acq_rel);
            mutex.unlock_write();
        }
    });

    std::vector<std::thread> readers;
    readers.reserve(4);
    for (int index = 0; index < 4; ++index) {
        readers.emplace_back([&] {
            int last_seen = 0;
            for (int iteration = 0; iteration < 4000; ++iteration) {
                mutex.lock_read();
                active_readers.fetch_add(1, std::memory_order_acq_rel);
                if (active_writers.load(std::memory_order_acquire) != 0) {
                    valid.store(false, std::memory_order_relaxed);
                }

                const int snapshot = shared_value;
                if (snapshot < last_seen) {
                    valid.store(false, std::memory_order_relaxed);
                }
                last_seen = snapshot;

                active_readers.fetch_sub(1, std::memory_order_acq_rel);
                mutex.unlock_read();
            }
        });
    }

    writer.join();
    for (auto& reader : readers) {
        reader.join();
    }

    EXPECT_TRUE(valid.load(std::memory_order_relaxed));
    EXPECT_EQ(shared_value, 2000);
}

void test_day51_countdown_latch() {
    CountDownLatch latch(5);
    std::atomic<int> finished{0};

    std::vector<std::thread> workers;
    workers.reserve(5);
    for (int index = 0; index < 5; ++index) {
        workers.emplace_back([&, index] {
            std::this_thread::sleep_for(std::chrono::milliseconds(10 + index * 5));
            finished.fetch_add(1, std::memory_order_relaxed);
            latch.count_down();
        });
    }

    EXPECT_TRUE(!latch.wait_for(std::chrono::milliseconds(5)));
    latch.wait();

    for (auto& worker : workers) {
        worker.join();
    }

    EXPECT_EQ(finished.load(std::memory_order_relaxed), 5);
    EXPECT_EQ(latch.count(), static_cast<std::size_t>(0));
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

void test_day52_pause_resume_and_await_termination() {
    ThreadPool pool(4);
    std::atomic<int> counter{0};

    for (int index = 0; index < 100; ++index) {
        pool.submit([&counter] {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            counter.fetch_add(1, std::memory_order_relaxed);
        });
    }

    pool.pause();

    CountDownLatch submitter_started(1);
    std::atomic<int> second_batch_submitted{0};
    std::thread submitter([&] {
        submitter_started.count_down();
        for (int index = 0; index < 100; ++index) {
            pool.submit([&counter] {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                counter.fetch_add(1, std::memory_order_relaxed);
            });
            second_batch_submitted.fetch_add(1, std::memory_order_relaxed);
        }
    });

    submitter_started.wait();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    EXPECT_EQ(second_batch_submitted.load(std::memory_order_relaxed), 0);

    const ThreadPool::Stats paused_stats = pool.snapshot_stats();
    EXPECT_TRUE(paused_stats.paused);
    EXPECT_TRUE(paused_stats.accepting);

    pool.resume();
    submitter.join();
    pool.await_termination();

    EXPECT_EQ(second_batch_submitted.load(std::memory_order_relaxed), 100);
    EXPECT_EQ(counter.load(std::memory_order_relaxed), 200);
}

BenchmarkResult run_blocking_queue_benchmark() {
    BlockingQueue<int> queue;

    const double elapsed_ms = measure_ms([&] {
        std::thread producer([&] {
            for (int value = 0; value < kQueueBenchmarkCount; ++value) {
                queue.push(value);
            }
        });

        std::thread consumer([&] {
            for (int expected = 0; expected < kQueueBenchmarkCount; ++expected) {
                const int value = queue.pop();
                EXPECT_EQ(value, expected);
            }
        });

        producer.join();
        consumer.join();
    });

    return BenchmarkResult{"blocking_queue_1p1c",
                           elapsed_ms,
                           kQueueBenchmarkCount / (elapsed_ms / 1000.0)};
}

LatencyBenchmarkResult run_thread_pool_latency_benchmark(std::size_t worker_count,
                                                         int sample_count) {
    ThreadPool pool(worker_count);
    std::vector<double> latencies;
    latencies.reserve(static_cast<std::size_t>(sample_count));
    std::mutex latency_mutex;

    for (int index = 0; index < sample_count; ++index) {
        const auto submitted_at = Clock::now();
        pool.submit([submitted_at, &latencies, &latency_mutex] {
            const auto started_at = Clock::now();
            const double latency_us = micros_between(submitted_at, started_at);
            std::lock_guard<std::mutex> lock(latency_mutex);
            latencies.push_back(latency_us);
        });
    }

    pool.await_termination();
    std::sort(latencies.begin(), latencies.end());

    return LatencyBenchmarkResult{percentile_us(latencies, 0.50),
                                  percentile_us(latencies, 0.99)};
}

}  // namespace

int main() {
    try {
        test_day50_rw_mutex();
        test_day51_countdown_latch();
        test_day36_basic_thread_pool();
        test_day37_future_submit();
        test_day38_await_termination();
        test_day39_dynamic_scaling();
        test_day41_stats_improvement();
        test_day52_pause_resume_and_await_termination();

        const BenchmarkResult blocking_queue = run_blocking_queue_benchmark();
        const BenchmarkResult single_thread = run_single_thread_benchmark();
        const BenchmarkResult pool4 = run_thread_pool_benchmark(4);
        const BenchmarkResult pool8 = run_thread_pool_benchmark(8);
        const LatencyBenchmarkResult latency4 = run_thread_pool_latency_benchmark(4, 2000);

        std::cout << std::fixed << std::setprecision(3);
        std::cout << "All Week 06-08 thread pool tests passed.\n";
        std::cout << "Day 54 benchmark summary\n";
        std::cout << "  " << blocking_queue.name << ": " << blocking_queue.elapsed_ms
                  << " ms, QPS=" << blocking_queue.qps << "\n";
        std::cout << "  single_task_submit_latency_4_threads: p50=" << latency4.p50_us
                  << " us, p99=" << latency4.p99_us << " us\n";
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