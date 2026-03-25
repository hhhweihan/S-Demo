## Day 6（Sat）— 阅读 libco / sylar 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 sylar（C++ 协程框架）的 `scheduler.cc` 和 `iomanager.cc`
- [ ] 回答：
  1. sylar 如何将 IOManager 与 Scheduler 结合（EventLoop + 协程）？
  2. 为什么 hook sleep 后的协程比线程池 sleep 内存效率高 100 倍以上？
  3. 协程与 C++20 coroutine（co_await）的核心区别是什么（有栈 vs 无栈）？

**完成标志：** 3 个问题有答案

---

