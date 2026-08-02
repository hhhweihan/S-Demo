# 单线程 Reactor 复盘总结

## 核心目标

Week30 实现 Reactor 核心骨架：`EventLoop`、`Channel`、`Poller`。

## Reactor 分层

- `Channel`：封装一个 fd 上的事件和回调。
- `Poller`：等待 IO 事件，返回活跃 Channel。
- `EventLoop`：循环调用 Poller，并分发事件回调。

本周用 `FakePoller` 替代真实 epoll，使结构能在 Windows 下测试。

## 为什么这样拆

Reactor 的价值是把“等待事件”和“处理事件”解耦。业务代码注册回调，EventLoop 负责调度，Poller 负责和操作系统 IO 多路复用接口交互。

## 复盘结论

掌握 Reactor 不一定要一开始就写完整 socket server。先把 Channel/Poller/EventLoop 的边界跑通，再替换 Poller 为 epoll，会更容易定位问题。
