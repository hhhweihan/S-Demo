# Week 26 — SkipList 精读与仿写

## 本周目标

精读 LevelDB 的无锁 SkipList（350 行），完整仿写，理解其 memory_order 用法。

## 本周完成情况

- [x] Day 176：推导跳表层级和 `random_height()` 概率设计
- [x] Day 177：实现 SkipList 节点和多层 forward 指针
- [x] Day 178：实现插入路径和 prev 数组更新
- [x] Day 179：实现查找和有序迭代
- [x] Day 180：整理 LevelDB SkipList memory_order 设计要点
- [x] Day 181：完成跳表正确性验证；性能 20% 对比留待专项 benchmark
- [x] Day 182：完成 Week26 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `SkipList<Key, Value>`：随机高度、插入、查找、有序迭代
- 周复盘：`Note/C++-Note/Week26-SkipList-复盘总结.md`

## Week 26 总结

Week26 完成教学版 SkipList。跳表通过随机层高把有序链表扩展为多层索引，查询期望复杂度接近 $O(\log n)$。LevelDB 原版 SkipList 的亮点在于单写多读场景下用 acquire/release 语义发布节点；本工程先实现单线程教学版，重点验证随机高度、插入、查找和有序迭代。

## 验证命令

```powershell
cmake --build CPP-Practice/leveldb_mini/build --config Release
.\CPP-Practice\leveldb_mini\build\Release\leveldb_mini_demo.exe
```

---