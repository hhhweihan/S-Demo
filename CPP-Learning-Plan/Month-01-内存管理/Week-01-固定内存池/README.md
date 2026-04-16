# Week 1 — 固定大小内存池（FixedAllocator）

## 本周目标

写一个只分配固定大小对象的内存池。用 free list + chunk 扩展策略，替代 new/delete。
完成后 benchmark：分配 100 万次比 new 快 3x 以上。

## 本周完成情况

- 已实现 `FixedAllocator` 的初始化、按块分配、单块回收与 chunk 扩容。
- 已补齐基础正确性测试，包括批量分配/释放、地址复用、自动扩容和随机混合操作。
- 已为关键路径补充注释，明确 free list、chunk 和边界检查逻辑。
- 已完成 `new/delete`、`malloc/free`、`FixedAllocator` 的基准测试对比。

## Week 01 总结

- 这一周完成了一个教学版固定大小内存池，核心结构是“预分配 chunk + free list O(1) 分配回收”。
- 关键收获有两个：
	1. 正确性不只是“能跑起来”，`deallocate()` 的指针范围和对齐检查同样重要，否则 free list 很容易被错误指针破坏。
	2. benchmark 不能把扩容和热路径混在一起测；先预分配，再测 allocate/deallocate，结果才有解释力。
- 这一周的 Release benchmark 结果已经达到目标：`new/delete = 22.543 ms`，`malloc/free = 22.225 ms`，`FixedAllocator = 2.121 ms`，相对 `new/delete` 约快 `10.63x`。
- 和 nginx pool 对照后可以看出，两者方向不同：当前 `FixedAllocator` 追求固定大小对象的 O(1) 单块回收，而 nginx pool 更偏向请求级生命周期内的批量释放。

---