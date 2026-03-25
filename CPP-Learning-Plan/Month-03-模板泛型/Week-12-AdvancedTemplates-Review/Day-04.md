## Day 4（Thu）— 编译期计算（constexpr）

**预计时间：1 小时**

**任务：**
- [ ] 用 `constexpr` 实现编译期：
  - 斐波那契数列
  - 快速排序（C++20 constexpr vector 或递归数组）
  - 质数筛（Sieve of Eratosthenes）
- [ ] 用 `static_assert` 验证编译期结果
- [ ] 实现编译期字符串哈希：
  ```cpp
  constexpr size_t hash(const char* s) { ... }
  static_assert(hash("hello") != hash("world"));
  ```

**完成标志：** 所有结果在编译期确定，static_assert 全部通过

---