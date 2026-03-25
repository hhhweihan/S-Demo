# Week 1 — 固定大小内存池（FixedAllocator）

## 本周目标

写一个只分配固定大小对象的内存池。用 free list + chunk 扩展策略，替代 new/delete。
完成后 benchmark：分配 100 万次比 new 快 3x 以上。

---