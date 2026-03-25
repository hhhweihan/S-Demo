## Day 2（Tue）— advance 和 distance 的高效实现

**预计时间：1 小时**

**任务：**
- [ ] 实现 `advance(it, n)`：根据迭代器类别选择最优实现
  ```cpp
  // random_access: it += n;  O(1)
  // bidirectional: 正负方向各自循环  O(n)
  // input/forward: 只能正向循环  O(n)
  // 用 tag dispatch 而非 if constexpr（先练习 tag dispatch）
  template<typename Iter>
  void advance(Iter& it, typename iterator_traits<Iter>::difference_type n) {
      advance_impl(it, n, typename iterator_traits<Iter>::iterator_category{});
  }
  ```
- [ ] 实现 `distance(first, last)`：同样 tag dispatch
- [ ] 测试：对 `int*`（random_access）、`std::list<int>::iterator`（bidirectional）分别计时

**完成标志：** random_access 版本 O(1)，bidirectional 版本 O(n)，结果正确

---

