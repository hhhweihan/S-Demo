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

