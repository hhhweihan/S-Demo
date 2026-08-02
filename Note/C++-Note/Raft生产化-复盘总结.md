# Raft 生产化复盘总结

## 核心目标

Week49 把 `CPP-Practice/raft_kv/` 中的确定性 Raft 仿真升级为更贴近生产的实现：持久化边界清晰、快照可追赶、读路径线性一致、选举不会被分区节点扰动。

## 真持久化边界

生产化的第一步是明确哪些状态必须活过崩溃：

- `currentTerm`
- `votedFor`
- `log`
- `snapshot`

这些状态通过 `Storage` 抽象落盘，`crash()` 会丢弃全部易失态，`restart()` 只能从 `Storage::load()` 重建。`commitIndex` 和 `lastApplied` 不持久化，重启后通过日志和快照重新推进。

## Snapshot / InstallSnapshot

Snapshot 用 `(lastIncludedIndex, lastIncludedTerm)` 加状态机 blob 压缩无界日志。难点在于所有日志索引都要变成 snapshot-aware：

- `firstIndex()` 不再固定为 1；
- `termAt(lastIncludedIndex)` 要回退到 snapshot 元信息；
- 当 follower 的 `nextIndex` 落后于 snapshot 边界时，Leader 改发 InstallSnapshot。

## ReadIndex 线性一致读

ReadIndex 不写日志，但必须确认当前 Leader 仍被多数派承认。实现上记录 read index，再通过不同节点的多数派心跳确认领导权，最后等待本地 apply 追上对应 index 后再返回读结果。

关键点是多数派确认必须按不同节点去重，不能让旧 Leader 在少数派分区里靠重复响应凑数。

## Pre-Vote 防扰动

Pre-Vote 在正式递增 term 前先询问“如果我发起选举，你会投我吗”。分区节点恢复时不会仅凭更高 term 逼健康 Leader 下台；只有预投票过半，节点才真正升 term 发起 RequestVote。

## 验证方式

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_tests -j
ctest --test-dir build -R raft_kv --output-on-failure
```

默认用例覆盖既有 chaos 场景和新增生产化场景，每步执行安全不变量校验。

## 复盘结论

Week49 的核心收获是把“协议正确性”推进到“崩溃恢复也正确”。持久化边界让 crash/restart 不再依赖活对象状态，snapshot-aware 索引解决日志压缩后的复制问题，ReadIndex 保证读不绕过多数派，Pre-Vote 则减少分区恢复对健康集群的扰动。
