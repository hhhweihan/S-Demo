# Set / Map 复盘总结

## 核心目标

Week18 在 RBTree 之上封装有序关联容器：`MySet`、`MyMap`、`MyMultiSet`、`MyMultiMap`。

## set 与 map

`set` 的 key 就是 value，主要接口是：

- `insert`
- `erase`
- `contains/find`
- `begin/end`

`map` 把 key 和 value 分离，key 参与排序，value 承载数据。`operator[]` 的语义是：如果 key 不存在，插入默认构造的 value，并返回引用。

## lower_bound / upper_bound

- `lower_bound(k)`：第一个不小于 `k` 的位置。
- `upper_bound(k)`：第一个大于 `k` 的位置。
- `equal_range(k)`：二者组成的范围。

这些接口依赖树的有序性，是 `map/set` 相比哈希表的重要优势。

## multi 容器

`multiset/multimap` 允许重复 key，因此 insert 总是保留新元素。当前实现用排序数组展示接口差异，重点是重复 key 的 `count` 语义。

## 复盘结论

有序关联容器的核心分层是：RBTree 负责平衡和顺序，容器封装负责接口语义。理解这层分离后，`set`、`map`、`multiset`、`multimap` 只是同一底层结构的不同约束。
