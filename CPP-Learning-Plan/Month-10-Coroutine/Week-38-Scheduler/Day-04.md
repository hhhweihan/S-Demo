## Day 4（Thu）— 协程优先级

**预计时间：1 小时**

**任务：**
- [ ] 为 Fiber 添加优先级（高优先级先执行）：
  ```cpp
  enum class Priority { LOW = 0, NORMAL = 1, HIGH = 2 };
  // 使用优先队列：priority_queue<Fiber*, vector, PriorityComp>
  ```
- [ ] 实现抢占式调度（某些场景下高优先级可以打断低优先级）
- [ ] 测试：高优先级协程先于低优先级完成

**完成标志：** 优先级调度符合预期

---

