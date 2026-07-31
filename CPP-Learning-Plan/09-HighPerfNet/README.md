# 高性能网络库

## 模块目标

在 模块 08 Reactor 框架基础上，实现连接池、io_uring 异步 IO、
定时器轮（Timer Wheel）、以及完整的高性能 TCP 服务端框架。

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
