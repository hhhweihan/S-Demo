# 异步日志系统

## 模块目标

从零构建一个高性能异步日志库：同步日志 → 双缓冲异步日志 → 多 sink 支持，
达到 muduo 日志库的设计水平。

## 技能树

```
异步日志系统
├── 同步日志基础          ← 阶段 21
│   ├── 日志级别（DEBUG/INFO/WARN/ERROR/FATAL）
│   ├── 格式化输出（时间戳 + 线程 ID + 文件行号）
│   └── 流式接口（LOG_INFO << "msg" << val）
├── 双缓冲异步日志        ← 阶段 22
│   ├── 前台缓冲区（业务线程写）
│   ├── 后台线程（被新日志/停止唤醒后批量写文件）
│   └── 双缓冲交换（zero-copy 思路）
├── 多 Sink 支持          ← 阶段 23
│   ├── FileSink（按天滚动）
│   ├── ConsoleSink（终端彩色输出）
│   └── Sink 抽象接口
└── 性能压测 + 完善       ← 阶段 24
    ├── 多线程并发写入压测
    ├── 与 spdlog / glog 性能对比
    └── 完整日志库整合
```

## 阶段拆解

| 阶段 | 主题 | 目录 |
|----|------|------|
| 阶段 21 | 同步日志基础 | [21-SyncLogger/](./21-SyncLogger/) |
| 阶段 22 | 双缓冲异步日志 | [22-AsyncLogger/](./22-AsyncLogger/) |
| 阶段 23 | 多 Sink 支持 | [23-MultiSink/](./23-MultiSink/) |
| 阶段 24 | 性能压测 + 整合 | [24-Perf-Integration/](./24-Perf-Integration/) |

## 验收标准

- [x] 多线程异步写入正确性验证通过：4 线程共 8000 条日志无丢失
- [x] FileSink 按日期命名日志文件，ConsoleSink 同步收到相同日志
- [x] 完成 enqueue latency smoke test：本机约 2.548 μs/message
- [ ] 多线程 100 万条日志，异步日志吞吐量 > 同步日志 5 倍以上（待专项 benchmark）
- [ ] 历史文件自动压缩（未实现，后续作为运维策略补充）
- [ ] 与 spdlog / glog 性能差距 < 2 倍（未接入第三方依赖）
- [ ] 日志库对业务线程的平均延迟 < 1μs（当前 smoke test 未达到）

## 模块完成情况

- 阶段 21：完成同步 logger、日志级别、格式化和流式接口
- 阶段 22：完成异步 logger、前台缓冲、后台线程、批量 flush 和关闭流程
- 阶段 23：完成 Sink 抽象、MemorySink、ConsoleSink、FileSink
- 阶段 24：完成多线程无丢失验证、多 sink 验证和 enqueue latency smoke test

## 模块总结

本模块已完成异步日志系统的主体闭环：前端以流式接口生成日志，logger 负责同步或异步派发，sink 负责具体输出。当前工程重点是结构正确性和可复用性；百万级吞吐、第三方对比、历史压缩和微秒级延迟属于后续性能工程任务。

详细总结见 `Note/C++-Note/Month06-异步日志系统实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/async_logger -B CPP-Practice/async_logger/build
cmake --build CPP-Practice/async_logger/build --config Release
.\CPP-Practice\async_logger\build\Release\async_logger_demo.exe
```
