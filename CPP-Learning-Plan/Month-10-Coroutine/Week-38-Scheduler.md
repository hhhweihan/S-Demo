# Week 38 — 协程调度器

## 本周目标

实现协程调度器：管理多个 Fiber 的运行队列，支持 yield/resume 语义，
扩展到多线程协程池。

---

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

## Day 2（Tue）— 协程间通信（回调 + 唤醒）

**预计时间：1 小时**

**任务：**
- [ ] 实现协程的等待/唤醒机制：
  ```cpp
  // Fiber 等待某个条件：
  scheduler.suspend_current([&](Fiber* f) {
      // 条件满足时调用 f->resume() 唤醒
      register_callback([f]{ scheduler.resume(f); });
  });
  ```
- [ ] 实现 `Fiber::await(future)`：等待一个 future 完成（避免阻塞线程）
- [ ] 测试：协程 A 等待协程 B 完成，B 完成后通知 A

**完成标志：** 协程间同步通知正确

---

## Day 3（Wed）— 工作窃取调度器（多线程）

**预计时间：1 小时**

**任务：**
- [ ] 实现多线程调度器（每个线程一个本地队列 + 全局队列）：
  ```cpp
  class WorkStealingScheduler {
      int num_threads_;
      vector<deque<Fiber*>> local_queues_;  // 每线程本地队列
      deque<Fiber*> global_queue_;          // 全局队列（溢出时放这）
  public:
      void spawn(function<void()> fn);
      // 各线程执行 worker_loop()
      // 本地队列空时，从其他线程窃取
  };
  ```
- [ ] 实现工作窃取（steal from the back of other thread's deque）
- [ ] 测试：10 万个轻量协程，分配到 4 个线程执行

**完成标志：** 4 线程负载均衡，总时间接近单线程的 1/4

---

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

## Day 5（Fri）— 调度器性能测试

**预计时间：1 小时**

**任务：**
- [ ] 压测：10 万个协程各 yield 10 次（总 100 万次切换）：
  ```
  单线程调度器耗时：__ ms
  4线程工作窃取调度器耗时：__ ms
  ```
- [ ] 对比：协程池 vs 线程池执行相同任务的耗时和内存占用

**完成标志：** 协程池内存占用远低于同等线程池（10万协程 vs 10万线程）

---

## Day 6（Sat）— 阅读 goroutine 调度器设计

**预计时间：2 小时**

**任务：**
- [ ] 阅读 Go 调度器 GMP 模型文档（`golang.org/s/go11sched`）
- [ ] 回答：
  1. G/M/P 分别代表什么，三者关系如何？
  2. Go 如何实现协程的抢占（信号机制，`SIGURG`）？
  3. Go 的工作窃取是从 runq 头部还是尾部窃取，为什么？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理调度器设计的关键决策（本地队列大小、工作窃取策略）
- [ ] `git commit`，推送
