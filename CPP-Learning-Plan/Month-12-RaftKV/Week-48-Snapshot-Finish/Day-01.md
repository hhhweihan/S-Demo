## Day 1（Mon）— 快照设计

**预计时间：1 小时**

**任务：**
- [ ] 理解快照的必要性：如果日志无限增长，重启后 apply 所有日志会很慢
- [ ] 快照格式：
  ```
  Snapshot {
      last_included_index: int64  ← 快照包含到哪条日志
      last_included_term: int64
      data: bytes                  ← 状态机序列化（KV 所有数据）
  }
  ```
- [ ] 触发时机：当日志条数超过阈值（如 1000 条）时触发快照
- [ ] 设计 `StateMachine::snapshot()` 接口：返回当前状态的序列化数据
- [ ] 设计 `StateMachine::restore(snapshot)` 接口：从快照恢复

**完成标志：** 接口设计完整，理解快照与日志的关系

---