# CPP-Practice

## 这个目录是干什么的

这里存放和主线学习计划配套的练习代码，重点是把 `CPP-Learning-Plan` 中的阶段目标落实为可编译、可测试、可复盘的小项目。

## 模块索引

| 模块 | 对应阶段 | 内容 |
| --- | --- | --- |
| [memory_pool](./memory_pool) | Month 01 | 固定块分配器、PoolAllocator、STL allocator 适配 |
| [month1_benchmark](./month1_benchmark) | Month 01 | 内存池和智能指针相关 benchmark |
| [raii](./raii) | Month 01 | FileGuard、ScopeGuard 等 RAII 练习 |
| [thread_pool](./thread_pool) | Month 02 | BlockingQueue、ThreadPool、同步原语 |
| [lock_free](./lock_free/README.md) | Month 02 | memory_order、SPSCQueue、无锁栈、ABA 对比 |
| [template_traits](./template_traits) | Month 03 | type_traits、SFINAE 和模板基础 |
| [variadic_tuple](./variadic_tuple) | Month 03 | 变参模板和手写 Tuple |
| [type_erasure](./type_erasure) | Month 03 | Optional、Variant、Function 等类型擦除练习 |
| [advanced_templates](./advanced_templates) | Month 03 | CRTP、Policy-Based Design、concepts、constexpr |
| [stl_sequence](./stl_sequence) | Month 04 | 序列容器、迭代器和适配器练习 |
| [stl_associative](./stl_associative) | Month 05 | 关联容器、哈希表和 allocator 练习 |
| [async_logger](./async_logger) | Month 06 | 同步/异步日志、多 Sink、性能 smoke test |
| [leveldb_mini](./leveldb_mini) | Month 07 | LevelDB / LSM Tree 教学实现 |
| [network_reactor](./network_reactor) | Month 08 | Reactor、HTTP 解析、同进程 RPC 模拟 |
| [high_perf_net](./high_perf_net) | Month 09 | 时间轮、io_uring 概念模拟、连接池 |
| [coroutine_lib](./coroutine_lib) | Month 10 | C++20 coroutine、调度器、Channel、异步模型 |
| [rpc_framework](./rpc_framework) | Month 11 | 帧协议、RPC 调用、服务发现和拦截器 |
| [raft_kv](./raft_kv) | Month 12 | Raft 选举、日志复制、KV 状态机、快照 |

## 从哪里开始看

- 先看上层计划说明：[../CPP-Learning-Plan/README.md](../CPP-Learning-Plan/README.md)
- 如果想看 Month 01，对应入口通常是 [memory_pool](./memory_pool) 和 [month1_benchmark](./month1_benchmark)
- 如果想直接看并发方向，可以从 [thread_pool](./thread_pool) 或 [lock_free/README.md](./lock_free/README.md) 开始
- 如果想看模板泛型基础，可以从 [template_traits](./template_traits) 开始，再顺着 [variadic_tuple](./variadic_tuple)、[type_erasure](./type_erasure) 和 [advanced_templates](./advanced_templates) 往后看
- 如果想看后半程项目，可以从 [async_logger](./async_logger)、[leveldb_mini](./leveldb_mini)、[network_reactor](./network_reactor)、[rpc_framework](./rpc_framework) 和 [raft_kv](./raft_kv) 中选择一个完整跑通

不同子目录基本都是独立构建的，通常需要进到对应目录里单独生成 `build` 再跑自己的构建命令。
