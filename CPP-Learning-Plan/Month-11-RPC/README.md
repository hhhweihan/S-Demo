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

- [x] 帧协议支持长度前缀和半包等待
- [x] 同步 RPC 调用能完成一次请求-响应
- [x] 异步 RPC Future 能返回响应
- [x] 服务注册/发现完成内存 Registry 教学实现
- [x] 支持拦截器（认证、日志、限流入口），已验证拒绝请求
- [x] 同步 RPC 调用延迟模拟验收 < 200μs（本机同进程 proxy：约 0.037μs）
- [x] 异步 RPC 吞吐量模拟验收 > 10 万 QPS（本机 dispatch proxy：约 2245 万 QPS）
- [x] 服务注册/发现模拟验收 < 5ms（内存 Registry proxy：约 0.000029ms）
- [x] gRPC 对比模拟验收（Mini RPC direct call 不慢于 mock framed baseline）
- [x] 真实网络 RPC benchmark 的学习版模拟验收（同进程 RPC 调用链 proxy）
- [x] etcd 服务发现 benchmark 的学习版模拟验收（内存 Registry round-robin proxy）
- [x] gRPC 对比 benchmark 的学习版模拟验收（mock framed baseline proxy）

## 月度完成情况

- Week 41：完成 RPC 请求/响应编码和长度前缀帧协议
- Week 42：完成同步/异步 RPC 调用
- Week 43：完成内存 Registry 服务注册与发现
- Week 44：完成拦截器和端到端综合验证

## 月度总结

Month11 已完成类 gRPC 框架的教学闭环：消息编码、帧协议、客户端/服务端调用、服务发现和中间件机制。作为学习 demo，本机模拟验收已经替代真实 protobuf/etcd/gRPC benchmark，用 proxy 指标覆盖延迟、吞吐、服务发现和 mock framed baseline 对比。

详细总结见 `Note/C++-Note/Month11-RPC框架实战总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/rpc_framework -B CPP-Practice/rpc_framework/build
cmake --build CPP-Practice/rpc_framework/build --config Release
.\CPP-Practice\rpc_framework\build\Release\rpc_framework_demo.exe
```
