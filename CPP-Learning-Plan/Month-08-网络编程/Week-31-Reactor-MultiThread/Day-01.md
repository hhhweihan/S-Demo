## Day 1（Mon）— one loop per thread

**预计时间：1 小时**

**任务：**
- [ ] 理解「one loop per thread」模型：
  - 主线程：1 个 EventLoop，只负责 accept 新连接
  - 子线程：N 个 EventLoop（EventLoopThreadPool），每个处理一组连接
  - 新连接以轮询方式分配给子线程的 EventLoop
- [ ] 实现 `EventLoopThread`：每个线程持有一个 EventLoop
- [ ] 实现 `EventLoopThreadPool`：
  ```cpp
  class EventLoopThreadPool {
      int num_threads_;
      std::vector<std::unique_ptr<EventLoopThread>> threads_;
      int next_ = 0;
  public:
      EventLoop* get_next_loop();  // 轮询返回下一个 EventLoop
  };
  ```

**完成标志：** 8 个 IO 线程能各自独立运行 EventLoop

---