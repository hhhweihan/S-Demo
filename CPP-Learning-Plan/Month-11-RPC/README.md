# Month 11 — RPC 框架

## 月度目标

构建一个类 gRPC 的 RPC 框架：protobuf 序列化、自定义帧协议、同步/异步调用、
连接池（复用 Month 9）、服务注册与发现。

## 技能树

```
RPC 框架
├── protobuf + 帧协议       ← Week 41
│   ├── protobuf 序列化/反序列化
│   ├── 帧格式（长度前缀）
│   └── RPC 请求/响应消息定义
├── 同步 + 异步 RPC         ← Week 42
│   ├── 同步调用（stub.Call() 阻塞等待）
│   ├── 异步调用（Future/Callback）
│   └── 超时与取消
├── 连接池 + 服务发现        ← Week 43
│   ├── 复用 Month 9 连接池
│   ├── 基于 etcd 的服务注册/发现
│   └── 客户端负载均衡
└── 整合 + 压测             ← Week 44
    ├── 完整 RPC 调用链路端到端测试
    ├── 与 gRPC 性能对比
    └── 中间件（拦截器）机制
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 41 | protobuf + 帧协议 | [Week-41-Protobuf-Frame/](./Week-41-Protobuf-Frame/) |
| Week 42 | 同步 + 异步 RPC | [Week-42-RPC-Call/](./Week-42-RPC-Call/) |
| Week 43 | 连接池 + 服务发现 | [Week-43-ConnPool-Discovery/](./Week-43-ConnPool-Discovery/) |
| Week 44 | 整合 + 压测 | [Week-44-Integration/](./Week-44-Integration/) |

## 验收标准

- [ ] 同步 RPC 调用延迟（同机） < 200μs
- [ ] 异步 RPC 吞吐量 > 10 万 QPS（4 核机器）
- [ ] 服务注册/发现从 etcd 拉取 < 5ms
- [ ] 支持拦截器（认证、日志、限流）
