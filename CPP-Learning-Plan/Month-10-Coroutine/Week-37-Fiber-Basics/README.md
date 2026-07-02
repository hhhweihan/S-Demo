# Week 37 — Fiber 基础（ucontext）

## 本周目标

使用 POSIX `ucontext_t` API 实现有栈协程（Fiber），理解上下文切换原理。

## 本周完成情况

- [x] Day 253：梳理 ucontext 有栈协程原理和 Windows 环境限制
- [x] Day 254：实现 C++20 `Task::promise_type`
- [x] Day 255：管理 `coroutine_handle` 生命周期
- [x] Day 256：实现 initial/final suspend 语义
- [x] Day 257：验证 Task 可启动、恢复和销毁
- [x] Day 258：整理有栈/无栈协程差异
- [x] Day 259：完成 Week37 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `Task`：C++20 coroutine promise/handle 生命周期
- 周复盘：`Note/C++-Note/Week37-Coroutine基础-复盘总结.md`

## Week 37 总结

Week37 原计划使用 POSIX `ucontext_t`，但当前 Windows/MSVC 环境不支持该 API。本周改用标准 C++20 coroutine 实现无栈协程版本，重点理解 promise、coroutine_handle、initial_suspend、final_suspend 和生命周期管理。有栈协程的栈切换与内存压测保留为 Linux/POSIX 专项。

## 验证命令

```powershell
cmake -S CPP-Practice/coroutine_lib -B CPP-Practice/coroutine_lib/build
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```

---
