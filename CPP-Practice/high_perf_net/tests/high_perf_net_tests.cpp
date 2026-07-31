#include "mini_high_perf_net.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <cstddef>
#include <stdexcept>

using mini_hpn::analyze_platform;
using mini_hpn::Backend;
using mini_hpn::ConnectionPool;
using mini_hpn::IoRequest;
using mini_hpn::IoUringSimulator;
using mini_hpn::LeastConnBalancer;
using mini_hpn::PrecisionTimerHeap;
using mini_hpn::RoundRobinBalancer;
using mini_hpn::TimerWheel;

TEST(TimerWheel, FiresTimerOnExactExpiryTick) {
    std::vector<int> fired;
    TimerWheel wheel(8, [&fired](int id) { fired.push_back(id); });
    wheel.add_or_refresh(1, 3);
    wheel.tick();  // now == 1
    wheel.tick();  // now == 2
    EXPECT_TRUE(fired.empty());
    wheel.tick();  // now == 3, timer expires
    ASSERT_EQ(fired.size(), 1u);
    EXPECT_EQ(fired.front(), 1);
    EXPECT_EQ(wheel.now(), std::size_t{3});
}

TEST(TimerWheel, RefreshPostponesExpiry) {
    // A stale slot entry from the first schedule must not fire once the timer is refreshed later.
    std::vector<int> fired;
    TimerWheel wheel(8, [&fired](int id) { fired.push_back(id); });
    wheel.add_or_refresh(1, 3);
    wheel.tick();                // now == 1
    wheel.add_or_refresh(1, 3);  // new expiry == 4
    wheel.tick();                // now == 2
    wheel.tick();                // now == 3, stale entry seen but timers_ says expiry 4
    EXPECT_TRUE(fired.empty());
    wheel.tick();  // now == 4, real expiry
    ASSERT_EQ(fired.size(), 1u);
    EXPECT_EQ(fired.front(), 1);
}

TEST(TimerWheel, FiresMultipleTimersInSlotInsertionOrder) {
    std::vector<int> fired;
    TimerWheel wheel(8, [&fired](int id) { fired.push_back(id); });
    wheel.add_or_refresh(10, 2);
    wheel.add_or_refresh(20, 2);
    wheel.tick();
    wheel.tick();
    ASSERT_EQ(fired.size(), 2u);
    EXPECT_EQ(fired[0], 10);
    EXPECT_EQ(fired[1], 20);
}

TEST(TimerWheel, CancelPreventsTimerFromFiring) {
    std::vector<int> fired;
    TimerWheel wheel(8, [&fired](int id) { fired.push_back(id); });
    wheel.add_or_refresh(1, 3);
    wheel.add_or_refresh(2, 3);
    EXPECT_TRUE(wheel.cancel(1));  // 取消存在的定时器返回 true
    wheel.tick();
    wheel.tick();
    wheel.tick();  // now == 3：两者原定在此到期，但 1 已取消
    ASSERT_EQ(fired.size(), 1u);
    EXPECT_EQ(fired.front(), 2);
}

TEST(TimerWheel, CancelUnknownKeyIsNoOp) {
    std::vector<int> fired;
    TimerWheel wheel(8, [&fired](int id) { fired.push_back(id); });
    wheel.add_or_refresh(1, 2);
    EXPECT_FALSE(wheel.cancel(99));  // 未知键：安全空操作，返回 false
    wheel.tick();
    wheel.tick();  // now == 2：未受影响的定时器照常触发
    ASSERT_EQ(fired.size(), 1u);
    EXPECT_EQ(fired.front(), 1);
}

TEST(TimerWheel, CancelThenReaddSchedulesFreshTimer) {
    std::vector<int> fired;
    TimerWheel wheel(8, [&fired](int id) { fired.push_back(id); });
    wheel.add_or_refresh(1, 2);
    EXPECT_TRUE(wheel.cancel(1));
    EXPECT_FALSE(wheel.cancel(1));  // 二次取消同一键已不存在
    wheel.add_or_refresh(1, 2);     // 重新加入，视为全新定时器
    wheel.tick();
    wheel.tick();  // now == 2
    ASSERT_EQ(fired.size(), 1u);
    EXPECT_EQ(fired.front(), 1);
}

