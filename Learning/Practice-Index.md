# Learning 实践项目索引

## 这个索引是干什么的

本页汇总各学习模块 `practice` 中的练习代码，重点是把阶段目标落实为可编译、可测试、可复盘的小项目。平时先读模块 README，然后进入同目录的实践工程，无需再跨顶层目录查找。

这些项目以学习和工程训练为主：每个模块尽量有 demo、GoogleTest/CTest 和 README 说明。对真实网络、真实磁盘崩溃、压测或外部系统依赖，README 会明确区分真实验证、教学模拟和 proxy benchmark。

## 社招能力训练入口

当前优先跟随 [嵌入式能力路线](Embedded-Roadmap.md)：复用内存、队列、容器与日志的核心原理及 host 测试，另外训练启动、外设、DMA、RTOS 和 BSP。现有线程／文件／网络接口不能直接充当 MCU 驱动，host 测试不能替代板上验证。

起步先做 [embedded_basics](00-Embedded-Start/practice/embedded_basics/README.md)：独立 CMake 的 C11 环形缓冲／有界帧解析练习，带初始红色验收，不提供实现答案，也不接入根级默认构建与 CI。

[后端能力路线](Career-Roadmap.md) 保留为备选：`network_reactor` 和 `async_logger` 用于后续真实网络／有界服务化训练，存储／分布式按方向深化，AI 推理后置选修。两条路线不同时作为必修。

这些升级是计划，不改变下方模块的当前实现边界；尤其不能将 host 算法、FakePoller、io_uring 模拟或同进程 RPC 视为硬件、真实网络与推理服务的验证。

## 模块索引

| 模块 | 对应阶段 | 内容 |
| --- | --- | --- |
| [memory_pool](01-内存管理/practice/memory_pool) | Month 01 | 固定块分配器、PoolAllocator、STL allocator 适配 |
| [month1_benchmark](01-内存管理/practice/month1_benchmark) | Month 01 | 内存池和智能指针相关 benchmark |
| [raii](01-内存管理/practice/raii) | Month 01 | FileGuard、ScopeGuard 等 RAII 练习 |
| [thread_pool](02-并发编程/practice/thread_pool) | Month 02 | BlockingQueue、ThreadPool、同步原语 |
| [lock_free](02-并发编程/practice/lock_free/README.md) | Month 02 | memory_order、SPSCQueue、无锁栈、ABA 对比 |
| [template_traits](03-模板泛型/practice/template_traits) | Month 03 | type_traits、SFINAE 和模板基础 |
| [variadic_tuple](03-模板泛型/practice/variadic_tuple) | Month 03 | 变参模板和手写 Tuple |
| [type_erasure](03-模板泛型/practice/type_erasure) | Month 03 | Optional、Variant、Function 等类型擦除练习 |
| [advanced_templates](03-模板泛型/practice/advanced_templates) | Month 03 | CRTP、Policy-Based Design、concepts、constexpr |
| [stl_sequence](04-STL-Sequence/practice/stl_sequence) | Month 04 | 序列容器、迭代器和适配器练习 |
| [stl_associative](05-STL-Associative/practice/stl_associative) | Month 05 | 关联容器、哈希表和 allocator 练习 |
| [async_logger](06-AsyncLogger/practice/async_logger) | Month 06 | 同步/异步日志、多 Sink、性能 smoke test |
| [leveldb_mini](07-LevelDB/practice/leveldb_mini) | Month 07 | LSM Tree、CRC WAL、块式 SSTable、tombstone、compaction、crash safety |
| [network_reactor](08-网络编程/practice/network_reactor) | Month 08 | 跨平台 Reactor 核、Buffer 拆包、HTTP core、同进程 RPC 模拟 |
| [high_perf_net](09-HighPerfNet/practice/high_perf_net) | Month 09 | 时间轮、io_uring 概念模拟、连接池 |
| [coroutine_lib](10-Coroutine/practice/coroutine_lib) | Month 10 | C++20 coroutine、调度器、Channel、异步模型 |
| [rpc_framework](11-RPC/practice/rpc_framework) | Month 11 | 帧协议、RPC 调用、服务发现和拦截器 |
| [raft_kv](12-RaftKV/practice/raft_kv) | Month 12 | Raft 选举、日志复制、KV 状态机、快照、生产化、真实网络化 |

