# 日志复制

## 本周目标

实现 Raft 日志复制：Leader 追加日志 → 复制给 Follower → 多数确认后 commit。
完成后：3 节点集群能正确复制并 commit 任意条目。

## 本周完成情况

- [x] Day 316：定义 Raft log entry
- [x] Day 317：实现 leader 本地追加日志
- [x] Day 318：实现 follower `AppendEntries`
- [x] Day 319：实现 commit index 和 apply 流程
- [x] Day 320：验证多数派提交
- [x] Day 321：验证 follower 应用 committed log
- [x] Day 322：完成 Week46 复盘

## 本周总结

本周完成日志复制闭环。Leader 收到写请求后追加本地日志，把完整日志复制给 follower，多数派确认后推进 commit index，并将日志应用到 KV 状态机。

## 验证命令

```powershell
cmake --build CPP-Practice/raft_kv/build --config Release
.\CPP-Practice\raft_kv\build\Release\raft_kv_demo.exe
```

---