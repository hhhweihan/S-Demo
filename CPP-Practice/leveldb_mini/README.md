# leveldb_mini

Month 07（Week 25-29）对应的 LevelDB / LSM Tree 教学实现放在这个目录。

## 项目定位

这是一个面向学习的 mini LSM 存储引擎：先实现 LevelDB 写入路径的主干，再把教学版升级到崩溃安全版本。目标不是复刻完整 LevelDB，而是把 WAL、MemTable、SSTable、BloomFilter、compaction 和 crash recovery 的关键设计写成可测试代码。

## 覆盖内容

- `Slice` / `Status` / `Arena` 基础设施
- `SkipList<Key, Value>`：随机高度、插入、查找、有序迭代
- `MemTable`：InternalKey = UserKey + SequenceNumber 的教学编码
- `BloomFilter`：快速排除不存在 key
- `SSTable`：块式二进制文件（前缀压缩 + restart 二分 + 持久 bloom + index block + 48B footer magic）
- `WAL`：CRC32C 记录帧、fsync、截断/损坏 replay 恢复
- `Tombstone`：删除遮蔽与 compaction 到最底层后的回收
- `MiniDB`：Put/Delete/Get/Flush/Recover/Compaction 最小 LSM 闭环

## 验证口径

- 默认测试覆盖 CRC 向量、块式 SSTable round-trip、WAL 截断/损坏恢复、tombstone 遮蔽、compaction 回收、跨 flush/compaction 恢复。
- kill-9/fork 场景用于验证已确认写入在进程猝死后仍能恢复。
- 当前没有实现完整 LevelDB 的 MANIFEST、VersionSet、TableCache、block cache 或完整 leveled compaction 策略。

## 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target leveldb_mini_tests -j
ctest --test-dir build -R leveldb_mini --output-on-failure
```