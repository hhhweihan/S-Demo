# Month 9 — 高性能网络库

## 月度目标

在 Month 8 Reactor 框架基础上，实现连接池、io_uring 异步 IO、
定时器轮（Timer Wheel）、以及完整的高性能 TCP 服务端框架。

## 技能树

```
高性能网络库
├── 定时器轮 + 心跳检测     ← Week 33
│   ├── 时间轮（Timing Wheel）
│   ├── 连接空闲超时检测
│   └── 精确定时器（最小堆方案）
├── io_uring 异步 IO        ← Week 34
│   ├── io_uring 基础（SQ/CQ 环形队列）
│   ├── 注册固定缓冲区（减少内核拷贝）
│   └── 与 epoll Reactor 性能对比
├── 连接池 + 负载均衡        ← Week 35
│   ├── TCP 连接池（预建立 + 复用）
│   ├── 多后端负载均衡（Round Robin / LeastConn）
│   └── 健康检查
└── 整合 + 性能优化          ← Week 36
    ├── 零拷贝 sendfile / splice
    ├── 多线程 accept（SO_REUSEPORT）
    └── 整月综合压测
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 33 | 定时器轮 + 心跳检测 | [Week-33-TimerWheel/](./Week-33-TimerWheel/) |
| Week 34 | io_uring | [Week-34-iouring/](./Week-34-iouring/) |
| Week 35 | 连接池 + 负载均衡 | [Week-35-ConnPool/](./Week-35-ConnPool/) |
| Week 36 | 整合 + 性能优化 | [Week-36-Optimization/](./Week-36-Optimization/) |

## 验收标准

- [ ] 单机 HTTP 服务器 QPS > 10 万（4 核机器）
- [ ] io_uring 比 epoll 吞吐量提升 20%+（大文件传输场景）
- [ ] 连接空闲 60 秒自动清理（定时器轮验证）
- [ ] 10 万并发连接稳定运行 1 分钟无崩溃
