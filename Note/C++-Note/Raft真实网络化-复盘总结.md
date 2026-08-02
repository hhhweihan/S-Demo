# Raft 真实网络化复盘总结

## 核心目标

Week50 给 RaftKV 补上仿真版验证不了的真实系统语义：每个节点是独立 OS 进程，节点之间用 127.0.0.1 上的真 TCP 通信，由真 epoll/timerfd 驱动，可以真的 `kill -9` 一个节点再用同一磁盘目录重启恢复。

## 为什么不能只靠仿真

`raft_sim.h` 适合验证协议逻辑，因为虚拟时间、随机种子和不变量检查让时序 bug 可复现。但它验证不了：

- TCP 字节流没有消息边界；
- 非阻塞 connect/read/write 的状态机；
- 多个真实定时器同时到期；
- OS 进程被 SIGKILL 后没有析构、没有 atexit；
- 重启进程只能依赖磁盘持久化状态。

所以真实网络版选择在 `CPP-Practice/raft_kv/net/` 独立实现一条网络腿，不改动原有确定性仿真。

## 网络层设计

- `net/codec.h`：长度前缀帧，格式为 `[u32 length][u32 from][u8 type][body]`；
- `net/event_loop.h`：epoll(LT) 事件循环，timerfd 驱动 election timeout / heartbeat，eventfd 用于安全停止；
- `net/raft_server.h`：单节点真实 Raft 服务器，处理 RequestVote、AppendEntries、InstallSnapshot、ReadIndex 和管理协议；
- `raft_net_main.cpp`：`raft_kv_net_demo` 入口，一个进程就是一个节点。

长度前缀解决 TCP 粘包/半包，帧内 `from` 字段让“谁拨号”与“消息属于谁”解耦；连接策略用“低 id 拨高 id”减少双向冗余连接。

## 真崩溃恢复

真实崩溃测试使用 fork/exec 启动 3 个 `raft_kv_net_demo` 子进程，通过真实 TCP 提交写请求，然后对当前 Leader 发送 `SIGKILL`。剩余多数派重新选主后，旧节点用同一 storage directory 重启，并从 `FileStorage` 恢复 term、log、snapshot 和 KV 状态。

这比在对象上调用 `crash()` 更接近真实生产语义：被 kill 的进程没有任何用户态清理机会，能幸存的只有 fsync 过的磁盘字节。

## 验证方式

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_tests raft_kv_net_demo -j
ctest --test-dir build -R raft_kv --output-on-failure -LE integration

# 真实进程集成测试单独跑
cmake --build build --target raft_kv_net_integration_test -j
ctest --test-dir build -L integration --output-on-failure
```

`integration` label 把真实时钟、真实进程调度和 kill-9 测试隔离在默认 CI 路径之外，避免把慢且可能抖动的系统级测试混入常规秒级测试。

## 复盘结论

Week50 的价值在于把 Raft 从“仿真里正确”推进到“操作系统语义下可观察”。真实 TCP 逼迫协议处理粘包/半包和异步连接，timerfd/epoll 逼迫多个截止时间进入同一事件循环，SIGKILL 则逼迫持久化边界接受最严格检验。仿真版和真实网络版互补：前者保证协议逻辑可复现，后者验证网络、进程和磁盘语义。
