# Week 22 — 日志复制

## 本周目标

实现 Raft 日志复制：Leader 追加日志 → 复制给 Follower → 多数确认后 commit。
完成后：3 节点集群能正确复制并 commit 任意条目。

---