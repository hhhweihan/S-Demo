#include "concurrent/blocking_queue.h"
#include "concurrent/bounded_queue.h"
#include "concurrent/lock_free_stack.h"
#include "concurrent/spsc_queue.h"

#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

TEST(SPSCQueue, PreservesFifoOrder) {
    SPSCQueue<int> queue(8);
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE(queue.push(i));
    }
    for (int i = 0; i < 5; ++i) {
        int value = -1;
        ASSERT_TRUE(queue.pop(value));
        EXPECT_EQ(value, i);
    }
    EXPECT_TRUE(queue.empty());
}

TEST(SPSCQueue, RejectsPushWhenFull) {
    // Effective capacity is the requested count; one slot is reserved as a sentinel.
    SPSCQueue<int> queue(3);
    EXPECT_EQ(queue.capacity(), 3u);
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(queue.push(i));
    }
    EXPECT_FALSE(queue.push(99));

    int value = -1;
    ASSERT_TRUE(queue.pop(value));
    EXPECT_EQ(value, 0);
    // A freed slot must become writable again.
    EXPECT_TRUE(queue.push(99));
}

TEST(SPSCQueue, PopFailsOnEmpty) {
    SPSCQueue<int> queue(4);
    int value = -1;
    EXPECT_FALSE(queue.pop(value));
}

TEST(SPSCQueue, ProducerConsumerMovesEveryItem) {
    // The core TSan target: a single producer and single consumer racing on the
    // ring indices. Every item must be delivered exactly once and in order.
    constexpr int kItems = 1'000'000;
    SPSCQueue<int> queue(1024);

    std::thread producer([&queue] {
        for (int i = 0; i < kItems; ++i) {
            while (!queue.push(i)) {
                std::this_thread::yield();  // spin until the consumer frees a slot
            }
        }
    });

    std::int64_t sum = 0;
    int expected = 0;
    std::thread consumer([&queue, &sum, &expected] {
        int received = 0;
        int value = 0;
        while (received < kItems) {
            if (queue.pop(value)) {
                EXPECT_EQ(value, expected);  // FIFO holds for a single consumer
                ++expected;
                sum += value;
                ++received;
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kItems - 1) * kItems / 2;
    EXPECT_EQ(sum, kExpectedSum);
    EXPECT_TRUE(queue.empty());
}

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

TEST(BlockingQueue, TryPopFailsWhenEmpty) {
    BlockingQueue<int> queue;
    int out = -1;
    EXPECT_FALSE(queue.try_pop(out));
    queue.push(7);
    ASSERT_TRUE(queue.try_pop(out));
    EXPECT_EQ(out, 7);
}

TEST(BlockingQueue, MultiProducerMultiConsumerConservesItems) {
    constexpr int kProducers = 4;
    constexpr int kConsumers = 4;
    constexpr int kPerProducer = 50'000;
    constexpr int kTotal = kProducers * kPerProducer;

    BlockingQueue<int> queue;
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
            try {
                for (;;) {
                    int v = queue.pop();
                    consumed_sum.fetch_add(v, std::memory_order_relaxed);
                    consumed_count.fetch_add(1, std::memory_order_relaxed);
                }
            } catch (const std::runtime_error&) {
                // Expected once the queue is shut down and drained.
            }
        });
    }

    for (auto& t : producers) {
        t.join();
    }
    queue.shutdown();
    for (auto& t : consumers) {
        t.join();
    }

    EXPECT_EQ(consumed_count.load(), kTotal);
    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kTotal - 1) * kTotal / 2;
    EXPECT_EQ(consumed_sum.load(), kExpectedSum);
}

TEST(LockFreeStack, PreservesLifoOrder) {
    LockFreeStack<int> stack;
    EXPECT_TRUE(stack.empty());
    for (int i = 0; i < 5; ++i) {
        stack.push(i);
    }
    for (int i = 4; i >= 0; --i) {
        int value = -1;
        ASSERT_TRUE(stack.pop(value));
        EXPECT_EQ(value, i);
    }
    EXPECT_TRUE(stack.empty());
    int value = -1;
    EXPECT_FALSE(stack.pop(value));
}

TEST(LockFreeStack, ConcurrentPushPopLosesNothing) {
    // The key TSan target: many threads pushing and popping the same stack. Every
    // pushed value must be popped exactly once, so the popped checksum and count
    // must equal what was pushed. Non-popped remnants are drained at the end.
    constexpr int kThreads = 8;
    constexpr int kPerThread = 100'000;
    constexpr int kTotal = kThreads * kPerThread;

    LockFreeStack<int> stack;
    std::atomic<std::int64_t> popped_sum{0};
    std::atomic<int> popped_count{0};

    std::vector<std::thread> workers;
    for (int t = 0; t < kThreads; ++t) {
        workers.emplace_back([&stack, &popped_sum, &popped_count, t] {
            for (int i = 0; i < kPerThread; ++i) {
                stack.push(t * kPerThread + i);
                int value = 0;
                // Interleave pops so push/pop CAS loops race on the same head.
                if (stack.pop(value)) {
                    popped_sum.fetch_add(value, std::memory_order_relaxed);
                    popped_count.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }
    for (auto& t : workers) {
        t.join();
    }

    // Drain anything left so the totals cover every pushed element exactly once.
    int value = 0;
    while (stack.pop(value)) {
        popped_sum.fetch_add(value, std::memory_order_relaxed);
        popped_count.fetch_add(1, std::memory_order_relaxed);
    }

    EXPECT_EQ(popped_count.load(), kTotal);
    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kTotal - 1) * kTotal / 2;
    EXPECT_EQ(popped_sum.load(), kExpectedSum);
    EXPECT_TRUE(stack.empty());
}
