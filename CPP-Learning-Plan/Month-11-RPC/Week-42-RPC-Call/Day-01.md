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

