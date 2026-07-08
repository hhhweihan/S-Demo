# Week28 WAL 与 LevelDB 整合复盘总结

## 核心目标

Week28 完成 WAL、MiniDB 和 Put/Get/Recover 闭环，串联 LevelDB 写入路径。

## WAL

WAL 是预写日志。写入时先 append WAL，再写 MemTable。这样即使进程在 MemTable flush 前崩溃，也可以通过 replay WAL 恢复内存表。

## MiniDB 数据流

```text
Put(key, value)
  -> WAL.Append(sequence, key, value)
  -> MemTable.Put(sequence, key, value)

Flush()
  -> MemTable EntriesByUserKey
  -> SSTable.Build

Recover()
  -> WAL.Replay
  -> MemTable.Put
```

## Compaction 分层

Compaction 把多个 SSTable 合并为更有序、更少重叠的层级，减少读放大并回收删除墓碑。Level-0 特殊是因为它由 MemTable 直接 flush 而来，文件 key range 可能重叠，所以读取时需要查多个 L0 文件。

## 复盘结论

LevelDB 写快的原因是将随机写转化为 WAL 顺序追加和 MemTable 内存写，真正复杂的整理工作交给后台 compaction。理解 WAL/MemTable/SSTable 这条链，基本就抓住了 LSM Tree 的主干。
