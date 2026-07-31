# io_uring 异步 IO

## 本周目标

掌握 io_uring 的 SQ/CQ 环形队列机制，实现基于 io_uring 的异步文件和网络 IO，
与 epoll 性能对比。

## 本周完成情况

- [x] Day 232：理解 io_uring SQ/CQ 环形队列
- [x] Day 233：实现 `IoUringSimulator::submit`
- [x] Day 234：实现 completion queue 处理
- [x] Day 235：验证 read/send 请求完成结果
- [x] Day 236：整理固定缓冲区和减少系统调用的思路
- [x] Day 237：明确原生 io_uring 与 epoll 对比需 Linux 环境
- [x] Day 238：完成 Week34 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `IoUringSimulator`：SQ/CQ 概念模拟
- 周复盘：`Note/C++-Note/Week34-io_uring-复盘总结.md`

## 阶段小结
Week34 先完成 io_uring 的核心模型理解：提交队列（SQ）描述待执行 IO，完成队列（CQ）返回执行结果。当前 Windows 环境不支持原生 io_uring，因此代码采用模拟器验证队列语义；吞吐对比和固定缓冲区 benchmark 需要在 Linux 上补测。

## 验证命令

```powershell
cmake --build CPP-Practice/high_perf_net/build --config Release
.\CPP-Practice\high_perf_net\build\Release\high_perf_net_demo.exe
```

---
