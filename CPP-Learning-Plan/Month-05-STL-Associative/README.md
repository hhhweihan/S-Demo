# Month 5 — 自编 STL（关联容器 + 算法 + allocator）

## 月度目标

实现红黑树（set/map 底层），哈希表（unordered 容器），核心 STL 算法，
并把 Month 1 的 PoolAllocator 完整接入整个 STL。

## 技能树

```
自编 STL（关联与算法部分）
├── 红黑树                 ← Week 17
│   ├── insert（旋转 + 变色）
│   ├── erase（6 种删除情况）
│   └── iterator（中序遍历）
├── set / map              ← Week 18
│   ├── 基于 RBTree 的 set / multiset
│   ├── map / multimap
│   └── lower_bound / upper_bound
├── unordered_map          ← Week 19
│   ├── 开链哈希表
│   ├── 负载因子 + rehash
│   └── unordered_set
└── 算法 + allocator 体系  ← Week 20
    ├── sort / stable_sort / heap_sort
    ├── binary_search 系列
    └── allocator_traits 完整接入
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 17 | 红黑树 from scratch | [Week-17-RBTree/](./Week-17-RBTree/) |
| Week 18 | set / map | [Week-18-Set-Map/](./Week-18-Set-Map/) |
| Week 19 | unordered_map / set | [Week-19-UnorderedMap/](./Week-19-UnorderedMap/) |
| Week 20 | 算法 + allocator 体系 | [Week-20-Algorithms-Allocator/](./Week-20-Algorithms-Allocator/) |

## 验收标准

- [ ] 红黑树插入 10 万随机数后，树高 ≤ 2×log₂(n)（黑高约束）
- [ ] `MyMap<string, int>` 支持 `lower_bound` 和 `upper_bound`
- [ ] `MyUnorderedMap` 在负载因子 0.75 时自动 rehash
- [ ] 整个 STL 库使用 PoolAllocator，比默认 allocator 快 20%+
