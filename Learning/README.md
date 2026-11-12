# Learning — 学习模块入口

这里按模块组织嵌入式主线与 C++ 后端素材：嵌入式课程从语言合同到外设、RTOS、BSP 与真板交付，原有后端模块继续作为基础素材与备选深化。
每个模块的计划、代码和测试现在放在同一个目录：先读模块 README，再打开同目录的 `practice`。全部实现项目可从 [代码索引](Practice-Index.md) 进入。

## 从任务直接进入代码

先从 [嵌入式课程总入口](Embedded/README.md) 看学习顺序，按 [知识地图](Embedded/Knowledge-Map.md) 查漏。恢复学习时跟随 [E0 起步任务](00-Embedded-Start/README.md)，先读 [环形缓冲教学](00-Embedded-Start/Byte-Ring-Guide.md)，再打开同目录的 [练习](00-Embedded-Start/practice/embedded_basics/README.md)。

```text
Learning/
  README.md                  学习总入口
  Embedded-Roadmap.md         嵌入式主线
  Embedded/                  E1–E6 教学正文与知识地图
  Career-Roadmap.md           后端备选
  Progress.md                能力进度
  Practice-Index.md           全部实现项目索引
  00-Embedded-Start/
    README.md                E0 任务与诊断
    Byte-Ring-Guide.md        环形缓冲教学
    Frame-Parser-Guide.md     帧解析教学
    practice/embedded_basics/ 接口、待实现代码、tests、CMake
  01-内存管理/
    README.md                模块目标与实践入口
    01-固定内存池/            原有任务记录
    practice/
      memory_pool/
      month1_benchmark/
      raii/
  ...                        02–12 同样按模块组织
```

已有实现模块的 README 都有实践入口，原有每日任务中的代码路径也改成可点击链接。新增嵌入式阶段当前只有教学正文，未来代码放在各模块 `practice` 中，不创建虚假的实现入口。工程名与测试目标名保持不变，E0 的初始红色练习仍独立构建，不纳入默认聚合测试。

目录移动后，旧构建缓存中的绝对路径不再有效；应重新配置一个新的构建目录，不手动修改缓存。根级构建入口仍为仓库根目录。

Windows 使用 LLVM-MinGW 时，建议在 CMake Tools 选择 Ninja 生成器；本次验证中 MinGW Makefiles 无法打开中文模块目录下的 makefile。MSVC 与 Linux 的既有入口不变，POSIX 专项仍需 Linux 环境。

### 本次目录迁移的验证边界

- 550 个计划／实践源文件均已保留，旧顶层目录已移除；模块入口、任务中的代码链接和外部笔记引用已更新。
- 根级 CMake 的 18 个项目均可配置；Windows LLVM-MinGW + Ninja 实际构建了其中 16 个模块的 demo 与测试目标。
- 执行这 16 个模块的测试，15 个模块通过；RAII 的两个文件测试因原有 `P_tmpdir` 临时路径写法失败。失败源码与迁移前相同，本次没有修改其逻辑。
- LevelDB／Raft 的既有 POSIX 依赖未在本机完成验证，不将配置成功写成全部构建／测试通过；Linux CI 与专项仍需在对应环境执行。
- E0 独立工程已在新位置重新构建，10 个用例仍因未实现状态预期失败；没有补入练习答案或升级掌握状态。

路线目标不是把每个方向都包装成“生产级项目”，而是把 C++ 后端核心主题拆成可实现、可测试、可复盘的训练模块。文档会尽量明确区分真实系统实现、教学模拟、proxy benchmark 和待补专项。

## 社招能力主线

面向 3–5 年经验层级的系统软件学习，按目标岗位选择路线：

