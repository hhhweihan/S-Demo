# Month05 STL 关联容器实战总结

## 月度目标回顾

Month05 完成自编 STL 关联容器、哈希表、算法和 allocator 接入。代码位于 `CPP-Practice/stl_associative/`。

## 已完成组件

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| RBTree | 插入、旋转、变色、中序迭代、边界查询、教学版 erase | 10 万随机插入、不变量、高度约束 |
| MySet/MyMap | 唯一 key 有序容器 | 有序遍历、查找、erase、lower/upper/equal_range |
| Multi 容器 | 重复 key 语义示例 | count 重复数量 |
| MyUnorderedMap | 开链哈希、负载因子、rehash、erase | rehash 触发后查找保持正确 |
| 算法 | heap_sort、stable_sort、partial_sort、binary_search | 排序、稳定性、前 k 个、查找验证 |
| Allocator | PoolAllocator 最小接口 | `std::vector` allocator_traits 接入 |

## 关键知识梳理

### 1. 树和哈希的取舍

红黑树提供稳定的 $O(\log n)$，支持有序遍历和范围查询；哈希表提供平均 $O(1)$，但不支持顺序语义。

### 2. 红黑树验证必须自动化

只靠肉眼看旋转结果不够。至少要验证：根黑、红节点无红子、黑高一致、BST 顺序、高度边界。

### 3. multi 容器的本质是插入语义变化

`set/map` 去重，`multiset/multimap` 保留重复 key。这个差异会影响 `insert` 返回值、`count`、`equal_range` 和迭代范围。

### 4. rehash 是哈希表性能的关键

负载因子过高会让 bucket 链变长，查找退化；rehash 通过增加 bucket 数重新分布元素，把平均链长拉回可控范围。

### 5. allocator 是容器的内存策略参数

allocator 把分配/释放从容器逻辑中拆出。当前实现完成接口接入；若要追求真实性能，需要固定块池、对象复用、批量分配和 benchmark 对照。

## 构建验证

```powershell
cmake -S CPP-Practice/stl_associative -B CPP-Practice/stl_associative/build
cmake --build CPP-Practice/stl_associative/build --config Release
.\CPP-Practice\stl_associative\build\Release\stl_associative_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续衔接

Month06 将进入异步日志系统，重点从容器实现切换到工程化并发组件：同步日志、异步双缓冲、多 sink、性能压测和集成。
