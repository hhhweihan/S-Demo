# Week45 Raft 选举复盘总结

## 核心目标

Week45 完成 Raft leader election 的教学实现。

## 状态字段

每个节点需要维护：

- role：Follower / Candidate / Leader
- current_term
- voted_for
- log
- alive

## RequestVote 关键条件

候选者能拿到票通常需要满足：

- term 不小于接收方 current_term
- 接收方本 term 没投过别人
- 候选者日志不比自己旧

当前实现保留了 term 和 voted_for 约束，并用 log index 做简化新旧判断。

## 多数派

3 节点集群的 majority 是 2。candidate 拿到 2 票后成为 leader。

## 复盘结论

Raft 选举的核心不是随机超时本身，而是 term 单调递增、每 term 最多一票、多数派保证同一 term 不会出现两个合法 leader。
