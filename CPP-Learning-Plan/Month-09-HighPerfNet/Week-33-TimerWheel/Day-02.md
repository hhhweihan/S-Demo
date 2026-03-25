## Day 2（Tue）— 时间轮设计

**预计时间：1 小时**

**任务：**
- [ ] 理解时间轮原理：
  ```
  槽数组（如 3600 个槽 = 1小时，每槽 1 秒）
  指针每秒移动一格，执行当前槽的所有定时器
  超过一圈的定时器：存到轮次字段，每圈减 1，轮次为 0 时才执行
  ```
- [ ] 实现 `TimingWheel`：
  ```cpp
  class TimingWheel {
      int slots_;            // 槽数量（如 3600）
      int tick_ms_;          // 每格毫秒数（如 1000ms）
      int current_slot_ = 0;
      vector<list<TimerEntry>> wheel_;
  public:
      int  add(int delay_ms, function<void()> cb);
      void tick();  // 每个 tick 调用一次，执行当前槽定时器
  };
  ```

**完成标志：** 时间轮设计图绘制完成，能解释与最小堆的区别

---

