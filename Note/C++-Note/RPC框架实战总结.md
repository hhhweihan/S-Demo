# RPC 框架实战总结

## 月度目标回顾

Month11 构建类 gRPC 的教学 RPC 框架。代码位于 `CPP-Practice/rpc_framework/`。

## 已完成组件

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| Message | RpcRequest / RpcResponse | 编解码保持字段 |
| FrameCodec | 长度前缀帧 | 半包等待、完整帧解析 |
| RpcServer | 方法注册和分派 | Echo handler |
| RpcClient | 同步和 Future 异步调用 | sync/async 返回响应 |
| Registry | 内存服务发现 | endpoint 轮询 |
| Interceptor | 中间件机制 | 拒绝 blocked 请求 |

## RPC 框架架构图

```text
Stub/RpcClient
  -> Codec + Frame
  -> Transport/ConnectionPool
  -> RpcServer
  -> Interceptors
  -> Service Handler
  -> Response
```

## 关键知识梳理

### 1. 帧协议解决消息边界

RPC 通常跑在 TCP 上，必须用长度字段或类似机制划分请求/响应。

### 2. request id 支撑异步和多路复用

异步调用需要把响应匹配回对应请求，request id 是 pending map 的 key。

### 3. 服务发现和连接池是客户端基础设施

发现决定 endpoint，连接池决定复用，负载均衡决定选择策略。

### 4. 中间件让横切逻辑可插拔

认证、日志、限流、trace 不应该散落在每个 handler 中，拦截器是更清晰的扩展点。

## 构建验证

```powershell
cmake -S CPP-Practice/rpc_framework -B CPP-Practice/rpc_framework/build
cmake --build CPP-Practice/rpc_framework/build --config Release
.\CPP-Practice\rpc_framework\build\Release\rpc_framework_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续衔接

Month12 将进入 RaftKV。RPC 框架可以作为 Raft 节点间 RequestVote / AppendEntries / 客户端 KV 请求的通信基础。
