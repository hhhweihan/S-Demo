# Raft 日志复制复盘总结

## 核心目标

Week46 完成 leader 追加日志、follower 接收日志、多数派 commit、状态机 apply。

## 写入链路

```text
client put
  -> leader append local log
  -> AppendEntries to followers
  -> majority ack
  -> advance commit index
  -> apply to KV state machine
```

## commit index

commit index 表示已经被多数派确认、可以应用到状态机的最后日志下标。状态机只能应用 committed entry，不能提前应用未确认日志。

## 当前实现边界

为了教学清晰，当前 `AppendEntries` 用 leader 完整日志覆盖 follower 日志。真实 Raft 会用 `prevLogIndex/prevLogTerm` 逐步修复冲突日志。

## 复盘结论

Raft 的可靠性来自多数派提交。只要一条日志被多数派持有，下一任 leader 仍然有机会保留并继续推进它。
