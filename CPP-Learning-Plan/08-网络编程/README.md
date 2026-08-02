# 网络编程与 Reactor 框架

## 模块目标

从 Socket 基础到 Reactor 框架，实现一个能承受 5000 QPS 的 HTTP Server。
为后续 RPC 和 Raft KV 提供网络层基础。

## 实现口径（真实落地 vs 跨平台模拟）

本模块提交进 `CPP-Practice/network_reactor/` 的是一套**便携 Reactor 核**（`mini_reactor.h`：Buffer 长度前缀拆包 + Channel + FakePoller 内存事件分发 + 单线程 EventLoop），并非真实 POSIX socket 或 `epoll_wait`。各日 `[x]` 记录的是 socket / select / epoll(LT/ET) / timerfd 的**概念与设计掌握**；阻塞、非阻塞、select、epoll 原生 echo server 在本模块为概念研究，尚未提交原生实现。

下一步 Linux 补课（原生实现待办）：

- [ ] 原生阻塞 / 非阻塞 socket echo server（socket/bind/listen/accept + `fcntl(O_NONBLOCK)`）
- [ ] `select(2)` 多路复用 echo server
- [ ] 真实 `epoll_create1` / `epoll_ctl` / `epoll_wait` 的 LT 与 ET echo server
- [ ] `timerfd_create` 定时器集成进 epoll 事件循环
- [ ] wrk/ab 压测（QPS > 5000）与 ASan/TSan（Linux/WSL2）

> 想看真实 epoll + timerfd 落地，见 `CPP-Practice/raft_kv/net/`（`event_loop.h` 真实 `epoll_create1`/`epoll_ctl`/`epoll_wait` LT 模式 + `timerfd`，`raft_server.h` 为其上层服务）。

## 技能树

```
网络编程
├── Socket 基础            ← 阶段 29
│   ├── TCP 粘包处理
│   ├── 非阻塞 Socket
│   └── select/epoll
├── Reactor 框架            ← 阶段 30
│   ├── EventLoop
│   ├── Channel + Poller
│   └── 单线程 Reactor
├── 多线程 Reactor          ← 阶段 31
│   ├── one loop per thread
│   ├── TcpConnection + Buffer
│   └── HTTP Server
└── HTTP/RPC 复盘          ← 阶段 32
    ├── protobuf 序列化
    └── 简单 RPC 框架
```

## 阶段拆解

| 阶段 | 主题 | 目录 |
|----|------|------|
| 阶段 29 | Socket 基础 + epoll | [29-Socket-epoll/](./29-Socket-epoll/) |
| 阶段 30 | Reactor 框架（单线程）| [30-Reactor-SingleThread/](./30-Reactor-SingleThread/) |
| 阶段 31 | 多线程 Reactor + HTTP Server | [31-Reactor-MultiThread/](./31-Reactor-MultiThread/) |
| 阶段 32 | HTTP 服务器 + 模块复盘 | [32-HTTP-Review/](./32-HTTP-Review/) |

## 验收标准

- [x] Buffer 能正确处理长度字段粘包/半包
- [x] Reactor 核心抽象能分发 Channel read callback
- [x] HTTP Server Core 能正确响应 GET 请求
- [x] RPC 框架能在同机上做一次完整的请求-响应
- [ ] epoll LT 模式 Echo Server 支持 1000 并发连接（需要 Linux/WSL2）
- [ ] wrk 测试 QPS > 5000（需要真实 socket server 与 Linux/WSL2 压测）
- [ ] 代码无内存泄漏（ASan/TSan，待 Linux/WSL2 执行）

## 模块完成情况

- 阶段 29：完成长度字段 Buffer，理解 TCP 粘包/半包和 epoll 模型
- 阶段 30：完成 Channel/Poller/EventLoop 单线程 Reactor 抽象
- 阶段 31：完成 EventLoopThreadPool 和 HTTP GET core
- 阶段 32：完成 RPC frame 往返和模块综合验证

## 模块总结

本模块已完成网络编程的跨平台核心练习：协议层拆包、Reactor 事件分发、HTTP GET 响应和 RPC 请求-响应。由于当前环境是 Windows，epoll、wrk、ASan/TSan 属于 Linux/WSL2 专项验证，文档中保留为未完成项。

详细总结见 `Note/C++-Note/网络编程Reactor实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/network_reactor -B CPP-Practice/network_reactor/build
cmake --build CPP-Practice/network_reactor/build --config Release
.\CPP-Practice\network_reactor\build\Release\network_reactor_demo.exe
```

## 参考资料

- 参考：muduo 网络库（`https://github.com/chenshuo/muduo`）
- 书：《Linux 多线程服务端编程》（陈硕）第 6-8 章
- 你的笔记：`Note/C++-Note/C++笔记3/Linux多进程开发.pdf`
- epoll 手册：`man 7 epoll`

> **注意：** 本模块任务在 Linux/WSL2 环境下进行（epoll 是 Linux 特有的）。
> Windows 用户请用 WSL2（推荐）或 Cygwin。
