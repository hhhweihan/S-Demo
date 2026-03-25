## Day 5（Fri）— 压测：同步 vs 异步 RPC

**预计时间：1 小时**

**任务：**
- [ ] 压测（Echo 接口，消息大小 100B）：
  ```
  同步 RPC（每线程串行）：__ QPS（8 线程）
  异步 RPC（Future+callback）：__ QPS
  协程 RPC（Month 10 协程池）：__ QPS
  ```
- [ ] 分析：为什么异步 RPC 能在更少的线程下达到更高 QPS

**完成标志：** 异步 RPC QPS > 同步 RPC 3 倍以上（相同线程数）

---

