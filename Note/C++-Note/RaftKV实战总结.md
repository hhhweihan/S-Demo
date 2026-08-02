# RaftKV 实战总结

## 月度目标回顾

Month12 完成 Mini Raft KV，并在重构后继续补上生产化和真实网络化两条关键能力。代码位于 `CPP-Practice/raft_kv/`。

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
| Productionization | Storage 抽象、FileStorage、InstallSnapshot、ReadIndex、Pre-Vote | 12 个仿真/生产化测试与每步不变量 |
| RealNetwork | 真 TCP、真 epoll/timerfd、独立 OS 进程、kill-9 恢复 | codec 单测与 integration 崩溃恢复测试 |

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

真实 Raft 必须持久化 current_term、voted_for、log 和 snapshot。重构后的工程用 `Storage` 抽象区分 `MemStorage` 与 `FileStorage`，`crash()` 丢弃易失态，`restart()` 只能从磁盘状态重建。

### 5. ReadIndex 让读也走多数派确认

线性一致读不能直接读本地 KV，否则少数派旧 Leader 可能返回过期值。ReadIndex 通过不同节点多数派心跳确认当前 Leader 仍有效，再等待本地 apply 追上对应 index 后返回。

### 6. Pre-Vote 降低分区恢复扰动

分区节点恢复时如果直接递增 term 发起选举，可能让健康 Leader 下台。Pre-Vote 先不改变 term 做预投票，只有预投票过半才正式进入选举。

### 7. 真实网络版验证 OS 语义

仿真版验证协议逻辑，真实网络版验证 TCP 字节流、非阻塞 IO、epoll/timerfd、独立进程和 SIGKILL 后的磁盘恢复。两条腿互补，不互相替代。

## 构建验证

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_demo raft_kv_net_demo raft_kv_tests -j
ctest --test-dir build -R raft_kv --output-on-failure -LE integration

# 真实网络 kill-9 / 重启恢复集成测试单独跑
cmake --build build --target raft_kv_net_integration_test -j
ctest --test-dir build -L integration --output-on-failure
```

默认验证覆盖 12 个仿真/生产化用例和 codec 单测；`integration` 路径覆盖真实 TCP、独立进程、kill-9 和同目录重启恢复。

## 后续专项

- joint consensus 成员变更
- 更完整的线性一致读压测和真实网络 benchmark
- 真实磁盘 fsync 策略、快照文件持久化与故障注入
- 与自研 LSM/LevelDB-like 存储层进一步整合
