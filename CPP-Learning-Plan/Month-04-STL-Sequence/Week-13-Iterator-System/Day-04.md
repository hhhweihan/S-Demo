## Day 4（Thu）— 迭代器基类 + 辅助函数

**预计时间：1 小时**

**任务：**
- [ ] 实现 `iterator<Category, T>` 基类（子类继承省去重复 typedef）：
  ```cpp
  template<typename Category, typename T,
           typename Distance = ptrdiff_t,
           typename Pointer  = T*,
           typename Reference = T&>
  struct iterator {
      using iterator_category = Category;
      using value_type        = T;
      using difference_type   = Distance;
      using pointer           = Pointer;
      using reference         = Reference;
  };
  ```
- [ ] 实现 `next(it, n=1)` 和 `prev(it, n=1)`（返回新迭代器，不修改原迭代器）
- [ ] 实现 `begin(container)` / `end(container)` 的自由函数版本

**完成标志：** 所有辅助函数能在自定义容器上正常工作

---

