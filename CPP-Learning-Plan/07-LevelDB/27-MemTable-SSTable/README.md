# MemTable / SSTable / BloomFilter

## 本周目标

理解 LSM Tree 的核心：数据如何从内存（MemTable）落到磁盘（SSTable），以及 BloomFilter 如何加速读取。

## 本周完成情况

- [x] Day 183：理解 InternalKey = UserKey + SequenceNumber 的编码思想
- [x] Day 184：实现 MemTable Put/Get，按 sequence 返回最新值
- [x] Day 185：实现 BloomFilter 快速排除不存在 key
- [x] Day 186：实现 SSTable 文本表文件和二分查找
- [x] Day 187：实现 MemTable flush 到 SSTable
- [x] Day 188：验证 missing key 走 BloomFilter/SSTable 查询路径
- [x] Day 189：完成 Week27 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `MemTable` / `BloomFilter` / `SSTable`
- 周复盘：`Note/C++-Note/Week27-MemTable-SSTable-复盘总结.md`

## 阶段小结
Week27 串起 LSM 的内存表和磁盘表。MemTable 负责接收最新写入，InternalKey 用 sequence 表达版本顺序；SSTable 把有序数据落盘，并用 BloomFilter 在查找不存在 key 时快速跳过文件。教学版 SSTable 使用文本文件和内存索引，突出数据流而不是 Block/TableBuilder 的全部工程细节。

## 验证命令

```powershell
cmake --build CPP-Practice/leveldb_mini/build --config Release
.\CPP-Practice\leveldb_mini\build\Release\leveldb_mini_demo.exe
```

---