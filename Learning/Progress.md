# 学习进度

> 这里只记录每个模块的**掌握状态**，不再按周/按天打卡。
> 节奏自定，按核心实现、独立修改／排障和验证证据判断——标准见 [README.md](./README.md#判断是否真正掌握)。
> 当前社招主线见 [Embedded-Roadmap.md](./Embedded-Roadmap.md)，后端备选见 [Career-Roadmap.md](./Career-Roadmap.md)；下表保留已有实现状态，不自动代表通过新验收。
> 教学已沉淀到 [课程正文](Embedded/README.md) 与 [知识地图](Embedded/Knowledge-Map.md)，文档准备状态和技能掌握状态分开记录。

## 状态说明

- **未开始**：还没进入这个模块。
- **学习中**：正在读 / 写，配套工程尚未跑通完整产出。
- **已实现**：配套工程写完并跑通测试。
- **已实现，待脱稿重写**：保留历史状态名称；代码和测试已闭环，但核心结构独立实现与工程验收仍待确认。
- **已掌握**：能解释、脱稿实现选定核心、独立修改／排障，并完成相应验证；不要求重写完整工程。
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

> 「已实现」升到「已掌握」时填写验收证据：核心实现、改动／排障案例、测试或真实实验入口与遗留边界，不仅勾选“看懂”或“跑通”。

## 社招能力阶段：嵌入式主线

以下记录新增训练目标的实际状态；模块已经实现不等于阶段已验收，host 测试不等于真板验证。不设日期，也不自动改变上方模块记录。后端备选阶段暂不并行打卡，避免同时展开两条必修路线。

| 阶段 | 能力目标 | 状态 | 验收证据／遗留问题 |
| --- | --- | --- | --- |
| E0 | 岗位校准、诊断与实验环境 | 学习中 | 已建立 [起步任务](./00-Embedded-Start/README.md) 与 [host 骨架](00-Embedded-Start/practice/embedded_basics/README.md)，独立构建与初始 10 个红色用例已验证；独立编码、岗位诊断和真板验收仍待完成 |
| E1 | [C / 受限 C++、启动与调试](Embedded/E1-Language-Startup/README.md) | 未开始 | 教学已准备；待记录核心实现、map 分析和 fault 定位 |
| E2 | [外设、DMA 与驱动](Embedded/E2-Peripherals-DMA/README.md) | 未开始 | 教学已准备；待记录真板 IO、时序、异常与恢复 |
| E3 | [FreeRTOS 与实时性](Embedded/E3-FreeRTOS-Realtime/README.md) | 未开始 | 教学已准备；待记录任务／ISR 交互、资源预算与时序测量 |
| E4 | [有界采集与诊断固件](Embedded/E4-Data-Acquisition/README.md) | 未开始 | 教学已准备；待记录协议、过载策略、资源与真板闭环 |
| E5 | [BSP、可靠性与交付](Embedded/E5-BSP-Delivery/README.md) | 未开始 | 教学已准备；待记录长测、复位诊断、干净构建与烧录 |
| E6 | [MCU/BSP 或 Linux BSP/驱动深化](Embedded/E6-Specialization/README.md) | 未开始 | 教学已准备；待选分支并记录移植／驱动或升级验证 |
| 求职准备（并行） | 编码、手册阅读、排障与项目深挖 | 未开始 | 待记录技术缺口与改进证据，不记个人或敏感资料 |

### 下一项任务

- 当前阶段：E0 起步；开发板尚未确认，真板项待验证。
- 三个优先缺口：待独立诊断后填写，不由已有代码或骨架推断。
- 下一项可验收任务：E0-03，独立实现字节环形缓冲；岗位校准与选板按 [起步任务](./00-Embedded-Start/README.md) 并行推进。
- 恢复阅读入口：[环形缓冲教学](00-Embedded-Start/Byte-Ring-Guide.md)，先复述合同，再只实现初始化；后续章节按 [课程](Embedded/README.md) 推进。
- 完成条件与验证入口：通过 `embedded_byte_ring` 的 5 个验收用例，解释容量、回绕、失败状态与非并发边界；接口与测试见 [embedded_basics](00-Embedded-Start/practice/embedded_basics/README.md)。

每通过一个阶段，记录 `任务 / 独立完成部分 / 测试或实验入口 / 板卡和版本 / 结果 / 已知限制 / 下一步`；真板、外部依赖或第二环境尚未验证时，明确保留待验证状态。
