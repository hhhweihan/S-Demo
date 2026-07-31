# list + forward_list

## 本周目标

实现双向链表 MyList 和单向链表 MyForwardList，理解节点式容器的迭代器设计。

## 本周完成情况

- [x] Day 99：实现 `MyList<T>` 哨兵节点结构
- [x] Day 100：完成 `push_front`、`push_back`、`pop_front`、`pop_back`
- [x] Day 101：实现双向迭代器并通过 `std::reverse`
- [x] Day 102：实现 `MyForwardList<T>` 单向节点结构
- [x] Day 103：完成前向迭代器和范围 for 验证
- [x] Day 104：整理节点式容器与连续容器的性能差异
- [x] Day 105：完成 Week15 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `MyList<T>`：哨兵节点、双向链表、双向迭代器
- `MyForwardList<T>`：单向链表、前向迭代器
- 周复盘：`Note/C++-Note/Week15-ListForwardList-复盘总结.md`

## 阶段小结
Week 15 的重点是节点式容器。相比 `vector`，链表不追求连续内存和随机访问，而是追求局部插入删除时少移动元素。`MyList` 使用哨兵节点统一空表、头尾插入和删除逻辑，让 `begin()` 指向首节点、`end()` 指向哨兵；双向迭代器满足 `std::reverse` 的最低要求。`MyForwardList` 则只保留单向链接，接口更轻，但无法支持反向移动。

这周最重要的结论是：STL 算法不是“容器特权”，而是“迭代器能力匹配”。只要迭代器类别、解引用、递增递减和比较语义正确，自定义容器也能自然进入标准算法。

## 验证命令

```powershell
cmake --build CPP-Practice/stl_sequence/build --config Release
.\CPP-Practice\stl_sequence\build\Release\stl_sequence_demo.exe
```

---
