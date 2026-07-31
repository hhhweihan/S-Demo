#include "concurrent/aba_demo.h"
#include "concurrent/blocking_queue.h"
#include "concurrent/bounded_queue.h"
#include "concurrent/lock_free_stack.h"
#include "concurrent/memory_order_experiments.h"
#include "concurrent/spsc_queue.h"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

// do/while(false) 包裹：让宏在 if/else 里作为单条语句安全展开。
#define EXPECT_TRUE(cond)                                           \
    do {                                                            \
        if (!(cond)) {                                              \
            throw std::runtime_error("EXPECT_TRUE failed: " #cond); \
        }                                                           \
    } while (false)

#define EXPECT_EQ(lhs, rhs) EXPECT_TRUE((lhs) == (rhs))

namespace {

using Clock = std::chrono::steady_clock;
constexpr int kExchangeCount = 1'000'000;
constexpr std::size_t kPublicationIterations = 2'000;
constexpr int kStackStressProducers = 4;
constexpr int kStackStressConsumers = 4;
constexpr int kStackStressValuesPerProducer = 50'000;
constexpr int kStackBenchmarkValuesPerProducer = 100'000;

struct BenchmarkResult {
    std::string name;
    double elapsed_ms = 0.0;
    double throughput = 0.0;
};

template <typename Fn>
double measure_ms(Fn&& fn) {
    const auto begin = Clock::now();
    fn();
    const auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

void test_day43_memory_order(PublicationExperimentResult& result) {
    result = memory_order_experiments::run_publication_experiment(kPublicationIterations);
    // release/acquire 建立 happens-before，payload 必然可见，错配数应严格为 0。
    EXPECT_EQ(result.release_acquire_mismatches, static_cast<std::size_t>(0));
}

void test_day44_lock_free_stack() {
    LockFreeStack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);

    int value = 0;
    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, 3);  // LIFO：最后压入的先弹出
    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, 2);
    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, 1);
    EXPECT_TRUE(!stack.pop(value));
}

void test_day44_lock_free_stack_parallel_push() {
    LockFreeStack<int> stack;
    const int total_values = kStackStressProducers * kStackStressValuesPerProducer;

    std::vector<std::thread> producers;
    producers.reserve(kStackStressProducers);

    // 每个生产者写不相交的值域，弹出后可用 seen[] 检测丢失或重复。
    for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {
        producers.emplace_back([&, producer_id] {
            const int begin = producer_id * kStackStressValuesPerProducer;
            const int end = begin + kStackStressValuesPerProducer;
            for (int value = begin; value < end; ++value) {
                stack.push(value);
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }

    std::vector<unsigned char> seen(static_cast<std::size_t>(total_values), 0);
    int popped_count = 0;
    while (popped_count < total_values) {
        int value = -1;
        EXPECT_TRUE(stack.pop(value));
        EXPECT_TRUE(value >= 0 && value < total_values);
        EXPECT_TRUE(seen[static_cast<std::size_t>(value)] == 0);  // 每个值恰好出现一次
        seen[static_cast<std::size_t>(value)] = 1;
        ++popped_count;
    }

    EXPECT_TRUE(stack.empty());
}

void test_day48_reclaiming_lock_free_stack() {
    ReclaimingLockFreeStack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);

    std::shared_ptr<int> value = stack.pop();
    EXPECT_TRUE(static_cast<bool>(value));
    EXPECT_EQ(*value, 30);

    value = stack.pop();
    EXPECT_TRUE(static_cast<bool>(value));
    EXPECT_EQ(*value, 20);

    value = stack.pop();
    EXPECT_TRUE(static_cast<bool>(value));
    EXPECT_EQ(*value, 10);

    value = stack.pop();
    EXPECT_TRUE(!value);  // 空栈返回空 shared_ptr
    EXPECT_TRUE(stack.empty());
}

void test_day46_tagged_pointer_stack() {
    TaggedPointerStack<int> stack;
    stack.push(7);
    stack.push(8);
    stack.push(9);

    int value = 0;
    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, 9);
    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, 8);
    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, 7);
    EXPECT_TRUE(!stack.pop(value));
    EXPECT_TRUE(stack.empty());
}

