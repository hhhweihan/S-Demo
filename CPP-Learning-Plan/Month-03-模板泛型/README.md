# Month 3 — 模板与泛型深入

## 月度目标

从 SFINAE 到变参模板，把 C++ 模板的核心机制全部用代码走一遍。
产出：手写 `tuple`、`variant`、`optional`、`function`（类型擦除版）。

## 技能树

```
模板编程
├── 基础工具            ← Week 9
│   ├── SFINAE + enable_if
│   └── 手写常用 type_traits
├── 变参模板            ← Week 10
│   ├── 折叠表达式
│   └── 手写 tuple（完整版）
├── 类型擦除            ← Week 11
│   ├── 手写 variant
│   ├── 手写 optional
│   └── 手写 function
└── 高级技巧            ← Week 12
    ├── CRTP 静态多态
    ├── Policy-based design
    └── C++20 concepts 入门
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 9 | SFINAE + type_traits | [Week-09.md](./Week-09.md) |
| Week 10 | 变参模板 + 手写 tuple | [Week-10.md](./Week-10.md) |
| Week 11 | 类型擦除（variant/optional/function）| [Week-11.md](./Week-11.md) |
| Week 12 | 高级技巧 + 整月复盘 | [Week-12.md](./Week-12.md) |

## 验收标准

- [x] 手写的 `Tuple<int, double, std::string>` 支持 `get<N>()` 和 `apply()`
- [x] 手写的 `Variant<int, std::string>` 支持 `visit()`
- [x] 手写的 `Function<int(int,int)>` 能存 lambda / 函数指针 / 带状态 lambda
- [x] 用 `concepts` 给线程池的 `submit` 加约束（C++20 下启用，保留 C++17 兼容）

## 月度完成情况

- Week 9：完成 SFINAE、`enable_if`、手写 `type_traits`，工程位于 `CPP-Practice/template_traits/`
- Week 10：完成变参模板、折叠表达式和手写 `Tuple`，工程位于 `CPP-Practice/variadic_tuple/`
- Week 11：完成 `Optional`、`Variant`、`Function` 和命令解析器整合实验，工程位于 `CPP-Practice/type_erasure/`
- Week 12：完成 CRTP、Policy-Based Design、concepts、constexpr 和整月综合测试，工程位于 `CPP-Practice/advanced_templates/`

## 月度总结

Month 3 已完成“模板机制 -> 类型计算 -> 变参展开 -> 类型擦除 -> concepts 约束”的完整闭环。这个月最重要的变化，是从“模板像一种复杂语法”转向“模板是一套编译期接口设计工具”：SFINAE 和 concepts 决定候选是否可用，`type_traits` 负责类型计算，变参模板和 `index_sequence` 负责批量展开，`Variant` / `Function` 这类组件则把编译期类型信息和运行时分发连接起来。

详细总结见 `Note/C++-Note/Month03-模板泛型实战总结.md`。

## 参考资料

- 你的笔记：`Note/C++-Note/C++笔记2/c++泛型编程.pdf`
- 你的笔记：`Note/C++-Note/C++笔记2/effective stl.md`
- libc++ 源码：tuple / variant / any 实现
- 书：《C++ Templates: The Complete Guide》第 2 版（按需查阅，不需要通读）
