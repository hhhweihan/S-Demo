## Day 1（Mon）— 无缓冲 Channel

**预计时间：1 小时**

**任务：**
- [ ] 实现无缓冲 Channel（同步握手，发送方和接收方都挂起等待对方）：
  ```cpp
  template<typename T>
  class Channel {
      queue<T>   buf_;
      int        cap_;     // 0 = 无缓冲
      deque<Fiber*> send_waiters_;
      deque<Fiber*> recv_waiters_;
      mutex mu_;
  public:
      void send(T val);    // 发送（可能挂起当前协程）
      T    recv();         // 接收（可能挂起当前协程）
      bool try_send(T val);
      bool try_recv(T& val);
      void close();
  };
  ```
- [ ] 测试：两个协程通过无缓冲 Channel 传递消息（生产者消费者）

**完成标志：** 无缓冲 Channel 的握手语义正确（先 send 者等 recv，反之亦然）

---

