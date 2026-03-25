## Day 4（Thu）— ABA 问题防御

**预计时间：1 小时**

**任务：**
- [ ] 理解带标记指针（tagged pointer）原理：
  ```cpp
  // 将指针和版本号打包成一个 64 位整数
  struct TaggedPtr {
      Node* ptr;
      uintptr_t tag;
  };
  std::atomic<TaggedPtr> head_;
  // push 时：tag 不变
  // pop 时：tag++，防止 ABA（即使地址相同，tag 不同，CAS 失败）
  ```
- [ ] 在 `LockFreeStack` 里加入 tagged pointer（需要平台支持 16 字节原子操作，或用 low bits）
- [ ] 分析：SPSC 队列为什么不需要处理 ABA？（提示：head 只有消费者写，tail 只有生产者写）

**完成标志：** 能书面解释 SPSC 为何天然 ABA 安全

---