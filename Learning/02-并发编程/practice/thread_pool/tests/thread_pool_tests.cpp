#include "concurrent/bounded_queue.h"
#include "concurrent/thread_pool.h"
#include "concurrent/work_stealing_deque.h"

#include <atomic>
#include <cstdint>
#include <functional>
#include <future>
#include <stdexcept>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

// ----------------------------- WorkStealingDeque -----------------------------

TEST(WorkStealingDeque, RoundsCapacityUpToPowerOfTwo) {
    WorkStealingDeque<int> deque(5);
    EXPECT_EQ(deque.capacity(), 8u);
    WorkStealingDeque<int> tiny(1);
    EXPECT_EQ(tiny.capacity(), 2u);
}

TEST(WorkStealingDeque, OwnerPopsLifo) {
    WorkStealingDeque<int> deque(8);
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(deque.push(i));
    }
    for (int i = 3; i >= 0; --i) {
        int value = -1;
        ASSERT_TRUE(deque.pop(value));
        EXPECT_EQ(value, i);  // owner 端 LIFO
    }
    int value = -1;
    EXPECT_FALSE(deque.pop(value));  // 空
}

TEST(WorkStealingDeque, ThiefStealsFifo) {
    WorkStealingDeque<int> deque(8);
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(deque.push(i));
    }
    for (int i = 0; i < 4; ++i) {
        int value = -1;
        ASSERT_TRUE(deque.steal(value));
        EXPECT_EQ(value, i);  // thief 端 FIFO（从最老一端偷）
    }
    int value = -1;
    EXPECT_FALSE(deque.steal(value));  // 空
}

TEST(WorkStealingDeque, RejectsPushWhenFull) {
    WorkStealingDeque<int> deque(2);
    EXPECT_TRUE(deque.push(1));
    EXPECT_TRUE(deque.push(2));
    EXPECT_FALSE(deque.push(3));  // 满：调用方应走全局 overflow
}

