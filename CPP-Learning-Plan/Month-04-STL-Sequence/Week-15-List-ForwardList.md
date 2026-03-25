# Week 15 — list + forward_list

## 本周目标

实现双向链表 MyList 和单向链表 MyForwardList，理解节点式容器的迭代器设计。

---

## Day 1（Mon）— MyList 节点设计

**预计时间：1 小时**

**任务：**
- [ ] 定义节点结构（带哨兵节点，避免边界处理）：
  ```cpp
  struct ListNode {
      ListNode* prev;
      ListNode* next;
  };
  template<typename T>
  struct ListNodeData : ListNode {
      T data;
  };
  ```
- [ ] 哨兵节点（dummy head）：`head_.next = &head_; head_.prev = &head_;`
- [ ] 实现 `insert_before(node, val)` 私有方法

**完成标志：** 能手动插入 3 个节点并用 while 遍历

---

## Day 2（Tue）— MyList 迭代器 + 常用接口

**预计时间：1 小时**

**任务：**
- [ ] 实现双向迭代器：
  ```cpp
  struct iterator {
      using iterator_category = bidirectional_iterator_tag;
      ListNode* node_;
      reference operator*()  { return static_cast<ListNodeData<T>*>(node_)->data; }
      iterator& operator++() { node_ = node_->next; return *this; }
      iterator& operator--() { node_ = node_->prev; return *this; }
  };
  ```
- [ ] 实现：`push_back / push_front / pop_back / pop_front / insert(pos, val) / erase(pos) / size / empty`
- [ ] 测试：`std::reverse(list.begin(), list.end())` 需要双向迭代器

**完成标志：** std::reverse 在 MyList 上正常工作

---

## Day 3（Wed）— MyList 高级操作

**预计时间：1 小时**

**任务：**
- [ ] 实现 `sort()`：链表排序，推荐归并排序（不移动数据，只改指针）
  ```cpp
  // 时间复杂度 O(n log n)，链表适合归并排序
  // 关键：merge(sorted_left, sorted_right) 只需改 next/prev 指针
  ```
- [ ] 实现 `splice(pos, other, it)`：把 other 中的 it 移动到当前 list 的 pos 前
- [ ] 实现 `unique()`：删除相邻重复元素
- [ ] 对比：MyList sort 和 std::list sort 各 10 万元素耗时

**完成标志：** sort/splice/unique 结果正确

---

## Day 4（Thu）— MyForwardList

**预计时间：1 小时**

**任务：**
- [ ] 实现单向链表 `forward_list<T>`（只有 next 指针）：
  - 迭代器类别：`forward_iterator_tag`
  - 不支持 `push_back`（只有 `push_front` 是 O(1)，push_back 是 O(n)）
  - 实现 `insert_after(pos, val)` / `erase_after(pos)` / `before_begin()`
- [ ] 验证：forward_list 的 iterator 不能用 `--`（编译报错）

**完成标志：** forward_list 基本操作正确，`--` 编译报错

---

## Day 5（Fri）— allocator 接入

**预计时间：1 小时**

**任务：**
- [ ] 给 MyList 加 `Alloc` 模板参数（默认 `std::allocator<T>`）：
  - 节点分配：`alloc_.allocate(1)` + `construct()`
  - 节点销毁：`destroy()` + `alloc_.deallocate(ptr, 1)`
- [ ] 把 Month 1 的 `PoolAllocator` 接入 MyList：
  ```cpp
  MyList<int, PoolAllocator<int>> fast_list;
  ```
- [ ] 对比：默认 allocator vs PoolAllocator，插入 10 万节点各自耗时

**完成标志：** PoolAllocator 版本比默认版本快

---

## Day 6（Sat）— 读 libc++ list 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 libc++ `include/list`（重点：`__list_node_base`、`__list_node`、迭代器实现）
- [ ] 回答：
  1. libc++ 为什么把节点指针和数据分成两个 struct（`__list_node_base` 和 `__list_node<T>`）？
  2. libc++ 的 `splice` 如何做到 O(1)（hint：不更新 size 计数的情况下 splice 是常数时间）？
  3. C++11 之后 list::size() 的复杂度为什么必须是 O(1)？（ABI 破坏问题）

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理迭代器类别与容器操作的关系表（哪些 STL 算法需要哪种迭代器）
- [ ] `git commit`，推送
