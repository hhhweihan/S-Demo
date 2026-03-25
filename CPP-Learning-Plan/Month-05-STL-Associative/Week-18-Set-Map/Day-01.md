## Day 1（Mon）— MySet 封装

**预计时间：1 小时**

**任务：**
- [ ] 创建 `mystl/set.h`，用 RBTree 封装：
  ```cpp
  template<typename Key, typename Compare = std::less<Key>,
           typename Alloc = std::allocator<Key>>
  class set {
      using tree_type = RBTree<Key, Key, identity<Key>, Compare, Alloc>;
      tree_type tree_;
  public:
      using iterator       = typename tree_type::const_iterator;  // set 不允许修改 key
      using const_iterator = typename tree_type::const_iterator;
      // insert / erase / find / begin / end / size / empty
  };
  ```
- [ ] 实现 `insert(key)` → 返回 `pair<iterator, bool>`（已存在则 bool=false）
- [ ] 实现 `erase(key)` / `erase(iterator)` / `find(key)`
- [ ] 测试：插入 1000 个随机数，验证 size 和 find 正确

**完成标志：** MySet 基本操作与 std::set 行为一致

---

