# Month10 协程库实战总结

## 月度目标回顾

Month10 原计划实现 ucontext 有栈协程。由于当前是 Windows/MSVC 环境，本月代码改用标准 C++20 coroutine，位于 `CPP-Practice/coroutine_lib/`。

## 已完成组件

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| Task | promise_type、handle、生命周期 | 启动和销毁 |
| Scheduler | ready queue、yield/resume | 两个任务交替累加 |
| Channel | 缓冲 send/recv | 生产者协程发送 1/2 |
| select_recv | 多 Channel 轮询接收 | 从 ready channel 取值 |
| AsyncHttpClientCore | 模拟协程式 GET | 多个请求写入结果 channel |

## 协程库架构图

```text
Task(coroutine frame)
  -> co_await yield / async_sleep
  -> Scheduler ready queue
  -> Channel message passing
  -> Async HTTP/RPC style API
```

## 关键知识梳理

### 1. C++20 coroutine 是编译器生成的状态机

协程帧保存局部状态，`co_await` 决定是否挂起，`coroutine_handle` 负责恢复和销毁。

### 2. 调度器决定恢复顺序

协程本身不等于并发运行。只有被调度器 resume，协程才继续执行。

### 3. Channel 是通信边界

用 Channel 传递结果，可以减少共享变量和锁，让协程之间的依赖更清楚。

### 4. 有栈协程与 hook 仍需 Linux/POSIX

ucontext、epoll hook、系统调用替换和真实 HTTP 并发压测都需要 Linux/POSIX 环境，本月未伪造这些结果。

## 构建验证

```powershell
cmake -S CPP-Practice/coroutine_lib -B CPP-Practice/coroutine_lib/build
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续衔接

Month11 将进入 RPC。Month10 的 Task/Scheduler/Channel 可以作为协程式 RPC client/server 的异步执行模型基础。
