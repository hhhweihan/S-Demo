## Day 3（Wed）— reverse_iterator

**预计时间：1 小时**

**任务：**
- [ ] 实现 `reverse_iterator<Iter>` 适配器：
  ```cpp
  template<typename Iter>
  class reverse_iterator {
      Iter current_;
  public:
      using iterator_category = typename iterator_traits<Iter>::iterator_category;
      using value_type        = typename iterator_traits<Iter>::value_type;
      using reference         = typename iterator_traits<Iter>::reference;

      explicit reverse_iterator(Iter it) : current_(it) {}
      reference operator*()  const { Iter tmp = current_; return *--tmp; }
      reverse_iterator& operator++() { --current_; return *this; }
      reverse_iterator& operator--() { ++current_; return *this; }
      bool operator!=(const reverse_iterator& o) const { return current_ != o.current_; }
  };
  ```
- [ ] 测试：用 `reverse_iterator` 反向遍历 `int arr[]`

**完成标志：** 反向遍历正确，`*rbegin()` 指向最后一个元素

---

