# vector + deque

## 本周目标

实现 `MyVector<T>`（完整动态数组）和 `MyDeque<T>`（分块双端队列），支持 STL 算法。

## 本周完成情况

- [x] Day 92：实现 `MyVector<T>` 基础结构、容量管理和迭代器
- [x] Day 93：完成 `push_back`、`pop_back`、`front/back`、`operator[]`
- [x] Day 94：用随机访问迭代器通过 `std::sort` 验证
- [x] Day 95：实现 `MyDeque<T>` 的环形缓冲结构和头尾插入
- [x] Day 96：完成 `MyDeque` 头尾删除、下标访问和范围 for
- [x] Day 97：整理 vector/deque 的扩容、连续性和迭代器差异
- [x] Day 98：完成 Week14 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `MyVector<T>`：动态数组、倍增扩容、随机访问迭代器
- `MyDeque<T>`：环形缓冲双端队列，支持头尾操作和下标访问
- 周复盘：`Note/C++-Note/Week14-VectorDeque-复盘总结.md`

## 阶段小结
Week 14 把 Week13 的迭代器协议落到序列容器上。`MyVector` 的关键是容量和大小分离：`size` 表示已构造元素数量，`capacity` 表示已申请空间；随机访问迭代器可以直接复用裸指针，因此能被 `std::sort` 使用。`MyDeque` 采用环形缓冲实现，重点是把逻辑下标映射为物理下标，让 `push_front` 和 `push_back` 都能保持稳定的头尾操作语义。

当前 `MyDeque` 是教学版环形缓冲，覆盖双端队列的核心接口；工业级 deque 的分块 map、迭代器跨块跳转和异常安全可以作为后续加深方向。

## 验证命令

```powershell
cmake --build CPP-Practice/stl_sequence/build --config Release
.\CPP-Practice\stl_sequence\build\Release\stl_sequence_demo.exe
```

---
