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

## 验收标准

- [x] 能不看源码画出 LevelDB 的完整写入路径图
- [x] 仿写 SkipList 并通过有序插入、查找、迭代验证；性能 20% 对比留待专项 benchmark
- [x] 能解释 Compaction 为什么分 Level，Level-0 为什么特殊处理
- [x] 写过一个 mini LevelDB/LSM 应用：`CPP-Practice/leveldb_mini/`

## 模块完成情况

- 阶段 25：完成 `Slice`、`Status`、`Arena` 基础设施
- 阶段 26：完成教学版 `SkipList`，支持随机高度、插入、查找和有序迭代
- 阶段 27：完成 `MemTable`、`BloomFilter`、`SSTable` 和 flush/query 路径
- 阶段 28：完成 `WAL`、`MiniDB`、Put/Get/Recover 的最小 LSM 闭环

## 模块总结

本模块已完成 LevelDB/LSM Tree 的主路径理解和教学实现。写入路径从 WAL 顺序追加开始，进入 MemTable 跳表，随后 flush 到 SSTable；读取路径先查 MemTable，再通过 BloomFilter/SSTable 查磁盘有序文件。当前工程突出机制理解，未复刻 LevelDB 的 Block、TableCache、VersionSet 和完整 Compaction 实现。

详细总结见 `Note/C++-Note/Month07-LevelDB源码精读总结.md`。

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
