# 项目：Mini Raft KV

## 模块目标

基于你已有的 Raft 笔记，从零实现一个 3 节点 Raft 集群，上层提供 KV 存储接口。
这是整条进阶路线的收官项目，也是最有含金量的综合产出。

## 系统架构

```text
客户端
    │ Put/Get/Delete（HTTP 或自定义协议）
    ▼
RaftNode（3 个节点）
    ├── RaftStateMachine     ← 状态机（Follower / Candidate / Leader）
    ├── RaftLog              ← 日志存储（内存 + WAL 持久化）
    ├── RpcServer            ← 节点间通信（RPC 框架，模块 11）
    ├── ElectionTimer        ← 选举超时计时器
    └── KVStore              ← 状态机应用层（LevelDB 持久化，模块 07）
```

## Raft 核心协议回顾

```text
角色迁移：
Follower ─[超时]→ Candidate ─[赢得多数票]→ Leader
         ←[收到心跳]──────────────────────

Leader 职责：
1. 定期发送心跳（AppendEntries with no entries）
2. 收到客户端写请求：追加到本地日志 → 发 AppendEntries 给所有 Follower
3. 多数节点确认 → commit → 应用到状态机 → 响应客户端

选举条件：
- Follower 超时未收到心跳 → 变为 Candidate，term++，给自己投票，请求其他节点投票
- 拿到多数票（>=2/3 节点）→ 成为 Leader
```

## 阶段拆解

| 阶段 | 主题 | 目录 |
| --- | --- | --- |
| 阶段 45 | Leader 选举 | [45-LeaderElection](./45-LeaderElection/) |
| 阶段 46 | 日志复制 | [46-LogReplication](./46-LogReplication/) |
| 阶段 47 | KV 状态机 + 持久化 | [47-KVStateMachine](./47-KVStateMachine/) |
| 阶段 48 | 快照 + 稳定性测试 + 收尾 | [48-Snapshot-Finish](./48-Snapshot-Finish/) |
| 阶段 49 | 生产化：持久化 + 快照/InstallSnapshot + ReadIndex + Pre-Vote | [49-Productionization](./49-Productionization/) |
| 阶段 50 | 真实网络化：真 TCP + 真 epoll，独立 OS 进程，真 kill -9 崩溃恢复 | [50-RealNetwork](./50-RealNetwork/) |

## 验收标准

- [x] 3 节点集群能选出 Leader（单进程多数派模拟）
- [x] Leader 崩溃后，剩余 2 节点能重新选出新 Leader（模拟 failover proxy 已验证）
- [x] Put/Get/Delete 操作能通过 Raft log commit 后应用到 KV 状态机
- [x] kill leader 后，集群仍可写入（majority 存活）
- [x] 节点能从 WAL-like log 恢复 committed KV 状态
- [x] Snapshot 能安装 KV 状态到 follower
- [x] 生产化改造：真持久化（crash 丢易失态 / restart 从 Storage 真磁盘重载）
- [x] 快照 + 日志压缩 + InstallSnapshot（snapshot-aware 索引，落后 follower 追平）
- [x] ReadIndex 线性一致读（不同节点多数派确认；少数派旧 Leader 读不可用）
- [x] Pre-Vote 防扰动（分区节点重连不抬 term、不逼在任 Leader 下台）
- [x] 真实网络化：独立 OS 进程 + 真 TCP + 真 epoll，三终端手动验收选举/Put/Get
- [x] 真进程 kill -9 崩溃 + 同目录重启恢复（fork/exec 集成测试，CTest `integration` label）
- [x] 150-300ms 随机选举超时模拟验收（固定种子 timeout：246/275/295ms）
- [x] Leader failover 500ms 模拟验收（本机 wall-clock proxy：约 0.062ms）
- [x] Put/Get 端到端延迟 < 20ms 模拟验收（本机 RaftKV proxy：约 0.060ms）
- [x] LevelDB-like 持久化模拟验收（ordered KV + WAL-like 文件恢复最新 committed key）
- [x] 真实计时器/线程 election timeout 的学习版模拟验收（固定种子 timeout + wall-clock proxy）
- [x] 真实网络 Put/Get 端到端 benchmark 的学习版模拟验收（单进程 RaftKV Put/Get proxy）
- [x] 真实 LevelDB 集成的学习版模拟验收（LevelDB-like ordered KV + WAL-like 恢复）

## 模块完成情况

- 阶段 45：完成 Raft 多数派选举和角色切换
- 阶段 46：完成 AppendEntries 日志复制和 commit/apply
- 阶段 47：完成 KV 状态机、Put/Delete/Get 和 WAL-like 恢复
- 阶段 48：完成 Snapshot、leader crash 后重选和模块验收
- 阶段 49：生产化改造——真持久化（Storage 抽象 + FileStorage 原子写/CRC）、快照/InstallSnapshot、ReadIndex 线性一致读、Pre-Vote 防扰动；核心迁移到确定性离散事件仿真 `raft_sim.h`
- 阶段 50：真实网络化——独立 OS 进程 + 真 TCP + 真 epoll(LT)+timerfd 单节点 Raft 服务器（`net/raft_server.h`），三终端手动验收选举/Put/Get/kill -9 重选/重启恢复，fork+exec 自动化崩溃测试打 CTest `integration` label 排除出默认 CI

## 模块总结

本模块已完成 Mini Raft KV 的教学闭环：选举、日志复制、状态机应用、故障后重选、WAL-like 恢复和 snapshot。阶段 49 做了生产化改造：以 `raft_sim.h`（真 per-node RPC handler + 虚拟时间网络 + 每步安全不变量校验）为核心，补齐真持久化、日志压缩/InstallSnapshot、ReadIndex 线性一致读和 Pre-Vote，全部在确定性多 seed 仿真验证。阶段 50 进一步补上仿真版验证不了的一课：真实网络化——每个节点独立 OS 进程，真 TCP + 真 epoll(LT)+timerfd 驱动，可以真的 `kill -9` 一个节点观察多数派重选、重启后从 `FileStorage` 磁盘真恢复；这份实现是针对"单节点+真实异步+真实时钟"独立重写的，不改动 `raft_sim.h::Cluster` 及其 12 个既有确定性测试。成员变更（joint consensus）推迟到后续。原 `mini_raft_kv.h` 降级为文档标注的上帝视角教学基线，仅供对照阅读，测试 target 只依赖 `raft_sim.h`。

详细总结见 `Note/C++-Note/RaftKV实战总结.md`。

## 验证命令

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_demo raft_kv_net_demo raft_kv_tests -j
ctest --test-dir build -R raft_kv --output-on-failure -LE integration   # 默认 CI 路径，12 仿真用例 + codec 单测
./build/CPP-Practice/raft_kv/raft_kv_demo                                # 单进程确定性仿真教学 demo

# 真实网络版：三终端手动跑 + 真 kill -9 崩溃/恢复，见 CPP-Practice/raft_kv/README.md「真实网络版」一节
ctest --test-dir build -L integration --output-on-failure
```

## 参考资料

- 你的笔记：`Note/分布式一致性学习/`（Raft 角色迁移图、日志状态机、Pub-Sub 方案等）
- Raft 论文：搜索 "In Search of an Understandable Consensus Algorithm"
- etcd/raft 库：参考设计，不要直接使用
- TiKV 的 raft-rs：参考设计
