# LevelDB 崩溃安全化复盘总结

## 核心目标

Week29（LevelDB 线）把 `CPP-Practice/leveldb_mini/` 从教学版 LSM 升级为崩溃安全存储引擎：已确认写入必须在进程猝死后幸存，崩溃瞬间没写完的尾部要被干净丢弃。

> 当前 `CPP-Learning-Plan` 里 Week29 同时存在于 LevelDB 的 `29-CrashSafety` 和网络编程的 `29-Socket-epoll`，所以 Note 目录保留两个 Week29 复盘文件分别对应两条线。

## WAL：从 append/replay 到 CRC 记录帧

新的 WAL 记录格式是 `length + type + CRC32C + payload`。Put/Delete 先写 WAL 并 fsync，成功后才修改 MemTable；恢复时逐条校验长度和 CRC，遇到首个截断或损坏 record 就停止回放。

这个策略的语义很清楚：

- fsync 返回前不对外确认；
- 已确认的记录必须能 replay；
- 崩溃瞬间半写入的尾部可以丢弃；
- 位翻转和截断都不会被误当成有效写入。

## SSTable：块式二进制布局

SSTable 从文本行升级为块式二进制结构：

- data block 使用前缀压缩；
- restart 点支持块内二分；
- index block 定位 data block；
- bloom 持久化到 meta block；
- footer magic 用于识别半写文件或错误文件。

这样读取一个 key 不再需要扫全表，而是先查 index/bloom，再定位少量 block。

## Tombstone 与 compaction

删除不能直接“擦掉旧值”，因为旧值可能已经在更底层 SSTable 中。正确做法是写入 tombstone，让读取路径看到删除标记后停止向下查找；只有 compaction 到最底层时，tombstone 才能和被遮蔽的旧版本一起回收。

## 验证方式

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target leveldb_mini_tests -j
ctest --test-dir build -R leveldb_mini --output-on-failure
```

测试覆盖 CRC 向量、块往返、WAL 截断/损坏恢复、tombstone 遮蔽、compaction 回收、跨 flush/compaction 恢复和 fork/_exit kill-9 持久性。

## 复盘结论

崩溃安全不是“写过文件”这么简单，而是把确认边界放在 fsync 之后，并让所有磁盘格式都能自校验、可截断恢复。WAL 负责保住已确认写，SSTable footer/CRC 负责识别半写文件，tombstone 负责跨层表达删除语义，kill-9 测试负责验证没有析构或内存对象在“帮忙作弊”。
