# Week31 多线程 Reactor / HTTP 复盘总结

## 核心目标

Week31 理解 one-loop-per-thread 模型，并完成 HTTP GET core。

## one-loop-per-thread

多线程 Reactor 通常让每个 IO 线程拥有自己的 EventLoop。连接被分配到某个 loop 后，后续 IO 事件尽量在同一线程处理，减少跨线程同步。

本周实现 `EventLoopThreadPool` 的轮转分配模型，用于表达连接分发策略。

## HTTP Core

本周实现了：

- 请求行解析：method / path / version
- header 解析
- GET handler 注册
- HTTP/1.1 响应生成

## 当前边界

没有在 Windows 下实现真实 socket accept/read/write，也没有跑 wrk QPS。当前验证的是 HTTP 协议 core 和 Reactor 抽象，真实压测需要 Linux/WSL2 环境。
