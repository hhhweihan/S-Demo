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

- [ ] `MyVector<int>` 通过 std::sort（需要 RandomAccess 迭代器）
- [ ] `MyList<int>` 通过基于双向迭代器的 std::reverse
- [ ] `MyPriorityQueue<int>` 使用自定义 Comparator 正确排序
- [ ] 所有容器支持范围 for 循环（begin/end）
- [ ] ASan 扫描无内存错误

## 参考资料

- 你的笔记：`Note/C++-Note/C++笔记2/c++泛型编程.pdf`
- 你的笔记：`Note/C++-Note/C++笔记2/effective stl.md`
- libc++ 源码：`include/iterator` / `include/vector` / `include/list`
- TinySTL（参考实现，不要直接看）：https://github.com/zouxiaohang/TinySTL
