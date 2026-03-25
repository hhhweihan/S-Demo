# Week 13 — 迭代器体系（iterator_traits + 5 种迭代器）

## 本周目标

建立 STL 迭代器的完整体系，理解迭代器类别对算法效率的影响。

---

## Day 1（Mon）— 迭代器类别标签

**预计时间：1 小时**

**任务：**
- [ ] 创建 `mystl/iterator.h`，定义 5 种迭代器类别标签（空结构体）：
  ```cpp
  struct input_iterator_tag {};
  struct output_iterator_tag {};
  struct forward_iterator_tag       : input_iterator_tag {};
  struct bidirectional_iterator_tag : forward_iterator_tag {};
  struct random_access_iterator_tag : bidirectional_iterator_tag {};
  ```
- [ ] 定义 `iterator_traits<Iterator>` 主模板和针对裸指针的偏特化：
  ```cpp
  template<typename Iter>
  struct iterator_traits {
      using iterator_category = typename Iter::iterator_category;
      using value_type        = typename Iter::value_type;
      using difference_type   = typename Iter::difference_type;
      using pointer           = typename Iter::pointer;
      using reference         = typename Iter::reference;
  };
  // 裸指针特化
  template<typename T>
  struct iterator_traits<T*> {
      using iterator_category = random_access_iterator_tag;
      using value_type        = T;
      using difference_type   = ptrdiff_t;
      using pointer           = T*;
      using reference         = T&;
  };
  ```
- [ ] 测试：`iterator_traits<int*>::iterator_category` 是 `random_access_iterator_tag`

**完成标志：** iterator_traits 针对裸指针和自定义迭代器都能正确推导

---

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

## Day 5（Fri）— 用 iterator_traits 优化 copy 算法

**预计时间：1 小时**

**任务：**
- [ ] 实现两版 `my_copy(first, last, dest)`：
  ```cpp
  // 版本1：通用，逐元素复制
  // 版本2：当 iterator_category = random_access_iterator_tag 且
  //         is_trivially_copyable<T> 时，用 memmove 批量复制
  ```
- [ ] 对 10 万个 int 的 copy：通用版 vs memmove 版，计时对比

**完成标志：** memmove 版比通用版快 5x 以上

---

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

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 给 `mystl/iterator.h` 补充完整注释
- [ ] 整理 tag dispatch vs if constexpr 的选择时机
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**advance 性能（对 100 万个元素）：**
| 迭代器类型 | 耗时 |
|-----------|------|
| random_access (int*) | — μs |
| bidirectional (list::iterator) | — ms |

**libc++ 对比笔记：**
1.
2.
3.
