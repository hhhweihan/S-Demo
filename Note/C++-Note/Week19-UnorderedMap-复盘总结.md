# Week19 UnorderedMap 复盘总结

## 核心目标

Week19 实现开链哈希表，理解 bucket、负载因子、rehash 和冲突处理。

## 开链哈希表

哈希表先用 `hash(key) % bucket_count` 定位 bucket。如果多个 key 落到同一个 bucket，就在链表中顺序查找。

## 负载因子

负载因子为：

$$
load\_factor = \frac{size}{bucket\_count}
$$

当负载因子超过阈值（本周设置为 0.75）时触发 rehash：创建更大的 bucket 数组，并按新 bucket 数重新分布所有元素。

## 与 map 的对比

| 容器 | 底层 | 查找 | 顺序 | 范围查询 |
|------|------|------|------|----------|
| map | 红黑树 | $O(\log n)$ | 有序 | 支持 |
| unordered_map | 哈希表 | 平均 $O(1)$ | 无序 | 不支持 |

## 复盘结论

哈希表的性能来自空间换时间：更多 bucket 可以减少链长，但会增加内存占用。实际工程里，hash 函数质量、负载因子和 rehash 时机比单个接口实现更影响整体表现。
