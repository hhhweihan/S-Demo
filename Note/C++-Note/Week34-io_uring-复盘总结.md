# Week34 io_uring 复盘总结

## 核心目标

Week34 理解 io_uring 的提交队列 SQ 和完成队列 CQ。

## SQ/CQ 模型

io_uring 把系统调用提交和完成结果拆成两个环形队列：

- SQ：用户态提交 IO 请求。
- CQ：内核完成请求后写入结果。

这种设计减少系统调用次数，适合批量提交和高并发 IO。

## 当前实现

Windows 环境不支持原生 io_uring，本周实现 `IoUringSimulator`：

- `submit` 写入 SQ。
- `process` 模拟内核消费 SQ 并生成 CQ。
- `peek_completion` 读取 CQ。

## 复盘结论

io_uring 的性能价值来自批处理、共享队列和减少内核往返。真正的吞吐对比必须在 Linux 上运行，模拟器只负责把概念模型跑通。
