# RPC 整合复盘总结

## 核心目标

Week44 完成 RPC 框架端到端整合和拦截器机制。

## Interceptor

Interceptor 在 handler 执行前介入，可以用于：

- 认证
- 日志
- 限流
- 熔断
- Trace 注入

当前 demo 验证了 interceptor 可以拒绝 payload 为 `blocked` 的请求。

## 已完成链路

```text
RpcClient
  -> RpcRequest
  -> encode/frame
  -> RpcServer.handle
  -> interceptor
  -> handler
  -> RpcResponse
```

## 未完成指标

未接入真实网络、etcd、gRPC，因此没有同步 P99、异步 QPS、服务发现延迟和 gRPC 对比数据。

## 复盘结论

RPC 框架的结构已经闭环，下一步要提升真实性能，需要接入 Month08/09 的网络层、连接池和压测环境。
