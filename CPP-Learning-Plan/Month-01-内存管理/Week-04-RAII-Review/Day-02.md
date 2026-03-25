## Day 2（Tue）— defer 宏 + RAII 文件句柄

**预计时间：1 小时**

**任务：**
- [ ] 用 scope_guard 实现 `DEFER` 宏（类似 Go 的 defer）：
  ```cpp
  #define DEFER(code) auto _defer_##__LINE__ = make_scope_guard([&]{ code; })

  // 使用：
  FILE* f = fopen("test.txt", "w");
  DEFER(fclose(f));   // 无论后面怎样，f 一定会被关闭
  ```
- [ ] 实现 `FileGuard`（RAII 封装 FILE*）：
  - 构造时 `fopen`，析构时 `fclose`
  - 禁止拷贝，支持移动
- [ ] 测试：`FileGuard` 构造失败（文件不存在）时正确抛异常，不会 double-close

**完成标志：** DEFER 宏能用，FileGuard 无资源泄漏

---