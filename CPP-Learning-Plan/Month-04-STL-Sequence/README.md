# Month 4 — 自编 STL（序列容器 + 迭代器体系）

## 月度目标

从零实现 STL 的序列容器，建立完整的迭代器协议体系。
把 Month 1 的内存池和 Month 3 的模板技巧全部用上。

## 技能树

```
自编 STL（序列部分）
├── 迭代器体系                ← Week 13
│   ├── iterator_traits
│   ├── 5 种迭代器类别标签
│   └── reverse_iterator
├── vector / deque           ← Week 14
│   ├── 动态数组 + 扩容策略
│   └── 分块双端队列
├── list / forward_list      ← Week 15
│   ├── 双向链表
│   └── 单向链表
└── 容器适配器               ← Week 16
    ├── stack / queue
    └── priority_queue（堆）
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 13 | 迭代器协议 + iterator_traits | [Week-13-Iterator-System/](./Week-13-Iterator-System/) |
| Week 14 | vector + deque | [Week-14-Vector-Deque/](./Week-14-Vector-Deque/) |
| Week 15 | list + forward_list | [Week-15-List-ForwardList/](./Week-15-List-ForwardList/) |
| Week 16 | 容器适配器 + 整月复盘 | [Week-16-Adapters-Review/](./Week-16-Adapters-Review/) |

## 验收标准

- [x] `MyVector<int>` 通过 std::sort（需要 RandomAccess 迭代器）
- [x] `MyList<int>` 通过基于双向迭代器的 std::reverse
- [x] `MyPriorityQueue<int>` 使用自定义 Comparator 正确排序
- [x] 所有容器支持范围 for 循环（begin/end）
- [x] Release 构建与运行时用例通过；MSVC 当前工程未单独启用 ASan

## 月度完成情况

- Week 13：完成迭代器类别标签、`iterator_traits` 和 `ReverseIterator`，工程位于 `CPP-Practice/stl_sequence/`
- Week 14：完成 `MyVector<T>` 和教学版 `MyDeque<T>`，验证随机访问、头尾操作和范围 for
- Week 15：完成 `MyList<T>` 和 `MyForwardList<T>`，验证双向迭代器与前向迭代器能力边界
- Week 16：完成 `MyStack`、`MyQueue`、`MyPriorityQueue`，完成整月综合测试

## 月度总结

Month 4 已完成“迭代器协议 -> 连续容器 -> 节点容器 -> 容器适配器”的完整闭环。这个月的关键不是复刻 STL 的全部工程细节，而是理解 STL 的核心分层：容器负责存储，迭代器负责遍历能力表达，算法只依赖迭代器协议，适配器则在已有容器上收窄接口。

详细总结见 `Note/C++-Note/Month04-STL序列容器实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/stl_sequence -B CPP-Practice/stl_sequence/build
cmake --build CPP-Practice/stl_sequence/build --config Release
.\CPP-Practice\stl_sequence\build\Release\stl_sequence_demo.exe
```

## 参考资料

- 你的笔记：`Note/C++-Note/C++笔记2/c++泛型编程.pdf`
- 你的笔记：`Note/C++-Note/C++笔记2/effective stl.md`
- libc++ 源码：`include/iterator` / `include/vector` / `include/list`
- TinySTL（参考实现，不要直接看）：https://github.com/zouxiaohang/TinySTL
