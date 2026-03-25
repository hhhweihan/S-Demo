## Day 3（Wed）— 时间轮实现 + 取消

**预计时间：1 小时**

**任务：**
- [ ] 完整实现 `TimingWheel`：
  - `add(delay_ms, cb)` → 计算槽位 `(current + delay/tick) % slots_` 和轮次
  - `tick()` → 遍历当前槽，轮次为 0 执行，>0 减 1
  - `cancel(id)` → 从槽链表中移除（用 id 查找）
- [ ] 性能对比：添加/取消 100 万定时器，时间轮 vs 最小堆耗时

**完成标志：** 时间轮 add/cancel 均为 O(1)

---

