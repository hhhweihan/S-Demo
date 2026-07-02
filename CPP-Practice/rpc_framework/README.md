# rpc_framework

Month 11（Week 41-44）对应的 RPC 框架教学工程放在这个目录。

## 覆盖内容

- `RpcRequest` / `RpcResponse`：请求响应消息定义
- `FrameCodec`：长度前缀帧协议，处理半包
- 简化 protobuf-like key-value 编解码
- `RpcServer` / `RpcClient`：同步和异步 RPC 调用
- `Registry`：内存版服务注册与发现，模拟 etcd 发现语义
- Interceptor：认证/限流/日志等中间件机制入口
- 模拟验收：同步延迟、10 万 QPS proxy、Registry 发现延迟、mock gRPC framed baseline 对比

## 模拟验收口径

当前 demo 是同进程教学框架，不接真实 socket、etcd 或 gRPC。验收项使用 proxy benchmark：

- sync RPC average latency < 200us
- async dispatch throughput proxy > 100000 QPS
- Registry discover average latency < 5ms
- direct Mini RPC call no slower than mock framed baseline

## 构建

```powershell
cmake -S CPP-Practice/rpc_framework -B CPP-Practice/rpc_framework/build
cmake --build CPP-Practice/rpc_framework/build --config Release
.\CPP-Practice\rpc_framework\build\Release\rpc_framework_demo.exe
```