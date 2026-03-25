## Day 1（Mon）— 跳表原理推导

**预计时间：1 小时**

**任务：**
- [ ] 从零推导跳表：
  1. 有序链表查询：O(n)，插入：O(n)
  2. 加一层"快速道"：最坏查询降为 O(n/2)
  3. 多层"快速道"：查询接近 O(log n)
- [ ] 手画一个高度 4 的跳表，模拟插入 5, 3, 8, 1, 7 的过程
- [ ] 理解「随机高度」：为什么每层以 1/4 概率晋升（LevelDB 的选择），整体高度期望是 O(log n)？
- [ ] 实现 `random_height()`：
  ```cpp
  int random_height() {
      static const int kBranching = 4;
      int height = 1;
      while (height < kMaxHeight && (rand() % kBranching == 0)) ++height;
      return height;
  }
  ```

**完成标志：** 能手绘跳表，理解随机高度的概率设计

---