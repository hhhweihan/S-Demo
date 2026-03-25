## Day 4（Thu）— 写 SSTable 解析工具

**预计时间：1 小时**

**任务：**
- [ ] 写一个小工具 `dump_sst.cpp`：
  1. 用 LevelDB 写入 100 个 key-value 对（`db->Put(...)`）
  2. 找到生成的 `.ldb` 文件（在 db 目录里）
  3. 用 `leveldb::Table::Open` 打开，遍历并打印所有 key-value
  ```cpp
  // 参考 table/table.cc 里的 Iterator 用法
  leveldb::Iterator* it = table->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
      std::cout << it->key().ToString() << " -> " << it->value().ToString() << "\n";
  }
  ```

**完成标志：** 工具能正确打印出所有 key-value

---