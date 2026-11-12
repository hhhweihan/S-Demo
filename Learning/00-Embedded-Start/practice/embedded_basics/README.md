# embedded_basics：E0 host 练习骨架

配合 [E0 起步任务](../../README.md)，先在 PC 上练习 C 接口、固定容量资源与协议边界，再进入 MCU 中断／DMA。

教学正文：[环形缓冲](../../Byte-Ring-Guide.md) · [有界帧解析](../../Frame-Parser-Guide.md)。先阅读机制与检查点，再独立实现；全程课程见 [嵌入式教学入口](../../../Embedded/README.md)。

**这是练习骨架，不是实现答案。** 所有待实现函数目前明确返回 `*_NOT_IMPLEMENTED`。初始验收测试应失败；没有跳过测试、反转测试结果或把未实现伪装成成功。只有独立实现后全部通过，才表示这组 host 合同已验收。

## 工程边界

- C11 练习代码，C++17 + 现有 GoogleTest 测试；C 接口通过 `extern "C"` 与测试链接。
- 独立 CMake 工程，复用仓库已有 GoogleTest、warning 和 sanitizer 配置。不接入根级聚合构建，避免初始红色练习影响已有模块与 CI。
- 不提供桌面 demo，也不引入 RTOS、线程或板卡 SDK。host 测试不能证明 ISR、DMA、时序或真板行为。
- 待实现处：[byte_ring.c](./byte_ring.c)、[frame_parser.c](./frame_parser.c)；接口：[byte_ring.h](./byte_ring.h)、[frame_parser.h](./frame_parser.h)；验收：[embedded_basics_tests.cpp](./tests/embedded_basics_tests.cpp)。

## 练习一：固定容量字节环形缓冲

### 合同

- 调用者提供存储与容量；容量大于零，所有槽位可用，不要求容量为 2 的幂。不动态分配、不覆盖未读数据，FIFO 顺序。
- `byte_ring_init`：成功后为空；空对象／存储指针或零容量返回 `BYTE_RING_INVALID_ARGUMENT`，失败不改变已有状态。
- `byte_ring_push`：成功返回 `BYTE_RING_OK`；满时返回 `BYTE_RING_FULL`，数据与状态不变；空对象指针返回参数错误。
- `byte_ring_pop`：成功写出一个字节；空时返回 `BYTE_RING_EMPTY`，输出与状态不变；空对象／输出指针返回参数错误，不消费数据。
- 操作的前置条件：除参数错误测试外，push/pop 的对象必须成功初始化，调用者不篡改其字段，存储在使用期间有效。对象与存储不重叠。
- 此版是单线程接口，**没有 ISR／任务或多线程同步保证**；以后增加并发时需另立合同与测试。

### 先做什么

1. 写下空／满状态与索引的关系，说明容量 1 和容量 3 为什么都应成立。
2. 先实现初始化，再实现入队／出队；每次只运行 `embedded_byte_ring`。
3. 保留失败状态，不为“让测试变绿”修改验收或消除错误检查。

## 练习二：长度字段有界帧解析

### 线协议

```text
[payload_size: u16，大端][payload: 0–32 字节]
```

例如 `00 03 11 22 33` 表示三字节 payload。只解析第一帧，多余输入留给调用者；此练习不是流式累积器，不负责缓存分片。

### 合同

- `FRAME_OK`：`frame.payload` 精确指向输入的第 3 个字节位置，`frame.size` 为 payload 大小；`consumed` 为该帧总大小，不消费尾随数据。
- `FRAME_INCOMPLETE`：头或合法长度的 payload 未完整到达；`input == NULL && input_size == 0` 也表示尚无输入。
- `FRAME_INVALID_LENGTH`：完整头中的长度超过 32，即使 payload 未到达也立即拒绝。大端长度不能截断成一个字节。
- `FRAME_INVALID_ARGUMENT`：输出指针为空，或输入为空但输入大小非零；参数检查优先于解析。
- 所有非成功结果不改变可用的输出对象及 `consumed`。输入与输出对象互不重叠，指针描述的可访问内存由调用者保证。
- 返回的是非拥有视图：输入生命周期必须覆盖使用期，释放或改写输入前不得继续使用该视图。零长度帧的 payload 指向头之后，不能解引用。
- 不动态分配、不使用未对齐的整数指针强转、不读取输入范围之外的字节。

### 先做什么

1. 对每一种失败状态列出“此时最多能读取几个字节”。
2. 只实现 `parse_frame`，运行 `embedded_frame_parser`；先保证边界正确，再谈优化。
3. 解释如何让调用者在收到后续分片后重试，以及何时回收成功返回的视图。

## 构建与验收入口

### VS Code

1. 用 VS Code 单独打开本练习目录，确保 C/C++ 与 CMake Tools 扩展、host C/C++ 编译器和 CMake 可用。
2. 在 CMake Tools 选择 host 编译工具链，执行 Configure，保持 `BUILD_TESTING=ON`。首次缺少系统 GoogleTest 时，现有配置会拉取固定版本；离线时先提供已安装包。
3. 执行 Build，选择 `embedded_basics_tests`。
4. 在 CMake 测试入口选择 `embedded_byte_ring` 或 `embedded_frame_parser`；全部验收时选择两者。

### 如何解释结果

| 结果 | 含义 |
| --- | --- |
| 无法配置／编译／链接 | 环境或骨架问题，先解决；不能当作预期红色测试 |
| 构建成功，测试报告 `*_NOT_IMPLEMENTED` 与期望状态不一致 | 初始预期红色，需要独立完成实现 |
| 全部 10 个验收用例通过 | 当前 host 合同通过；仍需解释不变量、生命周期与错误边界 |
| host sanitizer 无报告 | 仅说明已执行 host 场景未发现对应问题，非真板保证 |

初始预期红色是学习反馈，不是测试豁免。不要添加 `WILL_FAIL`、跳过验收或保留占位函数后宣称已实现。Linux/GCC/Clang 环境可使用已有 `S_DEMO_SANITIZER=address` 做后续验证；工具支持边界遵循仓库统一配置。

## 骨架验证状态

- 独立工程已使用 Windows host 便携 CMake／LLVM-MinGW 配置、编译与链接，源文件格式检查通过；未修改系统 PATH。
- 两个验收组已实际执行：共 10 个用例因明确的未实现状态失败，无通过或跳过，符合初始红色预期。
- 当前工作区的 CMake Tools 未发现此独立工程的目标；在 VS Code 中使用时，仍需单独打开本目录并选择可用 host 工具链。便携工具留在会话目录，不作为长期环境依赖。
- 尚未实现算法、运行实现后的 sanitizer 或进行任何真板验证；这些不能由骨架构建结果推导。
