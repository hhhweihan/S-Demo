# raft_kv

Month 12（Week 45-50）对应的 Mini Raft KV 工程放在这个目录，分两条腿：

- `raft_sim.h`：单进程、虚拟时钟驱动的**确定性仿真** Raft（种子化 RNG、每步安全不变量校验），
  是 chaos/生产化测试的核心，`raft_kv_tests` 默认跑的 12 个用例都在这条腿上。
- `net/`：真实网络版 Raft ——每个节点是独立 OS 进程，走 127.0.0.1 上的真 TCP + 真 epoll，
  可以真的 `kill -9` 一个节点观察剩余多数派重选、重启后从磁盘恢复。见下方「真实网络版」一节。

两条腿**互不改动对方**：仿真版继续给可复现的 chaos 覆盖，真实网络版给一次"节点之间真的隔着网络"
的第一手经验（超时重传、粘包、真崩溃恢复）。

## 项目定位

这是当前仓库最适合作为分布式方向代表作的项目。它的目标不是复刻 etcd/raft，而是把 Raft 的核心安全性、崩溃恢复边界和真实网络语义拆开验证：仿真版负责可复现的协议正确性，真实网络版负责 TCP/epoll/进程崩溃这些操作系统层面的经验。

## 覆盖内容

- `RaftNode`：Follower/Candidate/Leader 状态、term、投票、日志
- `RaftCluster`：3 节点多数派选举、leader crash 后重选
- `AppendEntries`：日志复制和 commit/apply
- `KVStore`：put/delete/get 状态机
- 真持久化（`Storage` 抽象 + `MemStorage`/`FileStorage` 原子写 + CRC）
- 快照 + 日志压缩 + InstallSnapshot、ReadIndex 线性一致读、Pre-Vote 防扰动
- 模拟验收：随机 election timeout、failover、Put/Get 延迟、崩溃持久化恢复
- **真实网络版**：真 TCP + 真 epoll 的单节点 Raft 服务器，独立 OS 进程通信，真 `kill -9` 崩溃/恢复

## 模拟验收口径（`raft_sim.h`）

`raft_kv_tests` 默认跑的是单进程确定性仿真集群，不接真实网络线程。验收项使用 proxy benchmark：

- randomized election timeout in 150-300ms window
- failover election wall-clock proxy < 500ms
- RaftKV Put/Get average latency proxy < 20ms
- 崩溃持久化：`crash()` 丢全部易失态，`restart()` 从 `Storage::load()` 真磁盘重载

## 边界与非目标

- 真实网络版已经使用真 TCP、真 epoll/timerfd 和独立 OS 进程，但目前仍是学习版单机 3 节点集群。
- 默认 CI 路径不跑真实进程 kill-9 集成测试；该测试使用 `integration` label 单独执行。
- 尚未实现 joint consensus 成员变更、完整生产级 snapshot 文件管理、跨机器部署和真实大规模压测。

## 真实网络版（`net/`）

`net/` 下是一份**独立重写**的单节点 Raft 服务器（不复用 `raft_sim.h::Cluster` 的上帝视角驱动逻辑），
用真实 epoll 事件循环 + 真实 TCP 驱动选举/复制/ReadIndex/Pre-Vote，复用的只是 `raft_storage.h` 里
与"仿真 vs 真磁盘"无关的纯数据层（`LogEntry`/`Storage`/`PersistentState`/编解码 helper）。

- `net/codec.h` —— TCP 帧编解码：`[u32 length][u32 from][u8 type][body]`，body 复用
  `raft_storage.h::raft_sim::detail` 里已有的定长小端序列化 helper。
- `net/event_loop.h` —— 薄 epoll(LT) 封装，`timerfd` 驱动 election timeout / heartbeat，
  `eventfd` 做线程/信号安全的自管道式 `Stop()`。
- `net/raft_server.h` —— 单节点真实 Raft 服务器：RequestVote/AppendEntries/InstallSnapshot 的
  发起与处理、Pre-Vote、ReadIndex（心跳确认领导权 + 延迟响应），管理协议处理 PUT/GET/STATUS。
- `raft_net_main.cpp` —— 可执行文件 `raft_kv_net_demo` 的入口：一个进程 = 一个节点。

### 三终端手动跑一个真实集群

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_net_demo -j

# 终端 1
./build/CPP-Practice/raft_kv/raft_kv_net_demo 0 "1=127.0.0.1:17002,2=127.0.0.1:17003" 17001 18001 /tmp/raft0
# 终端 2
./build/CPP-Practice/raft_kv/raft_kv_net_demo 1 "0=127.0.0.1:17001,2=127.0.0.1:17003" 17002 18002 /tmp/raft1
# 终端 3
./build/CPP-Practice/raft_kv/raft_kv_net_demo 2 "0=127.0.0.1:17001,1=127.0.0.1:17002" 17003 18003 /tmp/raft2
```

每个进程每秒在 stdout 打一行 `role=... term=... commit=... applied=... leader=... pid=...`，
肉眼就能看到只有一个节点是 `Leader`。第四个终端用文本协议读写（非 leader 的写请求会返回
`NOT_LEADER <leaderId>`）：

```bash
printf 'STATUS\n' | nc 127.0.0.1 18001
printf 'PUT foo bar\n' | nc 127.0.0.1 18001   # 对着 leader 的 admin 端口发
printf 'GET foo\n' | nc 127.0.0.1 18002       # 从任意节点读，ReadIndex 保证线性一致
```

### 真崩溃 + 真恢复

```bash
kill -9 <当前 leader 的 pid>       # SIGKILL 绕过所有用户态清理路径，没有析构、没有 atexit
# 观察剩余两个终端在一个 election timeout 内（150-300ms 量级）选出新 leader
# 用同一个 storage-dir 重新启动被杀的节点：
./build/CPP-Practice/raft_kv/raft_kv_net_demo 0 "1=127.0.0.1:17002,2=127.0.0.1:17003" 17001 18001 /tmp/raft0
# STATUS/GET 确认它从 FileStorage 磁盘重放出一致的 term/commit/applied，追上集群
```

### 自动化崩溃/恢复测试

`tests/raft_net_integration_test.cpp` 把上面的手动流程自动化：fork+exec 3 个真实
`raft_kv_net_demo` 子进程，走真实 TCP 提交写请求，`kill(pid, SIGKILL)` 真 leader 进程，
轮询剩余节点直到选出新 leader，重启被杀节点指向同一 `FileStorage` 目录确认恢复。

真实时钟 + 真实进程调度，天然比确定性仿真慢、理论上更容易偶发抖动——打了 CTest `integration`
label，**默认 `ctest`/CI 不会跑它**（和 `leveldb_mini` 现有的 fork/kill-9 崩溃测试走普通 Debug、
不进 sanitizer 矩阵是同一个处理思路）。手动单独跑：

```bash
cmake --build build --target raft_kv_net_integration_test -j
ctest --test-dir build -L integration --output-on-failure
```

## 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_demo raft_kv_net_demo -j
./build/CPP-Practice/raft_kv/raft_kv_demo        # 单进程确定性仿真教学 demo
./build/CPP-Practice/raft_kv/raft_kv_net_demo ... # 真实网络节点，见上面「真实网络版」

# 默认测试（12 个仿真用例 + codec 单测，不含 integration）：
cmake --build build --target raft_kv_tests -j
ctest --test-dir build -R raft_kv --output-on-failure -LE integration
```
