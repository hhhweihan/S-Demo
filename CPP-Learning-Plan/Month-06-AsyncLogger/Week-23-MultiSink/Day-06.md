## Day 6（Sat）— 阅读 spdlog 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 spdlog `include/spdlog/sinks/` 目录（重点：`base_sink.h`、`rotating_file_sink.h`）
- [ ] 回答：
  1. spdlog 如何用模板参数 `<Mutex>` 区分线程安全版本和非线程安全版本？
  2. spdlog 的 `pattern_formatter` 如何高效格式化（预编译 pattern）？
  3. spdlog 的 `async_logger` 内部用的是什么数据结构（MPSC 无锁队列）？

**完成标志：** 3 个问题有答案

---

