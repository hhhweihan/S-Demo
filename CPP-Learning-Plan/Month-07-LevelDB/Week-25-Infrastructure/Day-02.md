## Day 2（Tue）— 读 Arena

**预计时间：1 小时**

**任务：**
- [ ] 读 `util/arena.h` + `util/arena.cc`（共约 150 行）
- [ ] 回答：
  1. Arena 分配内存的策略是什么（整块申请、线性分配）？
  2. `AllocateAligned` 和 `Allocate` 的区别？
  3. Arena 不支持释放单个对象，这在 LevelDB 里不会造成内存浪费吗？（提示：Arena 和 MemTable 的生命周期）
- [ ] 对比你在 Month 1 写的 FixedAllocator：Arena 解决了什么不同的问题？

**完成标志：** 3 个问题有答案，能说清楚 Arena 的设计权衡

---