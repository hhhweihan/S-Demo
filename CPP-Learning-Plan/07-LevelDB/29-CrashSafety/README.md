# LSM 崩溃安全化（旗舰改造）

## 本阶段目标

把「能跑的教学版 LSM」升级为**崩溃安全**的存储引擎：任何已确认（acked）的写入在进程猝死 / 掉电后
都不丢，崩溃瞬间没写完的尾部被干净丢弃；SSTable 采用真正的块式布局；删除用 tombstone 表达并在
compaction 到最底层时回收。全部在本机 macOS(arm64)/libc++ 上以 POSIX 文件 IO 可完整验证。

## 本阶段完成情况

- [x] 编号 197：CRC32C 记录帧 + fsync + 尾部截断/损坏恢复（WAL 重写）
- [x] 编号 198：块式 SSTable —— data block(前缀压缩+restart 二分) + 持久 bloom + index + footer
- [x] 编号 199：tombstone MemTable —— 二进制 internal key（user_key+tag）、三态查找
- [x] 编号 200：分层 compaction —— L0→L1 归并、最底层 tombstone 回收、单调文件号
- [x] 编号 201：kill-9 持久性测试范式 —— fork/_exit 猝死后重开库 Recover，断言 acked 写幸存

## 本阶段产出

- 代码：`CPP-Practice/leveldb_mini/lsm/`（crc32c / coding / record_log / block / table / memtable /
  version）+ 重写的 `mini_leveldb.h`（WAL 封装 + MiniDB）
- 测试：`CPP-Practice/leveldb_mini/tests/leveldb_mini_tests.cpp`（CRC 向量、块往返、WAL 截断/损坏、
  tombstone 遮蔽、compaction 回收、跨 flush/compaction 恢复、kill-9 持久性）

## 阶段小结

崩溃安全的地基是**先写日志再改内存、写完 fsync 才应答**：Put/Delete 先把记录以 `length+type+crc32c+
payload` 帧追加进 WAL 并 fsync，之后才改 MemTable。回放时逐条校验 CRC，遇首个截断/损坏 record 即停并
丢弃其后全部字节——这正是「只丢崩溃瞬间那条没写完的、保住之前所有已确认写」的语义。SSTable 从文本行
升级为块式二进制：data block 内前缀压缩 + restart 点二分，index block 定位块，footer(magic) 自校验，
bloom 持久化到 meta block；半写文件因 magic 缺失被判 invalid 并在恢复时跳过，其数据仍可由未轮转的 WAL
重放恢复。删除不再原地抹除，而是写 tombstone 遮蔽旧值，直到 compaction 到最底层才真正回收。

关键收获：

1. durability 不是「写了文件」而是「fsync 返回」；macOS 上更要用 `F_FULLFSYNC`（fsync 不保证落盘片）。
2. CRC + 长度双重校验让「截断」和「位翻转」两类损坏都能被识别，且恢复策略统一为「止于首个坏 record」。
3. tombstone 是 LSM 表达删除的唯一正确方式——删除信息必须能落盘、能跨层遮蔽，否则底层旧值会「复活」。
4. 块式布局把「读一个 key」的代价从「扫全表」降到「二分 index + 解析一个块」，也是压缩/缓存/校验的粒度。
5. kill-9 测试（fork 子进程猝死 `_exit`）是验证崩溃安全的黄金范式：不给析构/atexit 任何补救机会。

## 验证命令

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target leveldb_mini_tests -j
ctest --test-dir build -R leveldb_mini --output-on-failure   # 含 kill-9 fork/_exit 崩溃测试

# ASan/UBSan（块/footer 解析的缓冲算术是风险点）
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DS_DEMO_SANITIZER=address
cmake --build build-asan --target leveldb_mini_tests -j
./build-asan/CPP-Practice/leveldb_mini/leveldb_mini_tests
```

---
