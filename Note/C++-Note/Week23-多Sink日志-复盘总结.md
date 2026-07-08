# Week23 多 Sink 日志复盘总结

## 核心目标

Week23 把日志输出抽象为 `Sink`，让同一个 logger 可以输出到内存、控制台和文件。

## Sink 设计

`Sink` 暴露两个接口：

- `write(line)`：写入一行日志
- `flush()`：刷新输出

具体实现包括：

- `MemorySink`：测试断言用，记录日志行数量。
- `ConsoleSink`：输出到指定 stream。
- `FileSink`：输出到按日期命名的日志文件。

## FileSink 边界

当前 FileSink 已按日期生成文件名，例如 `demo-YYYYMMDD.log`，为按天滚动提供基础。历史文件压缩、保留天数、异步压缩线程等属于运维策略，未在本周实现。

## 复盘结论

Sink 抽象让 logger 不关心日志最终落点，这也是日志库可扩展的关键。后续要支持网络、Kafka、按大小滚动或压缩，只需要扩展 sink，而不是修改 logger 主体。
