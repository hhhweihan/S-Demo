# 协程异步 IO 复盘总结

## 核心目标

Week40 将协程调度器、Channel 和异步接口风格串起来，完成协程式 HTTP client core。

## async_sleep

`async_sleep` 是教学版 awaiter：挂起当前协程，等待一段时间后重新 schedule。真实工程里应由 timer wheel 或 IO loop 唤醒，而不是直接 sleep 当前线程。

## 协程式 HTTP core

`AsyncHttpClientCore::get` 用同步风格表达异步流程：

```cpp
co_await async_sleep(...);
out.send(response);
```

这展示了协程的主要价值：把回调式异步流程写成顺序代码。

## 当前边界

未实现：

- hook read/write/sleep。
- epoll 等待恢复协程。
- 1000 真实 HTTP 请求。
- 100ns 切换 benchmark。

## 复盘结论

协程式 IO 的核心是“等待时挂起，事件到达时恢复”。本周完成了模型验证，真实网络 IO 需要和 Linux epoll 或 io_uring 事件源对接。
