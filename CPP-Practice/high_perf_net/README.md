# high_perf_net

Month 09（Week 33-36）对应的高性能网络库教学工程放在这个目录。

当前环境是 Windows，io_uring、sendfile/splice、SO_REUSEPORT 和 10 万连接压测都需要 Linux/WSL2。因此本工程实现跨平台可验证的核心算法和抽象。

## 覆盖内容

- `TimerWheel`：连接空闲超时和刷新
- `PrecisionTimerHeap`：精确定时器最小堆
- `IoUringSimulator`：SQ/CQ 环形队列概念模拟
- `ConnectionPool`：预建立连接池语义、RoundRobin、LeastConn、健康检查
- `OptimizationReport`：平台能力与 Linux 专项优化边界

## 构建

```powershell
cmake -S CPP-Practice/high_perf_net -B CPP-Practice/high_perf_net/build
cmake --build CPP-Practice/high_perf_net/build --config Release
.\CPP-Practice\high_perf_net\build\Release\high_perf_net_demo.exe
```