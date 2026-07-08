# Week 18 — set / map（基于 RBTree）

## 本周目标

在 Week 17 红黑树的基础上，封装 `MySet`、`MyMultiSet`、`MyMap`、`MyMultiMap`，
达到与 `std::map` / `std::set` 接口兼容。

## 本周完成情况

- [x] Day 120：封装 `MySet`，支持 insert/erase/find/begin/end/size
- [x] Day 121：封装 `MyMap`，支持 `operator[]`、find、lower/upper/equal_range
- [x] Day 122：实现 `MyMultiSet` / `MyMultiMap` 的重复 key 语义
- [x] Day 123：验证 set/map 中序遍历顺序
- [x] Day 124：验证 map 边界查找结果
- [x] Day 125：整理 set/map 与 multi 容器接口差异
- [x] Day 126：完成 Week18 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `MySet<T>` / `MyMap<K, V>`：基于 RBTree 的唯一 key 容器
- `MyMultiSet<T>` / `MyMultiMap<K, V>`：重复 key 语义示例
- 周复盘：`Note/C++-Note/Week18-SetMap-复盘总结.md`

## Week 18 总结

Week18 体现了“底层树结构”和“上层容器语义”的分离。`set` 的 key 也是 value，`map` 的 key 用于排序，value 用于承载业务数据；`multiset/multimap` 的核心差异是 insert 不再去重，`count` 和范围查询要能表达重复 key。

本周实现以接口语义和验证为主，multi 容器采用排序数组示例来突出重复 key 行为；唯一 key 容器则复用 Week17 的 RBTree。

## 验证命令

```powershell
cmake --build CPP-Practice/stl_associative/build --config Release
.\CPP-Practice\stl_associative\build\Release\stl_associative_demo.exe
```

---
