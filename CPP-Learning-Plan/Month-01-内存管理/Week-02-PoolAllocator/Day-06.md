## Day 6（Sat）— 读 jemalloc 设计文档

**预计时间：2 小时**

**任务：**
- [ ] 搜索并阅读 "A Scalable Concurrent malloc(3) Implementation for FreeBSD"（Jason Evans，2006）
- [ ] 只读前 10 页，回答：
  1. jemalloc 用 arena 解决了什么问题？（提示：多线程竞争）
  2. jemalloc 的 size class 如何划分（比你的更精细还是更粗糙）？
  3. tcache（线程局部缓存）的作用是什么？为什么能减少锁竞争？
  4. jemalloc 和你实现的 PoolAllocator 的核心差距在哪里？

**完成标志：** 4 个问题都有答案，写在「读文档笔记」里

---