# Week 16 — 容器适配器 + 整月复盘

## 本周目标

实现 stack/queue/priority_queue，完成整月代码整合和性能对比。

---

## Day 1（Mon）— stack + queue

**预计时间：1 小时**

**任务：**
- [ ] 实现 `stack<T, Container = MyDeque<T>>`（容器适配器，不是独立容器）：
  ```cpp
  template<typename T, typename Container = deque<T>>
  class stack {
      Container c_;
  public:
      void push(const T& v) { c_.push_back(v); }
      void pop()            { c_.pop_back(); }
      T&   top()            { return c_.back(); }
      bool empty() const    { return c_.empty(); }
      size_t size() const   { return c_.size(); }
  };
  ```
- [ ] 同理实现 `queue<T, Container = MyDeque<T>>`（push_back / pop_front）
- [ ] 测试：括号匹配算法（用 stack），BFS（用 queue）

**完成标志：** 括号匹配和 BFS 结果正确

---

## Day 2（Tue）— priority_queue（二叉堆）

**预计时间：1 小时**

**任务：**
- [ ] 实现 `priority_queue<T, Container = MyVector<T>, Compare = std::less<T>>`：
  ```cpp
  template<typename T, typename Container = vector<T>,
           typename Compare = std::less<T>>
  class priority_queue {
      Container c_;
      Compare   cmp_;
  public:
      void push(const T& v) {
          c_.push_back(v);
          push_heap(c_.begin(), c_.end(), cmp_);  // 先用 std::push_heap
      }
      void pop() {
          pop_heap(c_.begin(), c_.end(), cmp_);
          c_.pop_back();
      }
      const T& top() const { return c_.front(); }
  };
  ```
- [ ] 测试：用自定义 comparator 实现最小堆

**完成标志：** priority_queue 支持最大堆和最小堆

---

## Day 3（Wed）— 手写堆算法（push_heap / pop_heap / make_heap）

**预计时间：1 小时**

**任务：**
- [ ] 不用 std::push_heap，自己实现：
  ```cpp
  // push_heap：新元素已在容器末尾，向上 sift-up
  // pop_heap：把 top 与末尾交换，末尾逻辑删除，对 top 做 sift-down
  // make_heap：对所有非叶节点从后到前做 sift-down  O(n)
  ```
- [ ] 把 priority_queue 换成使用你自己的堆函数
- [ ] 测试：堆排序（make_heap + 逐个 pop_heap）10万元素

**完成标志：** 堆排序结果正确，复杂度 O(n log n)

---

## Day 4（Thu）— allocator_traits 基础

**预计时间：1 小时**

**任务：**
- [ ] 了解 `allocator_traits<Alloc>` 的作用：统一分配器接口
  ```cpp
  // 不管 Alloc 是否有 construct/destroy，都通过 allocator_traits 调用
  allocator_traits<Alloc>::construct(alloc, ptr, args...);
  allocator_traits<Alloc>::destroy(alloc, ptr);
  ```
- [ ] 给 MyVector 和 MyList 改用 `allocator_traits` 而不是直接调用 `alloc_.construct()`
- [ ] 测试：自定义一个只有 `allocate/deallocate` 的最简 Allocator，验证 allocator_traits 能自动补全

**完成标志：** 最简 Allocator 能在 MyVector 中工作

---

## Day 5（Fri）— 整月综合测试

**预计时间：1 小时**

**任务：**
- [ ] 对所有容器跑 benchmark（各插入/查找 100 万次）：
  ```
  MyVector vs std::vector
  MyList   vs std::list
  priority_queue sort vs std::sort
  ```
- [ ] 用 ASan + `-D_GLIBCXX_DEBUG`（如果 GCC）运行全部测试
- [ ] 把结果填到「本周结果」

**完成标志：** 无内存错误，有性能数据

---

## Day 6（Sat）— 写总结文章

**预计时间：2 小时**

- [ ] 写「自编STL序列容器总结」（至少 800 字）：
  1. 迭代器类别体系：为什么要分 5 种，tag dispatch 的优雅在哪里
  2. vector 扩容策略：翻倍 vs 1.5x，各有什么优缺点
  3. deque 分块设计：为什么 O(1) push_front 而 vector 不行
  4. list 为什么适合用 PoolAllocator（节点大小固定）

---

## Day 7（Sun）— 规划 Month 5

**预计时间：1 小时**

- [ ] 规划下月（关联容器）：先思考红黑树的 insert/delete 逻辑
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**Benchmark（100 万次操作）：**
| 容器 | 我的实现 | std 实现 | 差距 |
|------|---------|---------|------|
| vector push_back | — | — | — |
| list push_back | — | — | — |