- **当前主线：[嵌入式能力路线](./Embedded-Roadmap.md)**。C/C++、Cortex-M 启动、外设／DMA、FreeRTOS、真板项目、BSP 与交付，再按岗位深化 MCU/BSP 或 Linux BSP/驱动。
- **教学正文：[嵌入式课程](Embedded/README.md) 与 [知识地图](Embedded/Knowledge-Map.md)**。逐课解释概念、练习步骤、检查点、证据和常见误区，不提供完整实现答案。
- **备选：[后端能力路线](./Career-Roadmap.md)**。C++ 基础、真实网络、可靠性、性能与交付，存储／分布式深化，AI 推理后置加分。

下面的 01–12 模块继续作为知识与实现素材库，不要求全部反复重写，也不要求完成所有选修后才开始投递。两条路线不同时作为必修；按能力验收推进，不设固定期限。新增专项均是计划，不表示现有工程已经达到相应标准。

## 如何使用这份路线

- **编号是顺序，不是排期。** 序号只表示推荐的学习次序和前后依赖（后面的模块会用到前面的产出），
  不代表哪周哪月必须做完。有人两周啃下一块，有人两个月，都正常——按自己的节奏走。
- **掌握以独立解决问题为准。** 核心结构需要能脱稿实现；完整工程需要能独立修改、定位故障、
  解释取舍并用测试／测量验证，不以反复重写所有组件为唯一标准（详见文末）。
- **先读模块 README，再动手。** 每个模块目录下的 README 说明目标、拆解和验收；
  直接打开本模块 `practice` 中的工程，跑通相应测试，再回填 [Progress.md](./Progress.md)。
- **实现不等于掌握。** [Progress.md](Progress.md) 中的“已实现，待脱稿重写”保留历史实现记录，仍须完成核心实现与独立工程验收，不能直接升级为已掌握。
- **路线是开放的。** 编号会一直往后加，后端要学的东西没有尽头（见文末「扩展区」）。

## 路线总览

序号只标顺序与依赖；「配套代码」是该模块要产出的可运行工程，「计划目录」是拆解与每日记录。

### 一、语言内功

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 01 | 内存管理 | 内存池库 + 手写智能指针 | [memory_pool](01-内存管理/practice/memory_pool)、[month1_benchmark](01-内存管理/practice/month1_benchmark)、[raii](01-内存管理/practice/raii) | [01-内存管理](./01-内存管理/README.md) |
| 02 | 并发编程 | 线程池库 + SPSC 无锁队列 | [thread_pool](02-并发编程/practice/thread_pool)、[lock_free](02-并发编程/practice/lock_free) | [02-并发编程](./02-并发编程/README.md) |
| 03 | 模板与泛型 | 手写 tuple / variant / type_list | [template_traits](03-模板泛型/practice/template_traits)、[variadic_tuple](03-模板泛型/practice/variadic_tuple)、[type_erasure](03-模板泛型/practice/type_erasure)、[advanced_templates](03-模板泛型/practice/advanced_templates) | [03-模板泛型](./03-模板泛型/README.md) |

### 二、自研标准库

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 04 | STL 序列容器 | MyVector / MyDeque / MyList + 迭代器体系 | [stl_sequence](04-STL-Sequence/practice/stl_sequence) | [04-STL-Sequence](./04-STL-Sequence/README.md) |
| 05 | STL 关联容器 | 红黑树 + set/map + unordered_map + 算法 | [stl_associative](05-STL-Associative/practice/stl_associative) | [05-STL-Associative](./05-STL-Associative/README.md) |

### 三、系统组件与存储

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 06 | 异步日志系统 | 双缓冲异步日志 + 多 Sink + PoolAllocator 接入 | [async_logger](06-AsyncLogger/practice/async_logger) | [06-AsyncLogger](./06-AsyncLogger/README.md) |
| 07 | LevelDB 存储引擎 | LSM 教学实现 + CRC WAL + 块式 SSTable + tombstone + crash safety | [leveldb_mini](07-LevelDB/practice/leveldb_mini) | [07-LevelDB](./07-LevelDB/README.md) |

