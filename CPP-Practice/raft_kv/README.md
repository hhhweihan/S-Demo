# raft_kv

Month 12（Week 45-48）对应的 Mini Raft KV 教学工程放在这个目录。

## 覆盖内容

- `RaftNode`：Follower/Candidate/Leader 状态、term、投票、日志
- `RaftCluster`：3 节点多数派选举、leader crash 后重选
- `AppendEntries`：日志复制和 commit/apply
- `KVStore`：put/delete/get 状态机
- WAL-like 持久化和恢复
- Snapshot 安装 KV 状态
- 模拟验收：随机 election timeout、failover、Put/Get 延迟、LevelDB-like 恢复

## 模拟验收口径

当前 demo 是单进程 Raft 集群，不接真实网络线程或 LevelDB。验收项使用 proxy benchmark：

- randomized election timeout in 150-300ms window
- failover election wall-clock proxy < 500ms
- RaftKV Put/Get average latency proxy < 20ms
- WAL-like logdb recovers latest committed key as LevelDB-like persistence check

## 构建

```powershell
cmake -S CPP-Practice/raft_kv -B CPP-Practice/raft_kv/build
cmake --build CPP-Practice/raft_kv/build --config Release
.\CPP-Practice\raft_kv\build\Release\raft_kv_demo.exe
```