## 代表项目

如果只想看几个最能体现工程能力的模块，优先看：

- [raft_kv](12-RaftKV/practice/raft_kv)：确定性 Raft 仿真 + 真持久化 + ReadIndex/Pre-Vote + 真实 TCP/epoll 多进程节点。
- [leveldb_mini](07-LevelDB/practice/leveldb_mini)：LSM 主路径 + 崩溃安全 WAL + 块式 SSTable + tombstone/compaction。
- [async_logger](06-AsyncLogger/practice/async_logger)：同步/异步日志、多 Sink、后台 flush 和并发组件边界。
- [rpc_framework](11-RPC/practice/rpc_framework)：长度前缀帧、protobuf-like 编码、同步/异步 RPC、服务发现和拦截器。

## 构建与验证

根目录提供聚合 CMake，可以一次构建全部模块并运行测试：

```bash
cmake -S .. -B ../build -DCMAKE_BUILD_TYPE=Debug
cmake --build ../build -j
ctest --test-dir ../build --output-on-failure
```

如果你在仓库根目录执行，则命令是：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure
```

常用选项：

- `-DS_DEMO_BUILD_TESTS=OFF`：只构建 demo，不拉取/构建 GoogleTest。
- `-DS_DEMO_SANITIZER=address|thread|undefined`：开启对应 sanitizer。
- 真实进程/真实网络类慢测试可能带 `integration` label，默认验证时可用 `-LE integration` 排除，专项验证时再用 `-L integration` 单独跑。

## 实现边界

- [network_reactor](08-网络编程/practice/network_reactor)、[rpc_framework](11-RPC/practice/rpc_framework)、[high_perf_net](09-HighPerfNet/practice/high_perf_net) 中部分指标是教学 proxy benchmark，不等价于真实网络压测。
- [raft_kv](12-RaftKV/practice/raft_kv) 同时包含确定性仿真版和真实网络版，两者各自验证不同问题，不互相替代。
- [leveldb_mini](07-LevelDB/practice/leveldb_mini) 已覆盖 crash safety 主线，但不是完整 LevelDB 复刻；MANIFEST、VersionSet、TableCache、block cache 等仍是后续专项。

## 从哪里开始看

- 先看上层计划说明：[../Learning/README.md](README.md)
- 如果想看 Month 01，对应入口通常是 [memory_pool](01-内存管理/practice/memory_pool) 和 [month1_benchmark](01-内存管理/practice/month1_benchmark)
- 如果想直接看并发方向，可以从 [thread_pool](02-并发编程/practice/thread_pool) 或 [lock_free/README.md](02-并发编程/practice/lock_free/README.md) 开始
- 如果想看模板泛型基础，可以从 [template_traits](03-模板泛型/practice/template_traits) 开始，再顺着 [variadic_tuple](03-模板泛型/practice/variadic_tuple)、[type_erasure](03-模板泛型/practice/type_erasure) 和 [advanced_templates](03-模板泛型/practice/advanced_templates) 往后看
- 如果想看后半程项目，可以从 [async_logger](06-AsyncLogger/practice/async_logger)、[leveldb_mini](07-LevelDB/practice/leveldb_mini)、[network_reactor](08-网络编程/practice/network_reactor)、[rpc_framework](11-RPC/practice/rpc_framework) 和 [raft_kv](12-RaftKV/practice/raft_kv) 中选择一个完整跑通

不同子目录基本都是独立构建的，通常需要进到对应目录里单独生成 `build` 再跑自己的构建命令；如果要跑测试和 sanitizer，优先使用根级聚合构建。