### 四、网络与异步

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 08 | 网络编程 | 跨平台 Reactor 核 + Buffer 拆包 + HTTP/RPC core | [network_reactor](08-网络编程/practice/network_reactor) | [08-网络编程](./08-网络编程/README.md) |
| 09 | 高性能网络库 | TimerWheel + io_uring 概念模拟 + 连接池/负载均衡 | [high_perf_net](09-HighPerfNet/practice/high_perf_net) | [09-HighPerfNet](./09-HighPerfNet/README.md) |
| 10 | 协程库 | C++20 coroutine + 调度器 + Channel | [coroutine_lib](10-Coroutine/practice/coroutine_lib) | [10-Coroutine](./10-Coroutine/README.md) |

### 五、分布式

| 序号 | 主题 | 核心产出 | 配套代码 | 计划目录 |
| --- | --- | --- | --- | --- |
| 11 | RPC 框架 | protobuf-like 编码 + 帧协议 + 服务发现 + 拦截器 | [rpc_framework](11-RPC/practice/rpc_framework) | [11-RPC](./11-RPC/README.md) |
| 12 | Mini Raft KV | Raft 仿真 + 真持久化 + ReadIndex/Pre-Vote + 真 TCP/epoll 节点 | [raft_kv](12-RaftKV/practice/raft_kv) | [12-RaftKV](./12-RaftKV/README.md) |

进度记录见 [Progress.md](./Progress.md)。

## 判断是否真正掌握

掌握需要同时满足以下条件，具体范围按模块和 [嵌入式阶段](./Embedded-Roadmap.md)／[后端阶段](./Career-Roadmap.md) 确定：

1. **解释**：讲清关键原理、适用边界、失败语义与至少一个替代方案。
2. **实现**：不借助 AI，能脱稿实现选定的核心结构；不要求默写完整标准库或整个系统。
3. **修改与排障**：能独立修改现有／陌生代码，用可复现证据定位问题并补回归测试。
4. **验证**：测试关键边界；涉及网络、磁盘、硬件、中断和时序的主张，必须有对应真实实验与测量方法，host 测试不替代板上验证。

这条路线不设 deadline：按能力与证据推进，不按阅读量、生成代码量或日历判断完成。

## 验证口径

- 默认验证以 [Learning](Practice-Index.md) 的根级 CMake/CTest 为准。
- 涉及真实网络、真实磁盘崩溃、压测或外部依赖的模块，会在模块 README 中单独说明是否为真实验证、教学模拟或 proxy benchmark。
- 对还没做真实 benchmark 的模块，文档应保留为“待专项”，不把本机 smoke test 写成生产指标。

## 扩展区（未完待续）

编号 12 不是终点，只是目前整理到的部分。后续会继续往后加，方向包括但不限于：

- [嵌入式能力路线](./Embedded-Roadmap.md) 中的 C、启动与链接、外设／DMA、RTOS、真板排障与 BSP（当前主线）
- [后端能力路线](./Career-Roadmap.md) 中的真实网络、资源上限、调试性能、可观测性与部署交付（后端方向优先）
- 本地 AI 推理服务：成熟运行时接入、有界调度、超时、输出校验与性能分析（后端方向加分，非嵌入式前置）
- io_uring 真实化（liburing，Linux）、高并发连接压测 + 火焰图
- lock-free 真实 hazard pointer / epoch-based reclamation
- Raft joint consensus、snapshot 文件持久化策略和跨机器部署验证
- LevelDB mini 的 MANIFEST / VersionSet / TableCache / block cache
- 更多存储 / 调度 / 可观测性主题

## 与仓库其他目录的关系

- 根目录 [README.md](../README.md) 是整个仓库的入口。
- [代码索引](Practice-Index.md) 汇总各模块 `practice` 中的工程；计划与代码不再拆成两个顶层目录。
- 涉及真实外部依赖或压测指标的模块，会在对应 README 中区分「本机教学模拟 / Proxy 验收」与「真实 benchmark 未执行」。
