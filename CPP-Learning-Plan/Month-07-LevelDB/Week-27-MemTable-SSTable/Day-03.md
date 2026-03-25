## Day 3（Wed）— 读 Block 内部编码

**预计时间：1 小时**

**任务：**
- [ ] 读 `table/block.h` + `table/block.cc`（约 200 行）
- [ ] 理解「前缀压缩」（prefix compression）：
  ```
  // 相邻 key 共享前缀，只记录差异部分
  // "hello world"  → 完整记录
  // "hello you"    → shared=6, non_shared="you", value=...
  ```
- [ ] 理解 restart point：每 16 个 key 设一个 restart point（完整记录），用于二分查找
- [ ] 回答：为什么不全用前缀压缩？（如果没有 restart point，无法从中间开始二分查找）

**完成标志：** 能解释 restart point 的必要性

---