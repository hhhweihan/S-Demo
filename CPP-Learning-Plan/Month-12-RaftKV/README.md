# Month 6 — 项目：Mini Raft KV

## 月度目标

基于你已有的 Raft 笔记，从零实现一个 3 节点 Raft 集群，上层提供 KV 存储接口。
这是整个 6 个月学习的最终产出，也是最有含金量的项目。

## 系统架构

```
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

```
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
|----|------|------|
| Week 21 | Leader 选举 | [Week-21.md](./Week-21.md) |
| Week 22 | 日志复制 | [Week-22.md](./Week-22.md) |
| Week 23 | KV 状态机 + 持久化 | [Week-23.md](./Week-23.md) |
| Week 24 | 快照 + 稳定性测试 + 收尾 | [Week-24.md](./Week-24.md) |

## 验收标准

- [ ] 3 节点集群能选出 Leader（在 150-300ms 随机超时内）
- [ ] Leader 崩溃后，剩余 2 节点能在 500ms 内重新选出新 Leader
- [ ] Put/Get 操作在正常情况下端到端延迟 < 20ms
- [ ] 随机 kill 1 个节点，集群仍可写入（majority 存活）
- [ ] 重启节点后，能从 WAL 恢复并重新加入集群

## 参考资料

- 你的笔记：`Note/分布式一致性学习/`（Raft 角色迁移图、日志状态机、Pub-Sub 方案等）
- Raft 论文：搜索 "In Search of an Understandable Consensus Algorithm"
- etcd/raft 库：参考设计，不要直接使用
- TiKV 的 raft-rs：参考设计
