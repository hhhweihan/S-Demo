## Day 4（Thu）— allocator_traits 基础

**预计时间：1 小时**

**任务：**
- [ ] 了解 `allocator_traits<Alloc>` 的作用：统一分配器接口
  ```cpp
  // 不管 Alloc 是否有 construct/destroy，都通过 allocator_traits 调用
  allocator_traits<Alloc>::construct(alloc, ptr, args...);
  allocator_traits<Alloc>::destroy(alloc, ptr);
  ```
- [ ] 给 MyVector 和 MyList 改用 `allocator_traits` 而不是直接调用 `alloc_.construct()`
- [ ] 测试：自定义一个只有 `allocate/deallocate` 的最简 Allocator，验证 allocator_traits 能自动补全

**完成标志：** 最简 Allocator 能在 MyVector 中工作

---

