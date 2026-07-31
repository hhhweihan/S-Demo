# 高性能网络库

## 模块目标

在 模块 08 Reactor 框架基础上，实现连接池、io_uring 异步 IO、
定时器轮（Timer Wheel）、以及完整的高性能 TCP 服务端框架。

## 实现口径（真实落地 vs 跨平台模拟）

本模块提交进 `CPP-Practice/high_perf_net/`（`mini_high_perf_net.h`）的是：`TimerWheel`（真实进程内 O(1) 增删/刷新/取消定时轮）、`IoUringSimulator`（在**用户态模拟** SQ/CQ 环，无 liburing、无真实 io_uring 系统调用）、以及进程内 `ConnectionPool` + `RoundRobinBalancer`/`LeastConnBalancer`（后端为进程内 `Backend` 对象，**非真实 TCP 连接/握手**）。各日 `[x]` 记录的是这些机制的**概念与设计掌握**；`sendfile`/`splice`/`SO_REUSEPORT`/固定缓冲区/SQPOLL 在本模块为概念研究。

下一步 Linux 补课（原生实现待办）：

- [ ] 原生 io_uring（liburing）：SQE/CQE 提交、TCP echo server、`register_buffers` 固定缓冲、SQPOLL
- [ ] `sendfile(2)` / `splice` 零拷贝与双向 TCP 代理
- [ ] `SO_REUSEPORT` 多线程 accept
- [ ] 真实 TCP 连接池 + 面向真实后端的负载均衡 / 健康探测 / 熔断
- [ ] 单机 QPS / 10 万并发压测（Linux/WSL2）

> 想看真实 epoll + timerfd 落地，见 `CPP-Practice/raft_kv/net/`（`event_loop.h` 真实 `epoll_create1`/`epoll_wait` LT 模式 + `timerfd`）。

## 技能树

```
高性能网络库
├── 定时器轮 + 心跳检测     ← 阶段 33
│   ├── 时间轮（Timing Wheel）
│   ├── 连接空闲超时检测
│   └── 精确定时器（最小堆方案）
├── io_uring 异步 IO        ← 阶段 34
│   ├── io_uring 基础（SQ/CQ 环形队列）
│   ├── 注册固定缓冲区（减少内核拷贝）
│   └── 与 epoll Reactor 性能对比
├── 连接池 + 负载均衡        ← 阶段 35
│   ├── TCP 连接池（预建立 + 复用）
│   ├── 多后端负载均衡（Round Robin / LeastConn）
│   └── 健康检查
└── 整合 + 性能优化          ← 阶段 36
    ├── 零拷贝 sendfile / splice
    ├── 多线程 accept（SO_REUSEPORT）
    └── 模块综合压测
```

## 阶段拆解

| 阶段 | 主题 | 目录 |
|----|------|------|
| 阶段 33 | 定时器轮 + 心跳检测 | [33-TimerWheel/](./33-TimerWheel/) |
| 阶段 34 | io_uring | [34-iouring/](./34-iouring/) |
| 阶段 35 | 连接池 + 负载均衡 | [35-ConnPool/](./35-ConnPool/) |
| 阶段 36 | 整合 + 性能优化 | [36-Optimization/](./36-Optimization/) |

## 验收标准

- [x] TimerWheel 能验证连接空闲自动清理和刷新
- [x] io_uring SQ/CQ 机制完成跨平台模拟验证
- [x] 连接池支持 RoundRobin、LeastConn 和健康检查
- [ ] 单机 HTTP 服务器 QPS > 10 万（需要 Linux/WSL2 与真实 server 压测）
- [ ] io_uring 比 epoll 吞吐量提升 20%+（需要 Linux 原生 io_uring）
- [ ] 10 万并发连接稳定运行 1 分钟无崩溃（需要 Linux/WSL2 与系统参数调优）

## 模块完成情况

- 阶段 33：完成 TimerWheel 和 PrecisionTimerHeap
- 阶段 34：完成 io_uring SQ/CQ 概念模拟
- 阶段 35：完成 ConnectionPool、RoundRobin、LeastConn、健康检查
- 阶段 36：完成平台优化边界分析和模块综合测试

## 模块总结

本模块已完成高性能网络库的跨平台核心组件：定时器、异步 IO 队列模型、连接池和负载均衡。当前环境无法运行 Linux 专属 io_uring、sendfile/splice、SO_REUSEPORT 和大规模连接压测，因此真实性能指标保留为专项验证。

详细总结见 `Note/C++-Note/Month09-高性能网络库实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/high_perf_net -B CPP-Practice/high_perf_net/build
cmake --build CPP-Practice/high_perf_net/build --config Release
.\CPP-Practice\high_perf_net\build\Release\high_perf_net_demo.exe
```
