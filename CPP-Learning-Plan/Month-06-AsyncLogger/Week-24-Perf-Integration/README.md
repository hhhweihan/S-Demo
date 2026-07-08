# Week 24 — 性能压测 + 日志库整合

## 本周目标

完成日志库的性能压测、与 spdlog/glog 对比，整合所有功能并发布为可复用库。

## 本周完成情况

- [x] Day 162：整合同步 logger、异步 logger 和多 sink
- [x] Day 163：完成多线程写入正确性验证
- [x] Day 164：完成本地 smoke test；spdlog/glog 对比留待外部依赖专项压测
- [x] Day 165：验证 FileSink/ConsoleSink 同时接收日志
- [x] Day 166：整理可复用库 README 和构建命令
- [x] Day 167：完成月度功能验收，明确未完成的性能对比项
- [x] Day 168：完成 Week24 和 Month06 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：`CPP-Practice/async_logger/`
- 周复盘：`Note/C++-Note/Week24-异步日志性能整合复盘总结.md`
- 月总结：`Note/C++-Note/Month06-异步日志系统实战总结.md`

## Week 24 总结

Week24 完成日志库整合与本地验证：同步写入、异步后台 flush、多线程无丢失、多 sink 输出和 enqueue latency smoke test 都已跑通。本周没有引入 spdlog/glog，也没有跑百万级基准，因此相关指标保留为后续专项压测，而不是在文档里勾成完成。

## 验证命令

```powershell
cmake -S CPP-Practice/async_logger -B CPP-Practice/async_logger/build
cmake --build CPP-Practice/async_logger/build --config Release
.\CPP-Practice\async_logger\build\Release\async_logger_demo.exe
```

---
