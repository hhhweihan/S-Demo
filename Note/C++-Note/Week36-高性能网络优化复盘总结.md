# Week36 高性能网络优化复盘总结

## 核心目标

Week36 整合 Month09 组件，并梳理 zero-copy、SO_REUSEPORT 和大规模压测边界。

## zero-copy

Linux 下 `sendfile` / `splice` 可以减少用户态和内核态之间的数据拷贝，适合静态文件传输和代理转发场景。

## SO_REUSEPORT

`SO_REUSEPORT` 允许多个线程/进程绑定同一端口，由内核分配连接，减少 accept 锁竞争。

## 当前验证

Windows 环境下完成：

- TimerWheel 空闲清理。
- io_uring SQ/CQ 模拟。
- ConnectionPool 负载均衡。
- 平台优化能力报告。

未执行：

- 10 万 QPS。
- io_uring vs epoll 吞吐对比。
- 10 万连接稳定性。

## 复盘结论

高性能网络优化高度依赖内核能力和压测环境。当前工程先把算法和架构边界明确下来，后续切 Linux/WSL2 后再补真实压测。
