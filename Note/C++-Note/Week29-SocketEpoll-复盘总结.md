# Week29 Socket / epoll 复盘总结

## 核心目标

Week29 理解 TCP 字节流、粘包/半包、非阻塞 IO 和 epoll LT/ET 模型。

## TCP 粘包/半包

TCP 是字节流协议，没有消息边界。应用层必须定义协议，例如：

- 定长包
- 分隔符协议
- 长度字段协议

本周实现采用长度字段 frame：先写 4 字节长度，再写 payload。接收端只有在完整 payload 到达后才返回一帧。

## 非阻塞 IO

非阻塞读写遇到暂时不可读/不可写时返回 EAGAIN。ET 模式下必须循环读到 EAGAIN，否则可能错过后续事件。

## epoll LT/ET

- LT：只要 fd 仍可读，epoll 会持续通知，容错更强。
- ET：状态变化时通知一次，性能潜力更高，但要求一次处理干净。

## 当前实现边界

当前环境是 Windows，未直接运行 epoll server。本周工程用跨平台 `Buffer` 验证 TCP 粘包/半包处理，这是后续真实 socket server 的协议层基础。
