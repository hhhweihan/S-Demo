# Leader 选举

## 本周目标

实现 Raft 的选举协议：Follower 超时 → Candidate 发 RequestVote → Leader 发心跳。
完成后：3 个节点能在 500ms 内选出 Leader。

## 本周完成情况

- [x] Day 309：梳理 Raft 角色和 term/vote 状态
- [x] Day 310：实现 `RaftNode` 基础状态
- [x] Day 311：实现 `RequestVote`
- [x] Day 312：实现多数派选举
- [x] Day 313：验证 3 节点集群选出 Leader
- [x] Day 314：整理心跳/选举计时器设计
- [x] Day 315：完成 Week45 复盘

## 本周总结

本周完成单进程 Raft 选举模型。节点维护 term、role、voted_for，候选者通过 RequestVote 拿到多数票后成为 leader。学习 demo 用固定种子随机 timeout 和显式 `elect()` 验证协议条件。

## 验证命令

```powershell
cmake --build CPP-Practice/raft_kv/build --config Release
.\CPP-Practice\raft_kv\build\Release\raft_kv_demo.exe
```

---