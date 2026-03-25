# Month 4 — LevelDB 源码精读

## 月度目标

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

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 13 | 基础设施：Arena / Slice / Status | [Week-13.md](./Week-13.md) |
| Week 14 | SkipList 精读与仿写 | [Week-14.md](./Week-14.md) |
| Week 15 | MemTable / SSTable / BloomFilter | [Week-15.md](./Week-15.md) |
| Week 16 | WAL + 整体串联 + 应用开发 | [Week-16.md](./Week-16.md) |

## 验收标准

- [ ] 能不看源码画出 LevelDB 的完整写入路径图
- [ ] 仿写的 SkipList 性能与 LevelDB 原版差距在 20% 以内
- [ ] 能解释 Compaction 为什么分 Level，Level-0 为什么特殊处理
- [ ] 用 LevelDB 写过一个实际的小应用

## 参考资料

- LevelDB 源码：`https://github.com/google/leveldb`
- LevelDB 文档：`leveldb/doc/impl.md`（仓库内）
- 谷歌论文：Bigtable（LSM Tree 的来源）
- 你的 Raft 笔记里有 WAL 的相关内容，可以对照
