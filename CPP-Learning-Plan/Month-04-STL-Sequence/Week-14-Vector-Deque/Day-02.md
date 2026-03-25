## Day 2（Tue）— push_back + 扩容策略

**预计时间：1 小时**

**任务：**
- [ ] 实现 `push_back(val)` 和扩容逻辑（容量翻倍）：
  ```cpp
  void push_back(const T& val) {
      if (end_ == cap_end_) reallocate(capacity() == 0 ? 1 : capacity() * 2);
      construct(end_++, val);
  }
  void reallocate(size_t new_cap) {
      T* new_data = alloc_.allocate(new_cap);
      // 移动旧元素（优先 move，fallback copy）
      // 销毁旧元素 + 释放旧内存
  }
  ```
- [ ] 实现 `pop_back()` / `clear()` / `reserve(n)` / `resize(n, val=T{})`
- [ ] 测试：push_back 1000 个元素，验证 size/capacity 正确

**完成标志：** 扩容过程中无内存泄漏（ASan）

---

