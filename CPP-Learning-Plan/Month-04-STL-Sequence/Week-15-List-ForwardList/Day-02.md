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

