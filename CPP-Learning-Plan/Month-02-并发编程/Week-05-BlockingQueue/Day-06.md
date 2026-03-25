## Day 6（Sat）— 读 folly MPMCQueue

**预计时间：2 小时**

**任务：**
- [ ] 在 GitHub 找到 `folly/MPMCQueue.h`
- [ ] 只读前 150 行注释（不需要看实现细节），回答：
  1. MPMC 是什么意思？和你的 BlockingQueue 有什么本质区别？
  2. folly 用什么机制替代了 mutex？
  3. 为什么 MPMC 无锁队列实现复杂度远高于 SPSC？

**完成标志：** 3 个问题有答案

---