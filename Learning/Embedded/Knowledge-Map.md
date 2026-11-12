# 嵌入式知识地图

本页用于检索与查漏，教学顺序见 [课程入口](README.md)。**必修**表示主线需要能独立解释与验证；**条件必修**依赖所选硬件／岗位；**选修**不阻塞主线。每项以证据验收，不按阅读次数或代码量打勾。

## 主线知识与证据

| 知识点 | 层级 | 教学定位 | 应留下的证据 |
| --- | --- | --- | --- |
| 固定容量、FIFO、回绕、失败不改变状态 | 必修 | [E0 环形缓冲](../00-Embedded-Start/Byte-Ring-Guide.md) | 独立实现、边界测试、不变量说明 |
| 字节序、长度边界、分片、非拥有视图 | 必修 | [E0 帧解析](../00-Embedded-Start/Frame-Parser-Guide.md) | 测试与输入生命周期说明 |
| 指针／数组、整数转换、对齐、别名与 UB | 必修 | [E1 L1](E1-Language-Startup/README.md#l1-指针与整数边界) | 最小复现、host 回归与边界解释 |
| RAII、移动、异常／RTTI与标准库成本 | 必修 | [E1 L2](E1-Language-Startup/README.md#l2-受限环境中的-c) | 工程规则、目标构建与尺寸比较 |
| 编译链接、ELF、LMA/VMA、map、ABI | 必修 | [E1 L3](E1-Language-Startup/README.md#l3-从源码到内存布局) | 段、符号、初始化与运行地址分析 |
| 向量表、reset、C/C++ 初始化、MSP/PSP | 必修 | [E1 L4](E1-Language-Startup/README.md#l4-cortex-m-启动与异常) | 真板启动追踪与异常现场 |
| FPU、MPU、D-cache、DWT | 条件必修 | [E1 L4](E1-Language-Startup/README.md#l4-cortex-m-启动与异常)、[E2 L5](E2-Peripherals-DMA/README.md#l5-dma-与缓冲所有权) | 芯片支持依据和对应实测 |
| GPIO、电平、复用、开漏、上拉、定时器 | 必修 | [E2 L1](E2-Peripherals-DMA/README.md#l1-手册与引脚先于代码) | 配置依据与周期／电平捕获 |
| UART 字节流、中断、错误与发送完成 | 必修 | [E2 L2](E2-Peripherals-DMA/README.md#l2-uart-从轮询到中断) | 连续输入、超速、错误恢复 |
| I2C 地址、ACK/NACK、超时与恢复 | 二选一必修 | [E2 L3](E2-Peripherals-DMA/README.md#l3-i2c-与-spi-至少选一种) | 真实器件与总线捕获 |
| SPI 时钟模式、片选、全双工与事务 | 二选一必修 | [E2 L3](E2-Peripherals-DMA/README.md#l3-i2c-与-spi-至少选一种) | 手册／波形／数据相互对应 |
| 寄存器语义、W1C、保留位与 HAL 边界 | 必修 | [E2 L4](E2-Peripherals-DMA/README.md#l4-驱动是一组状态与合同) | 一条独立寄存器路径和错误状态说明 |
| DMA 生命周期、可访问区、cache 一致性 | 必修／按芯片补 cache | [E2 L5](E2-Peripherals-DMA/README.md#l5-dma-与缓冲所有权) | 缓冲交接、回绕、覆盖与恢复测试 |
| 抢占、任务状态、tick、周期与回绕 | 必修 | [E3 L1](E3-FreeRTOS-Realtime/README.md#l1-调度与时间不是同一个问题) | 任务时间线与超期记录 |
| 队列／通知／信号量／互斥锁／事件组 | 必修 | [E3 L2](E3-FreeRTOS-Realtime/README.md#l2-同步原语按语义选) | 选择理由、所有权与满／空行为 |
| ISR API、NVIC、RTOS 临界区 | 必修 | [E3 L3](E3-FreeRTOS-Realtime/README.md#l3-isr-与任务的交界) | port／优先级依据和交错实验 |
| 优先级反转、继承、死锁、饥饿 | 必修 | [E3 L4](E3-FreeRTOS-Realtime/README.md#l4-资源与实时测量) | 一次定位与回归证据 |
| 栈水位、堆策略、测量开销、WCET 边界 | 必修 | [E3 L4](E3-FreeRTOS-Realtime/README.md#l4-资源与实时测量) | 声明负载下的资源／时序数据 |
| 协议、任务分层、有界队列与背压 | 必修 | [E4 L1](E4-Data-Acquisition/README.md#l1-先定义合同再画架构)、[E4 L2](E4-Data-Acquisition/README.md#l2-容量与数据核对) | 端到端序号、容量和错误核对 |
| 复位原因、fault 现场、诊断输出 | 必修 | [E4 L3](E4-Data-Acquisition/README.md#l3-诊断不能成为新的故障源) | 自造故障的现场与恢复边界 |
| Flash 擦写、CRC、提交标记、磨损与恢复 | 条件必修 | [E4 L4](E4-Data-Acquisition/README.md#l4-持久化是可选的第二条链路) | 真实器件、断写模型与恢复实验 |
| BSP 分层、工具链固定、配置与产物 | 必修 | [E5 L1](E5-BSP-Delivery/README.md#l1-bsp-是边界不是万能抽象) | 小型 BSP 与干净构建／烧录 |
| 看门狗、brown-out、停滞与恢复 | 必修 | [E5 L2](E5-BSP-Delivery/README.md#l2-复位与看门狗要有业务含义) | 停滞检测、复位原因与数据边界 |
| 压力长测、资源趋势、交付与回归 | 必修 | [E5 L3](E5-BSP-Delivery/README.md#l3-验证矩阵与交付产物) | 声明版本与负载的完整报告 |

## 深化知识与岗位选择

| 知识点 | 层级 | 教学入口 | 最低证明方式 |
| --- | --- | --- | --- |
| 板级移植、时钟／引脚差异、资源布局 | 分支必修 | [MCU/BSP](E6-Specialization/MCU-BSP.md) | 第二种配置／板卡实跑 |
| Bootloader、分区、镜像校验、失败恢复 | 分支内选修 | [MCU/BSP](E6-Specialization/MCU-BSP.md) | 截断／损坏镜像和可启动路径 |
| BootROM/U-Boot/kernel/rootfs、设备树 | Linux 分支必修 | [Linux BSP/驱动](E6-Specialization/Linux-BSP-Drivers.md) | 真板启动与硬件描述对应 |
| Buildroot 或 Yocto、内核配置与依赖 | Linux 分支必修 | [Linux BSP/驱动](E6-Specialization/Linux-BSP-Drivers.md) | 固定版本的镜像复现 |
| device/bus/driver、probe/remove、regmap、子系统 | Linux 分支必修 | [Linux BSP/驱动](E6-Specialization/Linux-BSP-Drivers.md) | 真实器件驱动与用户态接口 |
| 内核锁、上下文、DMA 地址、异步生命周期 | Linux 分支必修 | [Linux BSP/驱动](E6-Specialization/Linux-BSP-Drivers.md) | 错误清理、解绑与竞态验证 |
| CAN/CAN FD、UDS/DoIP、AUTOSAR Classic | 按岗位选修 | [分支选择](E6-Specialization/README.md) | 隔离实验、协议／栈与适配证据 |
| 低功耗、功耗测量、唤醒与恢复 | 按硬件选修 | [分支选择](E6-Specialization/README.md) | 功耗与唤醒后功能核对 |
| 安全启动、签名、回滚保护、功能安全 | 原理选修 | [分支选择](E6-Specialization/README.md) | 明确威胁／故障模型；不宣称认证 |

## 和现有 C++ 素材的关系

- [内存管理](../01-内存管理/README.md) 用于生命周期、RAII、对齐与分配器原理，不能直接证明 MCU 存储预算。
- [并发](../02-并发编程/README.md) 用于同步与队列原理，不能将 host 线程池作为 RTOS 任务／ISR 验证。
- [模板](../03-模板泛型/README.md) 与 [容器](../04-STL-Sequence/README.md) 用于接口与容器成本分析，不要求完整重写标准库。
- [日志](../06-AsyncLogger/practice/async_logger/README.md) 提供缓冲与错误合同素材；MCU 输出、任务与持久化需要对应适配。
- [后端路线](../Career-Roadmap.md) 的存储、分布式、真实网络与 AI 作为备选深化，不和嵌入式课程同时设为必修。

## 使用这张地图

发现缺口后，在 [进度](../Progress.md) 记录对应课号与一项可验收任务。把“读懂解释”“host 通过”“板上通过”“声明负载通过”分开，缺少哪层证据就保留哪层待验证。
