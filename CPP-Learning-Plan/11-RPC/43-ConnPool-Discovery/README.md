# 连接池 + 服务发现

## 本周目标

将 Month 9 连接池接入 RPC 框架，实现基于 etcd 的服务注册与发现，
完成客户端负载均衡。

## 本周完成情况

- [x] Day 295：实现内存版 `Registry` 服务注册
- [x] Day 296：实现 endpoint 发现和轮转
- [x] Day 297：整理 etcd watch/lease/TTL 设计
- [x] Day 298：梳理连接池与 RPC client 的衔接方式
- [x] Day 299：验证 Registry 轮询返回 endpoint
- [x] Day 300：整理客户端负载均衡策略
- [x] Day 301：完成 Week43 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `Registry`：内存版服务注册与发现
- 周复盘：`Note/C++-Note/Week43-RPC服务发现-复盘总结.md`

## 阶段小结
Week43 完成服务发现教学版本。真实 etcd 需要网络依赖、租约和 watch 流；当前 Registry 用内存结构模拟服务名到 endpoint 列表的映射，并用 round-robin 返回 endpoint。这样先把 RPC client 如何依赖发现结果讲清楚。

## 验证命令

```powershell
cmake --build CPP-Practice/rpc_framework/build --config Release
.\CPP-Practice\rpc_framework\build\Release\rpc_framework_demo.exe
```

---
