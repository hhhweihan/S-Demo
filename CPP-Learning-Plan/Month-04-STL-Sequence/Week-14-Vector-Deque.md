# Week 14 — vector + deque

## 本周目标

实现 `MyVector<T>`（完整动态数组）和 `MyDeque<T>`（分块双端队列），支持 STL 算法。

---

## Day 1（Mon）— MyVector 骨架 + 内存管理

**预计时间：1 小时**

**任务：**
- [ ] 创建 `mystl/vector.h`，实现基础框架：
  ```cpp
  template<typename T, typename Alloc = std::allocator<T>>
  class vector {
      T*     begin_    = nullptr;  // 数据起始
      T*     end_      = nullptr;  // 已用结束
      T*     cap_end_  = nullptr;  // 分配结束
  public:
      // 类型别名、构造/析构、begin/end/size/capacity
  };
  ```
- [ ] 实现构造：`vector()` / `vector(n, val)` / `vector(first, last)`
- [ ] 实现析构：逐元素 `~T()` + 释放内存
- [ ] 实现 `begin() / end() / size() / capacity() / empty()`

**完成标志：** 能用范围 for 遍历，size/capacity 正确

---

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

## Day 3（Wed）— insert + erase + 异常安全

**预计时间：1 小时**

**任务：**
- [ ] 实现 `insert(pos, val)`：pos 之后的元素右移，可能触发扩容
- [ ] 实现 `erase(pos)` 和 `erase(first, last)`：左移填补
- [ ] 异常安全保证：`push_back` 若 T 的构造函数抛异常，vector 的状态不变（strong guarantee）
  - 技巧：先在新空间里构造，成功后再更新指针
- [ ] 实现 `emplace_back(args...)` 和 `emplace(pos, args...)`（原地构造）

**完成标志：** 随机插入/删除结果正确，ASan 无报错

---

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

## Day 5（Fri）— MyDeque 设计与实现

**预计时间：1 小时**

**任务：**
- [ ] 理解 deque 的分块存储原理：中控数组（map）+ 固定大小 buffer
  ```
  map:  [ptr0][ptr1][ptr2][ptr3]
         ↓     ↓     ↓     ↓
        [buf] [buf] [buf] [buf]  ← 每个 buf 固定 512 字节
  iterator 需要知道：cur_ptr, buf_start, buf_end, map_node
  ```
- [ ] 实现 `deque` 的 iterator（4 个指针 + 跨 buffer 的 `operator++`）
- [ ] 实现 `push_front(val)` / `push_back(val)` / `front()` / `back()`

**完成标志：** 能交替 push_front 和 push_back 各 100 次，front/back 正确

---

## Day 6（Sat）— 读 libc++ vector 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 libc++ `include/vector`（重点：`__vector_base`、`push_back`、`_M_reallocate`）
- [ ] 回答：
  1. libc++ 的 `push_back` 为什么分 `push_back(const T&)` 和 `push_back(T&&)` 两个重载？
  2. `emplace_back` 如何用 placement new 原地构造，避免额外拷贝？
  3. `insert` 为什么要先处理「目标位置在已有元素中间」和「目标位置在末尾」两种情况？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 补充 MyVector 和 MyDeque 的注释
- [ ] 对比 MyVector 和 std::vector 的 push_back 性能（各 100 万次）
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**push_back 100 万次：**
| 实现 | 耗时 |
|------|------|
| MyVector | — ms |
| std::vector | — ms |
