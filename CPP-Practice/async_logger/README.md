# async_logger

Month 06（Week 21-24）对应的异步日志系统练习代码放在这个目录。

## 覆盖内容

- `SyncLogger`：同步日志、日志级别、时间戳、线程 ID、文件行号、流式接口
- `AsyncLogger`：前台缓冲、后台线程、条件变量唤醒、批量 flush
- `Sink` 抽象：`MemorySink`、`ConsoleSink`、`FileSink`
- `FileSink`：按日期命名日志文件，为按天滚动提供基础
- 多线程写入和异步 enqueue 延迟 smoke test

## 构建

```powershell
cmake -S CPP-Practice/async_logger -B CPP-Practice/async_logger/build
cmake --build CPP-Practice/async_logger/build --config Release
.\CPP-Practice\async_logger\build\Release\async_logger_demo.exe
```