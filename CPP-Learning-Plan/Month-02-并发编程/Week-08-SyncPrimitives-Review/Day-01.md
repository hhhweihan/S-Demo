## Day 1（Mon）— 读写锁（RWMutex）

**预计时间：1 小时**

**任务：**
- [ ] 创建 `concurrent/rw_mutex.h`
- [ ] 实现 `RWMutex`：
  ```cpp
  // 规则：多个读者可以并发；写者独占（读写互斥，写写互斥）
  class RWMutex {
      std::mutex mtx_;
      std::condition_variable cv_;
      int readers_ = 0;
      bool writing_ = false;
  public:
      void lock_read();    // 等待 !writing_，然后 ++readers_
      void unlock_read();  // --readers_，如果归零 notify_all
      void lock_write();   // 等待 !writing_ && readers_==0
      void unlock_write(); // writing_=false，notify_all
  };
  ```
- [ ] 测试：4 个读线程 + 1 个写线程并发运行，用 TSan 验证无数据竞争

**完成标志：** TSan 无报错，读写互斥逻辑正确

---