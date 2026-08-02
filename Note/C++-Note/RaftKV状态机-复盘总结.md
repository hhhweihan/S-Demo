# RaftKV 状态机复盘总结

## 核心目标

Week47 在 Raft log 上实现 KVStore 状态机和 WAL-like 恢复。

## 状态机原则

写请求不能直接修改 KV，必须先进入 Raft log：

```text
put/delete -> log -> commit -> apply -> KVStore
```

这样 follower 和 leader 只要按相同顺序 apply committed log，就能得到一致状态。

## WAL-like 恢复

当前 demo 将 term、commit_index 和 log entry 写入文件，恢复时重放 committed log，重建 KVStore。

## 线性一致性读

真实线性一致性读需要确认当前 leader 仍持有多数派，常见方式包括 ReadIndex 或 lease read。当前 demo 的 get 从 leader KVStore 读取，只覆盖教学闭环。

## 复盘结论

Raft 负责复制和顺序，KVStore 负责确定性 apply。两者边界清楚，系统才容易推理。
