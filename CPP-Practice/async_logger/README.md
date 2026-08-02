# async_logger

Month 06（Week 21-24）对应的异步日志系统练习代码放在这个目录。

## 项目定位

这是一个系统组件练习项目，重点是把同步日志、多 Sink 抽象和异步后台 flush 串成一个可测试的小型日志库。它用于训练并发组件的边界设计，而不是宣称已经达到生产日志库的吞吐或可靠性标准。

## 覆盖内容

- `SyncLogger`：同步日志、日志级别、时间戳、线程 ID、文件行号、流式接口
- `AsyncLogger`：前台缓冲、后台线程、条件变量唤醒、批量 flush
- `Sink` 抽象：`MemorySink`、`ConsoleSink`、`FileSink`
- `FileSink`：按日期命名日志文件，为按天滚动提供基础
- 多线程写入和异步 enqueue 延迟 smoke test

## 验证口径

- 单元测试覆盖日志级别、格式字段、Sink 写入、多线程 enqueue 和后台 flush。
- smoke test 用来验证异步路径能跑通，不等价于真实生产压测。
- 后续如果要作为代表作继续打磨，建议补充 bounded queue/backpressure、日志滚动策略、fsync 策略、丢日志策略和多线程压力测试数据。

## 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target async_logger_tests -j
ctest --test-dir build -R async_logger --output-on-failure
```