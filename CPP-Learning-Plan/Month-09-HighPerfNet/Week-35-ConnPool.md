# Week 35 — 连接池 + 负载均衡

## 本周目标

实现 TCP 连接池（预建立 + 复用），配合简单负载均衡策略，
作为 RPC 框架（Month 11）的客户端底层。

---

## Day 1（Mon）— 连接池基础设计

**预计时间：1 小时**

**任务：**
- [ ] 设计连接池接口：
  ```cpp
  class TcpConnPool {
  public:
      TcpConnPool(const string& host, int port,
                  int min_size = 4, int max_size = 32);
      // 获取连接（没有空闲则新建，超过 max 则阻塞等待）
      shared_ptr<TcpConn> acquire(int timeout_ms = 1000);
      // 归还连接（正常归还 or 连接异常时销毁）
      void release(shared_ptr<TcpConn> conn, bool healthy = true);
  private:
      deque<shared_ptr<TcpConn>> idle_;
      mutex mu_;
      condition_variable cv_;
      int active_count_ = 0;
  };
  ```
- [ ] 实现 `acquire` 和 `release` 基础版（单后端）
- [ ] 测试：4 个工作线程共享连接池，并发请求不报错

**完成标志：** 连接池基本复用功能正常

---

## Day 2（Tue）— 连接健康检查

**预计时间：1 小时**

**任务：**
- [ ] 实现后台健康检测线程：
  - 定期对空闲连接发 PING（或 TCP keepalive）
  - 检测到断开则从池中移除
- [ ] 实现连接重建：空闲连接不足 min_size 时自动补充
- [ ] 处理 `acquire` 拿到已断开连接的情况（使用前验活）

**完成标志：** 服务端重启后连接池自动重连

---

## Day 3（Wed）— 多后端负载均衡

**预计时间：1 小时**

**任务：**
- [ ] 实现 `LoadBalancer`，持有多个后端的连接池：
  ```cpp
  class LoadBalancer {
      vector<unique_ptr<TcpConnPool>> backends_;
      atomic<int> rr_counter_ = 0;  // Round Robin 计数器
  public:
      shared_ptr<TcpConn> acquire();  // 按策略选一个后端
  };
  ```
- [ ] 实现三种策略：
  - **Round Robin**：轮流选取
  - **Least Connections**：选当前活跃连接数最少的后端
  - **Random**：随机选取
- [ ] 测试：3 个后端，Round Robin 负载分布是否均匀

**完成标志：** 三种策略均可切换，负载分布符合预期

---

## Day 4（Thu）— 后端故障摘除

**预计时间：1 小时**

**任务：**
- [ ] 实现熔断机制（Circuit Breaker）：
  - 后端连续失败 5 次 → 标记为不可用（OPEN 状态）
  - OPEN 状态持续 30 秒 → 进入 HALF-OPEN（试探）
  - 试探成功 → 恢复 CLOSED 状态
  ```cpp
  enum class CircuitState { CLOSED, OPEN, HALF_OPEN };
  ```
- [ ] 健康检查通过后自动将后端加回负载均衡
- [ ] 测试：模拟一个后端宕机，验证流量切走 + 恢复

**完成标志：** 熔断机制状态转移正确

---

## Day 5（Fri）— 连接池压测

**预计时间：1 小时**

**任务：**
- [ ] 压测：8 线程 × 各 1 万次请求，连接池大小 = 16
  ```
  无连接池（每次新建 TCP）耗时：__ ms
  有连接池（复用）耗时：         __ ms
  提升倍数：                      __x
  ```
- [ ] 分析：连接池收益主要来自减少哪个开销（TCP 握手 + 内核资源分配）

**完成标志：** 连接池比每次新建快 3 倍以上

---

## Day 6（Sat）— 阅读 gRPC 连接池设计

**预计时间：2 小时**

**任务：**
- [ ] 阅读 gRPC `src/core/client_channel/` 的连接管理部分
- [ ] 回答：
  1. gRPC 的 SubChannel 和 ConnectedSubChannel 分别管理什么？
  2. gRPC 如何实现 `pick_first` 和 `round_robin` 负载均衡策略？
  3. gRPC 的 backoff 重连策略是什么（指数退避）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理连接池 + 负载均衡的设计决策
- [ ] `git commit`，推送
