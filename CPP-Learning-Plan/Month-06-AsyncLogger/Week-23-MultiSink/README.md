# Week 23 — 多 Sink 支持

## 本周目标

将日志库扩展为多目标输出（Sink）架构，支持文件、控制台、网络等多种后端，
并实现 Sink 的热插拔。

## 本周完成情况

- [x] Day 155：定义 `Sink` 抽象接口
- [x] Day 156：实现 `ConsoleSink` 和测试缓冲输出
- [x] Day 157：实现 `FileSink` 和按日期命名日志文件
- [x] Day 158：实现 `MemorySink` 支持自动化断言
- [x] Day 159：验证同一条日志写入多个 sink
- [x] Day 160：整理 sink 热插拔设计边界
- [x] Day 161：完成 Week23 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `Sink` / `MemorySink` / `ConsoleSink` / `FileSink`
- FileSink 按日期命名文件，为按天滚动打基础
- 周复盘：`Note/C++-Note/Week23-多Sink日志-复盘总结.md`

## Week 23 总结

Week23 的重点是把“日志生产”和“日志去哪里”解耦。Logger 只面对 `Sink` 抽象，具体输出到内存、控制台还是文件都由 sink 决定。当前 `FileSink` 已完成按日期命名日志文件；历史压缩和自动清理属于后续运维策略，未在本周 demo 中实现。

## 验证命令

```powershell
cmake --build CPP-Practice/async_logger/build --config Release
.\CPP-Practice\async_logger\build\Release\async_logger_demo.exe
```

---
