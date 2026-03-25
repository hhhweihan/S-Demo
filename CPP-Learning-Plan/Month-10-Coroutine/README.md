# Month 10 — 协程库（ucontext Fiber）

## 月度目标

基于 `ucontext_t` 实现有栈协程（Fiber），构建协程调度器、Channel 通信、
并将协程与网络 IO 整合（协程式异步编程）。

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

- [ ] 10 万个协程并发运行，内存占用 < 4GB（每协程栈 < 40KB）
- [ ] Channel 实现 Go 语言风格的 goroutine 通信
- [ ] 协程式 HTTP 客户端能并发发出 1000 个请求
- [ ] 协程切换开销 < 100ns（vs 线程切换 ~1μs）
