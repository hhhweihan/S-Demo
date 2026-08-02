# 协程库（C++20 coroutine / ucontext Fiber）

## 模块目标

基于 `ucontext_t` 实现有栈协程（Fiber），构建协程调度器、Channel 通信、
并将协程与网络 IO 整合（协程式异步编程）。

## 实现口径（真实落地 vs 跨平台模拟）

本模块提交进 `CPP-Practice/coroutine_lib/`（`mini_coroutine.h`）的是 **C++20 无栈协程**：`Task` + `promise_type` + **单线程 FIFO** `Scheduler` + `Channel`。这里没有 `ucontext_t` 有栈协程，也没有多线程工作窃取调度器——它们是设计/概念内容。各日 `[x]` 记录的是协程对象、调度、Channel 通信的**概念与设计掌握**。（当前 Windows/MSVC 环境不提供 POSIX `ucontext_t`，故底层 fiber 与 epoll hook 留待 Linux/POSIX 专项。）

下一步 Linux 补课（原生实现待办）：

- [ ] `ucontext_t` 有栈协程 Fiber（栈分配/管理 + 上下文切换）
- [ ] 多线程工作窃取调度器（每线程本地 deque + steal from back）
- [ ] 协程 hook 系统调用（read/write/sleep）+ 协程式 epoll 等待
- [ ] 10 万有栈协程并发 / 协程切换开销 benchmark

> 想看真实 epoll + timerfd 事件循环落地，见 `CPP-Practice/raft_kv/net/`（`event_loop.h` 真实 `epoll_create1`/`epoll_wait` LT 模式 + `timerfd`）。

## 技能树

```
ucontext 协程库
├── Fiber 基础              ← 阶段 37
│   ├── ucontext_t 上下文切换
│   ├── 栈分配与管理
│   └── Fiber 生命周期
├── 协程调度器              ← 阶段 38
│   ├── 运行队列（ready queue）
│   ├── yield / resume 语义
│   └── 多线程协程池
├── Channel 通信            ← 阶段 39
│   ├── 无缓冲 Channel（同步握手）
│   ├── 有缓冲 Channel（异步投递）
│   └── select 多路等待
└── 协程 + 网络 IO 整合    ← 阶段 40
    ├── hook 系统调用（read/write/sleep）
    ├── 协程式 epoll 等待
    └── 协程 HTTP 客户端示例
```

## 阶段拆解

| 阶段 | 主题 | 目录 |
|----|------|------|
| 阶段 37 | Fiber 基础 | [37-Fiber-Basics/](./37-Fiber-Basics/) |
| 阶段 38 | 协程调度器 | [38-Scheduler/](./38-Scheduler/) |
| 阶段 39 | Channel 通信 | [39-Channel/](./39-Channel/) |
| 阶段 40 | 协程 + 网络 IO | [40-Async-IO/](./40-Async-IO/) |

## 验收标准

- [x] C++20 `Task` 支持启动、yield、resume 和销毁
- [x] Scheduler ready queue 能调度多个协程
- [x] Channel 支持缓冲 send/recv 和 `select_recv`
- [x] 协程式 HTTP client core 能完成多个模拟请求
- [ ] 10 万个有栈协程并发运行，内存占用 < 4GB（需要 ucontext/POSIX 专项）
- [ ] 协程式 HTTP 客户端并发发出 1000 个真实请求（需要网络 client/server 环境）
- [ ] 协程切换开销 < 100ns（需要专项 benchmark）

## 模块完成情况

- 阶段 37：完成 C++20 Task/promise/coroutine_handle 生命周期
- 阶段 38：完成 Scheduler、yield/resume 和多任务调度
- 阶段 39：完成 Channel、缓冲收发和 select_recv
- 阶段 40：完成 async_sleep 和协程式 HTTP client core

## 模块总结

本模块已完成标准 C++20 coroutine 的核心闭环。虽然没有在 Windows 下实现 POSIX ucontext 有栈协程，但已经把协程对象、调度器、Channel 通信和异步接口风格跑通。后续切 Linux/POSIX 后，可以用同一套调度/Channel 思路替换底层 fiber 或 epoll hook。

详细总结见 `Note/C++-Note/协程库实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/coroutine_lib -B CPP-Practice/coroutine_lib/build
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```
