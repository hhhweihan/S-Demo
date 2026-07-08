# Week 16 — 容器适配器 + 整月复盘

## 本周目标

实现 stack/queue/priority_queue，完成整月代码整合和性能对比。

## 本周完成情况

- [x] Day 106：实现 `MyStack<T>`，基于底层容器暴露 LIFO 接口
- [x] Day 107：实现 `MyQueue<T>`，基于链表暴露 FIFO 接口
- [x] Day 108：实现 `MyPriorityQueue<T, Compare>`，基于 heap 算法维护优先级
- [x] Day 109：验证自定义 comparator 下的优先队列顺序
- [x] Day 110：整合 Month04 容器测试并统一构建入口
- [x] Day 111：补充整月复盘和 C++ Note 知识总结
- [x] Day 112：规划 Month05 关联容器；`git commit / push` 等待明确确认后再执行

## 本周产出

- `MyStack<T>`：LIFO 容器适配器
- `MyQueue<T>`：FIFO 容器适配器
- `MyPriorityQueue<T, Compare>`：支持自定义比较器的堆适配器
- 周复盘：`Note/C++-Note/Week16-容器适配器与整月复盘总结.md`
- 月总结：`Note/C++-Note/Month04-STL序列容器实战总结.md`

## Week 16 总结

Week 16 体现了“适配器不是新容器，而是受限接口视图”。`stack` 限制为后进先出，`queue` 限制为先进先出，`priority_queue` 则把底层序列容器和堆算法组合起来，对外只暴露 `top/push/pop`。这种设计减少了用户误用空间，也让底层容器可以替换。

Month04 的整体验收已经串起来：`MyVector<int>` 进入 `std::sort`，`MyList<int>` 进入 `std::reverse`，`MyPriorityQueue<int, std::greater<int>>` 形成小顶堆，所有容器都通过范围 for 或标准算法做了运行时验证。

## 验证命令

```powershell
cmake -S CPP-Practice/stl_sequence -B CPP-Practice/stl_sequence/build
cmake --build CPP-Practice/stl_sequence/build --config Release
.\CPP-Practice\stl_sequence\build\Release\stl_sequence_demo.exe
```

---
