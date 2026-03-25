## Day 1（Mon）— 定时器最小堆方案（基准）

**预计时间：1 小时**

**任务：**
- [ ] 先实现最小堆定时器（作为对比基准）：
  ```cpp
  struct Timer {
      int64_t expiry;      // 到期时间（毫秒）
      function<void()> cb; // 回调
      int id;
  };
  class TimerHeap {
      priority_queue<Timer, vector<Timer>, greater<>> heap_;
  public:
      int  add(int64_t delay_ms, function<void()> cb);
      void cancel(int id);
      int  poll(int64_t now_ms);  // 执行到期定时器，返回下次最近到期时间
  };
  ```
- [ ] 集成到 EventLoop（用 timerfd_create 实现）
- [ ] 测试：添加 100 个定时器，验证到期顺序

**完成标志：** 定时器到期回调按时触发，cancel 有效

---

