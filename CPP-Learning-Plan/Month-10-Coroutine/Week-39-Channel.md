# Week 39 — Channel 通信

## 本周目标

实现 Go 风格的 Channel（无缓冲 + 有缓冲），以及 select 多路等待，
作为协程间通信的核心原语。

---

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

## Day 2（Tue）— 有缓冲 Channel

**预计时间：1 小时**

**任务：**
- [ ] 实现有缓冲 Channel（缓冲未满时 send 不阻塞，缓冲非空时 recv 不阻塞）：
  - 内部用环形缓冲区（避免 deque 的额外分配）
- [ ] 实现 `close()`：
  - 关闭后 send 抛异常，recv 直到缓冲耗尽后返回零值
- [ ] 测试：缓冲大小为 5，生产者比消费者快 2 倍，验证流控

**完成标志：** 缓冲 Channel 流控正确，关闭语义正确

---

## Day 3（Wed）— select 多路等待

**预计时间：1 小时**

**任务：**
- [ ] 实现 `select` 语句（等待多个 Channel 中第一个就绪的）：
  ```cpp
  // 目标用法：
  select(
      on_recv(ch1, [](int v){ ... }),
      on_recv(ch2, [](string s){ ... }),
      on_send(ch3, 42, []{ ... }),
      default_([]{ ... })  // 可选 default（非阻塞）
  );
  ```
- [ ] 实现原理：将当前 Fiber 注册到所有备选 Channel 的等待列表，
        任意一个就绪时唤醒（并从其他 Channel 取消注册）
- [ ] 测试：select 从两个 Channel 中接收，先就绪的优先

**完成标志：** select 语义正确，无死锁

---

## Day 4（Thu）— 扇入 + 扇出模式

**预计时间：1 小时**

**任务：**
- [ ] 实现 `merge(channels...)` → 扇入（将多个 Channel 合并为一个）：
  ```cpp
  Channel<T> merged = merge(ch1, ch2, ch3);
  // 任意源 Channel 有数据时转发到 merged
  ```
- [ ] 实现 `broadcast(ch, n)` → 扇出（将一个 Channel 分发给 n 个消费者）
- [ ] 测试：pipeline 模式（生产 → 过滤 → 转换 → 消费，各阶段用 Channel 连接）

**完成标志：** pipeline 模式端到端数据流正确

---

## Day 5（Fri）— Channel 性能测试

**预计时间：1 小时**

**任务：**
- [ ] 对比 Channel 与传统同步原语的吞吐量：
  ```
  测试：1 生产者 + 1 消费者，传递 100 万条消息
  Channel（无缓冲）耗时：__ ms
  Channel（缓冲=64）耗时：__ ms
  mutex + queue 耗时：    __ ms
  ```
- [ ] 分析：有缓冲 Channel 为什么比无缓冲快（减少协程切换次数）

**完成标志：** 有缓冲 Channel 比 mutex+queue 快（或相近）

---

## Day 6（Sat）— 阅读 Go Channel 实现

**预计时间：2 小时**

**任务：**
- [ ] 阅读 Go runtime `chan.go`（重点：`hchan` 结构、sendq/recvq 等待队列）
- [ ] 回答：
  1. Go Channel 的环形缓冲区 `buf` 如何实现（指针+索引，避免内存移动）？
  2. 为什么 Go Channel 发送时先尝试从 `recvq` 直接拷贝（fast path）？
  3. `select` 语句在多个 case 都就绪时如何保证公平性（随机顺序）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理 Channel 的各种使用模式（生产消费、pipeline、扇入扇出）
- [ ] `git commit`，推送
