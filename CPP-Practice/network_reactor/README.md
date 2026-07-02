# network_reactor

Month 08（Week 29-32）对应的网络编程与 Reactor 教学工程放在这个目录。

当前工作环境是 Windows，计划中的 epoll、wrk 和 ASan 压测需要 Linux/WSL2。因此本工程先实现跨平台核心抽象和协议逻辑，用自动化测试覆盖网络编程主线。

## 覆盖内容

- `Buffer`：长度字段拆包，解决 TCP 粘包/半包问题
- `Channel` / `FakePoller` / `EventLoop`：Reactor 核心抽象
- `EventLoopThreadPool`：one-loop-per-thread 分配模型
- `HttpParser` / `HttpServerCore`：HTTP GET 解析和响应
- `RpcServerCore` / `RpcClientCore`：长度字段帧上的同进程 RPC 往返

## 构建

```powershell
cmake -S CPP-Practice/network_reactor -B CPP-Practice/network_reactor/build
cmake --build CPP-Practice/network_reactor/build --config Release
.\CPP-Practice\network_reactor\build\Release\network_reactor_demo.exe
```