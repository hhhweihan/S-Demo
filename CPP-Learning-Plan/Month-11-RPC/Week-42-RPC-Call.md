# Week 42 — 同步 + 异步 RPC

## 本周目标

在基础 RPC 框架上添加异步调用（Future/Callback）、超时、取消，
实现多路复用（单连接处理并发请求）。

---

## Day 1（Mon）— 多路复用 + 异步响应匹配

**预计时间：1 小时**

**任务：**
- [ ] 实现基于 request_id 的异步响应匹配：
  ```cpp
  class AsyncRpcClient {
      atomic<uint32_t> next_id_ = 0;
      map<uint32_t, Promise<RpcResponse>> pending_;
      mutex mu_;
  public:
      Future<RpcResponse> async_call(const string& svc, const string& method,
                                      const Message& req) {
          uint32_t id = ++next_id_;
          auto [fut, prom] = make_future_promise<RpcResponse>();
          {
              lock_guard lock(mu_);
              pending_[id] = std::move(prom);
          }
          send_frame(id, svc, method, req);
          return fut;
      }
      void on_response(uint32_t id, RpcResponse resp) {
          lock_guard lock(mu_);
          if (auto it = pending_.find(id); it != pending_.end()) {
              it->second.set_value(std::move(resp));
              pending_.erase(it);
          }
      }
  };
  ```
- [ ] 测试：同一连接上并发发出 100 个请求，响应乱序到达，匹配正确

**完成标志：** 乱序响应正确匹配到对应请求

---

## Day 2（Tue）— 实现 Promise/Future

**预计时间：1 小时**

**任务：**
- [ ] 实现简单的 Future/Promise（如果还没有）：
  ```cpp
  template<typename T>
  class Future {
  public:
      T get();                     // 阻塞等待（同步使用）
      void then(function<void(T)> cb);  // 注册回调（异步使用）
      bool ready() const;
  };
  ```
- [ ] 支持两种使用方式：
  - `.get()` → 阻步等待（同步客户端）
  - `.then(cb)` → 回调（异步客户端）
- [ ] 测试：与协程结合（Month 10 的 `co_await future`）

**完成标志：** Future.get() 和 Future.then() 均正常工作

---

## Day 3（Wed）— 超时与取消

**预计时间：1 小时**

**任务：**
- [ ] 实现 RPC 超时（集成 Month 9 定时器）：
  ```cpp
  // 发送请求时同时添加超时定时器：
  timer_id = timer.add(timeout_ms, [id, &pending_]{
      auto it = pending_.find(id);
      if (it != pending_.end()) {
          it->second.set_exception(RpcTimeoutException{});
          pending_.erase(it);
      }
  });
  // 响应到达时取消定时器：
  on_response(id, resp) { timer.cancel(timer_id); ... }
  ```
- [ ] 实现 `cancel(request_id)` → 取消还未发出的请求或放弃等待
- [ ] 测试：服务端故意延迟 2 秒，客户端 1 秒超时触发

**完成标志：** 超时正确触发，无资源泄漏（pending_ map 清理）

---

## Day 4（Thu）— 重试机制

**预计时间：1 小时**

**任务：**
- [ ] 实现自动重试（幂等接口）：
  ```cpp
  // 重试策略：最多 3 次，指数退避（1s, 2s, 4s）
  class RetryPolicy {
      int max_retries_ = 3;
      int base_ms_ = 100;
  public:
      bool should_retry(int attempt, const RpcStatus& status);
      int  backoff_ms(int attempt);  // 100ms * 2^attempt
  };
  ```
- [ ] 只重试幂等错误（网络超时、连接断开），不重试逻辑错误
- [ ] 测试：服务端随机拒绝 30% 的请求，重试后成功率 > 99%

**完成标志：** 重试机制工作，日志记录重试次数

---

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

## Day 6（Sat）— 阅读 muduo RPC 示例

**预计时间：2 小时**

**任务：**
- [ ] 阅读 muduo `examples/protobuf/rpc/`（重点：RpcChannel、RpcServer）
- [ ] 回答：
  1. muduo 的 `RpcChannel::CallMethod` 如何将 protobuf Service 调用转为 TCP 发送？
  2. muduo 如何用 `google::protobuf::RpcController` 传递错误信息？
  3. 多路复用 vs 连接池：什么时候用多路复用更好（低延迟 vs 高隔离性）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理 RPC 调用链路图（Client → Codec → Transport → Server → Handler）
- [ ] `git commit`，推送
