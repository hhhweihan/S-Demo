# S-Demo

C++ 后端进阶路线图：从语言内功到自研 STL、存储引擎、网络异步与分布式，18 个可编译模块配 GoogleTest/CI。此外还有阅读笔记和一些早期归档内容。
 
## 目录职责

| 目录 | 定位 | 维护状态 |
| --- | --- | --- |
| [CPP-Learning-Plan](./CPP-Learning-Plan/README.md) | C++ 后端进阶路线图，按模块拆解与复盘 | 当前主线 |
| [CPP-Practice](./CPP-Practice/README.md) | 和学习计划配套的可编译练习项目 | 当前主线 |
| [Note](./Note) | 阅读笔记、主题总结和零散沉淀 | 持续补充 |
| [history_test/TestProject](./history_test/TestProject) | 第三方源码阅读材料 | 保留原貌，少量索引 |
| [history_test/Test](./history_test/Test) | 早期课程实验和零散测试代码 | 历史归档 |
| [history_test/xuanyuan_webserver](./history_test/xuanyuan_webserver) | 早期网络编程项目与阶段代码 | 历史归档 |

## 入口

- 主线学习看 [CPP-Learning-Plan/README.md](./CPP-Learning-Plan/README.md)
- 配套代码看 [CPP-Practice/README.md](./CPP-Practice/README.md)
- 笔记沉淀看 [Note](./Note)
- 第三方源码阅读材料看 [history_test/TestProject](./history_test/TestProject)
- 早期归档材料看 [history_test/Test](./history_test/Test) 和 [history_test/xuanyuan_webserver](./history_test/xuanyuan_webserver)

## 推荐阅读路径

如果只是快速了解仓库，建议按这个顺序看：

1. 先看 [CPP-Learning-Plan/README.md](./CPP-Learning-Plan/README.md)，理解整条路线的模块划分和完成标准。
2. 再看 [CPP-Practice/README.md](./CPP-Practice/README.md)，确认每个模块对应的代码目录。
3. 重点看下面的代表项目，而不是从历史归档目录开始翻。
4. 最后按需查 [Note/C++-Note](./Note/C++-Note) 里的模块总结和阶段复盘，了解每个阶段的设计取舍。

## 代表项目

| 项目 | 主要技术点 | 当前验证口径 | 适合展示的能力 |
| --- | --- | --- | --- |
| [raft_kv](./CPP-Practice/raft_kv) | Raft 选举、日志复制、快照、ReadIndex、Pre-Vote、真实 TCP/epoll 节点 | 默认跑确定性仿真和 codec 单测；真实多进程 kill -9 恢复走 `integration` label 单独跑 | 分布式一致性、故障恢复、真实 OS 语义边界 |
| [leveldb_mini](./CPP-Practice/leveldb_mini) | LSM Tree、WAL、块式 SSTable、BloomFilter、tombstone、compaction | GoogleTest/CTest 覆盖 WAL 损坏恢复、块表、compaction 和 kill-9 持久性 | 存储引擎、崩溃安全、磁盘格式设计 |
| [async_logger](./CPP-Practice/async_logger) | 同步/异步日志、双缓冲、多 Sink、后台 flush | 单元测试和 smoke test；真实性能压测仍作为后续专项 | 并发组件、生产工具基础设施 |
| [rpc_framework](./CPP-Practice/rpc_framework) | 长度前缀帧、protobuf-like 编码、同步/异步 RPC、服务发现、中间件 | 同进程教学框架，使用 proxy benchmark，不宣称真实网络 QPS | 协议设计、框架抽象、接口边界 |
| [network_reactor](./CPP-Practice/network_reactor) | Buffer 拆包、Reactor 抽象、HTTP core、RPC core | 跨平台 FakePoller 教学实现；真实 epoll/wrk 压测未作为当前结果 | 网络编程主线理解、边界诚实表达 |

## 当前主线

### CPP-Learning-Plan — C++ 后端进阶路线图

这是当前的主线学习路线，从语言内功到自研标准库、系统组件、网络异步再到分布式，按模块顺序推进。
编号只表示学习顺序和依赖，不设 deadline；完成以「能脱稿重写」为准，具体路线放在计划目录里。

入口： [详细计划](./CPP-Learning-Plan/README.md) | [学习进度](./CPP-Learning-Plan/Progress.md)

### CPP-Practice — 配套练习代码

这里放每个月对应的小型可运行项目。每个子目录既能独立 CMake 构建（只编译 demo），也能由根级
聚合 CMake 统一构建并跑测试；模块级 README 会说明覆盖内容和验证命令。

- **单元测试**：全部 18 个模块用 GoogleTest（CMake `FetchContent` 拉取）+ CTest。
- **Sanitizer**：`-DS_DEMO_SANITIZER=address|thread` 一键开启 ASan/UBSan/TSan，主要给并发模块查竞态与 UB。
- **CI**：`.github/workflows/ci.yml` 在 gcc/clang 矩阵下跑构建+测试，外加 ASan/UBSan、TSan、clang-format 检查。
- **代码风格**：根级 [.clang-format](./.clang-format)（Google / 4 空格 / 100 列）与 [.clang-tidy](./.clang-tidy) 统一约束。

```bash
# 根级聚合构建 + 全部测试
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure
```

入口： [练习代码索引](./CPP-Practice/README.md)

## 实现边界与非目标

- 这是学习成长仓库，不把教学模拟伪装成生产级 benchmark。
- README 中出现的 proxy benchmark 只说明本机教学路径的相对行为，不等价于真实网络/真实磁盘/真实压测指标。
- 历史归档目录保留学习痕迹，当前主线以 [CPP-Learning-Plan](./CPP-Learning-Plan/README.md)、[CPP-Practice](./CPP-Practice/README.md) 和 [Note](./Note) 为准。
- “已实现”只表示代码和测试闭环完成；“已掌握”仍以不看资料能从零重写为标准。

## 授权边界

- 本仓库原创内容默认以 [LICENSE](./LICENSE) 中的条款发布。
- 第三方源码、镜像材料及其衍生说明不自动适用根级授权；如果对应目录内存在单独授权或来源说明，以该目录说明为准。
- 历史归档目录主要用于保留早期学习痕迹，不承诺统一的结构或维护节奏。
