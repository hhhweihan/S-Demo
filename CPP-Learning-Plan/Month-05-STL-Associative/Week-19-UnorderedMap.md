# Week 19 — unordered_map / unordered_set（开链哈希表）

## 本周目标

从零实现开链哈希表，封装 `MyUnorderedMap` 和 `MyUnorderedSet`，
理解负载因子、rehash 时机和哈希碰撞处理。

---

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

## Day 2（Tue）— rehash + 负载因子

**预计时间：1 小时**

**任务：**
- [ ] 实现 `load_factor()` = `size_ / buckets_.size()`
- [ ] 实现 `rehash(new_bucket_count)`：
  ```cpp
  void rehash(size_t new_n) {
      // 新桶数组 ≥ new_n，且是质数（选质数减少碰撞）
      std::vector<Node*> new_buckets(next_prime(new_n), nullptr);
      // 将所有节点重新挂到新桶
      for (auto* head : buckets_)
          while (head) { ... }  // 逐节点迁移
      buckets_ = std::move(new_buckets);
  }
  ```
- [ ] 在 `insert` 中自动触发 rehash（当 load_factor > max_load_factor_）
- [ ] 实现质数序列（28 个质数：53, 97, 193, ... 用于桶数选取）
- [ ] 测试：插入 1000 元素，观察 rehash 触发次数

**完成标志：** load_factor 始终不超过 0.75

---

## Day 3（Wed）— find / erase / 迭代器

**预计时间：1 小时**

**任务：**
- [ ] 实现 `find(key)` → 遍历对应桶的链表，O(1) 平均
- [ ] 实现 `erase(key)` → 找到节点并从链表摘除
- [ ] 实现前向迭代器（遍历所有桶的所有节点）：
  ```cpp
  struct iterator {
      Node*   cur_;    // 当前节点
      size_t  bucket_; // 当前桶索引
      HashTable* ht_;  // 回指哈希表（跨桶时需要跳到下一个非空桶）
      iterator& operator++() {
          cur_ = cur_->next;
          if (!cur_) advance_bucket();  // 跳到下一个非空桶
          return *this;
      }
  };
  ```
- [ ] 测试：range-for 遍历所有元素

**完成标志：** 遍历输出与插入元素集合一致（无序）

---

## Day 4（Thu）— MyUnorderedMap + MyUnorderedSet 封装

**预计时间：1 小时**

**任务：**
- [ ] `unordered_map<K,V>`：在 HashTable<K,V> 上封装 `operator[]` / `at()` / `count()`
- [ ] `unordered_set<K>`：HashTable<K, void> 特化，只存 key
- [ ] 实现 `bucket_count()` / `bucket_size(b)` / `max_load_factor(f)`
- [ ] 测试：`unordered_map<string,int>` 词频统计，与 Week 18 的 map 版本对比性能

**完成标志：** 词频统计正确，unordered_map 比 map 快（O(1) vs O(log n)）

---

## Day 5（Fri）— 自定义 Hash + 性能测试

**预计时间：1 小时**

**任务：**
- [ ] 为自定义类型提供 Hash：
  ```cpp
  struct Point { int x, y; };
  struct PointHash {
      size_t operator()(const Point& p) const {
          return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 32);
      }
  };
  MyUnorderedMap<Point, int, PointHash> map;
  ```
- [ ] 性能测试（各 100 万次 insert + find）：
  - `MyUnorderedMap` vs `std::unordered_map`
  - `MyMap`（红黑树）vs `MyUnorderedMap`（哈希）

**完成标志：** 性能数据已记录，unordered 在无序场景下更快

---

## Day 6（Sat）— 读 libc++ unordered_map 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 libc++ `include/__hash_table`（重点：`__bucket_list_`、rehash 策略、iterator）
- [ ] 回答：
  1. libc++ 使用 forward_list 而非普通链表存储桶内节点，为什么？
  2. libc++ 如何保证 `end()` 迭代器的合法性（所有桶用一个链表串联？）？
  3. 为什么哈希表的桶数选质数可以减少碰撞？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 对比红黑树（有序）与哈希表（无序）的使用场景
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**性能对比（100 万次操作）：**
| 操作 | MyMap | MyUnorderedMap | std::unordered_map |
|------|-------|---------------|-------------------|
| insert | — | — | — |
| find   | — | — | — |
