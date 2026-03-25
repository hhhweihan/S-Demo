## Day 1（Mon）— 单线程调度器

**预计时间：1 小时**

**任务：**
- [ ] 实现 `Scheduler`（单线程版本）：
  ```cpp
  class Scheduler {
      deque<shared_ptr<Fiber>> ready_queue_;
      shared_ptr<Fiber> current_fiber_;
  public:
      void spawn(function<void()> fn);  // 创建新协程并加入就绪队列
      void yield();                     // 当前协程让出，切到下一个就绪协程
      void run();                       // 开始调度（循环直到所有协程完成）
  private:
      void schedule_next();             // 选择下一个协程执行
  };
  ```
- [ ] 测试：spawn 10 个协程，每个 yield 3 次，验证调度顺序

**完成标志：** 所有协程正确完成，调度顺序符合 Round Robin

---

