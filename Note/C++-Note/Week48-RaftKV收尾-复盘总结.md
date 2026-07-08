# Week48 RaftKV 收尾复盘总结

## 核心目标

Week48 完成 snapshot、leader failover、majority 写入和最终验收。

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

## 复盘结论

RaftKV 的项目价值在于把共识协议和存储状态机连接起来。真实生产版本还要补网络、持久化 fsync、日志冲突修复、snapshot 安装 RPC 和混沌测试。
