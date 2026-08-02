# Channel 通信复盘总结

## 核心目标

Week39 实现 Channel 通信和 select 风格多路等待。

## Channel

Channel 是协程间传递数据的抽象。本周实现的是缓冲 Channel：

- `send`：缓冲区未满时写入。
- `recv`：缓冲区非空时取出。
- `select_recv`：按顺序查找第一个可读 channel。

## Go 风格 Channel 的后续方向

真正的无缓冲 Channel 需要 sender 和 receiver 同步握手。如果没有接收方，发送方应挂起；如果没有发送方，接收方应挂起。这需要 Channel 与 Scheduler 深度集成。

## 复盘结论

Channel 把共享状态转为消息传递，是协程系统里降低耦合的重要工具。当前版本完成了教学闭环，后续可以扩展阻塞 awaiter 和 select 随机公平性。
