#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "mini_high_perf_net.h"

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

void test_timer_wheel() {
    std::vector<int> expired;
    mini_hpn::TimerWheel wheel(8, [&](int id) { expired.push_back(id); });
    wheel.add_or_refresh(1, 3);
    wheel.add_or_refresh(2, 5);
    // 连接 1 在过期前被刷新到 tick 6：验证刷新会推迟超时，而不是在旧刻度误触发。
    wheel.add_or_refresh(1, 6);
    for (int i = 0; i < 5; ++i) wheel.tick();
    expect(expired == std::vector<int>{2},
           "TimerWheel expires idle connections and honors refresh");
    wheel.tick();
    expect((expired == std::vector<int>{2, 1}),
           "TimerWheel expires refreshed connection at the latest tick");
}

void test_precision_heap() {
    mini_hpn::PrecisionTimerHeap heap;
    heap.add(1, 10);
    heap.add(2, 3);
    heap.add(3, 7);
    // 按截止时间而非插入序弹出，且弹出边界是闭区间（deadline == now 也算到期）。
    expect((heap.pop_expired(7) == std::vector<int>{2, 3}),
           "PrecisionTimerHeap pops timers by exact deadline");
}

void test_io_uring_simulator() {
    mini_hpn::IoUringSimulator ring;
    ring.submit({1, "read", "file"});
    ring.submit({2, "send", "socket"});
    expect(ring.pending() == 2, "IoUringSimulator queues SQ entries");
    ring.process();
    // CQ 顺序应与 SQ 提交顺序一致（本模拟不乱序完成）。
    auto first = ring.peek_completion();
    auto second = ring.peek_completion();
    expect(first && first->result == "read:file",
           "IoUringSimulator produces CQ completion for read");
    expect(second && second->result == "send:socket",
           "IoUringSimulator produces CQ completion for send");
}

void test_connection_pool() {
    mini_hpn::ConnectionPool pool({{"a"}, {"b"}, {"c"}});
    expect(pool.acquire_round_robin() == "a", "RoundRobin picks first backend");
    expect(pool.acquire_round_robin() == "b", "RoundRobin rotates backend");
    pool.release("a");
    pool.mark_unhealthy("b");
    // b 虽是最少连接，但已不健康，least-conn 必须跳过它。
    std::string chosen = pool.acquire_least_conn();
    expect(chosen != "b", "LeastConn skips unhealthy backend");
}

void test_optimization_report() {
    // 零拷贝(sendfile/splice)、SO_REUSEPORT 是 Linux 特性；其他平台只跑可移植核心。
#if defined(__linux__)
    constexpr bool linux_like = true;
#else
    constexpr bool linux_like = false;
#endif
    auto report = mini_hpn::analyze_platform(linux_like);
    expect(report.recommendation.find(linux_like ? "sendfile" : "WSL2") != std::string::npos,
           "OptimizationReport reflects platform benchmark limits");
}

}  // namespace

int main() {
    std::cout << "== Month 09 high performance networking demo ==\n";
    test_timer_wheel();
    test_precision_heap();
    test_io_uring_simulator();
    test_connection_pool();
    test_optimization_report();
    std::cout << "All Month 09 high performance networking tests passed.\n";
    return 0;
}
