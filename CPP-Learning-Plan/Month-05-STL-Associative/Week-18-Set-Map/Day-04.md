## Day 4（Thu）— 迭代器与 range-for 支持

**预计时间：1 小时**

**任务：**
- [ ] 确认 set/map 的 iterator 满足双向迭代器要求（`--it` 正确）
- [ ] 验证 range-for 遍历输出有序：
  ```cpp
  for (auto& [k, v] : mymap) { ... }  // C++17 结构化绑定
  ```
- [ ] 实现 `rbegin() / rend()`（反向迭代器）：
  ```cpp
  using reverse_iterator = std::reverse_iterator<iterator>;
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  ```
- [ ] 测试：正向 + 反向遍历均有序

**完成标志：** 反向遍历输出逆序，range-for 输出正序

---

