# Week15 List / ForwardList 复盘总结

## 核心目标

Week15 实现节点式序列容器：`MyList<T>` 和 `MyForwardList<T>`。重点是理解链表迭代器的移动能力，以及节点容器和连续容器的取舍。

## `MyList<T>`

双向链表使用哨兵节点后，很多边界条件可以统一：

- 空表时 `sentinel->next == sentinel`
- `begin()` 返回首节点
- `end()` 返回哨兵
- 头插、尾插、头删、尾删都可以转化为节点前后指针重连

双向迭代器需要支持：

- `operator*`
- `operator++`
- `operator--`
- `operator== / !=`

因此 `MyList<int>` 可以进入 `std::reverse`。

## `MyForwardList<T>`

单向链表只保存 `next`，空间更轻，移动能力也更弱。它适合只需要单向遍历和头部操作的场景，不适合反向遍历和随机访问。

## 节点式容器特点

优点：

- 插入删除不需要整体搬迁元素
- 节点地址相对稳定
- 可以在特定位置做局部修改

缺点：

- 不支持随机访问
- 每个节点都有额外指针开销
- 缓存局部性通常弱于连续容器

## 本周代码落点

- `CPP-Practice/stl_sequence/mini_stl.h`
- `CPP-Practice/stl_sequence/main.cpp`

## 验证结果

已验证：

- `MyList<int>` 可被 `std::reverse` 反转
- `MyList` 支持范围 for
- `MyForwardList` 支持前向范围 for

## 复盘结论

节点式容器的价值不是“更快”，而是在特定操作模型下避免元素搬迁。选择容器时要先看访问模式：频繁随机访问优先 `vector`，频繁局部插入删除再考虑链表。