void test_day48_reclaiming_lock_free_stack_parallel() {
    ReclaimingLockFreeStack<int> stack;
    const int total_values = kStackStressProducers * kStackStressValuesPerProducer;
    std::vector<std::atomic<int>> seen(static_cast<std::size_t>(total_values));
    for (auto& slot : seen) {
        slot.store(0, std::memory_order_relaxed);
    }

    std::atomic<int> producers_done{0};
    std::atomic<int> consumed{0};
    std::atomic<bool> valid{true};

    std::vector<std::thread> producers;
    producers.reserve(kStackStressProducers);
    for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {
        producers.emplace_back([&, producer_id] {
            const int begin = producer_id * kStackStressValuesPerProducer;
            const int end = begin + kStackStressValuesPerProducer;
            for (int value = begin; value < end; ++value) {
                stack.push(value);
            }
            producers_done.fetch_add(1, std::memory_order_release);
        });
    }

    std::vector<std::thread> consumers;
    consumers.reserve(kStackStressConsumers);
    for (int consumer_id = 0; consumer_id < kStackStressConsumers; ++consumer_id) {
        consumers.emplace_back([&] {
            while (true) {
                if (consumed.load(std::memory_order_acquire) >= total_values) {
                    break;
                }

                std::shared_ptr<int> value = stack.pop();
                if (!value) {
                    // 栈暂空：只有确认生产者全部完成且消费达标才收工，否则可能是生产者还没跟上。
                    if (producers_done.load(std::memory_order_acquire) == kStackStressProducers &&
                        consumed.load(std::memory_order_acquire) >= total_values) {
                        break;
                    }
                    std::this_thread::yield();
                    continue;
                }

                const int index = *value;
                if (index < 0 || index >= total_values) {
                    valid.store(false, std::memory_order_relaxed);
                    continue;
                }

                // fetch_add 返回非 0 说明该值被弹了两次——无锁回收出 bug 的典型症状。
                if (seen[static_cast<std::size_t>(index)].fetch_add(1, std::memory_order_relaxed) !=
                    0) {
                    valid.store(false, std::memory_order_relaxed);
                }
                consumed.fetch_add(1, std::memory_order_release);
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }
    for (auto& consumer : consumers) {
        consumer.join();
    }

    EXPECT_TRUE(valid.load(std::memory_order_relaxed));
    EXPECT_EQ(consumed.load(std::memory_order_acquire), total_values);
    EXPECT_TRUE(stack.empty());
}

void test_day45_spsc_queue() {
    SPSCQueue<int> queue(1024);
    std::atomic<bool> order_ok{true};

    std::thread producer([&] {
        for (int value = 0; value < kExchangeCount; ++value) {
            while (!queue.push(value)) {
                std::this_thread::yield();  // 满则退让重试；SPSCQueue 无阻塞语义
            }
        }
    });

    std::thread consumer([&] {
        for (int expected = 0; expected < kExchangeCount; ++expected) {
            int actual = -1;
            while (!queue.pop(actual)) {
                std::this_thread::yield();
            }
            if (actual != expected) {
                order_ok.store(false, std::memory_order_relaxed);
            }
        }
    });

    producer.join();
    consumer.join();

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));
    EXPECT_TRUE(queue.empty());
}

void test_day46_aba_defense() {
    const ABAExperimentResult result = aba_demo::run_aba_compare_demo();
    EXPECT_TRUE(result.plain_head_cas_succeeded);    // 裸 CAS 被 ABA 骗过
    EXPECT_TRUE(!result.tagged_head_cas_succeeded);  // 带 tag 的 CAS 识破变化而失败
    EXPECT_EQ(result.final_plain_value, static_cast<std::uint32_t>(3));
    EXPECT_EQ(result.final_tagged_index, static_cast<std::uint32_t>(1));  // index 未被改写
    EXPECT_EQ(result.final_tagged_tag, static_cast<std::uint32_t>(2));    // tag 已推进
}

BenchmarkResult run_std_queue_baseline() {
    long long checksum = 0;  // 累加校验和，防止空循环被编译器优化掉
    const double elapsed_ms = measure_ms([&] {
        std::queue<int> queue;
        for (int value = 0; value < kExchangeCount; ++value) {
            queue.push(value);
            checksum += queue.front();
            queue.pop();
        }
    });

    const long long expected =
        static_cast<long long>(kExchangeCount - 1) * static_cast<long long>(kExchangeCount) / 2;
    EXPECT_EQ(checksum, expected);
    return BenchmarkResult{"std::queue_baseline", elapsed_ms,
                           kExchangeCount / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_blocking_queue_benchmark() {
    BlockingQueue<int> queue;
    std::atomic<bool> order_ok{true};

    const double elapsed_ms = measure_ms([&] {
        std::thread producer([&] {
            for (int value = 0; value < kExchangeCount; ++value) {
                queue.push(value);
            }
        });

        std::thread consumer([&] {
            for (int expected = 0; expected < kExchangeCount; ++expected) {
                const int actual = queue.pop();
                if (actual != expected) {
                    order_ok.store(false, std::memory_order_relaxed);
                }
            }
        });

        producer.join();
        consumer.join();
    });

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));
    return BenchmarkResult{"BlockingQueue_1P1C", elapsed_ms,
                           kExchangeCount / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_bounded_queue_benchmark() {
    BoundedQueue<int> queue(1024);
    std::atomic<bool> order_ok{true};

    const double elapsed_ms = measure_ms([&] {
        std::thread producer([&] {
            for (int value = 0; value < kExchangeCount; ++value) {
                queue.push(value);
            }
        });

        std::thread consumer([&] {
            for (int expected = 0; expected < kExchangeCount; ++expected) {
                const int actual = queue.pop();
                if (actual != expected) {
                    order_ok.store(false, std::memory_order_relaxed);
                }
            }
        });

        producer.join();
        consumer.join();
    });

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));
    return BenchmarkResult{"BoundedQueue_1P1C_cap1024", elapsed_ms,
                           kExchangeCount / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_spsc_benchmark() {
    SPSCQueue<int> queue(1024);
    std::atomic<bool> order_ok{true};

    const double elapsed_ms = measure_ms([&] {
        std::thread producer([&] {
            for (int value = 0; value < kExchangeCount; ++value) {
                while (!queue.push(value)) {
                    std::this_thread::yield();
                }
            }
        });

        std::thread consumer([&] {
            for (int expected = 0; expected < kExchangeCount; ++expected) {
                int actual = -1;
                while (!queue.pop(actual)) {
                    std::this_thread::yield();
                }
                if (actual != expected) {
                    order_ok.store(false, std::memory_order_relaxed);
                }
            }
        });

        producer.join();
        consumer.join();
    });

    EXPECT_TRUE(order_ok.load(std::memory_order_relaxed));
    EXPECT_TRUE(queue.empty());
    return BenchmarkResult{"SPSCQueue_1P1C_cap1024", elapsed_ms,
                           kExchangeCount / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_lock_free_stack_push_drain_benchmark() {
    LockFreeStack<int> stack;
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;

    const double elapsed_ms = measure_ms([&] {
        std::vector<std::thread> producers;
        producers.reserve(kStackStressProducers);

        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {
            producers.emplace_back([&, producer_id] {
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;
                const int end = begin + kStackBenchmarkValuesPerProducer;
                for (int value = begin; value < end; ++value) {
                    stack.push(value);
                }
            });
        }

        for (auto& producer : producers) {
            producer.join();
        }

        int popped = 0;
        int value = 0;
        while (popped < total_values) {
            EXPECT_TRUE(stack.pop(value));
            ++popped;
        }
    });

    return BenchmarkResult{"LockFreeStack_4P1C_push_drain", elapsed_ms,
                           total_values / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_reclaiming_stack_push_drain_benchmark() {
    ReclaimingLockFreeStack<int> stack;
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;

    const double elapsed_ms = measure_ms([&] {
        std::vector<std::thread> producers;
        producers.reserve(kStackStressProducers);

        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {
            producers.emplace_back([&, producer_id] {
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;
                const int end = begin + kStackBenchmarkValuesPerProducer;
                for (int value = begin; value < end; ++value) {
                    stack.push(value);
                }
            });
        }

        for (auto& producer : producers) {
            producer.join();
        }

        int popped = 0;
        while (popped < total_values) {
            std::shared_ptr<int> value = stack.pop();
            EXPECT_TRUE(static_cast<bool>(value));
            ++popped;
        }
    });

    return BenchmarkResult{"ReclaimStack_4P1C_push_drain", elapsed_ms,
                           total_values / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_tagged_pointer_stack_push_drain_benchmark() {
    TaggedPointerStack<int> stack;
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;

    const double elapsed_ms = measure_ms([&] {
        std::vector<std::thread> producers;
        producers.reserve(kStackStressProducers);

        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {
            producers.emplace_back([&, producer_id] {
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;
                const int end = begin + kStackBenchmarkValuesPerProducer;
                for (int value = begin; value < end; ++value) {
                    stack.push(value);
                }
            });
        }

        for (auto& producer : producers) {
            producer.join();
        }

        int popped = 0;
        int value = 0;
        while (popped < total_values) {
            EXPECT_TRUE(stack.pop(value));
            ++popped;
        }
    });

    return BenchmarkResult{"TaggedStack_4P1C_push_drain", elapsed_ms,
                           total_values / (elapsed_ms / 1000.0)};
}

BenchmarkResult run_reclaiming_stack_mpmc_benchmark() {
    ReclaimingLockFreeStack<int> stack;
    const int total_values = kStackStressProducers * kStackBenchmarkValuesPerProducer;
    std::atomic<int> producers_done{0};
    std::atomic<int> consumed{0};

    const double elapsed_ms = measure_ms([&] {
        std::vector<std::thread> producers;
        producers.reserve(kStackStressProducers);
        for (int producer_id = 0; producer_id < kStackStressProducers; ++producer_id) {
            producers.emplace_back([&, producer_id] {
                const int begin = producer_id * kStackBenchmarkValuesPerProducer;
                const int end = begin + kStackBenchmarkValuesPerProducer;
                for (int value = begin; value < end; ++value) {
                    stack.push(value);
                }
                producers_done.fetch_add(1, std::memory_order_release);
            });
        }

        std::vector<std::thread> consumers;
        consumers.reserve(kStackStressConsumers);
        for (int consumer_id = 0; consumer_id < kStackStressConsumers; ++consumer_id) {
            consumers.emplace_back([&] {
                while (consumed.load(std::memory_order_acquire) < total_values) {
                    std::shared_ptr<int> value = stack.pop();
                    if (value) {
                        consumed.fetch_add(1, std::memory_order_release);
                        continue;
                    }

                    // 栈暂空且所有生产者已完成、消费达标才退出，避免误判提前收工。
                    if (producers_done.load(std::memory_order_acquire) == kStackStressProducers &&
                        consumed.load(std::memory_order_acquire) >= total_values) {
                        break;
                    }
                    std::this_thread::yield();
                }
            });
        }

        for (auto& producer : producers) {
            producer.join();
        }
        for (auto& consumer : consumers) {
            consumer.join();
        }
    });

    EXPECT_EQ(consumed.load(std::memory_order_acquire), total_values);
    return BenchmarkResult{"ReclaimStack_4P4C_push_pop", elapsed_ms,
                           total_values / (elapsed_ms / 1000.0)};
}

std::string format_publication_summary(const PublicationExperimentResult& result) {
    std::ostringstream out;
    out << "Day 43 publication experiment (iterations=" << result.iterations << ")\n";
    out << "  relaxed mismatches: " << result.relaxed_mismatches << "\n";
    out << "  release/acquire mismatches: " << result.release_acquire_mismatches << "\n";
    out << "  note: relaxed mismatch may remain 0 on x86, but the ordering is still not guaranteed "
           "by the model.\n";
    return out.str();
}

void print_benchmark(const BenchmarkResult& result) {
    std::cout << "  " << std::setw(28) << std::left << result.name << " " << std::setw(10)
              << std::right << result.elapsed_ms << " ms"
              << "  throughput=" << result.throughput << " ops/s\n";
}

}  // namespace

int main() {
    try {
        PublicationExperimentResult publication_result;
        test_day43_memory_order(publication_result);
        test_day44_lock_free_stack();
        test_day44_lock_free_stack_parallel_push();
        test_day45_spsc_queue();
        test_day46_aba_defense();
        test_day46_tagged_pointer_stack();
        test_day48_reclaiming_lock_free_stack();
        test_day48_reclaiming_lock_free_stack_parallel();

        const BenchmarkResult baseline = run_std_queue_baseline();
        const BenchmarkResult blocking = run_blocking_queue_benchmark();
        const BenchmarkResult bounded = run_bounded_queue_benchmark();
        const BenchmarkResult spsc = run_spsc_benchmark();
        const BenchmarkResult lock_free_stack_push_drain =
            run_lock_free_stack_push_drain_benchmark();
        const BenchmarkResult tagged_stack_push_drain =
            run_tagged_pointer_stack_push_drain_benchmark();
        const BenchmarkResult reclaim_stack_push_drain =
            run_reclaiming_stack_push_drain_benchmark();
        const BenchmarkResult reclaim_stack_mpmc = run_reclaiming_stack_mpmc_benchmark();

        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Week 07 lock-free practice validation passed.\n";
        std::cout << format_publication_summary(publication_result);
        std::cout << "Stack stress tests\n";
        std::cout << "  LockFreeStack skeleton: concurrent push + single-thread pop passed\n";
        std::cout << "  TaggedPointerStack prototype: single-thread push/pop passed\n";
        std::cout << "  ReclaimingLockFreeStack: concurrent push/pop passed\n";
        std::cout << "Stack benchmark ("
                  << (kStackStressProducers * kStackBenchmarkValuesPerProducer) << " values)\n";
        print_benchmark(lock_free_stack_push_drain);
        print_benchmark(tagged_stack_push_drain);
        print_benchmark(reclaim_stack_push_drain);
        print_benchmark(reclaim_stack_mpmc);
        std::cout << "Day 47 / Day 49 benchmark (" << kExchangeCount << " int handoffs, 1P1C)\n";
        print_benchmark(baseline);
        print_benchmark(blocking);
        print_benchmark(bounded);
        print_benchmark(spsc);
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Week 07 validation failed: " << ex.what() << "\n";
        return 1;
    }
}
