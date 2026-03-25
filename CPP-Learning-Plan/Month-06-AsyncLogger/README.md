# Month 6 — 异步日志系统

## 月度目标

从零构建一个高性能异步日志库：同步日志 → 双缓冲异步日志 → 多 sink 支持，
达到 muduo 日志库的设计水平。

## 技能树

```
异步日志系统
├── 同步日志基础          ← Week 21
│   ├── 日志级别（DEBUG/INFO/WARN/ERROR/FATAL）
│   ├── 格式化输出（时间戳 + 线程 ID + 文件行号）
│   └── 流式接口（LOG_INFO << "msg" << val）
├── 双缓冲异步日志        ← Week 22
│   ├── 前台缓冲区（业务线程写）
│   ├── 后台线程（定时 flush 到文件）
│   └── 双缓冲交换（zero-copy 思路）
├── 多 Sink 支持          ← Week 23
│   ├── FileSink（按天滚动）
│   ├── ConsoleSink（终端彩色输出）
│   └── Sink 抽象接口
└── 性能压测 + 完善       ← Week 24
    ├── 多线程并发写入压测
    ├── 与 spdlog / glog 性能对比
    └── 完整日志库整合
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 21 | 同步日志基础 | [Week-21-SyncLogger/](./Week-21-SyncLogger/) |
| Week 22 | 双缓冲异步日志 | [Week-22-AsyncLogger/](./Week-22-AsyncLogger/) |
| Week 23 | 多 Sink 支持 | [Week-23-MultiSink/](./Week-23-MultiSink/) |
| Week 24 | 性能压测 + 整合 | [Week-24-Perf-Integration/](./Week-24-Perf-Integration/) |

## 验收标准

- [ ] 多线程 100 万条日志，异步日志吞吐量 > 同步日志 5 倍以上
- [ ] 日志文件按天自动滚动，历史文件自动压缩
- [ ] 与 spdlog 性能差距 < 2 倍
- [ ] 日志库对业务线程的平均延迟 < 1μs
