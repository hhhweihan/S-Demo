# 整合 + 压测

## 本周目标

完整端到端测试 RPC 框架，添加拦截器机制，与 gRPC 性能对比，
完成月度验收。

## 本周完成情况

- [x] Day 302：整合编解码、RPC server/client、Registry
- [x] Day 303：实现 Interceptor 中间件机制
- [x] Day 304：完成端到端功能验证；gRPC 对比留待外部依赖压测
- [x] Day 305：验证 interceptor 可拒绝请求
- [x] Day 306：整理认证、日志、限流中间件设计
- [x] Day 307：整理 RPC 框架架构图
- [x] Day 308：完成 Week44 和 Month11 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- Interceptor 机制
- 周复盘：`Note/C++-Note/Week44-RPC整合复盘总结.md`
- 月总结：`Note/C++-Note/Month11-RPC框架实战总结.md`

## 阶段小结
Week44 完成 RPC 框架端到端验证：编解码、帧协议、同步调用、异步 Future、服务发现、拦截器都已跑通。作为学习 demo，真实网络、etcd 和 gRPC 对比改用本机 proxy benchmark 验收。

## 验证命令

```powershell
cmake -S CPP-Practice/rpc_framework -B CPP-Practice/rpc_framework/build
cmake --build CPP-Practice/rpc_framework/build --config Release
.\CPP-Practice\rpc_framework\build\Release\rpc_framework_demo.exe
```

---
