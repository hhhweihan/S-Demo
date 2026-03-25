## Day 2（Tue）— 有锁阻塞队列

**预计时间：1 小时**

**任务：**
- [ ] 创建 `concurrent/blocking_queue.h`
- [ ] 实现 `BlockingQueue<T>`：
  ```cpp
  template<typename T>
  class BlockingQueue {
      std::queue<T> queue_;
      std::mutex mtx_;
      std::condition_variable cv_;
  public:
      void push(T value);        // 加锁，push，notify_one
      T pop();                   // 加锁，等待非空，pop 并返回
      bool try_pop(T& out);      // 非阻塞，失败返回 false
      size_t size() const;
      bool empty() const;
      void shutdown();           // 标记关闭，所有阻塞的 pop 返回
  };
  ```
- [ ] 注意 `pop()` 里 condition_variable 要用 while 判断（防止 spurious wakeup）：
  ```cpp
  std::unique_lock<std::mutex> lock(mtx_);
  cv_.wait(lock, [this]{ return !queue_.empty() || shutdown_; });
  ```

**完成标志：** 基本 push/pop 正常，shutdown 后阻塞的 pop 能返回

---