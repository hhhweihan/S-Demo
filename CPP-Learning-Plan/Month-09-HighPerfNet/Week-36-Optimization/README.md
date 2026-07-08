# Week 36 — 整合 + 性能优化

## 本周目标

整合 Month 9 所有组件，实现零拷贝文件传输和多线程 accept，完成月度验收。

## 本周完成情况

- [x] Day 246：整理 zero-copy sendfile/splice 的适用场景
- [x] Day 247：整理 SO_REUSEPORT 多线程 accept 模型
- [x] Day 248：实现平台优化能力报告
- [x] Day 249：整合 TimerWheel、io_uring simulator、ConnectionPool 测试
- [x] Day 250：完成跨平台功能验收；10 万 QPS/连接压测留待 Linux/WSL2
- [x] Day 251：整理高性能网络技术栈图
- [x] Day 252：完成 Week36 和 Month09 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `OptimizationReport`
- 周复盘：`Note/C++-Note/Week36-高性能网络优化复盘总结.md`
- 月总结：`Note/C++-Note/Month09-高性能网络库实战总结.md`

## Week 36 总结

Week36 完成整合和优化边界梳理。zero-copy、SO_REUSEPORT、io_uring 性能对比和 10 万连接稳定性都强依赖 Linux 内核和系统参数；当前 Windows 工程验证的是组件模型和接口行为，并在文档中保留真实压测项。

## 验证命令

```powershell
cmake -S CPP-Practice/high_perf_net -B CPP-Practice/high_perf_net/build
cmake --build CPP-Practice/high_perf_net/build --config Release
.\CPP-Practice\high_perf_net\build\Release\high_perf_net_demo.exe
```

---
