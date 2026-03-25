## Day 6（Sat）— 读 libc++ iterator 实现

**预计时间：2 小时**

**任务：**
- [ ] 阅读 libc++ `include/__iterator/iterator_traits.h`（约 150 行）
- [ ] 找出 3 处 libc++ 比你实现更精细的地方：
  1. libc++ 如何检测一个类型是不是迭代器（`__has_iterator_typedefs`）？
  2. 为什么 libc++ 有 `__iter_value_type`、`__iter_reference` 等辅助别名？
  3. C++20 的 `std::ranges::iterator_t` 与 `iterator_traits` 有什么不同？
- [ ] 做笔记

**完成标志：** 3 个问题有答案

---

