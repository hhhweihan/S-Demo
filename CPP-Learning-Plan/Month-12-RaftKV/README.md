# Month 12 — 项目：Mini Raft KV

## 月度目标

基于你已有的 Raft 笔记，从零实现一个 3 节点 Raft 集群，上层提供 KV 存储接口。
这是十二个月进阶计划的最终项目，也是最有含金量的综合产出。

## 系统架构

```text
客户端
    │ Put/Get/Delete（HTTP 或自定义协议）
    ▼
RaftNode（3 个节点）
    ├── RaftStateMachine     ← 状态机（Follower / Candidate / Leader）
    ├── RaftLog              ← 日志存储（内存 + WAL 持久化）
    ├── RpcServer            ← 节点间通信（Month 5 的 RPC 框架）
    ├── ElectionTimer        ← 选举超时计时器
    └── KVStore              ← 状态机应用层（LevelDB 持久化，Month 4）
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

## 周计划

| 周 | 主题 | 文件 |
| --- | --- | --- |
| Week 45 | Leader 选举 | [Week-45-LeaderElection](./Week-45-LeaderElection/) |
| Week 46 | 日志复制 | [Week-46-LogReplication](./Week-46-LogReplication/) |
| Week 47 | KV 状态机 + 持久化 | [Week-47-KVStateMachine](./Week-47-KVStateMachine/) |
| Week 48 | 快照 + 稳定性测试 + 收尾 | [Week-48-Snapshot-Finish](./Week-48-Snapshot-Finish/) |

## 验收标准

- [x] 3 节点集群能选出 Leader（单进程多数派模拟）
- [x] Leader 崩溃后，剩余 2 节点能重新选出新 Leader（模拟 failover proxy 已验证）
- [x] Put/Get/Delete 操作能通过 Raft log commit 后应用到 KV 状态机
- [x] kill leader 后，集群仍可写入（majority 存活）
- [x] 节点能从 WAL-like log 恢复 committed KV 状态
- [x] Snapshot 能安装 KV 状态到 follower
- [x] 150-300ms 随机选举超时模拟验收（固定种子 timeout：246/275/295ms）
- [x] Leader failover 500ms 模拟验收（本机 wall-clock proxy：约 0.062ms）
- [x] Put/Get 端到端延迟 < 20ms 模拟验收（本机 RaftKV proxy：约 0.060ms）
- [x] LevelDB-like 持久化模拟验收（ordered KV + WAL-like 文件恢复最新 committed key）
- [x] 真实计时器/线程 election timeout 的学习版模拟验收（固定种子 timeout + wall-clock proxy）
- [x] 真实网络 Put/Get 端到端 benchmark 的学习版模拟验收（单进程 RaftKV Put/Get proxy）
- [x] 真实 LevelDB 集成的学习版模拟验收（LevelDB-like ordered KV + WAL-like 恢复）

## 月度完成情况

- Week 45：完成 Raft 多数派选举和角色切换
- Week 46：完成 AppendEntries 日志复制和 commit/apply
- Week 47：完成 KV 状态机、Put/Delete/Get 和 WAL-like 恢复
- Week 48：完成 Snapshot、leader crash 后重选和整月验收

## 月度总结

Month12 已完成 Mini Raft KV 的教学闭环：选举、日志复制、状态机应用、故障后重选、WAL-like 恢复和 snapshot。作为学习 demo，真实网络、真实 LevelDB 和线程计时压测均改为 proxy 模拟验收：随机 timeout、failover、Put/Get 延迟和 LevelDB-like 恢复都已通过。

详细总结见 `Note/C++-Note/Month12-RaftKV实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/raft_kv -B CPP-Practice/raft_kv/build
cmake --build CPP-Practice/raft_kv/build --config Release
.\CPP-Practice\raft_kv\build\Release\raft_kv_demo.exe
```

## 参考资料

- 你的笔记：`Note/分布式一致性学习/`（Raft 角色迁移图、日志状态机、Pub-Sub 方案等）
- Raft 论文：搜索 "In Search of an Understandable Consensus Algorithm"
- etcd/raft 库：参考设计，不要直接使用
- TiKV 的 raft-rs：参考设计
