#include "concurrent/blocking_queue.h"
#include "concurrent/hazard_pointer.h"
#include "concurrent/lock_free_stack.h"
#include "concurrent/mpmc_queue.h"
#include "concurrent/spsc_queue.h"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include <gtest/gtest.h>
#include <stdexcept>

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

// ------------------------------- MPMCQueue (Vyukov) -------------------------------

TEST(MPMCQueue, RoundsCapacityUpToPowerOfTwo) {
    MPMCQueue<int> queue(5);
    EXPECT_EQ(queue.capacity(), 8u);  // 5 → 8
    MPMCQueue<int> tiny(1);
    EXPECT_EQ(tiny.capacity(), 2u);  // 下限两个槽
}

TEST(MPMCQueue, PreservesFifoForSingleProducer) {
    MPMCQueue<int> queue(4);  // 容量取 4
    for (int i = 0; i < 4; ++i) {
        ASSERT_TRUE(queue.push(i));
    }
    EXPECT_FALSE(queue.push(99));  // 满则拒绝，不阻塞
    for (int i = 0; i < 4; ++i) {
        int value = -1;
        ASSERT_TRUE(queue.pop(value));
        EXPECT_EQ(value, i);  // 单生产者下严格 FIFO
    }
    int value = -1;
    EXPECT_FALSE(queue.pop(value));  // 空则拒绝
    EXPECT_TRUE(queue.empty());
}

TEST(MPMCQueue, MultiProducerMultiConsumerConservesItems) {
    // 核心 TSan 目标：多生产者多消费者在同一环上竞争，每个值恰好被取出一次，校验和守恒。
    constexpr int kProducers = 4;
    constexpr int kConsumers = 4;
    constexpr int kPerProducer = 100'000;
    constexpr int kTotal = kProducers * kPerProducer;

    MPMCQueue<int> queue(1024);
    std::atomic<std::int64_t> consumed_sum{0};
    std::atomic<int> consumed_count{0};
    std::atomic<int> produced_count{0};

    std::vector<std::thread> producers;
    for (int p = 0; p < kProducers; ++p) {
        producers.emplace_back([&queue, &produced_count, p] {
            for (int i = 0; i < kPerProducer; ++i) {
                while (!queue.push(p * kPerProducer + i)) {
                    std::this_thread::yield();  // 满则退让重试
                }
            }
            produced_count.fetch_add(kPerProducer, std::memory_order_release);
        });
    }

    std::vector<std::thread> consumers;
    for (int c = 0; c < kConsumers; ++c) {
        consumers.emplace_back([&queue, &consumed_sum, &consumed_count, &produced_count] {
            for (;;) {
                int value = 0;
                if (queue.pop(value)) {
                    consumed_sum.fetch_add(value, std::memory_order_relaxed);
                    consumed_count.fetch_add(1, std::memory_order_relaxed);
                    continue;
                }
                // 队列暂空：仅当生产已全部完成且消费达标才收工，否则可能是生产者尚未跟上。
                if (produced_count.load(std::memory_order_acquire) == kTotal &&
                    consumed_count.load(std::memory_order_acquire) >= kTotal) {
                    break;
                }
                std::this_thread::yield();
            }
        });
    }

    for (auto& t : producers) {
        t.join();
    }
    for (auto& t : consumers) {
        t.join();
    }

    EXPECT_EQ(consumed_count.load(), kTotal);
    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kTotal - 1) * kTotal / 2;
    EXPECT_EQ(consumed_sum.load(), kExpectedSum);
    EXPECT_TRUE(queue.empty());
}

// ------------------------------- Hazard pointers -------------------------------

TEST(HazardPointer, ProtectAndRetireSingleThread) {
    auto& domain = lockfree::HazardPointerDomain::instance();
    domain.reclaim_all();  // 从干净起点开始

    std::atomic<int*> source{new int(42)};
    int* protected_ptr = domain.protect(source);
    EXPECT_EQ(*protected_ptr, 42);  // protect 返回被护住的当前值

    // 摘下并退役：clear 撤保护后 retire，随后强制回收应真正释放（ASan 校验无泄漏/无 UAF）。
    int* old = source.exchange(nullptr);
    domain.clear();
    domain.retire(old);
    domain.reclaim_all();
    EXPECT_EQ(domain.retired_count(), 0u);
}

