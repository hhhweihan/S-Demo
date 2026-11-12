#include "mini_coroutine.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

using mini_coro::AsyncHttpClientCore;
using mini_coro::Channel;
using mini_coro::Scheduler;
using mini_coro::SchedulerScope;
using mini_coro::Task;

namespace {

// Writes a value once resumed; proves a queued Task actually runs to completion.
Task produce_value(int& out, int value) {
    out = value;
    co_return;
}

// Two-phase task: increments, yields back to the scheduler, then increments again.
Task cooperative(std::vector<int>& log, int id) {
    log.push_back(id);
    co_await mini_coro::yield();
    log.push_back(id + 100);
}

}  // namespace

TEST(Scheduler, RunsQueuedTaskToCompletion) {
    Scheduler scheduler;
    int result = 0;
    Task task = produce_value(result, 42);
    EXPECT_FALSE(task.done());  // initial_suspend keeps it parked until scheduled
    task.start(scheduler);
    scheduler.run();
    EXPECT_EQ(result, 42);
    EXPECT_TRUE(task.done());
    EXPECT_EQ(scheduler.pending(), 0u);
}

TEST(Scheduler, YieldRequeuesAndInterleavesTasks) {
    // Cooperative tasks must round-robin: both first halves run before either second half.
    Scheduler scheduler;
    SchedulerScope scope(scheduler);  // yield() reaches the scheduler via thread-local pointer
    std::vector<int> log;
    Task a = cooperative(log, 1);
    Task b = cooperative(log, 2);
    a.start(scheduler);
    b.start(scheduler);
    scheduler.run();
    ASSERT_EQ(log.size(), 4u);
    EXPECT_EQ(log[0], 1);
    EXPECT_EQ(log[1], 2);
    EXPECT_EQ(log[2], 101);
    EXPECT_EQ(log[3], 102);
}

TEST(Channel, BufferedSendReceivePreservesFifoOrder) {
    Channel<int> channel(4);
    EXPECT_TRUE(channel.send(1));
    EXPECT_TRUE(channel.send(2));
    EXPECT_TRUE(channel.send(3));
    EXPECT_EQ(channel.size(), 3u);
    EXPECT_EQ(*channel.recv(), 1);
    EXPECT_EQ(*channel.recv(), 2);
    EXPECT_EQ(*channel.recv(), 3);
    EXPECT_FALSE(channel.recv().has_value());
    EXPECT_TRUE(channel.empty());
}

TEST(Channel, RejectsSendWhenBufferFull) {
    Channel<int> channel(2);
    EXPECT_TRUE(channel.send(1));
    EXPECT_TRUE(channel.send(2));
    EXPECT_FALSE(channel.send(3));  // capacity reached
    EXPECT_EQ(channel.size(), 2u);
}

TEST(Channel, ZeroCapacityHoldsSingleValue) {
    Channel<int> channel;  // capacity 0: one pending value at a time
    EXPECT_EQ(channel.capacity(), 0u);
    EXPECT_TRUE(channel.send(7));
    EXPECT_FALSE(channel.send(8));
    EXPECT_EQ(*channel.recv(), 7);
    EXPECT_TRUE(channel.send(8));  // slot freed after receive
}

TEST(Channel, SelectRecvReturnsFirstAvailableInOrder) {
    Channel<int> empty_channel(2);
    Channel<int> ready_channel(2);
    ready_channel.send(99);
    std::vector<Channel<int>*> channels{&empty_channel, &ready_channel};
    auto value = mini_coro::select_recv(channels);
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(*value, 99);
}

TEST(AsyncHttpClient, GetCoroutineWritesResponseToChannel) {
    Scheduler scheduler;
    SchedulerScope scope(scheduler);  // async_sleep awaiter reschedules via the current scheduler
    Channel<std::string> out(1);
    AsyncHttpClientCore client;
    Task task = client.get("http://example.com", out);
    task.start(scheduler);
    scheduler.run();
    auto response = out.recv();
    ASSERT_TRUE(response.has_value());
    EXPECT_EQ(*response, "GET http://example.com -> 200 OK");
}
