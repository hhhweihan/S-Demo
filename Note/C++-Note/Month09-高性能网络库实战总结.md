# Month09 高性能网络库实战总结

## 月度目标回顾

Month09 在 Reactor 基础上扩展高性能网络组件。代码位于 `CPP-Practice/high_perf_net/`。

## 已完成组件

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| TimerWheel | 空闲连接超时和刷新 | tick 后触发正确连接 |
| PrecisionTimerHeap | 精确 deadline | 按过期时间弹出 |
| IoUringSimulator | SQ/CQ 模型 | submit/process/completion |
| ConnectionPool | 连接复用语义 | acquire/release |
| LoadBalancer | RoundRobin / LeastConn | 轮转、跳过 unhealthy |
| OptimizationReport | 平台优化边界 | Windows 下提示 WSL2/Linux 压测 |

## 技术栈图

```text
HighPerfNet
  -> TimerWheel / TimerHeap
  -> Reactor / io_uring model
  -> ConnectionPool
  -> LoadBalancer
  -> zero-copy / reuse-port benchmark plan
```

## 关键知识梳理

### 1. 定时器是连接生命周期管理核心

高并发服务不能给每个连接一个独立线程或重型 timer。时间轮能用较低成本管理大量 idle timeout。

### 2. io_uring 是 Linux 专属能力

io_uring 的优势来自共享队列、批量提交和减少系统调用，但必须在 Linux 上才能验证真实性能。

### 3. 连接池是 RPC 客户端底座

连接池复用连接，负载均衡选择后端，健康检查保护调用路径。

### 4. 性能指标必须在目标环境压测

10 万 QPS、10 万连接和 io_uring 对比都需要 Linux/WSL2、系统参数调优和真实 server，当前 Windows 工程不伪造这些结论。

## 构建验证

```powershell
cmake -S CPP-Practice/high_perf_net -B CPP-Practice/high_perf_net/build
cmake --build CPP-Practice/high_perf_net/build --config Release
.\CPP-Practice\high_perf_net\build\Release\high_perf_net_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续衔接

Month10 将进入协程。Timer、异步 IO 队列和连接池都可以作为协程调度器和异步 IO 的上层应用场景。
