# Month12 RaftKV 实战总结

## 月度目标回顾

Month12 完成 Mini Raft KV。代码位于 `CPP-Practice/raft_kv/`。

## 已完成模块

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| RaftNode | role/term/vote/log/commit_index | leader election test |
| RequestVote | 多数派投票 | 3 节点选主 |
| AppendEntries | 日志复制 | follower 应用 committed log |
| KVStore | put/delete/get | Raft commit 后读取 |
| Failover | leader crash 后重选 | 新 leader 继续写入 |
| WAL-like | 持久化 log 和 commit index | 恢复 KV 状态 |
| Snapshot | 导出/安装 KV 状态 | follower 安装 snapshot |

## 核心链路

```text
Client Put/Delete
  -> Leader append log
  -> Followers append entries
  -> Majority ack
  -> Commit index advance
  -> Apply to KVStore
  -> Client observes result
```

## 关键知识点

### 1. 多数派是 Raft 的安全基础

选举和提交都依赖多数派，两个多数派必然相交，这保证了 leader 和 committed log 的连续性。

### 2. 状态机只能应用 committed log

未提交日志可能被未来 leader 覆盖，提前 apply 会破坏一致性。

### 3. Snapshot 是日志压缩机制

随着 log 增长，需要把状态机压缩成 snapshot，落后节点可以通过 snapshot 快速追平。

### 4. 持久化决定崩溃恢复能力

真实 Raft 必须持久化 current_term、voted_for 和 log；本工程用 WAL-like 文件演示 committed 状态恢复。

## 构建验证

```powershell
cmake -S CPP-Practice/raft_kv -B CPP-Practice/raft_kv/build
cmake --build CPP-Practice/raft_kv/build --config Release
.\CPP-Practice\raft_kv\build\Release\raft_kv_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续专项

- 接入真实 RPC 网络层
- 使用真实计时器完成随机 election timeout 和 heartbeat
- 实现 prevLogIndex/prevLogTerm 冲突修复
- 接入 LevelDB 或自研 LSM
- 增加混沌测试和性能压测
