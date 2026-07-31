# unordered_map / unordered_set（开链哈希表）

## 本周目标

从零实现开链哈希表，封装 `MyUnorderedMap` 和 `MyUnorderedSet`，
理解负载因子、rehash 时机和哈希碰撞处理。

## 本周完成情况

- [x] Day 127：实现 bucket 数组和开链节点结构
- [x] Day 128：实现 `operator[]` / insert 路径
- [x] Day 129：实现 find / erase
- [x] Day 130：实现负载因子和 0.75 rehash 触发
- [x] Day 131：验证 rehash 后元素仍可查找
- [x] Day 132：整理哈希冲突、负载因子与有序树对比
- [x] Day 133：完成 Week19 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `MyUnorderedMap<K, V>`：开链哈希表、自动 rehash、erase
- 周复盘：`Note/C++-Note/Week19-UnorderedMap-复盘总结.md`

## 阶段小结
Week19 从有序树切换到哈希表。哈希表的平均 $O(1)$ 来自桶定位和较低负载因子；当元素数量超过 `bucket_count * max_load_factor` 时，需要扩容并把所有元素重新分布到新桶。开链法让冲突元素挂在同一个 bucket 链表中，删除时只需要从链表摘除对应节点。

与 `map` 相比，`unordered_map` 不保证遍历有序，但查找常数通常更小；当需要范围查询、顺序遍历或稳定有序语义时仍应选择树结构。

## 验证命令

```powershell
cmake --build CPP-Practice/stl_associative/build --config Release
.\CPP-Practice\stl_associative\build\Release\stl_associative_demo.exe
```

---
