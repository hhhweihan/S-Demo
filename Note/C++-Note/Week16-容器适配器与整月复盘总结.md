# Week16 容器适配器与整月复盘总结

## 核心目标

Week16 实现 `MyStack`、`MyQueue`、`MyPriorityQueue`，并把 Month04 的序列容器工程统一验证。

## 容器适配器

适配器不是重新发明一种存储结构，而是在已有容器上收窄接口。

### `MyStack<T>`

`stack` 暴露 LIFO 语义：

- `push`
- `pop`
- `top`

底层可以使用 `MyVector<T>`，只需要尾部插入和尾部删除。

### `MyQueue<T>`

`queue` 暴露 FIFO 语义：

- `push`
- `pop`
- `front`

底层使用 `MyList<T>` 可以自然支持尾插和头删。

### `MyPriorityQueue<T, Compare>`

优先队列基于堆算法维护局部有序：

- `push` 后执行 `std::push_heap`
- `pop` 前执行 `std::pop_heap`
- `top` 访问堆顶

自定义 comparator 决定堆顶是最大值还是最小值。

## Month04 整体验收

已完成并运行验证：

- `MyVector<int>` 通过 `std::sort`
- `MyDeque<int>` 支持头尾操作和下标访问
- `MyList<int>` 通过 `std::reverse`
- `MyForwardList<int>` 支持前向范围 for
- `MyPriorityQueue<int, std::greater<int>>` 形成小顶堆

## 复盘结论

Month04 的主线是 STL 分层设计：

- 容器管理存储
- 迭代器描述遍历能力
- 算法依赖迭代器协议
- 适配器限制接口并复用底层容器

掌握这条线后，再看标准库源码时就不会只看到模板细节，而能看出容器、算法和 traits 之间的协作边界。