TEST(PrecisionTimerHeap, PopsExpiredInAscendingOrder) {
    PrecisionTimerHeap heap;
    heap.add(1, 30);
    heap.add(2, 10);
    heap.add(3, 20);
    std::vector<int> due = heap.pop_expired(20);
    ASSERT_EQ(due.size(), 2u);
    EXPECT_EQ(due[0], 2);  // expire_at 10 comes before 20
    EXPECT_EQ(due[1], 3);
    std::vector<int> rest = heap.pop_expired(100);
    ASSERT_EQ(rest.size(), 1u);
    EXPECT_EQ(rest[0], 1);
}

TEST(PrecisionTimerHeap, NothingExpiresBeforeDeadline) {
    PrecisionTimerHeap heap;
    heap.add(1, 50);
    EXPECT_TRUE(heap.pop_expired(49).empty());
}

TEST(IoUringSimulator, SubmitThenProcessProducesFifoCompletions) {
    IoUringSimulator ring;
    ring.submit(IoRequest{1, "read", "a"});
    ring.submit(IoRequest{2, "write", "b"});
    EXPECT_EQ(ring.pending(), 2u);
    ring.process();
    EXPECT_EQ(ring.pending(), 0u);

    auto first = ring.peek_completion();
    ASSERT_TRUE(first.has_value());
    EXPECT_EQ(first->id, 1);
    EXPECT_EQ(first->result, "read:a");

    auto second = ring.peek_completion();
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(second->id, 2);
    EXPECT_EQ(second->result, "write:b");

    EXPECT_FALSE(ring.peek_completion().has_value());
}

TEST(ConnectionPool, RoundRobinAcquireTracksActiveAndReleaseDecrements) {
    ConnectionPool pool({Backend{"a"}, Backend{"b"}});
    std::string first = pool.acquire_round_robin();
    std::string second = pool.acquire_round_robin();
    EXPECT_NE(first, second);
    EXPECT_EQ(pool.active(first), 1);
    EXPECT_EQ(pool.active(second), 1);
    pool.release(first);
    EXPECT_EQ(pool.active(first), 0);
}

TEST(ConnectionPool, RoundRobinSkipsUnhealthyBackends) {
    ConnectionPool pool({Backend{"a"}, Backend{"b"}});
    pool.mark_unhealthy("a");
    EXPECT_EQ(pool.acquire_round_robin(), "b");
    EXPECT_EQ(pool.acquire_round_robin(), "b");
}

TEST(ConnectionPool, LeastConnPrefersFewestActive) {
    ConnectionPool pool({Backend{"a"}, Backend{"b"}});
    std::string first = pool.acquire_least_conn();   // both at 0, min_element picks "a"
    std::string second = pool.acquire_least_conn();  // "a" now busy, picks "b"
    EXPECT_EQ(first, "a");
    EXPECT_EQ(second, "b");
}

TEST(RoundRobinBalancer, ThrowsWhenNoHealthyBackend) {
    std::vector<Backend> backends{Backend{"a"}};
    backends[0].healthy = false;
    RoundRobinBalancer balancer(&backends);
    EXPECT_THROW(balancer.pick(), std::runtime_error);
}

TEST(LeastConnBalancer, ThrowsWhenNoHealthyBackend) {
    std::vector<Backend> backends{Backend{"a"}};
    backends[0].healthy = false;
    LeastConnBalancer balancer(&backends);
    EXPECT_THROW(balancer.pick(), std::runtime_error);
}

TEST(PlatformAnalysis, ReportsCapabilitiesByPlatform) {
    auto linux_report = analyze_platform(true);
    EXPECT_TRUE(linux_report.zero_copy_supported_by_os);
    EXPECT_TRUE(linux_report.reuse_port_supported_by_os);

    auto portable_report = analyze_platform(false);
    EXPECT_FALSE(portable_report.zero_copy_supported_by_os);
    EXPECT_FALSE(portable_report.reuse_port_supported_by_os);
}
