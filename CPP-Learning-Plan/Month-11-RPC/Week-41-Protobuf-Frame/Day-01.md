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

