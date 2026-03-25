## Day 4（Thu）— 基础 RPC Server

**预计时间：1 小时**

**任务：**
- [ ] 实现 `RpcServer`（基于 Month 8 的 Reactor 框架）：
  ```cpp
  class RpcServer {
      TcpServer server_;
      map<string, ServiceBase*> services_;
  public:
      void register_service(ServiceBase* svc);
      void start(int port);
  private:
      void on_message(TcpConn* conn, Buffer& buf);  // 解帧 → 分发 → 编帧回复
  };
  ```
- [ ] 测试：用 netcat 发送手工构造的 RPC 帧，验证服务端响应

**完成标志：** RpcServer 能处理手工构造的请求

---

