# Week 2 — 通用内存池（PoolAllocator）

## 本周目标

支持任意大小对象的分配，内部按 size class 分流到对应的 FixedAllocator。
完成后能作为 `std::vector` 的 allocator 使用。

---