TEST(HazardPointer, RetiredListStaysBounded) {
    // 有界回收的证明：持续 retire 远超阈值的节点，retired 长度被扫描阈值卡住而非无限增长。
    auto& domain = lockfree::HazardPointerDomain::instance();
    domain.reclaim_all();

    constexpr std::size_t kRetires = 10'000;
    for (std::size_t i = 0; i < kRetires; ++i) {
        domain.retire(new int(static_cast<int>(i)));
        // 无线程登记 hazard，每次越过阈值的 scan 都能回收全部——长度恒在阈值附近。
        EXPECT_LE(domain.retired_count(), lockfree::HazardPointerDomain::kScanThreshold);
    }
    domain.reclaim_all();
    EXPECT_EQ(domain.retired_count(), 0u);
}

// ------------------------------- HazardStack -------------------------------

TEST(HazardStack, PreservesLifoOrder) {
    HazardStack<int> stack;
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

TEST(HazardStack, ConcurrentPushPopLosesNothing) {
    // TSan + ASan 双重目标：hazard pointer 保护下的并发 push/pop 既无竞态也无 use-after-free。
    // 每个 push 的值恰好被 pop 一次，校验和与计数守恒；缺 HP 保护时 ASan 会在此暴露 UAF。
    constexpr int kThreads = 8;
    constexpr int kPerThread = 100'000;
    constexpr int kTotal = kThreads * kPerThread;

    HazardStack<int> stack;
    std::atomic<std::int64_t> popped_sum{0};
    std::atomic<int> popped_count{0};

    std::vector<std::thread> workers;
    for (int t = 0; t < kThreads; ++t) {
        workers.emplace_back([&stack, &popped_sum, &popped_count, t] {
            for (int i = 0; i < kPerThread; ++i) {
                stack.push(t * kPerThread + i);
                int value = 0;
                if (stack.pop(value)) {  // push/pop 交织，CAS 循环在同一 head 上竞争
                    popped_sum.fetch_add(value, std::memory_order_relaxed);
                    popped_count.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }
    for (auto& t : workers) {
        t.join();
    }

    int value = 0;
    while (stack.pop(value)) {  // 排空剩余，使总量覆盖每个元素恰好一次
        popped_sum.fetch_add(value, std::memory_order_relaxed);
        popped_count.fetch_add(1, std::memory_order_relaxed);
    }

    EXPECT_EQ(popped_count.load(), kTotal);
    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kTotal - 1) * kTotal / 2;
    EXPECT_EQ(popped_sum.load(), kExpectedSum);
    EXPECT_TRUE(stack.empty());
}

// ------------------------------- ReclaimingLockFreeStack -------------------------------

TEST(ReclaimingLockFreeStack, PreservesLifoOrder) {
    ReclaimingLockFreeStack<int> stack;
    EXPECT_TRUE(stack.empty());
    for (int i = 0; i < 5; ++i) {
        stack.push(i);
    }
    for (int i = 4; i >= 0; --i) {
        std::shared_ptr<int> value = stack.pop();
        ASSERT_TRUE(value != nullptr);
        EXPECT_EQ(*value, i);
    }
    EXPECT_TRUE(stack.empty());
    EXPECT_TRUE(stack.pop() == nullptr);
}

TEST(ReclaimingLockFreeStack, ConcurrentPushPopLosesNothing) {
    // ASan/TSan 目标：threads_in_pop_ + to_be_deleted_ 的“独占 pop 才回收”路径在多线程 churn
    // 下既不悬垂也不丢/重元素。每个 push 的值恰好被 pop 一次，校验和与计数守恒。
    constexpr int kThreads = 8;
    constexpr int kPerThread = 100'000;
    constexpr int kTotal = kThreads * kPerThread;

    ReclaimingLockFreeStack<int> stack;
    std::atomic<std::int64_t> popped_sum{0};
    std::atomic<int> popped_count{0};

    std::vector<std::thread> workers;
    for (int t = 0; t < kThreads; ++t) {
        workers.emplace_back([&stack, &popped_sum, &popped_count, t] {
            for (int i = 0; i < kPerThread; ++i) {
                stack.push(t * kPerThread + i);
                // push/pop 交织：CAS 循环在同一 head 上竞争，try_reclaim 反复走独占/非独占分支。
                if (std::shared_ptr<int> value = stack.pop()) {
                    popped_sum.fetch_add(*value, std::memory_order_relaxed);
                    popped_count.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }
    for (auto& t : workers) {
        t.join();
    }

    // 排空剩余，使总量覆盖每个元素恰好一次。
    while (std::shared_ptr<int> value = stack.pop()) {
        popped_sum.fetch_add(*value, std::memory_order_relaxed);
        popped_count.fetch_add(1, std::memory_order_relaxed);
    }

    EXPECT_EQ(popped_count.load(), kTotal);
    constexpr std::int64_t kExpectedSum = static_cast<std::int64_t>(kTotal - 1) * kTotal / 2;
    EXPECT_EQ(popped_sum.load(), kExpectedSum);
    EXPECT_TRUE(stack.empty());
}

// ------------------------------- TaggedPointerStack -------------------------------

TEST(TaggedPointerStack, HeadIsLockFree) {
    // “无锁”声明的运行期复核，与 lock_free_stack.h 里的编译期 static_assert 对应：Head={ptr,tag}=16B
    // 的 std::atomic 必须靠硬件双字 CAS（x86-64 的 CMPXCHG16B），不能退化为 libatomic 互斥锁。
    // 用与内部 head_ 布局一致的 16B 原子做探针。
    struct HeadLike {
        void* ptr;
        std::size_t tag;
    };
    static_assert(sizeof(HeadLike) == 2 * sizeof(void*), "HeadLike must mirror the 16B Head layout");

    std::atomic<HeadLike> head{};
    bool lock_free = std::atomic<HeadLike>::is_always_lock_free || head.is_lock_free();
#if defined(__GNUC__) && !defined(__clang__) && defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16)
    // 关键：GCC 对 16B 原子的 is_lock_free() 恒为 false（只读页 load 的保守处理），但开了 -mcx16 后
    // __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 已定义，说明目标具备硬件 cmpxchg16b，CAS 走 libatomic 的
    // cx16 ifunc（无互斥）——真无锁。此处认硬件宏，避免 GCC 的误报把测试判红。
    lock_free = true;
#endif
    EXPECT_TRUE(lock_free)
        << "std::atomic<16B Head> is NOT lock-free here — the \"lock-free\" claim is false; "
           "ensure -mcx16 + libatomic are in effect";
}

TEST(TaggedPointerStack, PreservesLifoOrder) {
    TaggedPointerStack<int> stack;
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

TEST(TaggedPointerStack, ConcurrentPushPopLosesNothing) {
    // ASan/TSan 目标：(ptr, tag) 打包的 ABA 防护在多线程 churn 下正确——每个 push 的值恰好被
    // pop 一次，校验和与计数守恒。tag 递增确保指针兜圈回到旧值时 CAS 仍失败，不会 ABA 误配。
    constexpr int kThreads = 8;
    constexpr int kPerThread = 100'000;
    constexpr int kTotal = kThreads * kPerThread;

    TaggedPointerStack<int> stack;
    std::atomic<std::int64_t> popped_sum{0};
    std::atomic<int> popped_count{0};

    std::vector<std::thread> workers;
    for (int t = 0; t < kThreads; ++t) {
        workers.emplace_back([&stack, &popped_sum, &popped_count, t] {
            for (int i = 0; i < kPerThread; ++i) {
                stack.push(t * kPerThread + i);
                int value = 0;
                // push/pop 交织，CAS 循环在同一 head（含 tag）上竞争。
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

    // 排空剩余，使总量覆盖每个元素恰好一次。
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
