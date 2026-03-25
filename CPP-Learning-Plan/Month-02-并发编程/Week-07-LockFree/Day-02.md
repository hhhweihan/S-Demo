## Day 2（Tue）— 无锁栈

**预计时间：1 小时**

**任务：**
- [ ] 实现单线程版无锁栈 `LockFreeStack<T>`（先不处理 ABA）：
  ```cpp
  template<typename T>
  class LockFreeStack {
      struct Node { T data; Node* next; };
      std::atomic<Node*> head_{nullptr};
  public:
      void push(T val) {
          Node* node = new Node{val, head_.load()};
          while (!head_.compare_exchange_weak(node->next, node));
      }
      bool pop(T& out) {
          Node* old = head_.load();
          while (old && !head_.compare_exchange_weak(old, old->next));
          if (!old) return false;
          out = old->data;
          delete old;  // 注意：这里有 ABA 问题，下一步处理
          return true;
      }
  };
  ```
- [ ] 触发 ABA 问题：写一个手动构造 ABA 的测试，说明为什么 `delete old` 是危险的

**完成标志：** 能说清楚 ABA 问题是什么，并能用代码演示

---