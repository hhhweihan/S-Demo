# 自编 STL（关联容器 + 算法 + allocator）

## 模块目标

实现红黑树（set/map 底层），哈希表（unordered 容器），核心 STL 算法，
并把 模块 01 的 PoolAllocator 完整接入整个 STL。

## 技能树

```
自编 STL（关联与算法部分）
├── 红黑树                 ← 阶段 17
│   ├── insert（旋转 + 变色）
│   ├── erase（6 种删除情况）
│   └── iterator（中序遍历）
├── set / map              ← 阶段 18
│   ├── 基于 RBTree 的 set / multiset
│   ├── map / multimap
│   └── lower_bound / upper_bound
├── unordered_map          ← 阶段 19
│   ├── 开链哈希表
│   ├── 负载因子 + rehash
│   └── unordered_set
└── 算法 + allocator 体系  ← 阶段 20
    ├── sort / stable_sort / heap_sort
    ├── binary_search 系列
    └── allocator_traits 完整接入
```

## 阶段拆解

| 阶段 | 主题 | 目录 |
|----|------|------|
| 阶段 17 | 红黑树 from scratch | [17-RBTree/](./17-RBTree/) |
| 阶段 18 | set / map | [18-Set-Map/](./18-Set-Map/) |
| 阶段 19 | unordered_map / set | [19-UnorderedMap/](./19-UnorderedMap/) |
| 阶段 20 | 算法 + allocator 体系 | [20-Algorithms-Allocator/](./20-Algorithms-Allocator/) |

## 验收标准

- [x] 红黑树插入 10 万随机数后，树高 ≤ 2×log₂(n)（黑高约束）
- [x] `MyMap<string, int>` 支持 `lower_bound` 和 `upper_bound`
- [x] `MyUnorderedMap` 在负载因子 0.75 时自动 rehash
- [x] `PoolAllocator<T>` 已接入 allocator_traits 并通过 `std::vector` 功能验证；20%+ 性能提升留待专项 benchmark

## 模块完成情况

- 阶段 17：完成 RBTree 插入、旋转、变色、中序迭代、边界查找和教学版 erase
- 阶段 18：完成 `MySet`、`MyMap`、`MyMultiSet`、`MyMultiMap` 接口语义验证
- 阶段 19：完成 `MyUnorderedMap` 开链哈希、负载因子、rehash、erase
- 阶段 20：完成排序/二分算法包装、allocator_traits 接入和模块综合测试

## 模块总结

本模块已完成“红黑树 -> 有序关联容器 -> 哈希关联容器 -> 算法/allocator”的闭环。这个模块的重点是理解两类关联容器的不同工程取舍：树结构提供有序遍历和范围查询，哈希表提供平均常数级查找；allocator 则把对象存储策略从容器逻辑中拆出去。

详细总结见 `Note/C++-Note/Month05-STL关联容器实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/stl_associative -B CPP-Practice/stl_associative/build
cmake --build CPP-Practice/stl_associative/build --config Release
.\CPP-Practice\stl_associative\build\Release\stl_associative_demo.exe
```
