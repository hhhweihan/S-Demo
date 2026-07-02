# leveldb_mini

Month 07（Week 25-28）对应的 LevelDB / LSM Tree 教学实现放在这个目录。

## 覆盖内容

- `Slice` / `Status` / `Arena` 基础设施
- `SkipList<Key, Value>`：随机高度、插入、查找、有序迭代
- `MemTable`：InternalKey = UserKey + SequenceNumber 的教学编码
- `BloomFilter`：快速排除不存在 key
- `SSTable`：有序文本表文件、二分查找
- `WAL`：追加日志和 replay 恢复
- `MiniDB`：Put/Get/Flush/Recover 最小 LSM 闭环

## 构建

```powershell
cmake -S CPP-Practice/leveldb_mini -B CPP-Practice/leveldb_mini/build
cmake --build CPP-Practice/leveldb_mini/build --config Release
.\CPP-Practice\leveldb_mini\build\Release\leveldb_mini_demo.exe
```