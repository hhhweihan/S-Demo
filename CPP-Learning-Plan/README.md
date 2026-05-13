# C++ 十二个月进阶计划

这里主要整理 12 个月的 C++ 学习路线、阶段说明和进度记录。配套代码主要放在 [CPP-Practice/README.md](../CPP-Practice/README.md)。

## 路线总览

| 月份 | 主题 | 核心产出 |
| --- | --- | --- |
| Month 01 | 内存管理实战 | 内存池库 + 手写智能指针 |
| Month 02 | 并发编程实战 | 线程池库 + SPSC 无锁队列 |
| Month 03 | 模板与泛型深入 | 手写 tuple / variant / type_list |
| Month 04 | 自编 STL（序列容器） | MyVector / MyDeque / MyList + 迭代器体系 |
| Month 05 | 自编 STL（关联容器） | 红黑树 + set/map + unordered_map + 算法 |
| Month 06 | 异步日志系统 | 双缓冲异步日志 + 多 Sink + PoolAllocator 接入 |
| Month 07 | LevelDB 源码精读 | SkipList + Arena + SSTable + Compaction 仿写 |
| Month 08 | 网络编程 | Reactor 框架 + HTTP Server |
| Month 09 | 高性能网络库 | io_uring + 定时器轮 + 连接池 |
| Month 10 | 协程库 | ucontext Fiber + 调度器 + Channel |
| Month 11 | RPC 框架 | protobuf + 帧协议 + 服务发现 |
| Month 12 | Mini Raft KV | Leader 选举 + 日志复制 + KV 状态机 |

## 快速导航

- [Month 01 — 内存管理](./Month-01-内存管理/README.md)
- [Month 02 — 并发编程](./Month-02-并发编程/README.md)
- [Month 03 — 模板泛型](./Month-03-模板泛型/README.md)
- [Month 04 — STL 序列容器](./Month-04-STL-Sequence/README.md)
- [Month 05 — STL 关联容器](./Month-05-STL-Associative/README.md)
- [Month 06 — 异步日志](./Month-06-AsyncLogger/README.md)
- [Month 07 — LevelDB](./Month-07-LevelDB/README.md)
- [Month 08 — 网络编程](./Month-08-网络编程/README.md)
- [Month 09 — 高性能网络库](./Month-09-HighPerfNet/README.md)
- [Month 10 — 协程库](./Month-10-Coroutine/README.md)
- [Month 11 — RPC 框架](./Month-11-RPC/README.md)
- [Month 12 — Raft KV](./Month-12-RaftKV/README.md)
- [学习进度](./Progress.md)

平时我一般是先看当月 README，再按周目录往下拆；代码写完后回填进度，周完成后更新 [Progress.md](./Progress.md)。

## 判断是否真正掌握

> 不看资料，能从零把这个模块重新写出来，才算真正掌握。
> 如果写不出来，说明还停留在“看懂”阶段，需要回到实现和验证。

## 与仓库其他目录的关系

- 根目录 [README.md](../README.md) 是整个仓库的入口。
- [CPP-Practice/README.md](../CPP-Practice/README.md) 放和这条主线配套的练习代码。
