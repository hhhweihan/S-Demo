# Socket 基础 + epoll

## 本周目标

从零搭一个 TCP 服务端/客户端，理解 TCP 粘包问题，掌握 epoll 的 LT 和 ET 两种模式。

## 本周完成情况

- [x] Day 197：梳理 TCP socket、listen/accept/read/write 生命周期
- [x] Day 198：实现长度字段 `Buffer`，验证粘包/半包处理
- [x] Day 199：整理非阻塞 IO 与 EAGAIN 处理模型
- [x] Day 200：梳理 select/poll/epoll 差异
- [x] Day 201：完成 epoll LT/ET 概念对比；真实 epoll demo 留待 Linux/WSL2
- [x] Day 202：完成跨平台拆包测试
- [x] Day 203：完成 Week29 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `Buffer`：长度字段拆包，覆盖 TCP 粘包/半包核心问题
- 周复盘：`Note/C++-Note/Week29-SocketEpoll-复盘总结.md`

## 阶段小结
Week29 的重点是理解网络 IO 的基本问题。TCP 是字节流，没有消息边界，因此需要应用层协议解决粘包/半包；非阻塞 IO 需要正确处理 EAGAIN；epoll 的 LT/ET 差异决定读取循环是否必须一次读到 EAGAIN。当前 Windows 环境无法直接运行 epoll，因此工程先验证跨平台 Buffer 协议层。

## 验证命令

```powershell
cmake -S CPP-Practice/network_reactor -B CPP-Practice/network_reactor/build
cmake --build CPP-Practice/network_reactor/build --config Release
.\CPP-Practice\network_reactor\build\Release\network_reactor_demo.exe
```

---