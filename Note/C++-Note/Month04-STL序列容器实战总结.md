# Month04 STL 序列容器实战总结

## 月度目标回顾

Month04 的目标是自编 STL 序列容器，完成从迭代器协议到容器适配器的闭环。代码集中在 `CPP-Practice/stl_sequence/`。

## 已完成组件

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| 迭代器体系 | tag、`iterator_traits`、`ReverseIterator` | 类型推导和反向解引用测试 |
| `MyVector<T>` | 动态数组、扩容、随机访问迭代器 | `std::sort`、范围 for |
| `MyDeque<T>` | 环形缓冲双端队列 | 头尾插入删除、下标访问、范围 for |
| `MyList<T>` | 哨兵双向链表、双向迭代器 | `std::reverse`、范围 for |
| `MyForwardList<T>` | 单向链表、前向迭代器 | 范围 for |
| 适配器 | stack、queue、priority_queue | LIFO/FIFO/自定义 comparator |

## 关键知识梳理

### 1. STL 的核心不是容器，而是协议

标准算法并不关心容器类型，只关心迭代器是否满足要求。`MyVector` 能排序，是因为它提供随机访问迭代器；`MyList` 能反转，是因为它提供双向迭代器。

### 2. 连续容器和节点容器的取舍

`vector` 适合随机访问和尾部追加，缓存友好；链表适合局部插入删除，但随机访问差、缓存局部性弱。`deque` 试图在头尾操作和随机访问之间做折中。

### 3. 适配器通过限制接口表达语义

`stack`、`queue`、`priority_queue` 都不是新的存储模型，而是对底层容器的接口约束。接口越窄，使用者越不容易破坏抽象语义。

### 4. 教学实现和工业实现的边界

本月实现重点在机制理解，因此简化了异常安全、allocator 接入、复杂构造/析构控制、deque 分块 map、迭代器失效规则等工业细节。后续如果继续打磨 TinySTL，需要把这些边界逐步补上。

## 构建验证

```powershell
cmake -S CPP-Practice/stl_sequence -B CPP-Practice/stl_sequence/build
cmake --build CPP-Practice/stl_sequence/build --config Release
.\CPP-Practice\stl_sequence\build\Release\stl_sequence_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续衔接

Month05 会进入关联容器，重点从“线性序列”切换到“有序树结构和哈希结构”：

- 红黑树旋转与染色
- `set/map` 的迭代器稳定性
- 哈希表桶、负载因子与 rehash
- allocator 与算法整合
