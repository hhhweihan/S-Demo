#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "mini_coroutine.h"

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

// co_await yield() 是挂起点：协程在此让出，两次 ++value 之间调度器可穿插运行其他任务。
mini_coro::Task increment_twice(int& value) {
    ++value;
    co_await mini_coro::yield();
    ++value;
}

mini_coro::Task produce(mini_coro::Channel<int>& channel) {
    channel.send(1);
    co_await mini_coro::yield();  // 让出后恢复，模拟异步生产的时间间隔
    channel.send(2);
}

void test_task_scheduler() {
    mini_coro::Scheduler scheduler;
    mini_coro::SchedulerScope scope(scheduler);
    int value = 0;
    auto task1 = increment_twice(value);
    auto task2 = increment_twice(value);
    task1.start(scheduler);
    task2.start(scheduler);
    scheduler.run();
    // 两任务各 +2；能得到 4 说明 yield 的挂起/重排队 + 调度器循环恢复都正确。
    expect(value == 4, "Scheduler resumes multiple coroutine Tasks through yield");
}

void test_channel_select() {
    mini_coro::Scheduler scheduler;
    mini_coro::SchedulerScope scope(scheduler);
    mini_coro::Channel<int> first(2);
    mini_coro::Channel<int> second(2);
    second.send(42);
    // select_recv 按 vector 顺序取第一个就绪通道：first 空、second 有值，应命中 second。
    std::vector<mini_coro::Channel<int>*> channels{&first, &second};
    expect(mini_coro::select_recv(channels).value_or(0) == 42,
           "select_recv receives from the first ready Channel");

    auto task = produce(first);
    task.start(scheduler);
    scheduler.run();
    expect(first.recv().value_or(0) == 1 && first.recv().value_or(0) == 2,
           "Buffered Channel transports coroutine messages");
}

void test_async_http_core() {
    mini_coro::Scheduler scheduler;
    mini_coro::SchedulerScope scope(scheduler);
    mini_coro::AsyncHttpClientCore client;
    mini_coro::Channel<std::string> results(4);
    // 两个请求协程并发挂起/恢复，最终都把响应写回同一 results 通道。
    auto a = client.get("/a", results);
    auto b = client.get("/b", results);
    a.start(scheduler);
    b.start(scheduler);
    scheduler.run();
    expect(results.size() == 2, "AsyncHttpClientCore completes multiple coroutine requests");
}

}  // namespace

int main() {
    std::cout << "== Month 10 coroutine library demo ==\n";
    test_task_scheduler();
    test_channel_select();
    test_async_http_core();
    std::cout << "All Month 10 coroutine library tests passed.\n";
    return 0;
}
