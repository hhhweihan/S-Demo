# 核心 STL 算法 + allocator_traits 完整接入

## 本周目标

实现 sort / stable_sort / binary_search 算法族，并将 PoolAllocator 完整接入
Month 4-5 自编 STL，完成整月综合验收。

## 本周完成情况

- [x] Day 134：实现 heap sort 验证路径
- [x] Day 135：实现 `stable_sort` / `partial_sort` 包装并验证稳定性和前 k 个结果
- [x] Day 136：实现 `binary_search` 验证路径
- [x] Day 137：实现 `PoolAllocator<T>` 并接入 `std::vector` allocator_traits
- [x] Day 138：完成功能接入；20%+ 性能提升目标保留为后续专项 benchmark
- [x] Day 139：整理 Month04-05 自编 STL 结构和接口差异
- [x] Day 140：规划 Month06 异步日志系统；`git commit / push` 等待明确确认后再执行

## 本周产出

- `heap_sort` / `stable_sort` / `partial_sort` / `binary_search`
- `PoolAllocator<T>` allocator_traits 接入示例
- 周复盘：`Note/C++-Note/算法Allocator与整月复盘总结.md`
- 月总结：`Note/C++-Note/STL关联容器实战总结.md`

## 阶段小结
Week20 把 Month05 收束到算法和 allocator。算法层面完成排序、稳定排序、部分排序和二分查找的验证；allocator 层面完成可被标准 `allocator_traits` 调用的最小接口。当前 allocator 是功能接入版，已验证可用于 `std::vector<int, PoolAllocator<int>>`；性能提升需要更细的固定块池、对象复用和稳定 benchmark，不能只靠一次 demo 结论替代。

## 验证命令

```powershell
cmake -S CPP-Practice/stl_associative -B CPP-Practice/stl_associative/build
cmake --build CPP-Practice/stl_associative/build --config Release
.\CPP-Practice\stl_associative\build\Release\stl_associative_demo.exe
```

---
