## Day 2（Tue）— MyMap 封装

**预计时间：1 小时**

**任务：**
- [ ] 创建 `mystl/map.h`，key 用于排序，value 可修改：
  ```cpp
  template<typename Key, typename Value,
           typename Compare = std::less<Key>,
           typename Alloc = std::allocator<std::pair<const Key, Value>>>
  class map {
      using tree_type = RBTree<Key, pair<const Key, Value>,
                                select1st<pair<const Key,Value>>, Compare, Alloc>;
      tree_type tree_;
  public:
      Value& operator[](const Key& k) {
          auto [it, ok] = insert({k, Value{}});
          return it->second;
      }
  };
  ```
- [ ] 实现 `operator[]`（不存在则插入默认值）
- [ ] 实现 `at(key)`（不存在则抛 std::out_of_range）
- [ ] 测试：词频统计（`map<string,int>` 统计文本词频）

**完成标志：** `map[key]++` 词频统计结果正确

---

