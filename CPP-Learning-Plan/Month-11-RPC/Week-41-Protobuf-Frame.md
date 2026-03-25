# Week 41 — protobuf + 帧协议

## 本周目标

掌握 protobuf 序列化，设计 RPC 帧格式，实现基础的请求/响应编解码层。

---

## Day 1（Mon）— protobuf 基础

**预计时间：1 小时**

**任务：**
- [ ] 安装 protobuf（`sudo apt install protobuf-compiler libprotobuf-dev`）
- [ ] 定义第一个 proto 文件：
  ```protobuf
  syntax = "proto3";
  package rpc;

  message EchoRequest  { string message = 1; }
  message EchoResponse { string message = 1; int32 code = 2; }

  service EchoService {
      rpc Echo(EchoRequest) returns (EchoResponse);
  }
  ```
- [ ] 生成 C++ 代码并使用：
  ```cpp
  EchoRequest req;
  req.set_message("hello");
  string bytes = req.SerializeAsString();
  EchoRequest req2;
  req2.ParseFromString(bytes);
  ```
- [ ] 测试：序列化后字节大小 vs JSON

**完成标志：** protobuf 序列化/反序列化正确，理解为什么比 JSON 小

---

## Day 2（Tue）— RPC 帧格式设计

**预计时间：1 小时**

**任务：**
- [ ] 设计帧格式（长度前缀协议）：
  ```
  ┌─────────────┬────────────┬──────────┬─────────────────┐
  │ magic(2B)   │ type(1B)   │ id(4B)   │ payload_len(4B) │
  ├─────────────┴────────────┴──────────┴─────────────────┤
  │ payload (protobuf 序列化的 RpcRequest 或 RpcResponse)   │
  └─────────────────────────────────────────────────────────┘
  type: REQUEST=1, RESPONSE=2, HEARTBEAT=3
  id:   请求 ID（用于匹配异步响应）
  ```
- [ ] 实现 `Codec`：
  ```cpp
  class Codec {
  public:
      // 编码：message → 帧字节序列
      static string encode(const google::protobuf::Message& msg, uint32_t id, uint8_t type);
      // 解码：从 Buffer 中提取完整帧（处理粘包）
      static bool decode(Buffer& buf, Frame& frame);
  };
  ```
- [ ] 测试：编码后解码，字段正确

**完成标志：** 帧编解码正确，处理粘包（缓冲区中有半包时等待）

---

## Day 3（Wed）— RPC 请求/响应消息

**预计时间：1 小时**

**任务：**
- [ ] 定义 RPC 元消息：
  ```protobuf
  message RpcRequest {
      string service_name = 1;   // "EchoService"
      string method_name  = 2;   // "Echo"
      bytes  payload      = 3;   // 序列化的请求参数
  }
  message RpcResponse {
      int32  code    = 1;  // 0=OK, 非0=错误
      string errmsg  = 2;
      bytes  payload = 3;  // 序列化的响应结果
  }
  ```
- [ ] 实现服务端分发：根据 `service_name` + `method_name` 找到处理函数
- [ ] 测试：服务端注册一个 EchoService，处理 Echo 请求

**完成标志：** 服务端正确路由到 Echo 处理函数

---

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

## Day 5（Fri）— 基础 RPC Client（同步版）

**预计时间：1 小时**

**任务：**
- [ ] 实现同步 `RpcClient`：
  ```cpp
  class RpcClient {
      TcpConn conn_;
      Codec   codec_;
  public:
      RpcClient(const string& host, int port);
      template<typename Req, typename Resp>
      Resp call(const string& service, const string& method, const Req& req);
  };
  ```
- [ ] 测试：Client 调用 Server 的 Echo，端到端验证

**完成标志：** 同步 RPC 调用成功，响应内容正确

---

## Day 6（Sat）— 阅读 brpc 帧协议设计

**预计时间：2 小时**

**任务：**
- [ ] 阅读 brpc `src/brpc/protocol.h` 和 `src/brpc/baidu_rpc_protocol.cc`
- [ ] 回答：
  1. brpc 为什么支持多种协议（baidu_std、http、redis）在同一端口上（协议识别）？
  2. brpc 的 controller 对象承担什么责任（超时、重试、上下文）？
  3. 为什么 RPC 帧中需要 request_id 字段（多路复用 + 异步响应匹配）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理帧协议设计的完整格式文档
- [ ] `git commit`，推送
