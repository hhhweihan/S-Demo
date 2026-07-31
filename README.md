# S-Demo

个人 C++ 学习仓库，主要放学习计划、练习代码、阅读笔记和一些早期归档内容。
 
## 目录职责

| 目录 | 定位 | 维护状态 |
| --- | --- | --- |
| [CPP-Learning-Plan](./CPP-Learning-Plan/README.md) | C++ 后端进阶路线图，按模块拆解与复盘 | 当前主线 |
| [CPP-Practice](./CPP-Practice/README.md) | 和学习计划配套的可编译练习项目 | 当前主线 |
| [Note](./Note/README.md) | 阅读笔记、主题总结和零散沉淀 | 持续补充 |
| [TestProject](./TestProject/README.md) | 第三方源码阅读材料 | 保留原貌，少量索引 |
| [Test](./Test/README.md) | 早期课程实验和零散测试代码 | 历史归档 |
| [xuanyuan_webserver](./xuanyuan_webserver/README.md) | 早期网络编程项目与阶段代码 | 历史归档 |

## 入口

- 主线学习看 [CPP-Learning-Plan/README.md](./CPP-Learning-Plan/README.md)
- 配套代码看 [CPP-Practice/README.md](./CPP-Practice/README.md)
- 笔记沉淀看 [Note/README.md](./Note/README.md)
- 第三方源码阅读材料看 [TestProject/README.md](./TestProject/README.md)
- 早期归档材料看 [Test/README.md](./Test/README.md) 和 [xuanyuan_webserver/README.md](./xuanyuan_webserver/README.md)

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
- **代码风格**：根级 `.clang-format`（Google / 4 空格 / 100 列）与 `.clang-tidy` 统一约束；注释约定见 [docs/comment-style.md](./docs/comment-style.md)。

```bash
# 根级聚合构建 + 全部测试
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure
```

入口： [练习代码索引](./CPP-Practice/README.md)

## 授权边界

- 本仓库原创内容默认以 [LICENSE](./LICENSE) 中的条款发布。
- 第三方源码、镜像材料及其衍生说明不自动适用根级授权，具体见 [THIRD_PARTY.md](./THIRD_PARTY.md)。
- 历史归档目录主要用于保留早期学习痕迹，不承诺统一的结构或维护节奏。
