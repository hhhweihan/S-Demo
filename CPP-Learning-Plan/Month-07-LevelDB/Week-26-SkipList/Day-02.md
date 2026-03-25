## Day 2（Tue）— 读 skiplist.h

**预计时间：1 小时**

**任务：**
- [ ] 精读 `db/skiplist.h`（约 350 行），重点标注：
  - `Node` 结构体：`next_` 数组用 `std::atomic<Node*>`
  - `Insert(key)`：找到每一层的前驱节点，然后从底层到顶层链入
  - `Contains(key)`：逐层向右向下搜索
  - `Iterator`：前向迭代器
- [ ] 找出所有 `NoBarrier_` 前缀的操作，理解为什么可以用 `relaxed`：
  - 提示：写 SkipList 的线程只有一个（MemTable 是单写多读）

**完成标志：** 能解释每处 memory_order 选择的理由

---