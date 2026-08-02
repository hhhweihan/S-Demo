# LevelDB 源码精读

## 模块目标

深度阅读 LevelDB（~2 万行），理解每个组件的设计决策。
核心产出：仿写 Arena（内存分配）、SkipList（有序数据结构），能独立解释整个写入路径。

## 架构总览

```
写入路径：
Put(key, val)
  → WAL（预写日志，保证持久化）
  → MemTable（内存跳表，支持快速写）
  → [MemTable 满] → Immutable MemTable
  → [后台线程] → SSTable（磁盘有序文件）
  → Compaction（合并多层 SSTable，回收空间）

读取路径：
Get(key)
  → MemTable（最新数据）
  → Immutable MemTable
  → SSTable Level-0 ~ Level-N（从新到旧）
  → BloomFilter（快速跳过不含 key 的文件）
```

## 阶段拆解

| 阶段 | 主题 | 目录 |
|----|------|------|
| 阶段 25 | 基础设施：Arena / Slice / Status | [25-Infrastructure/](./25-Infrastructure/) |
| 阶段 26 | SkipList 精读与仿写 | [26-SkipList/](./26-SkipList/) |
| 阶段 27 | MemTable / SSTable / BloomFilter | [27-MemTable-SSTable/](./27-MemTable-SSTable/) |
| 阶段 28 | WAL + 整体串联 + 应用开发 | [28-WAL-Walkthrough/](./28-WAL-Walkthrough/) |
| 阶段 29 | 崩溃安全化（旗舰改造）：CRC WAL / 块式 SSTable / tombstone / compaction / kill-9 | [29-CrashSafety/](./29-CrashSafety/) |

## 验收标准

- [x] 能不看源码画出 LevelDB 的完整写入路径图
- [x] 仿写 SkipList 并通过有序插入、查找、迭代验证；性能 20% 对比留待专项 benchmark
- [x] 能解释 Compaction 为什么分 Level，Level-0 为什么特殊处理
- [x] 写过一个 mini LevelDB/LSM 应用：`CPP-Practice/leveldb_mini/`
- [x] 把教学版升级为崩溃安全存储引擎：CRC WAL + fsync + 尾部恢复、块式 SSTable、tombstone、compaction、kill-9 持久性测试

## 模块完成情况

- 阶段 25：完成 `Slice`、`Status`、`Arena` 基础设施
- 阶段 26：完成教学版 `SkipList`，支持随机高度、插入、查找和有序迭代
- 阶段 27：完成 `MemTable`、`BloomFilter`、`SSTable` 和 flush/query 路径
- 阶段 28：完成 `WAL`、`MiniDB`、Put/Get/Recover 的最小 LSM 闭环
- 阶段 29（旗舰）：崩溃安全化改造。WAL 重写为 `length+type+CRC32C+payload` 记录帧并 fsync（macOS `F_FULLFSYNC`），回放止于首个截断/损坏 record；SSTable 升级为块式二进制（前缀压缩 + restart 二分 + index + 持久 bloom + footer magic 自校验）；删除用 tombstone 表达、compaction 到最底层回收；文件号单调 + MANIFEST-free 目录扫描恢复；fork/`_exit` kill-9 测试证明 acked 写幸存

## 模块总结

本模块已完成 LevelDB/LSM Tree 的主路径理解和教学实现，并进一步做了崩溃安全化旗舰改造。写入路径从 WAL 顺序追加（CRC 记录帧 + fsync 落盘才应答）开始，进入 MemTable 跳表（二进制 internal key + tombstone），随后 flush 到块式 SSTable；读取路径先查 MemTable，再通过 BloomFilter/SSTable 从新到旧查磁盘有序文件，遇 tombstone 即止。崩溃恢复扫描目录按文件名重建分层文件集，回放 WAL 补齐未落盘的已确认写，崩溃瞬间没写完的尾部被干净丢弃。当前工程突出机制理解与崩溃安全，未复刻 LevelDB 的 TableCache、VersionSet/MANIFEST 与多层 leveled compaction 的完整策略。

详细总结见 `Note/C++-Note/LevelDB源码精读总结.md`。

## 验证命令

```powershell
cmake -S CPP-Practice/leveldb_mini -B CPP-Practice/leveldb_mini/build
cmake --build CPP-Practice/leveldb_mini/build --config Release
.\CPP-Practice\leveldb_mini\build\Release\leveldb_mini_demo.exe
```

## 参考资料

- LevelDB 源码：`https://github.com/google/leveldb`
- LevelDB 文档：`leveldb/doc/impl.md`（仓库内）
- 谷歌论文：Bigtable（LSM Tree 的来源）
- 你的 Raft 笔记里有 WAL 的相关内容，可以对照
