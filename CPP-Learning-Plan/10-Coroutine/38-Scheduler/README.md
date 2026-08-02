# 协程调度器

## 本周目标

实现协程调度器：管理多个 Fiber 的运行队列，支持 yield/resume 语义，
扩展到多线程协程池。

## 本周完成情况

- [x] Day 260：实现 Scheduler ready queue
- [x] Day 261：实现 `yield()` awaiter，把当前协程重新放回队列
- [x] Day 262：实现 `SchedulerScope` 管理当前调度器
- [x] Day 263：验证多个 Task 交替 resume
- [x] Day 264：整理 yield/resume 语义
- [x] Day 265：梳理多线程协程池设计边界
- [x] Day 266：完成 Week38 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `Scheduler`
- `YieldAwaiter`
- 周复盘：`Note/C++-Note/协程调度器-复盘总结.md`

## 阶段小结
Week38 完成最小调度器。ready queue 保存可运行 coroutine handle，`yield` 挂起当前协程并把它重新排队，`run` 循环恢复任务直到队列为空。这个模型虽然简单，但已经覆盖协作式调度的关键语义。

## 验证命令

```powershell
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```

---
