# Week 33 — 定时器轮 + 心跳检测

## 本周目标

实现时间轮（Timing Wheel）定时器，集成到 Reactor 事件循环中，
用于连接心跳检测和超时清理。

---

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

## Day 4（Thu）— 心跳检测集成

**预计时间：1 小时**

**任务：**
- [ ] 给每个 TCP 连接添加活跃时间戳，接收到数据时更新
- [ ] 用时间轮实现空闲连接检测（60 秒无数据则关闭）：
  ```cpp
  // 连接建立时：
  timer_id_ = timer_wheel_.add(60000, [conn_wp = weak_ptr(conn)](){
      if (auto conn = conn_wp.lock()) conn->close();
  });
  // 收到数据时：
  timer_wheel_.cancel(timer_id_);
  timer_id_ = timer_wheel_.add(60000, ...);  // 重置
  ```
- [ ] 测试：建立 1000 个连接，60 秒不发数据后全部被清理

**完成标志：** 空闲连接在 60 秒内被清理，活跃连接不被清理

---

## Day 5（Fri）— 层级时间轮（可选挑战）

**预计时间：1 小时**

**任务：**
- [ ] 了解层级时间轮（Linux 内核方案）：tv1（256槽,1ms) / tv2（64槽,256ms) / ...
- [ ] 分析：单层时间轮的槽数 vs 精度的权衡
- [ ] 实现 2 层时间轮（精度 1ms，范围 65 秒）

**完成标志：** 2 层时间轮精度和范围满足需求

---

## Day 6（Sat）— 阅读 Netty HashedWheelTimer 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 Netty Java 版 `HashedWheelTimer`（理解设计思路，语言无关）
- [ ] 回答：
  1. Netty 为什么使用单独工作线程而不是集成在 EventLoop 中执行定时器？
  2. Netty 的 `pendingTimeouts` 计数器有什么用（防止内存泄漏）？
  3. Linux 内核 timer wheel 的 tv1-tv5 分别管理什么范围的定时器？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理定时器实现方案对比表（最小堆/时间轮/层级时间轮）
- [ ] `git commit`，推送
