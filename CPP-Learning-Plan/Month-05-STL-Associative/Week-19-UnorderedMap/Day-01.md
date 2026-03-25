## Day 1（Mon）— 哈希表基础结构

**预计时间：1 小时**

**任务：**
- [ ] 设计哈希表：桶数组 + 链表节点（开链法）：
  ```cpp
  template<typename Key, typename Value,
           typename Hash = std::hash<Key>,
           typename Equal = std::equal_to<Key>>
  class HashTable {
      struct Node { Key key; Value val; Node* next; };
      std::vector<Node*> buckets_;  // 桶数组
      size_t size_ = 0;
      float   max_load_factor_ = 0.75f;
      Hash    hash_;
      Equal   equal_;
  };
  ```
- [ ] 实现 `bucket_index(key)` → `hash_(key) % buckets_.size()`
- [ ] 实现 `insert(key, val)` 基础版（不含 rehash）
- [ ] 测试：插入 10 个元素，遍历所有桶打印

**完成标志：** 能手动验证哈希分布

---

