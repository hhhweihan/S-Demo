# WAL + 整体串联 + 应用开发

## 本周目标

完整串联从 Put 到磁盘的全路径，用 gdb 跟踪执行，写一个真实应用，完成 LevelDB 月度总结。

## 本周完成情况

- [x] Day 190：实现 WAL append，保证先写日志再写 MemTable
- [x] Day 191：实现 WAL replay 恢复 MemTable
- [x] Day 192：实现 `MiniDB::Put/Get/Flush/Recover`
- [x] Day 193：串联 Put -> WAL -> MemTable -> SSTable 写入路径
- [x] Day 194：整理 Compaction 分层和 Level-0 特殊性
- [x] Day 195：完成 mini LevelDB 应用式 demo
- [x] Day 196：完成 LevelDB 源码精读总结；`git commit / push` 等待明确确认后再执行

## 本周产出

- `WAL` / `MiniDB`：追加、恢复、Put/Get/Flush
- 周复盘：`Note/C++-Note/WAL与LevelDB整合复盘总结.md`
- 月总结：`Note/C++-Note/LevelDB源码精读总结.md`

## 阶段小结
Week28 把 LSM 写入路径串起来：`Put` 先追加 WAL，再写 MemTable；flush 时将有序用户 key 写入 SSTable；重启时通过 WAL replay 恢复未落盘数据。这个流程解释了 LevelDB 为什么写入快：写入主要是顺序追加和内存结构更新，随机 IO 被推迟到后台 compaction。

## 验证命令

```powershell
cmake -S CPP-Practice/leveldb_mini -B CPP-Practice/leveldb_mini/build
cmake --build CPP-Practice/leveldb_mini/build --config Release
.\CPP-Practice\leveldb_mini\build\Release\leveldb_mini_demo.exe
```

---