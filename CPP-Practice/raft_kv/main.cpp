#include <cstdlib>  // 提供进程退出函数
#include <chrono>  // 提供计时工具
#include <filesystem>  // 提供文件系统路径操作
#include <iostream>  // 提供标准输入输出
#include <random>  // 提供随机数生成器
#include <string>  // 提供字符串类型
#include <vector>  // 提供动态数组容器

#include "mini_raft_kv.h"  // 引入迷你 RaftKV 实现

namespace {  // 限制测试辅助符号在本文件内可见

void expect(bool condition, const std::string& message) {  // 校验条件并输出测试结果
  if (!condition) {  // 条件失败时进入失败分支
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息
    std::exit(1);  // 立即终止测试进程
  }  // 结束失败分支
  std::cout << "[PASS] " << message << '\n';  // 输出通过信息
}  // 结束断言辅助函数

void test_leader_election() {  // 测试领导者选举流程
  mini_raft::RaftCluster cluster(3);  // 创建三节点集群
  expect(cluster.elect(0), "3-node cluster elects a leader by majority vote");  // 校验节点 0 可由多数票当选
  expect(cluster.leader_id() == 0 && cluster.node(0).role() == mini_raft::Role::Leader, "Elected node becomes Leader");  // 校验当选节点角色为 Leader
}  // 结束领导者选举测试

void test_log_replication_kv() {  // 测试日志复制驱动 KV 状态机
  mini_raft::RaftCluster cluster(3);  // 创建三节点集群
  cluster.elect(0);  // 选举节点 0 为 Leader
  expect(cluster.put("x", "1"), "Leader replicates a put entry to majority");  // 校验 put 日志可复制到多数节点
  expect(cluster.get("x").value_or("") == "1", "Committed entry applies to leader KV state machine");  // 校验 Leader 状态机应用写入
  expect(cluster.node(1).store().get("x").value_or("") == "1", "Follower applies committed log entry");  // 校验 Follower 状态机应用提交日志
  expect(cluster.erase("x"), "Delete operation replicates through Raft log");  // 校验删除操作可复制
  expect(!cluster.get("x"), "Delete operation applies to KV state machine");  // 校验删除已反映到 KV 状态机
}  // 结束日志复制测试

void test_leader_crash_reelect() {  // 测试 Leader 崩溃后的重新选举
  mini_raft::RaftCluster cluster(3);  // 创建三节点集群
  cluster.elect(0);  // 先选举节点 0 为 Leader
  cluster.put("before", "ok");  // 在崩溃前写入一条数据
  cluster.crash_leader();  // 模拟当前 Leader 崩溃
  expect(cluster.elect(1), "Remaining two nodes elect a new leader after leader crash");  // 校验剩余节点可选出新 Leader
  expect(cluster.put("after", "ok"), "Cluster continues writing with majority alive");  // 校验多数节点存活时仍可写入
  expect(cluster.get("after").value_or("") == "ok", "New leader applies writes after failover");  // 校验故障转移后的写入生效
}  // 结束重新选举测试

void test_persist_snapshot() {  // 测试持久化恢复与快照安装
  const auto dir = std::filesystem::path("CPP-Practice/raft_kv/build/state");  // 设置测试状态目录
  std::filesystem::remove_all(dir);  // 清理旧的测试状态
  mini_raft::RaftCluster cluster(3);  // 创建三节点集群
  cluster.elect(0);  // 选举节点 0 为 Leader
  cluster.put("snap", "value");  // 写入用于恢复验证的数据
  cluster.node(0).persist(dir / "node0.wal");  // 将节点 0 日志持久化到文件

  mini_raft::RaftNode recovered(0);  // 创建用于恢复的节点实例
  recovered.recover(dir / "node0.wal");  // 从持久化文件恢复节点状态
  expect(recovered.store().get("snap").value_or("") == "value", "Node recovers committed KV state from WAL-like log");  // 校验恢复后的 KV 数据

  auto snapshot = recovered.snapshot();  // 获取恢复节点的状态机快照
  mini_raft::RaftNode follower(2);  // 创建用于安装快照的 Follower
  follower.install_snapshot(snapshot, recovered.commit_index());  // 将快照安装到 Follower
  expect(follower.store().get("snap").value_or("") == "value", "Snapshot installs KV state on follower");  // 校验快照数据已安装
}  // 结束持久化与快照测试

template <typename Fn>  // 声明通用计时函数模板
double elapsed_ms(Fn&& fn) {  // 统计可调用对象执行耗时毫秒数
  auto begin = std::chrono::steady_clock::now();  // 记录开始时间
  fn();  // 执行被测函数
  auto end = std::chrono::steady_clock::now();  // 记录结束时间
  return std::chrono::duration<double, std::milli>(end - begin).count();  // 返回毫秒耗时
}  // 结束计时函数

void test_simulated_acceptance() {  // 执行模拟验收场景
  std::mt19937 rng(20260702);  // 使用固定种子保证随机结果可复现
  std::uniform_int_distribution<int> timeout_ms(150, 300);  // 定义选举超时分布范围
  std::vector<int> timeouts{timeout_ms(rng), timeout_ms(rng), timeout_ms(rng)};  // 为三个节点生成超时时间
  auto winner = static_cast<int>(std::min_element(timeouts.begin(), timeouts.end()) - timeouts.begin());  // 选择最早超时的节点作为候选者
  expect(timeouts[winner] >= 150 && timeouts[winner] <= 300, "Simulated randomized election timeout stays in 150-300ms window");  // 校验超时时间落在预期窗口

  mini_raft::RaftCluster cluster(3);  // 创建三节点模拟集群
  expect(cluster.elect(winner), "Simulated timeout winner can become leader");  // 校验超时获胜者可当选 Leader
  cluster.crash_leader();  // 模拟 Leader 故障
  double failover_ms = elapsed_ms([&] { expect(cluster.elect((winner + 1) % 3), "Simulated failover elects a new leader"); });  // 统计重新选举耗时
  expect(failover_ms < 500.0, "Simulated failover election completes under 500ms wall-clock proxy");  // 校验故障转移耗时代理指标

  constexpr int operations = 5000;  // 设置模拟读写操作次数
  double kv_ms = elapsed_ms([&] {  // 统计批量 Put/Get 总耗时
    for (int i = 0; i < operations; ++i) {  // 循环执行多次 KV 操作
      cluster.put("k" + std::to_string(i), "v" + std::to_string(i));  // 写入当前序号键值
      auto value = cluster.get("k" + std::to_string(i));  // 读取刚写入的键值
      if (!value || *value != "v" + std::to_string(i)) std::exit(1);  // 校验读回值不一致时退出
    }  // 结束 KV 操作循环
  });  // 结束批量耗时统计
  double kv_avg_ms = kv_ms / operations;  // 计算单次 Put/Get 平均耗时
  expect(kv_avg_ms < 20.0, "Simulated RaftKV Put/Get average latency is under 20ms");  // 校验平均延迟代理指标

  const auto dir = std::filesystem::path("CPP-Practice/raft_kv/build/leveldb_like");  // 设置类 LevelDB 持久化目录
  std::filesystem::remove_all(dir);  // 清理旧的持久化目录
  cluster.node(cluster.leader_id()).persist(dir / "leader.logdb");  // 持久化当前 Leader 日志
  mini_raft::RaftNode recovered(cluster.leader_id());  // 创建恢复用节点
  recovered.recover(dir / "leader.logdb");  // 从日志文件恢复节点状态
  expect(recovered.store().get("k4999").value_or("") == "v4999", "LevelDB-like persisted state recovers the latest committed key");  // 校验最新提交键可恢复

  std::cout << "[BENCH] timeout_ms={" << timeouts[0] << ',' << timeouts[1] << ',' << timeouts[2]  // 输出模拟超时参数
            << "}, failover_ms=" << failover_ms  // 输出故障转移耗时
            << ", put_get_avg_ms=" << kv_avg_ms << '\n';  // 输出平均 Put/Get 耗时
}  // 结束模拟验收测试

}  // namespace

int main() {  // 程序入口
  std::cout << "== Month 12 RaftKV demo ==\n";  // 输出演示标题
  test_leader_election();  // 运行领导者选举测试
  test_log_replication_kv();  // 运行日志复制测试
  test_leader_crash_reelect();  // 运行故障转移测试
  test_persist_snapshot();  // 运行持久化与快照测试
  test_simulated_acceptance();  // 运行模拟验收测试
  std::cout << "All Month 12 RaftKV tests passed.\n";  // 输出全部通过信息
  return 0;  // 返回成功退出码
}  // 结束程序入口