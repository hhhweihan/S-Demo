# Week 47 — KV 状态机 + 持久化

## 本周目标

实现 KV 状态机，集成 LevelDB 持久化，支持线性一致性读，完成端到端 Put/Get 测试。

## 本周完成情况

- [x] Day 323：实现 `KVStore`
- [x] Day 324：实现 Put 应用
- [x] Day 325：实现 Delete/Get 行为
- [x] Day 326：通过 Raft commit 驱动状态机 apply
- [x] Day 327：实现 WAL-like 日志持久化
- [x] Day 328：验证恢复 committed KV 状态
- [x] Day 329：完成 Week47 复盘

## 本周总结

本周完成 Raft 上层状态机。Put/Delete 先作为日志进入 Raft，提交后再 apply 到 KVStore，避免未提交日志污染状态机。学习 demo 使用教学版 WAL-like 文件和 LevelDB-like 有序 KV 验收持久化语义。

## 验证命令

```powershell
cmake --build CPP-Practice/raft_kv/build --config Release
.\CPP-Practice\raft_kv\build\Release\raft_kv_demo.exe
```

---