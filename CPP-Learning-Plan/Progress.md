# 学习进度

> 这里只记录每个模块的**掌握状态**，不再按周/按天打卡。
> 节奏自定，完成以「能脱稿重写」为准——判定标准见 [README.md](./README.md#判断是否真正掌握)。

## 状态说明

- **未开始**：还没进入这个模块。
- **学习中**：正在读 / 写，配套工程尚未跑通完整产出。
- **已实现**：配套工程写完并跑通测试。
- **已实现，待脱稿重写**：代码和测试已闭环，但还需要找时间不看资料从零重写，才能升级为已掌握。
- **已掌握**：不看资料能从零重写——这是每个模块真正的终点。
- **待复习**：曾经掌握，隔了一段时间，需要回炉。

## 模块进度

| 序号 | 模块 | 状态 | 备注 |
| --- | --- | --- | --- |
| 01 | 内存管理 | 已实现，待脱稿重写 | 固定内存池 + PoolAllocator + 手写智能指针 + RAII 工具已完成 |
| 02 | 并发编程 | 已实现，待脱稿重写 | 阻塞队列 + 线程池 + 无锁队列 + 高级同步原语已完成 |
| 03 | 模板与泛型 | 已实现，待脱稿重写 | type_traits / SFINAE / Tuple / Optional / Variant / Function / CRTP / concepts / constexpr 已有配套练习 |
| 04 | STL 序列容器 | 已实现，待脱稿重写 | 迭代器体系、Vector/Deque、List/ForwardList、容器适配器已有配套练习 |
| 05 | STL 关联容器 | 已实现，待脱稿重写 | 红黑树、set/map、unordered_map、allocator 与算法练习已有实现 |
| 06 | 异步日志系统 | 已实现，待脱稿重写 | 同步日志、双缓冲异步日志、多 Sink、性能 smoke test 已完成 |
| 07 | LevelDB 存储引擎 | 已实现，待脱稿重写 | LSM 教学闭环 + CrashSafety：CRC WAL、块式 SSTable、tombstone、compaction、kill-9 持久性 |
| 08 | 网络编程 | 已实现，待脱稿重写 | 跨平台 Reactor 核、Buffer 拆包、HTTP core、同进程 RPC；真实 epoll/wrk 仍是 Linux 专项 |
| 09 | 高性能网络库 | 已实现，待脱稿重写 | TimerWheel、io_uring 概念模拟、连接池与负载均衡练习已完成；真实 liburing/压测待专项 |
| 10 | 协程库 | 已实现，待脱稿重写 | C++20 coroutine、调度器、Channel、异步 IO 模型练习已完成 |
| 11 | RPC 框架 | 已实现，待脱稿重写 | 帧协议、RPC 调用、服务发现、拦截器与 proxy benchmark 已完成；真实 socket/etcd/gRPC 非当前口径 |
| 12 | Mini Raft KV | 已实现，待脱稿重写 | 选举/复制/KV/快照/生产化(持久化+ReadIndex+Pre-Vote)+真实网络化(真 TCP+epoll+进程级 kill -9)全部跑通测试 |

> 「已实现」升到「已掌握」由自己判定：找一天不看资料，把该模块从零重写一遍并通过测试，就升级状态。
