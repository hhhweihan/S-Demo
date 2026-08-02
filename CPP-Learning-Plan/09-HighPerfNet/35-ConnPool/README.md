# 连接池 + 负载均衡

## 本周目标

实现 TCP 连接池（预建立 + 复用），配合简单负载均衡策略，
作为 RPC 框架（Month 11）的客户端底层。

## 本周完成情况

- [x] Day 239：实现连接池预建立和复用语义
- [x] Day 240：实现 RoundRobin 负载均衡
- [x] Day 241：实现 LeastConn 负载均衡
- [x] Day 242：实现健康检查跳过 unhealthy backend
- [x] Day 243：完成连接池功能验证；8 线程压测留待专项 benchmark
- [x] Day 244：整理连接池与 RPC 客户端的关系
- [x] Day 245：完成 Week35 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `ConnectionPool`
- `RoundRobinBalancer` / `LeastConnBalancer`
- 周复盘：`Note/C++-Note/连接池负载均衡-复盘总结.md`

## 阶段小结
Week35 的重点是客户端侧复用和后端选择。连接池减少频繁建连成本，RoundRobin 保证简单均匀分配，LeastConn 更适合请求耗时差异较大的场景，健康检查避免把请求发到不可用后端。当前 demo 验证了分配、释放和跳过 unhealthy backend。

## 验证命令

```powershell
cmake --build CPP-Practice/high_perf_net/build --config Release
.\CPP-Practice\high_perf_net\build\Release\high_perf_net_demo.exe
```

---
