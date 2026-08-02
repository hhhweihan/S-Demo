# Coroutine 基础复盘总结

## 核心目标

Week37 原计划使用 POSIX ucontext 实现有栈协程，但 Windows/MSVC 不支持 ucontext，因此本周采用标准 C++20 coroutine 实现无栈协程基础。

## C++20 coroutine 关键对象

- `promise_type`：协程状态和返回对象的控制中心。
- `coroutine_handle`：恢复、销毁协程帧的句柄。
- `initial_suspend`：控制协程创建后是否立即执行。
- `final_suspend`：控制协程结束后帧如何保留给调用者清理。

## 有栈与无栈差异

有栈协程保存完整调用栈，适合把阻塞式调用整体挂起；无栈协程只在 `co_await` 挂起点保存状态，要求异步边界显式写出。

## 复盘结论

无栈协程更符合标准 C++，跨平台性好；ucontext 有栈协程更接近传统 fiber，但依赖 POSIX。当前工程先掌握标准协程机制，后续可在 Linux 下补有栈实现。
