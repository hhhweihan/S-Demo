# C++ 后端进阶路线图

这是一条自定节奏的 C++ 后端学习路线：从语言内功到自研标准库、系统组件、网络异步，最后到分布式。
每个模块配套一个可编译工程，放在 [CPP-Practice/README.md](../CPP-Practice/README.md)。

路线目标不是把每个方向都包装成“生产级项目”，而是把 C++ 后端核心主题拆成可实现、可测试、可复盘的训练模块。文档会尽量明确区分真实系统实现、教学模拟、proxy benchmark 和待补专项。

## 如何使用这份路线

- **编号是顺序，不是排期。** 序号只表示推荐的学习次序和前后依赖（后面的模块会用到前面的产出），
  不代表哪周哪月必须做完。有人两周啃下一块，有人两个月，都正常——按自己的节奏走。
- **完成的唯一标准是「能脱稿重写」。** 不看资料，能从零把这个模块重新写出来，才算掌握；
  写不出来就说明还停在「看懂」，回到实现和验证即可（详见文末）。
- **先读模块 README，再动手。** 每个模块目录下的 README 说明目标、拆解和验收；
  动手实现对应的 CPP-Practice 工程，跑通测试，再回填 [Progress.md](./Progress.md)。
- **实现不等于掌握。** `Progress.md` 中的“已实现，待脱稿重写”表示代码和测试已经闭环，但还没有升级为“不看资料从零重写”的已掌握状态。
- **路线是开放的。** 编号会一直往后加，后端要学的东西没有尽头（见文末「扩展区」）。

## 路线总览

序号只标顺序与依赖；「配套代码」是该模块要产出的可运行工程，「计划目录」是拆解与每日记录。

### 一、语言内功

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 01 | 内存管理 | 内存池库 + 手写智能指针 | [memory_pool](../CPP-Practice/memory_pool)、[month1_benchmark](../CPP-Practice/month1_benchmark)、[raii](../CPP-Practice/raii) | [01-内存管理](./01-内存管理/README.md) |
| 02 | 并发编程 | 线程池库 + SPSC 无锁队列 | [thread_pool](../CPP-Practice/thread_pool)、[lock_free](../CPP-Practice/lock_free) | [02-并发编程](./02-并发编程/README.md) |
| 03 | 模板与泛型 | 手写 tuple / variant / type_list | [template_traits](../CPP-Practice/template_traits)、[variadic_tuple](../CPP-Practice/variadic_tuple)、[type_erasure](../CPP-Practice/type_erasure)、[advanced_templates](../CPP-Practice/advanced_templates) | [03-模板泛型](./03-模板泛型/README.md) |

### 二、自研标准库

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 04 | STL 序列容器 | MyVector / MyDeque / MyList + 迭代器体系 | [stl_sequence](../CPP-Practice/stl_sequence) | [04-STL-Sequence](./04-STL-Sequence/README.md) |
| 05 | STL 关联容器 | 红黑树 + set/map + unordered_map + 算法 | [stl_associative](../CPP-Practice/stl_associative) | [05-STL-Associative](./05-STL-Associative/README.md) |

### 三、系统组件与存储

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 06 | 异步日志系统 | 双缓冲异步日志 + 多 Sink + PoolAllocator 接入 | [async_logger](../CPP-Practice/async_logger) | [06-AsyncLogger](./06-AsyncLogger/README.md) |
| 07 | LevelDB 存储引擎 | LSM 教学实现 + CRC WAL + 块式 SSTable + tombstone + crash safety | [leveldb_mini](../CPP-Practice/leveldb_mini) | [07-LevelDB](./07-LevelDB/README.md) |

### 四、网络与异步

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 08 | 网络编程 | 跨平台 Reactor 核 + Buffer 拆包 + HTTP/RPC core | [network_reactor](../CPP-Practice/network_reactor) | [08-网络编程](./08-网络编程/README.md) |
| 09 | 高性能网络库 | TimerWheel + io_uring 概念模拟 + 连接池/负载均衡 | [high_perf_net](../CPP-Practice/high_perf_net) | [09-HighPerfNet](./09-HighPerfNet/README.md) |
| 10 | 协程库 | C++20 coroutine + 调度器 + Channel | [coroutine_lib](../CPP-Practice/coroutine_lib) | [10-Coroutine](./10-Coroutine/README.md) |

### 五、分布式

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 11 | RPC 框架 | protobuf-like 编码 + 帧协议 + 服务发现 + 拦截器 | [rpc_framework](../CPP-Practice/rpc_framework) | [11-RPC](./11-RPC/README.md) |
| 12 | Mini Raft KV | Raft 仿真 + 真持久化 + ReadIndex/Pre-Vote + 真 TCP/epoll 节点 | [raft_kv](../CPP-Practice/raft_kv) | [12-RaftKV](./12-RaftKV/README.md) |

进度记录见 [Progress.md](./Progress.md)。

## 判断是否真正掌握

> 不看资料，能从零把这个模块重新写出来，才算真正掌握。
> 如果写不出来，说明还停留在「看懂」阶段，需要回到实现和验证。

这也是为什么这条路线不设 deadline：掌握以「能重写」为准，而不是以「时间到了」为准。

## 验证口径

- 默认验证以 [CPP-Practice](../CPP-Practice/README.md) 的根级 CMake/CTest 为准。
- 涉及真实网络、真实磁盘崩溃、压测或外部依赖的模块，会在模块 README 中单独说明是否为真实验证、教学模拟或 proxy benchmark。
- 对还没做真实 benchmark 的模块，文档应保留为“待专项”，不把本机 smoke test 写成生产指标。

## 扩展区（未完待续）

编号 12 不是终点，只是目前整理到的部分。后续会继续往后加，方向包括但不限于：

- io_uring 真实化（liburing，Linux）、高并发连接压测 + 火焰图
- lock-free 真实 hazard pointer / epoch-based reclamation
- Raft joint consensus、snapshot 文件持久化策略和跨机器部署验证
- LevelDB mini 的 MANIFEST / VersionSet / TableCache / block cache
- 更多存储 / 调度 / 可观测性主题

## 与仓库其他目录的关系

- 根目录 [README.md](../README.md) 是整个仓库的入口。
- [CPP-Practice/README.md](../CPP-Practice/README.md) 放和这条主线配套的练习代码。
- 涉及真实外部依赖或压测指标的模块，会在对应 README 中区分「本机教学模拟 / Proxy 验收」与「真实 benchmark 未执行」。
