## Day 5（Fri）— allocator 接入

**预计时间：1 小时**

**任务：**
- [ ] 给 MyList 加 `Alloc` 模板参数（默认 `std::allocator<T>`）：
  - 节点分配：`alloc_.allocate(1)` + `construct()`
  - 节点销毁：`destroy()` + `alloc_.deallocate(ptr, 1)`
- [ ] 把 Month 1 的 `PoolAllocator` 接入 MyList：
  ```cpp
  MyList<int, PoolAllocator<int>> fast_list;
  ```
- [ ] 对比：默认 allocator vs PoolAllocator，插入 10 万节点各自耗时

**完成标志：** PoolAllocator 版本比默认版本快

---

