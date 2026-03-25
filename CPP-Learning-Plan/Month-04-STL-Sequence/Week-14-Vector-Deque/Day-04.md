## Day 4（Thu）— 随机访问迭代器 + STL 兼容

**预计时间：1 小时**

**任务：**
- [ ] 实现完整的 `iterator` 内部类（继承自 `mystl::iterator<random_access_iterator_tag, T>`）：
  - `operator++(int)` / `operator--(int)` / `operator+=(n)` / `operator[]`
  - `operator<` / `operator-`（两个迭代器相减）
- [ ] 测试 STL 算法兼容性：
  ```cpp
  MyVector<int> v = {5, 3, 1, 4, 2};
  std::sort(v.begin(), v.end());           // 需要随机访问迭代器
  std::for_each(v.begin(), v.end(), ...);  // 需要 input 迭代器
  ```

**完成标志：** `std::sort` 在 MyVector 上正常工作

---

