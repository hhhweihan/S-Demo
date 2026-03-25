# Week 20 — 核心 STL 算法 + allocator_traits 完整接入

## 本周目标

实现 sort / stable_sort / binary_search 算法族，并将 PoolAllocator 完整接入
Month 4-5 自编 STL，完成整月综合验收。

---

## Day 1（Mon）— sort（introsort）

**预计时间：1 小时**

**任务：**
- [ ] 实现 `my_sort(first, last, cmp)`：
  ```
  introsort = quicksort + heapsort + insertion sort
  - 递归深度 > 2*log(n)：切换 heapsort 防止最坏情况
  - 分区小于 16 元素：切换 insertion sort
  ```
- [ ] 实现三路划分 partition（处理大量重复元素）
- [ ] 测试：10 万随机数排序，与 std::sort 结果一致并对比耗时

**完成标志：** 排序结果正确，性能与 std::sort 在同一量级

---

## Day 2（Tue）— stable_sort + partial_sort

**预计时间：1 小时**

**任务：**
- [ ] 实现 `my_stable_sort(first, last, cmp)`：
  - 归并排序（额外 O(n) 空间版本）
  - 保证相等元素的原始顺序不变
- [ ] 实现 `my_partial_sort(first, middle, last)`：
  - 只把前 k 小的元素排好，其余不保证顺序
  - 用堆：`make_heap(first, middle)` 然后对 [middle, last) 各元素与堆顶比较
- [ ] 测试：stable_sort 验证稳定性，partial_sort 验证前 k 个正确

**完成标志：** stable_sort 稳定性验证通过

---

## Day 3（Wed）— binary_search 算法族

**预计时间：1 小时**

**任务：**
- [ ] 实现以下算法（均基于 lower_bound）：
  ```cpp
  lower_bound(first, last, val)   // 第一个 >= val 的位置
  upper_bound(first, last, val)   // 第一个 > val 的位置
  binary_search(first, last, val) // 是否存在
  equal_range(first, last, val)   // {lower_bound, upper_bound}
  ```
- [ ] 这些算法要求随机访问迭代器，验证在 MyVector 上工作
- [ ] 对比：在 10 万有序数组上各查找 1 万次，与 std 版本性能相同

**完成标志：** 结果与 std 算法一致

---

## Day 4（Thu）— allocator_traits 完整接入

**预计时间：1 小时**

**任务：**
- [ ] 将 Month 4-5 所有容器改为通过 `allocator_traits` 调用分配器：
  ```cpp
  // Before（直接调用 allocator）：
  alloc_.construct(ptr, args...);
  // After（通过 allocator_traits）：
  allocator_traits<Alloc>::construct(alloc_, ptr, args...);
  ```
- [ ] 接入 Month 1 的 PoolAllocator：
  ```cpp
  MyVector<int, PoolAllocator<int>> pv;
  MyList<int, PoolAllocator<int>>   pl;
  MyMap<int, int, std::less<int>, PoolAllocator<pair<const int,int>>> pm;
  ```
- [ ] 验证：所有容器用 PoolAllocator 能正常工作（ASan 无报错）

**完成标志：** PoolAllocator 在 MyVector/MyList/MyMap 上均正常

---

## Day 5（Fri）— 整月综合 benchmark

**预计时间：1 小时**

**任务：**
- [ ] 跑完整 benchmark（各 100 万次操作）：
  ```
  MyVector push_back      vs std::vector
  MyList   push_back      vs std::list
  MyMap    insert/find    vs std::map
  MyUnorderedMap insert   vs std::unordered_map
  MyVector + PoolAllocator vs MyVector + std::allocator
  ```
- [ ] 用 ASan + Valgrind（如果 Linux）检测内存泄漏
- [ ] 验证月度验收标准：
  - [ ] 红黑树 10 万数后树高 ≤ 2×log₂(n)
  - [ ] MyMap 支持 lower_bound/upper_bound
  - [ ] MyUnorderedMap 在 load_factor 0.75 自动 rehash
  - [ ] PoolAllocator 版本比默认快 20%+

**完成标志：** 所有验收标准达成，数据已记录

---

## Day 6（Sat）— 写总结文章

**预计时间：2 小时**

**任务：**
- [ ] 写「自编 STL 关联容器总结」（至少 800 字）：
  1. 红黑树 5 条性质如何保证 O(log n) 高度约束
  2. set/map 与 multiset/multimap 在 insert 实现上的区别
  3. 哈希表负载因子为什么选 0.75（空间与时间的权衡）
  4. PoolAllocator 为什么在链式容器（list、map）上收益更大

---

## Day 7（Sun）— 复盘 + 规划 Month 6

**预计时间：1 小时**

- [ ] 整理 Month 4-5 自编 STL 代码库结构，补充 README
- [ ] 规划 Month 6 异步日志系统：先思考双缓冲设计
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**Month 4-5 验收：**
| 验收项 | 结果 | 备注 |
|--------|------|------|
| 红黑树树高约束 | — | — |
| lower/upper_bound 正确性 | — | — |
| rehash 触发 | — | — |
| PoolAllocator 性能提升 | — | — |
