# 快照 + 稳定性测试 + 年度回顾

## 本周目标

实现日志压缩（快照），完成混沌测试，写项目文档，做六个月总结。

## 本周完成情况

- [x] Day 330：实现 snapshot 数据导出
- [x] Day 331：实现 follower 安装 snapshot
- [x] Day 332：验证 leader crash 后重新选主
- [x] Day 333：验证 majority 存活仍可写入
- [x] Day 334：整理稳定性测试边界
- [x] Day 335：完成 RaftKV 项目文档
- [x] Day 336：完成 Week48 与 Month12 复盘

## 本周总结

本周完成 RaftKV 的整体验收：snapshot、leader failover、majority 写入和恢复路径全部通过 demo 验证。混沌测试、网络延迟、磁盘 fsync 和 LevelDB 集成都按学习 demo proxy 口径验收。

## 验证命令

```powershell
cmake -S CPP-Practice/raft_kv -B CPP-Practice/raft_kv/build
cmake --build CPP-Practice/raft_kv/build --config Release
.\CPP-Practice\raft_kv\build\Release\raft_kv_demo.exe
```

---