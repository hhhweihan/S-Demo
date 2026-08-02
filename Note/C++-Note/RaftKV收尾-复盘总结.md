# RaftKV 收尾复盘总结

## 核心目标

Week48 完成 snapshot、leader failover、majority 写入和阶段性验收，为后续 Week49 生产化和 Week50 真实网络化打基础。

## Snapshot

Snapshot 将状态机当前状态保存下来，用于日志压缩和落后 follower 快速追平。

当前 demo：

- 从 leader/recovered node 导出 KV map
- follower 安装 snapshot
- 清理本地旧日志

## Failover

测试流程：

```text
elect node0
put before=ok
crash node0
elect node1
put after=ok
get after == ok
```

这验证了 majority 存活时集群仍能继续写入。

## 与后续阶段的衔接

Week48 的重点仍是教学闭环：单进程内完成选举、日志复制、KV apply、leader crash 后重选和 snapshot 安装。重构后的计划已经继续补上：

- Week49：真持久化、InstallSnapshot、ReadIndex、Pre-Vote 和确定性 chaos 测试；
- Week50：独立 OS 进程、真 TCP、真 epoll/timerfd、真 kill-9 崩溃恢复。

## 复盘结论

RaftKV 的项目价值在于把共识协议和存储状态机连接起来。Week48 完成的是“协议教学闭环”，不是最终生产形态；真正贴近生产的持久化、线性一致读、防扰动选举和真实网络语义已在后续 Week49/Week50 中补齐。
