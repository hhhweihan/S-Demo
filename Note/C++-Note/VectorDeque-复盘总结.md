# Vector / Deque 复盘总结

## 核心目标

Week14 把迭代器协议落到连续/近连续序列容器：实现 `MyVector<T>` 和教学版 `MyDeque<T>`，并验证它们能参与范围 for 和标准算法。

## `MyVector<T>`

`vector` 的核心是三件事：

- `size`：当前有效元素数量
- `capacity`：当前已申请空间容量
- `data`：连续内存起点

当 `push_back` 超过容量时，容器需要申请更大空间、移动旧元素、更新容量。因为 `vector` 是连续内存，裸指针天然满足随机访问迭代器要求，所以 `MyVector<int>` 可以直接进入 `std::sort`。

## `MyDeque<T>`

本周实现采用教学版环形缓冲：

- `head_` 记录逻辑首元素所在物理位置
- 逻辑下标通过 `(head_ + index) % capacity_` 映射到物理下标
- `push_front` 向前移动 `head_`
- `push_back` 写入逻辑尾部

工业级 deque 通常使用分块 map，避免一次性大块搬迁；这里先完成双端队列语义和迭代器接入。

## 关键对比

| 容器 | 内存模型 | 随机访问 | 头部插入 | 尾部插入 |
|------|----------|----------|----------|----------|
| vector | 连续内存 | 快 | 慢 | 均摊快 |
| deque | 分块/环形 | 较快 | 均摊快 | 均摊快 |
| list | 节点链表 | 不支持 | 快 | 快 |

## 本周代码落点

- `CPP-Practice/stl_sequence/mini_stl.h`
- `CPP-Practice/stl_sequence/main.cpp`

## 验证结果

已验证：

- `MyVector<int>` 可被 `std::sort` 排序
- `MyVector` 支持范围 for
- `MyDeque` 支持头尾插入/删除、下标访问、范围 for

## 复盘结论

`vector` 的强项来自连续内存和随机访问，代价是扩容搬迁；`deque` 的强项来自头尾操作更平衡，代价是迭代器和内存映射更复杂。学习这两个容器，关键是把接口语义、内存布局和迭代器能力联系起来。
