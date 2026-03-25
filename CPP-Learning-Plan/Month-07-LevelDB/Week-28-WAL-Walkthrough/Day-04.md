## Day 4（Thu）— gdb 跟踪 Compaction

**预计时间：1 小时**

**任务：**
- [ ] 写一个程序：循环 Put 直到触发 Minor Compaction（写超过 write_buffer_size）
- [ ] 在 `CompactMemTable` 处打断点，跟踪：
  ```
  CompactMemTable → WriteLevel0Table → BuildTable → TableBuilder::Add → Block 编码
  ```
- [ ] 观察：Compaction 完成后，.ldb 文件出现在哪里

**完成标志：** 能解释 Compaction 时文件是如何生成的

---