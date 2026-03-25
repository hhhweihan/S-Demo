## Day 1（Mon）— 基础线程池

**预计时间：1 小时**

**任务：**
- [ ] 创建 `concurrent/thread_pool.h`
- [ ] 最简版：固定 N 个 worker 线程 + `BlockingQueue<std::function<void()>>`
  ```cpp
  class ThreadPool {
      std::vector<JoiningThread> workers_;
      BlockingQueue<std::function<void()>> queue_;
      std::atomic<bool> running_{true};
  public:
      explicit ThreadPool(size_t n);   // 启动 n 个线程
      void submit(std::function<void()> task);
      void shutdown();
  };
  // worker 线程的循环：
  // while(running_) { auto task = queue_.pop(); if(task) task(); }
  ```
- [ ] 测试：提交 100 个打印任务，主线程 join，验证 100 个都执行了

**完成标志：** 100 个任务全部执行，shutdown 后线程正常退出

---