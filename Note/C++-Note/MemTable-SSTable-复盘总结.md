# MemTable / SSTable / BloomFilter 复盘总结

## 核心目标

Week27 串起 LSM Tree 的内存表和磁盘表：MemTable 接收写入，SSTable 持久化有序数据，BloomFilter 加速不存在 key 的判断。

## MemTable

LevelDB 的 MemTable 使用 InternalKey：

```text
InternalKey = UserKey + SequenceNumber + ValueType
```

SequenceNumber 用于表达版本顺序和快照读，ValueType 用于区分普通写入和删除墓碑。删除不立即移除旧值，是因为旧版本可能仍被快照读取，且真正回收要等 compaction。

## SSTable

SSTable 是磁盘上的有序文件。真实 LevelDB 会按 block 存储，并配合 block index、filter block、table cache 等结构。本工程使用文本表文件和内存向量二分查找，重点验证 flush/query 数据流。

## BloomFilter

BloomFilter 可以快速回答“某个 key 一定不存在”或“可能存在”。它有误判但无漏判，适合在读 SSTable 前先过滤不相关文件。

## 复盘结论

LSM 的读写取舍非常清楚：写入尽量顺序化，读取通过内存表、过滤器、索引和 compaction 降低放大。MemTable/SSTable 是理解这个取舍的核心。
