# RPC 服务发现复盘总结

## 核心目标

Week43 实现服务注册与发现的教学版本，并梳理连接池和负载均衡关系。

## Registry

`Registry` 维护：

```text
service -> [endpoint1, endpoint2, ...]
```

客户端发现服务时按 round-robin 返回 endpoint。

## etcd 真实语义

生产环境常用 etcd 做服务发现：

- 服务启动时注册 key。
- 租约 TTL 保证进程崩溃后 key 自动过期。
- watch 机制让客户端感知 endpoint 变化。

## 复盘结论

服务发现解决“调用谁”的问题，连接池解决“如何复用连接”的问题，负载均衡解决“多个 endpoint 如何选择”的问题。三者通常一起出现。
