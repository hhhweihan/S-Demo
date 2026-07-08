# Month07 LevelDB 源码精读总结

## 月度目标回顾

Month07 目标是理解 LevelDB 的核心写入路径，并仿写关键组件。代码位于 `CPP-Practice/leveldb_mini/`。

## 已完成组件

| 模块 | 实现内容 | 验证方式 |
|------|----------|----------|
| 基础设施 | Slice / Status / Arena | 字节视图、状态、内存用量断言 |
| SkipList | 随机高度、插入、查找、有序迭代 | 插入 5/3/8/1/7 后有序遍历 |
| MemTable | InternalKey 教学编码、最新 sequence 查询 | 同 key 更新返回最新值 |
| BloomFilter | 多 hash 位图 | missing key 查询排除 |
| SSTable | 有序文本表、二分查找 | flush 后 unique user key 数量正确 |
| WAL | append / replay | 重启恢复后仍能读取值 |
| MiniDB | Put / Get / Flush / Recover | 最小 LSM 闭环测试 |

## LevelDB 写入路径

```text
Put
  -> 写 WAL
  -> 写 MemTable(SkipList)
  -> MemTable 达阈值变 Immutable
  -> 后台 flush 为 SSTable
  -> 后台 Compaction 合并层级
```

## LSM Tree 为什么写快

LSM 把随机写转成顺序写：WAL 是追加，MemTable 是内存结构，SSTable 是批量顺序落盘。代价是读取路径更复杂，需要查 MemTable、immutable MemTable、多层 SSTable，并通过 BloomFilter、索引和 compaction 控制读放大。

## SkipList 设计亮点

SkipList 用随机层级替代复杂旋转，插入实现比平衡树简单。LevelDB 原版依赖单写多读场景和 acquire/release 指针发布，避免给读路径加重锁。

## Compaction 与 Level-0

Compaction 的目标是合并重叠 key range、删除旧版本和墓碑、降低文件数量。Level-0 直接由 MemTable flush 得到，文件之间可能重叠，因此读取 L0 时可能要查多个文件；Level-1 及以下通常维护不重叠区间，读取效率更稳定。

## 与 B-Tree 对比

| 结构 | 优势 | 代价 | 适合场景 |
|------|------|------|----------|
| LSM Tree | 写入吞吐高、顺序 IO 友好 | 读放大、写放大、compaction 复杂 | 写多读少、日志型 KV、SSD |
| B-Tree | 点查和范围查稳定、原地更新直观 | 随机写多、页分裂复杂 | 读多、事务索引、传统数据库 |

## 构建验证

```powershell
cmake -S CPP-Practice/leveldb_mini -B CPP-Practice/leveldb_mini/build
cmake --build CPP-Practice/leveldb_mini/build --config Release
.\CPP-Practice\leveldb_mini\build\Release\leveldb_mini_demo.exe
```

验证结果：全部 `[PASS]`。

## 后续衔接

Month08 将进入网络编程。LevelDB 的 WAL、SSTable 和 MiniDB 可以作为后续网络 KV 服务的存储后端原型。
