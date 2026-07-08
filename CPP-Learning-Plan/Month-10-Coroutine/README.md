# Month 10 — 协程库（C++20 coroutine / ucontext Fiber）

## 月度目标

基于 `ucontext_t` 实现有栈协程（Fiber），构建协程调度器、Channel 通信、
并将协程与网络 IO 整合（协程式异步编程）。

> 当前 Windows/MSVC 环境不提供 POSIX `ucontext_t`，本月代码采用标准 C++20 coroutine 完成无栈协程教学实现；ucontext 有栈协程与 epoll hook 保留为 Linux/POSIX 专项。

## 技能树

```
ucontext 协程库
├── Fiber 基础              ← Week 37
│   ├── ucontext_t 上下文切换
│   ├── 栈分配与管理
│   └── Fiber 生命周期
├── 协程调度器              ← Week 38
│   ├── 运行队列（ready queue）
│   ├── yield / resume 语义
│   └── 多线程协程池
├── Channel 通信            ← Week 39
│   ├── 无缓冲 Channel（同步握手）
│   ├── 有缓冲 Channel（异步投递）
│   └── select 多路等待
└── 协程 + 网络 IO 整合    ← Week 40
    ├── hook 系统调用（read/write/sleep）
    ├── 协程式 epoll 等待
    └── 协程 HTTP 客户端示例
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 37 | Fiber 基础 | [Week-37-Fiber-Basics/](./Week-37-Fiber-Basics/) |
| Week 38 | 协程调度器 | [Week-38-Scheduler/](./Week-38-Scheduler/) |
| Week 39 | Channel 通信 | [Week-39-Channel/](./Week-39-Channel/) |
| Week 40 | 协程 + 网络 IO | [Week-40-Async-IO/](./Week-40-Async-IO/) |

## 验收标准

- [x] C++20 `Task` 支持启动、yield、resume 和销毁
- [x] Scheduler ready queue 能调度多个协程
- [x] Channel 支持缓冲 send/recv 和 `select_recv`
- [x] 协程式 HTTP client core 能完成多个模拟请求
- [ ] 10 万个有栈协程并发运行，内存占用 < 4GB（需要 ucontext/POSIX 专项）
- [ ] 协程式 HTTP 客户端并发发出 1000 个真实请求（需要网络 client/server 环境）
- [ ] 协程切换开销 < 100ns（需要专项 benchmark）

## 月度完成情况

- Week 37：完成 C++20 Task/promise/coroutine_handle 生命周期
- Week 38：完成 Scheduler、yield/resume 和多任务调度
- Week 39：完成 Channel、缓冲收发和 select_recv
- Week 40：完成 async_sleep 和协程式 HTTP client core

## 月度总结

Month10 已完成标准 C++20 coroutine 的核心闭环。虽然没有在 Windows 下实现 POSIX ucontext 有栈协程，但已经把协程对象、调度器、Channel 通信和异步接口风格跑通。后续切 Linux/POSIX 后，可以用同一套调度/Channel 思路替换底层 fiber 或 epoll hook。

详细总结见 `Note/C++-Note/Month10-协程库实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/coroutine_lib -B CPP-Practice/coroutine_lib/build
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```
