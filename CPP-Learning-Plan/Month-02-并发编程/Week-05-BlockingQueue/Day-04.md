## Day 4（Thu）— 有界阻塞队列

**预计时间：1 小时**

**任务：**
- [ ] 创建 `concurrent/bounded_queue.h`
- [ ] 实现 `BoundedQueue<T>` 带容量限制：
  - 满时 `push` 阻塞（等待消费者取走）
  - 空时 `pop` 阻塞（等待生产者放入）
  - 需要两个 condition_variable：`not_full_`、`not_empty_`
- [ ] 测试：设容量为 10，1 个生产者快速 push 100 个，1 个消费者慢速 pop（sleep 1ms），
  验证生产者被阻塞（整个过程不超时/死锁）

**完成标志：** 生产者被背压阻塞，无死锁

---