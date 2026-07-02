# Week41 RPC 帧协议复盘总结

## 核心目标

Week41 完成 RPC 请求/响应消息结构、序列化和长度前缀帧协议。

## 请求/响应结构

请求包含：

- request id
- service
- method
- payload

响应包含：

- request id
- ok/error
- payload

request id 是后续多路复用和异步回包匹配的关键。

## 帧协议

长度前缀帧格式：

```text
uint32 length + payload bytes
```

接收端只有在完整 payload 到达后才解析，能处理 TCP 半包和粘包。

## 当前边界

本周未接入真实 protobuf 编译器，而是用 protobuf-like key-value 编码突出 RPC 消息结构。真实工程可以把 payload 替换为 protobuf bytes。
