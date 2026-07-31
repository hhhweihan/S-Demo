#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <algorithm>
#include "mini_raft_kv.h"

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

void test_leader_election() {
    mini_raft::RaftCluster cluster(3);
    // 3 节点多数派为 2：候选者自投 1 票 + 至少 1 张同意票即可当选。
    expect(cluster.elect(0), "3-node cluster elects a leader by majority vote");
    expect(cluster.leader_id() == 0 && cluster.node(0).role() == mini_raft::Role::Leader,
           "Elected node becomes Leader");
}

void test_log_replication_kv() {
    mini_raft::RaftCluster cluster(3);
    cluster.elect(0);
    expect(cluster.put("x", "1"), "Leader replicates a put entry to majority");
    expect(cluster.get("x").value_or("") == "1",
           "Committed entry applies to leader KV state machine");
    // 状态机安全性：Follower 只在日志被提交后才应用，此处验证提交项已下推到 Follower。
    expect(cluster.node(1).store().get("x").value_or("") == "1",
           "Follower applies committed log entry");
    expect(cluster.erase("x"), "Delete operation replicates through Raft log");
    expect(!cluster.get("x"), "Delete operation applies to KV state machine");
}

void test_leader_crash_reelect() {
    mini_raft::RaftCluster cluster(3);
    cluster.elect(0);
    cluster.put("before", "ok");
    cluster.crash_leader();
    // 3 节点挂 1 仍余 2 = 多数派，集群可选出新 Leader 并继续服务（容忍 1 故障的边界）。
    expect(cluster.elect(1), "Remaining two nodes elect a new leader after leader crash");
    expect(cluster.put("after", "ok"), "Cluster continues writing with majority alive");
    expect(cluster.get("after").value_or("") == "ok", "New leader applies writes after failover");
}

void test_persist_snapshot() {
    const auto dir = std::filesystem::path("CPP-Practice/raft_kv/build/state");
    std::filesystem::remove_all(dir);  // 清掉上轮残留，保证从空状态起测
    mini_raft::RaftCluster cluster(3);
    cluster.elect(0);
    cluster.put("snap", "value");
    cluster.node(0).persist(dir / "node0.wal");

    // 崩溃恢复：新节点仅凭 WAL 重放即应还原出已提交的 KV 状态。
    mini_raft::RaftNode recovered(0);
    recovered.recover(dir / "node0.wal");
    expect(recovered.store().get("snap").value_or("") == "value",
           "Node recovers committed KV state from WAL-like log");

    // 快照安装：落后过多的 Follower 直接接收状态机快照，而非逐条补日志。
    auto snapshot = recovered.snapshot();
    mini_raft::RaftNode follower(2);
    follower.install_snapshot(snapshot, recovered.commit_index());
    expect(follower.store().get("snap").value_or("") == "value",
           "Snapshot installs KV state on follower");
}

template <typename Fn>
double elapsed_ms(Fn&& fn) {
    auto begin = std::chrono::steady_clock::now();  // steady_clock：单调，不受墙钟调整干扰
    fn();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

void test_simulated_acceptance() {
    std::mt19937 rng(20260702);  // 固定种子：随机选举超时可复现（离散模拟的核心诉求）
    std::uniform_int_distribution<int> timeout_ms(150, 300);  // Raft 论文推荐的选举超时区间
    std::vector<int> timeouts{timeout_ms(rng), timeout_ms(rng), timeout_ms(rng)};
    // 随机化超时的目的：最早超时者先发起选举，降低多候选者同时竞争导致的分票。
    auto winner =
        static_cast<int>(std::min_element(timeouts.begin(), timeouts.end()) - timeouts.begin());
    expect(timeouts[winner] >= 150 && timeouts[winner] <= 300,
           "Simulated randomized election timeout stays in 150-300ms window");

    mini_raft::RaftCluster cluster(3);
    expect(cluster.elect(winner), "Simulated timeout winner can become leader");
    cluster.crash_leader();
    double failover_ms = elapsed_ms(
        [&] { expect(cluster.elect((winner + 1) % 3), "Simulated failover elects a new leader"); });
    expect(failover_ms < 500.0,
           "Simulated failover election completes under 500ms wall-clock proxy");

    constexpr int operations = 5000;
    double kv_ms = elapsed_ms([&] {
        for (int i = 0; i < operations; ++i) {
            cluster.put("k" + std::to_string(i), "v" + std::to_string(i));
            auto value = cluster.get("k" + std::to_string(i));
            if (!value || *value != "v" + std::to_string(i))
                std::exit(1);  // 写后即读，验证线性一致的读语义
        }
    });
    double kv_avg_ms = kv_ms / operations;
    expect(kv_avg_ms < 20.0, "Simulated RaftKV Put/Get average latency is under 20ms");

    const auto dir = std::filesystem::path("CPP-Practice/raft_kv/build/leveldb_like");
    std::filesystem::remove_all(dir);
    cluster.node(cluster.leader_id()).persist(dir / "leader.logdb");
    mini_raft::RaftNode recovered(cluster.leader_id());
    recovered.recover(dir / "leader.logdb");
    expect(recovered.store().get("k4999").value_or("") == "v4999",
           "LevelDB-like persisted state recovers the latest committed key");

    std::cout << "[BENCH] timeout_ms={" << timeouts[0] << ',' << timeouts[1] << ',' << timeouts[2]
              << "}, failover_ms=" << failover_ms << ", put_get_avg_ms=" << kv_avg_ms << '\n';
}

}  // namespace

int main() {
    std::cout << "== Month 12 RaftKV demo ==\n";
    test_leader_election();
    test_log_replication_kv();
    test_leader_crash_reelect();
    test_persist_snapshot();
    test_simulated_acceptance();
    std::cout << "All Month 12 RaftKV tests passed.\n";
    return 0;
}
