## Day 3（Wed）— gdb 跟踪写入路径

**预计时间：1 小时**

**任务：**
- [ ] 用 debug 版本编译 LevelDB（`-DCMAKE_BUILD_TYPE=Debug`）
- [ ] 写一个小程序，在 `DB::Put` 处打断点，用 gdb 单步跟踪：
  ```
  DB::Put → DBImpl::Write → WAL AddRecord → MemTable::Add → SkipList::Insert
  ```
- [ ] 记录每一步调用栈的关键信息（函数名 + 做了什么）

**完成标志：** 能用文字描述完整的写入调用栈

---