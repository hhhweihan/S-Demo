## Day 3（Wed）— chunk 扩展 + 边界检查

**预计时间：1 小时**

**任务：**
- [ ] 当 `free_list_ == nullptr` 时，自动申请新 chunk（容量翻倍策略：1, 2, 4, 8...）
  ```cpp
  void FixedAllocator::expand() {
      size_t new_count = current_capacity_ * 2;
      void* new_chunk  = malloc(block_size_ * new_count);
      // 初始化新 chunk 的 free list，接在现有 free list 后面
      current_capacity_ = new_count;
      chunks_.push_back(new_chunk);
  }
  ```
- [ ] `stats()` 返回：总块数、已分配块数、chunk 数量
- [ ] 析构函数：遍历 `chunks_` 逐一 `free`
- [ ] 测试：分配超过初始容量，验证自动扩展；验证 stats 数字正确

**完成标志：** 扩展后能继续分配，析构无内存泄漏

---