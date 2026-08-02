# RPC 调用复盘总结

## 核心目标

Week42 实现 RPC server/client 调用链，包括同步调用和 Future 异步调用。

## Server

`RpcServer` 维护 `service.method -> handler` 映射。收到请求后按服务和方法分派，返回 `RpcResponse`。

## Client

`RpcClient::call` 发起同步调用并返回响应；`async_call` 用 `std::future` 表达异步调用。

## 后续扩展

真实 RPC 还需要：

- request id -> promise 的 pending map
- 超时取消
- 单连接多路复用
- 网络错误重试

## 复盘结论

RPC 调用链的核心是“本地方法调用外观 + 网络消息分派”。先把同进程调用链跑通，再替换传输层，会更容易定位问题。
