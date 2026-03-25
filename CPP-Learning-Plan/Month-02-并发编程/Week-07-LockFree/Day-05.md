## Day 5（Fri）— 性能对比

**预计时间：1 小时**

**任务：**
- [ ] 单生产者单消费者场景，交换 100 万个 int，对比：
  ```
  SPSCQueue（无锁）
  BlockingQueue（有锁）
  BoundedQueue（有界有锁）
  ```
- [ ] 预期：SPSCQueue 比 BlockingQueue 快 5-10x
- [ ] 分析原因（写在「本周结果」）

**完成标志：** SPSCQueue 至少快 3x，有原因分析

---