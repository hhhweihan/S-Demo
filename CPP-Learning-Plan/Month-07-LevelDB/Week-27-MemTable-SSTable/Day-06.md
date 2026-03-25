## Day 6（Sat）— 画 Compaction 流程图

**预计时间：2 小时**

**任务：**
- [ ] 读 `db/db_impl.cc` 的以下函数（只读，不需要理解全部细节）：
  - `MaybeScheduleCompaction()`：何时触发
  - `CompactMemTable()`：MemTable → SSTable（Minor Compaction）
  - `DoCompactionWork()`：多个 SSTable 合并（Major Compaction）
- [ ] 画出 Compaction 流程图，包含：
  - Minor Compaction 触发条件和结果
  - Major Compaction 触发条件（Level-0 文件数 / 各 Level 数据量）
  - 为什么 Level-0 和 Level-1+ 的 Compaction 策略不同？

**完成标志：** 流程图完整，能解释 Level 设计的意义

---