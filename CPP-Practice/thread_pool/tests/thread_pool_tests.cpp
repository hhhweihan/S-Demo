#include "concurrent/thread_pool.h"

#include <atomic>
#include <stdexcept>

#include <gtest/gtest.h>

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
