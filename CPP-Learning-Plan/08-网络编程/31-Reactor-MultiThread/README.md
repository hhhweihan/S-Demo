# 多线程 Reactor + HTTP Server

## 本周目标

实现 one-loop-per-thread 多线程 Reactor，完成 HTTP Server，wrk 测试 QPS > 5000。

## 本周完成情况

- [x] Day 211：实现 `EventLoopThreadPool` 轮转分配模型
- [x] Day 212：梳理 one-loop-per-thread 线程模型
- [x] Day 213：实现 HTTP 请求行和 header 解析
- [x] Day 214：实现 HTTP GET handler 与响应生成
- [x] Day 215：完成跨平台 HTTP core 功能验证
- [x] Day 216：整理真实 socket server 与当前 core demo 的差异
- [x] Day 217：完成 Week31 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `EventLoopThreadPool`
- `HttpParser` / `HttpServerCore`
- 周复盘：`Note/C++-Note/多线程ReactorHTTP-复盘总结.md`

## 阶段小结
Week31 的重点是把单线程 Reactor 扩展成 one-loop-per-thread 模型，并接入 HTTP 协议层。当前实现验证了 loop 轮转分配和 HTTP GET core 逻辑；wrk 5000 QPS 需要真实 Linux socket server 和压测环境，未在 Windows 下执行。

## 验证命令

```powershell
cmake --build CPP-Practice/network_reactor/build --config Release
.\CPP-Practice\network_reactor\build\Release\network_reactor_demo.exe
```

---