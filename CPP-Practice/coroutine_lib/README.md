# coroutine_lib

Month 10（Week 37-40）对应的协程库教学工程放在这个目录。

当前 Windows/MSVC 环境不提供 POSIX `ucontext_t`，因此本工程使用标准 C++20 coroutine 实现无栈协程版本，用来验证调度器、Channel 和异步编程模型。

## 覆盖内容

- `Task`：C++20 coroutine promise/handle 生命周期
- `Scheduler`：ready queue、yield/resume 语义
- `Channel<T>`：缓冲 channel 和 `select_recv`
- `AsyncHttpClientCore`：协程式异步 HTTP client 教学模拟

## 构建

```powershell
cmake -S CPP-Practice/coroutine_lib -B CPP-Practice/coroutine_lib/build
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```