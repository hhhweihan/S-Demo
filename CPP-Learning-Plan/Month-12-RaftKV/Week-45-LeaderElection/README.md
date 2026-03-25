# Week 21 — Leader 选举

## 本周目标

实现 Raft 的选举协议：Follower 超时 → Candidate 发 RequestVote → Leader 发心跳。
完成后：3 个节点能在 500ms 内选出 Leader。

---