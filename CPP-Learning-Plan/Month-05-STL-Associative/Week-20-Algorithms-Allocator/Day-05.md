## Day 5（Fri）— 整月综合 benchmark

**预计时间：1 小时**

**任务：**
- [ ] 跑完整 benchmark（各 100 万次操作）：
  ```
  MyVector push_back      vs std::vector
  MyList   push_back      vs std::list
  MyMap    insert/find    vs std::map
  MyUnorderedMap insert   vs std::unordered_map
  MyVector + PoolAllocator vs MyVector + std::allocator
  ```
- [ ] 用 ASan + Valgrind（如果 Linux）检测内存泄漏
- [ ] 验证月度验收标准：
  - [ ] 红黑树 10 万数后树高 ≤ 2×log₂(n)
  - [ ] MyMap 支持 lower_bound/upper_bound
  - [ ] MyUnorderedMap 在 load_factor 0.75 自动 rehash
  - [ ] PoolAllocator 版本比默认快 20%+

**完成标志：** 所有验收标准达成，数据已记录

---

