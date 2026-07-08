# Week 40 — 协程 + 网络 IO 整合

## 本周目标

将协程与 epoll 事件循环整合：hook 阻塞系统调用，实现协程式异步编程（
写同步代码，实际执行异步 IO）。

## 本周完成情况

- [x] Day 274：实现 `async_sleep` awaiter 教学版本
- [x] Day 275：实现 `AsyncHttpClientCore` 模拟协程式 GET
- [x] Day 276：整理 hook sleep/read/write 的 POSIX 实现边界；未在 Windows hook 系统调用
- [x] Day 277：验证多个模拟 HTTP 请求完成
- [x] Day 278：完成 C++20 coroutine 功能验收；10 万协程内存压测留待专项 benchmark
- [x] Day 279：整理协程库架构图
- [x] Day 280：完成 Week40 和 Month10 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `SleepAwaiter`
- `AsyncHttpClientCore`
- 周复盘：`Note/C++-Note/Week40-协程异步IO复盘总结.md`
- 月总结：`Note/C++-Note/Month10-协程库实战总结.md`

## Week 40 总结

Week40 完成协程式异步编程模型的教学闭环：协程可以挂起、调度器可以恢复、Channel 可以传递结果、HTTP client core 可以用同步风格表达异步流程。真实 epoll hook 和系统调用替换需要 Linux/POSIX 环境，当前工程不伪造该部分结果。

## 验证命令

```powershell
cmake -S CPP-Practice/coroutine_lib -B CPP-Practice/coroutine_lib/build
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```

---
