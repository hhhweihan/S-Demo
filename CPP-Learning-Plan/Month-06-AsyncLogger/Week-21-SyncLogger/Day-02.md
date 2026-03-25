## Day 2（Tue）— 格式化（时间戳 + 线程 ID）

**预计时间：1 小时**

**任务：**
- [ ] 实现日志格式：`[2024-01-15 10:30:45.123] [INFO] [TID:12345] [main.cpp:42] message`
  ```cpp
  // 时间戳：用 gettimeofday / clock_gettime 获取毫秒级时间
  // 线程 ID：syscall(SYS_gettid) 或 pthread_self()
  // 格式化到栈缓冲区（避免 heap 分配）：
  char buf[256];
  snprintf(buf, sizeof(buf), "[%s] [%s] [TID:%lu] [%s:%d] ",
           time_str, level_str, tid, file, line);
  ```
- [ ] 实现文件名截短（只取 `__FILE__` 的最后一段，去掉路径前缀）
- [ ] 测试：多线程同时输出，TID 不同

**完成标志：** 日志格式正确，多线程不串行

---

