# Week 18 — set / map（基于 RBTree）

## 本周目标

在 Week 17 红黑树的基础上，封装 `MySet`、`MyMultiSet`、`MyMap`、`MyMultiMap`，
达到与 `std::map` / `std::set` 接口兼容。

---

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

## Day 3（Wed）— MultiSet + MultiMap

**预计时间：1 小时**

**任务：**
- [ ] 实现 `multiset`：允许重复 key，insert 总是成功（返回 iterator）：
  - RBTree 的 `insert_equal` 版本：遇到相同 key 也插入，不去重
- [ ] 实现 `multimap`：允许重复 key
- [ ] 实现 `count(key)` → 返回 key 的个数
- [ ] 实现 `equal_range(key)` → `{lower_bound(key), upper_bound(key)}`
- [ ] 测试：插入重复元素，验证 count 和 equal_range

**完成标志：** multiset::count(k) 返回正确重复数

---

## Day 4（Thu）— 迭代器与 range-for 支持

**预计时间：1 小时**

**任务：**
- [ ] 确认 set/map 的 iterator 满足双向迭代器要求（`--it` 正确）
- [ ] 验证 range-for 遍历输出有序：
  ```cpp
  for (auto& [k, v] : mymap) { ... }  // C++17 结构化绑定
  ```
- [ ] 实现 `rbegin() / rend()`（反向迭代器）：
  ```cpp
  using reverse_iterator = std::reverse_iterator<iterator>;
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  ```
- [ ] 测试：正向 + 反向遍历均有序

**完成标志：** 反向遍历输出逆序，range-for 输出正序

---

## Day 5（Fri）— lower_bound + upper_bound 完整测试

**预计时间：1 小时**

**任务：**
- [ ] 对 MyMap 运行与 Week 17 Day 5 相同的 lower/upper_bound 测试
- [ ] 额外测试场景：
  ```cpp
  // 区间查询：找出所有 key 在 [lo, hi] 范围内的元素
  auto it = mymap.lower_bound(lo);
  auto end = mymap.upper_bound(hi);
  for (; it != end; ++it) { ... }
  ```
- [ ] 对比 MyMap 和 std::map 的区间查询性能（10 万次查询）

**完成标志：** 区间查询结果与 std::map 一致，性能对比数据已记录

---

## Day 6（Sat）— 读 libc++ map 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 libc++ `include/map`（重点：`__map_iterator`、`operator[]`、`insert`）
- [ ] 回答：
  1. 为什么 `map::iterator` 的 `value_type` 是 `pair<const Key, Value>` 而不是 `pair<Key, Value>`？
  2. `operator[]` 为什么会使 `map` 不能是 const 的？
  3. libc++ 如何实现 `hint` 插入（`insert(hint, value)`）来达到均摊 O(1)？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理 set/map/multiset/multimap 的接口差异表
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**性能对比（10 万次区间查询）：**
| 容器 | 我的实现 | std 实现 | 差距 |
|------|---------|---------|------|
| map lower_bound | — | — | — |
