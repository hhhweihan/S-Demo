# Week20 算法 / Allocator 与整月复盘总结

## 核心目标

Week20 完成 Month05 的算法和 allocator 收束：排序算法、二分查找、allocator_traits 接入和整月综合验证。

## 算法族

本周验证了：

- `heap_sort`：用 heap 建堆和出堆排序。
- `stable_sort`：保持等价元素原有相对顺序。
- `partial_sort`：只保证前 k 个元素有序且是全局最小 k 个。
- `binary_search`：在有序区间内查找。

## Allocator

`PoolAllocator<T>` 实现了标准 allocator 最小接口：

- `value_type`
- `allocate`
- `deallocate`
- 跨类型构造
- `operator== / !=`

当前验证重点是能被 `allocator_traits` 识别并用于 `std::vector`。性能提升目标需要更完整的对象池、批量分配和稳定 benchmark，不能只凭一次运行下结论。

## Month05 总结

Month05 的主线是：

1. RBTree 提供有序平衡结构。
2. set/map 封装树语义。
3. unordered_map 提供哈希查找语义。
4. 算法和 allocator 把 STL 的“算法-容器-内存策略”三层关系串起来。

## 复盘结论

关联容器的重点是按访问需求选结构：需要有序和范围查询时选树，需要平均快速定位时选哈希。allocator 则提醒我们：容器性能不只来自算法复杂度，还来自内存分配策略。