TEST(WorkStealingDeque, OwnerAndThievesConserveItems) {
    // owner 持续 push/pop，多个 thief 并发 steal，同一元素不能被取两次也不能丢。
    // 单元素与最后一个元素的 owner/thief 竞争由 top CAS 裁决——TSan 会盯这条路径。
    constexpr int kItems = 200'000;
    constexpr int kThieves = 4;
    WorkStealingDeque<int> deque(1024);

    std::atomic<std::int64_t> sum{0};
    std::atomic<int> count{0};
    std::atomic<bool> producing{true};

    std::vector<std::thread> thieves;
    for (int t = 0; t < kThieves; ++t) {
        thieves.emplace_back([&deque, &sum, &count, &producing] {
            int value = 0;
            while (producing.load(std::memory_order_acquire) || !deque.empty()) {
                if (deque.steal(value)) {
                    sum.fetch_add(value, std::memory_order_relaxed);
                    count.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }

    // owner 线程：push，偶尔自己 pop 一个（制造 owner/thief 在 bottom/top 两端的真实竞争）。
    std::thread owner([&deque, &sum, &count] {
        int value = 0;
        for (int i = 0; i < kItems; ++i) {
            while (!deque.push(i)) {
                // 满了：自己 pop 一个消化掉，腾出空间（也顺带算进总和）。
                if (deque.pop(value)) {
                    sum.fetch_add(value, std::memory_order_relaxed);
                    count.fetch_add(1, std::memory_order_relaxed);
                }
            }
            if ((i & 7) == 0 && deque.pop(value)) {
                sum.fetch_add(value, std::memory_order_relaxed);
                count.fetch_add(1, std::memory_order_relaxed);
            }
        }
        // 排空自己剩下的。
        while (deque.pop(value)) {
            sum.fetch_add(value, std::memory_order_relaxed);
            count.fetch_add(1, std::memory_order_relaxed);
        }
    });

    owner.join();
    producing.store(false, std::memory_order_release);
    for (auto& t : thieves) {
        t.join();
    }

    EXPECT_EQ(count.load(), kItems);
    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kItems - 1) * kItems / 2;
    EXPECT_EQ(sum.load(), kExpectedSum);
}

// ----------------------------- Work-stealing pool -----------------------------

TEST(ThreadPool, WorkStealingRunsNestedTasksExactlyOnce) {
    // worker 内部提交的子任务进各自的本地 deque，会被空闲 worker 偷走执行。
    // 每个根任务派生 kFanout 个子任务；总执行数必须恰好 = 根 + 子，一个不多一个不少。
    constexpr int kRoots = 500;
    constexpr int kFanout = 20;
    constexpr int kTotal = kRoots + kRoots * kFanout;

    std::atomic<int> executed{0};
    {
        ThreadPool pool(8);
        for (int r = 0; r < kRoots; ++r) {
            pool.submit([&pool, &executed] {
                executed.fetch_add(1, std::memory_order_relaxed);
                for (int c = 0; c < kFanout; ++c) {
                    // 从 worker 内部提交：进本 worker 的 deque，供他人 steal。
                    pool.submit([&executed] { executed.fetch_add(1, std::memory_order_relaxed); });
                }
            });
        }
        pool.await_termination();
    }
    EXPECT_EQ(executed.load(), kTotal);
}

TEST(ThreadPool, ReturnsFutureValue) {
    ThreadPool pool(4);
    auto future = pool.submit([](int a, int b) { return a + b; }, 2, 3);
    EXPECT_EQ(future.get(), 5);
}

TEST(ThreadPool, PropagatesExceptionThroughFuture) {
    ThreadPool pool(2);
    auto future = pool.submit([] { throw std::runtime_error("boom"); });
    // packaged_task stores the exception; it must resurface at the consumer, not crash a worker.
    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST(ThreadPool, RejectsEmptyTask) {
    ThreadPool pool(1);
    EXPECT_THROW(pool.submit(ThreadPool::Task{}), std::invalid_argument);
}

TEST(ThreadPool, RunsEverySubmittedTaskExactlyOnce) {
    // The interesting case under TSan: many workers racing on one counter. If the pool
    // dropped or double-ran a task, or await_termination returned early, the sum breaks.
    constexpr int kTasks = 10000;
    std::atomic<int> counter{0};
    {
        ThreadPool pool(8);
        for (int i = 0; i < kTasks; ++i) {
            pool.submit([&counter] { counter.fetch_add(1, std::memory_order_relaxed); });
        }
        pool.await_termination();
    }
    EXPECT_EQ(counter.load(), kTasks);
}

TEST(ThreadPool, SubmitAfterShutdownThrows) {
    ThreadPool pool(2);
    pool.shutdown();
    EXPECT_THROW(pool.submit([] {}), std::runtime_error);
}

TEST(ThreadPool, DynamicPoolGrowsUnderLoad) {
    // min<max enables the monitor thread; just assert it stays correct, not a specific size.
    ThreadPoolOptions options;
    options.min_threads = 2;
    options.max_threads = 8;
    ThreadPool pool(options);

    std::atomic<int> done{0};
    for (int i = 0; i < 2000; ++i) {
        pool.submit([&done] { done.fetch_add(1, std::memory_order_relaxed); });
    }
    pool.await_termination();
    EXPECT_EQ(done.load(), 2000);
}

// ----------------------------- BoundedQueue -----------------------------
// 从 lock_free/ 迁入：mutex + 双条件变量的带锁有界阻塞队列，与本模块其它带锁原语归堆。

TEST(BoundedQueue, RejectsZeroCapacity) {
    EXPECT_THROW(BoundedQueue<int>(0), std::invalid_argument);
}

TEST(BoundedQueue, TryPushRespectsCapacity) {
    BoundedQueue<int> queue(2);
    EXPECT_TRUE(queue.try_push(1));
    EXPECT_TRUE(queue.try_push(2));
    EXPECT_FALSE(queue.try_push(3));  // full

    int out = -1;
    ASSERT_TRUE(queue.try_pop(out));
    EXPECT_EQ(out, 1);
    EXPECT_EQ(queue.size(), 1u);
}

TEST(BoundedQueue, MultiProducerMultiConsumerConservesItems) {
    // MPMC blocking correctness: producers push a known range, consumers drain it,
    // and the summed checksum must match. shutdown() releases blocked consumers.
    constexpr int kProducers = 4;
    constexpr int kConsumers = 4;
    constexpr int kPerProducer = 50'000;
    constexpr int kTotal = kProducers * kPerProducer;

    BoundedQueue<int> queue(256);
    std::atomic<std::int64_t> consumed_sum{0};
    std::atomic<int> consumed_count{0};

    std::vector<std::thread> producers;
    for (int p = 0; p < kProducers; ++p) {
        producers.emplace_back([&queue, p] {
            for (int i = 0; i < kPerProducer; ++i) {
                queue.push(p * kPerProducer + i);
            }
        });
    }

    std::vector<std::thread> consumers;
    for (int c = 0; c < kConsumers; ++c) {
        consumers.emplace_back([&queue, &consumed_sum, &consumed_count] {
            int value = 0;
            while (queue.try_pop(value)) {
                consumed_sum.fetch_add(value, std::memory_order_relaxed);
                consumed_count.fetch_add(1, std::memory_order_relaxed);
            }
            // Drain via blocking pop until shutdown wakes us with an empty queue.
            try {
                for (;;) {
                    int v = queue.pop();
                    consumed_sum.fetch_add(v, std::memory_order_relaxed);
                    consumed_count.fetch_add(1, std::memory_order_relaxed);
                }
            } catch (const std::runtime_error&) {
                // Expected: pop on a shutdown, drained queue.
            }
        });
    }

    for (auto& t : producers) {
        t.join();
    }
    queue.shutdown();  // unblock consumers once all items are enqueued
    for (auto& t : consumers) {
        t.join();
    }

    EXPECT_EQ(consumed_count.load(), kTotal);
    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kTotal - 1) * kTotal / 2;
    EXPECT_EQ(consumed_sum.load(), kExpectedSum);
}
