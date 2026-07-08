# Week 32 — HTTP 服务器 + 整月复盘

## 本周目标

在 Reactor 框架上实现完整 HTTP/1.1 服务器，完成整月网络编程综合验收。

## 本周完成情况

- [x] Day 218：完善 HTTP GET 解析和响应生成
- [x] Day 219：实现长度字段 frame 上的 RPC server/client core
- [x] Day 220：验证同机 RPC 请求-响应往返
- [x] Day 221：整理 Reactor 模式完整设计图
- [x] Day 222：完成跨平台功能验收；epoll ET、ASan/TSan 留待 Linux/WSL2
- [x] Day 223：整理网络编程知识总结
- [x] Day 224：完成 Week32 和 Month08 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `RpcServerCore` / `RpcClientCore`
- 周复盘：`Note/C++-Note/Week32-HTTP与网络复盘总结.md`
- 月总结：`Note/C++-Note/Month08-网络编程Reactor实战总结.md`

## Week 32 总结

Week32 完成 HTTP/RPC 和整月复盘。当前工程不是完整网络服务器，而是可在 Windows 下验证的协议与 Reactor 核心：拆包、事件分发、HTTP GET、RPC frame 往返。真实 epoll ET、wrk QPS、ASan/TSan 需要切换到 Linux/WSL2 后补测。

## 验证命令

```powershell
cmake -S CPP-Practice/network_reactor -B CPP-Practice/network_reactor/build
cmake --build CPP-Practice/network_reactor/build --config Release
.\CPP-Practice\network_reactor\build\Release\network_reactor_demo.exe
```

---
