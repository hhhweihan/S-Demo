# Month08 网络编程 Reactor 实战总结

## 月度目标回顾

Month08 目标是理解 Socket、Reactor、HTTP Server 和 RPC 基础。当前 Windows 环境无法直接执行 epoll/wrk/ASan，因此代码位于 `CPP-Practice/network_reactor/`，实现跨平台核心抽象和协议验证。

## 已完成组件

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| Buffer | 长度字段 frame、粘包/半包处理 | 分片输入后连续解包 |
| Channel | fd + read callback | active 后触发回调 |
| FakePoller | 可测试 Poller | 返回活跃 Channel |
| EventLoop | 单次事件分发 | callback 计数 |
| EventLoopThreadPool | one-loop-per-thread 分配模型 | loop 轮转 |
| HTTP Core | GET 解析和响应 | 返回 200 + body |
| RPC Core | 长度字段 frame 上的调用 | echo 往返 |

## Reactor 设计图

```text
EventLoop
  -> Poller.poll()
  -> active Channels
  -> Channel.handle_read()
  -> connection/protocol callback
```

## 关键知识梳理

### 1. TCP 是字节流

应用层必须自己定义消息边界。长度字段协议是最常见方案之一，适合 RPC 和二进制协议。

### 2. Reactor 负责事件分发

Reactor 不应该夹杂业务逻辑。EventLoop 管调度，Poller 管等待，Channel 管回调，业务协议放在上层。

### 3. 多线程 Reactor 要控制连接归属

one-loop-per-thread 模型让一个连接固定在一个 IO loop 上，减少锁和跨线程状态迁移。

### 4. 性能压测需要真实环境

wrk QPS、epoll ET、ASan/TSan 都需要 Linux/WSL2 和真实 socket server。当前工程完成的是跨平台设计闭环。

## 构建验证

```powershell
cmake -S CPP-Practice/network_reactor -B CPP-Practice/network_reactor/build
cmake --build CPP-Practice/network_reactor/build --config Release
.\CPP-Practice\network_reactor\build\Release\network_reactor_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续衔接

Month09 将进入高性能网络主题，可以在本月 Reactor 抽象基础上继续扩展 TimerWheel、io_uring 概念、连接池和性能优化。
