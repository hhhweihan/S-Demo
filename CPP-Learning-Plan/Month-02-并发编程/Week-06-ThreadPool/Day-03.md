## Day 3（Wed）— 优雅关闭

**预计时间：1 小时**

**任务：**
- [ ] 实现 `shutdown()`：
  - 标记 `running_ = false`
  - 往队列里放 N 个哨兵任务（nullptr）让每个 worker 退出
  - join 所有 worker（JoiningThread 析构自动 join）
- [ ] 实现 `await_termination()`：等待队列里当前任务全部执行完，再关闭
- [ ] 测试：提交 1000 个任务，立刻调用 `await_termination()`，验证所有任务都执行完

**完成标志：** await_termination 后 1000 个任务全部完成

---