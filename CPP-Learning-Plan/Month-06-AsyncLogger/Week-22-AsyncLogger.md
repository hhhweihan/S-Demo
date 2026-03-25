# Week 22 — 双缓冲异步日志

## 本周目标

实现双缓冲（Double Buffering）异步日志，业务线程写前台缓冲，后台线程将满缓冲
刷到磁盘，消除同步日志的锁竞争和 IO 阻塞。

---

## Day 1（Mon）— 双缓冲设计

**预计时间：1 小时**

**任务：**
- [ ] 理解双缓冲原理：
  ```
  前台：current_buffer_（业务线程写）
        next_buffer_（备用，current 满后立刻切换）
  后台线程：每 3 秒（或 current 满时被唤醒）
           交换 buffers，批量写入文件
  ```
- [ ] 定义 `Buffer`（固定大小，如 4MB）：
  ```cpp
  class Buffer {
      static const int kSize = 4 * 1024 * 1024;
      char data_[kSize];
      int  used_ = 0;
  public:
      bool full() const { return used_ >= kSize; }
      void append(const char* s, int len);
      void reset() { used_ = 0; }
  };
  ```
- [ ] 设计 `AsyncLogger` 的成员：2 个当前缓冲区 + 待写队列 + 后台线程

**完成标志：** 能画出双缓冲的数据流图

---

## Day 2（Tue）— 前台写入逻辑

**预计时间：1 小时**

**任务：**
- [ ] 实现业务线程的 `append(msg)` 方法：
  ```cpp
  void AsyncLogger::append(const char* msg, int len) {
      std::lock_guard lock(mu_);
      if (current_->avail() >= len) {
          current_->append(msg, len);
      } else {
          buffers_.push_back(std::move(current_));
          current_ = std::move(next_);      // 切换到备用缓冲
          if (!current_) current_ = std::make_unique<Buffer>();
          current_->append(msg, len);
          cv_.notify_one();  // 通知后台线程
      }
  }
  ```
- [ ] 关键：`mu_` 只保护指针交换，不保护 IO，锁粒度极小
- [ ] 测试：单线程验证缓冲切换时机

**完成标志：** 缓冲切换逻辑正确，无死锁

---

## Day 3（Wed）— 后台线程写入逻辑

**预计时间：1 小时**

**任务：**
- [ ] 实现后台线程 `backend_thread()`：
  ```cpp
  void AsyncLogger::backend_thread() {
      while (running_) {
          {
              std::unique_lock lock(mu_);
              cv_.wait_for(lock, 3s, [this]{ return !buffers_.empty(); });
              buffers_.push_back(std::move(current_));
              current_ = std::make_unique<Buffer>();
          }
          // 在锁外批量写入文件（IO 不持锁）
          for (auto& buf : buffers_to_write)
              file_.write(buf->data(), buf->used());
          file_.flush();
          buffers_to_write.clear();
      }
  }
  ```
- [ ] 测试：前台 4 线程连续写，后台线程正确 flush

**完成标志：** 无日志丢失，后台线程定时 flush 有效

---

## Day 4（Thu）— 优雅关闭 + 边界处理

**预计时间：1 小时**

**任务：**
- [ ] 实现 `stop()`：设置 `running_ = false`，flush 剩余缓冲后退出后台线程
- [ ] 边界处理：
  - 前台产生日志速度远超后台 flush 速度时（丢弃策略 or 阻塞策略）
  - 日志库初始化前的日志（静态对象初始化顺序）
- [ ] 测试：程序退出时最后一批日志不丢失

**完成标志：** 程序退出后日志文件内容完整

---

## Day 5（Fri）— 性能压测（与同步日志对比）

**预计时间：1 小时**

**任务：**
- [ ] 压测：4 线程各 25 万条日志（总 100 万条）：
  ```
  同步日志耗时：  __ ms（Week 21 基准）
  异步日志耗时：  __ ms
  提升倍数：      __x
  ```
- [ ] 压测：极端情况下（每条日志只有 1 字节）的吞吐量
- [ ] 分析：双缓冲异步日志的延迟分布（P99、P999）

**完成标志：** 异步日志吞吐量 > 同步日志 5 倍以上

---

## Day 6（Sat）— 阅读 muduo AsyncLogging 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 muduo `base/AsyncLogging.cc`
- [ ] 回答：
  1. muduo 为什么准备两个 `nextBuffer_` 而不是一个（减少内存分配）？
  2. muduo 后台线程 `swap` 缓冲列表时为什么还要检查 `buffersToWrite` 超过 25 个（防内存爆）？
  3. 双缓冲与生产者-消费者队列的本质区别是什么（批量 vs 逐条）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理双缓冲异步日志的时序图
- [ ] `git commit`，推送
