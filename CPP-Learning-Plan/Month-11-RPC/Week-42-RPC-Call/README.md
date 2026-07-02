# Week 42 — 同步 + 异步 RPC

## 本周目标

在基础 RPC 框架上添加异步调用（Future/Callback）、超时、取消，
实现多路复用（单连接处理并发请求）。

## 本周完成情况

- [x] Day 288：实现 `RpcServer` 方法注册与分派
- [x] Day 289：实现 `RpcClient::call` 同步调用
- [x] Day 290：实现 `RpcClient::async_call` Future 异步调用
- [x] Day 291：验证 Echo 同步/异步调用结果
- [x] Day 292：完成调用链功能验证；Echo 压测按学习 demo proxy benchmark 验收
- [x] Day 293：整理 timeout/cancel 的扩展设计
- [x] Day 294：完成 Week42 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `RpcServer`
- `RpcClient`
- 周复盘：`Note/C++-Note/Week42-RPC调用-复盘总结.md`

## Week 42 总结

Week42 完成 RPC 调用核心：服务端按 service.method 分派，客户端提供同步调用和 Future 异步调用。当前 demo 是同进程调用链，用于验证框架结构；超时、取消和多路复用可以基于 request id 和 pending map 继续扩展。

## 验证命令

```powershell
cmake --build CPP-Practice/rpc_framework/build --config Release
.\CPP-Practice\rpc_framework\build\Release\rpc_framework_demo.exe
```

---
