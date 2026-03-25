# Month 5 — 网络编程与 Reactor 框架

## 月度目标

从 Socket 基础到 Reactor 框架，实现一个能承受 5000 QPS 的 HTTP Server。
为 Month 6 的 Raft KV 提供网络层基础。

## 技能树

```
网络编程
├── Socket 基础            ← Week 17
│   ├── TCP 粘包处理
│   ├── 非阻塞 Socket
│   └── select/epoll
├── Reactor 框架            ← Week 18
│   ├── EventLoop
│   ├── Channel + Poller
│   └── 单线程 Reactor
├── 多线程 Reactor          ← Week 19
│   ├── one loop per thread
│   ├── TcpConnection + Buffer
│   └── HTTP Server
└── RPC 基础               ← Week 20
    ├── protobuf 序列化
    └── 简单 RPC 框架
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 17 | Socket 基础 + epoll | [Week-17.md](./Week-17.md) |
| Week 18 | Reactor 框架（单线程）| [Week-18.md](./Week-18.md) |
| Week 19 | 多线程 Reactor + HTTP Server | [Week-19.md](./Week-19.md) |
| Week 20 | protobuf + RPC | [Week-20.md](./Week-20.md) |

## 验收标准

- [ ] epoll LT 模式的 Echo Server 能支持 1000 并发连接
- [ ] HTTP Server 能正确响应 GET 请求，wrk 测试 QPS > 5000
- [ ] RPC 框架能在同机上做一次完整的请求-响应
- [ ] 代码无内存泄漏（ASan）

## 参考资料

- 参考：muduo 网络库（`https://github.com/chenshuo/muduo`）
- 书：《Linux 多线程服务端编程》（陈硕）第 6-8 章
- 你的笔记：`Note/C++-Note/C++笔记3/Linux多进程开发.pdf`
- epoll 手册：`man 7 epoll`

> **注意：** 本月任务在 Linux/WSL2 环境下进行（epoll 是 Linux 特有的）。
> Windows 用户请用 WSL2（推荐）或 Cygwin。
