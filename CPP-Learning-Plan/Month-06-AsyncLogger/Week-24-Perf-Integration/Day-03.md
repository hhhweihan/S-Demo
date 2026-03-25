## Day 3（Wed）— 与 spdlog 性能对比

**预计时间：1 小时**

**任务：**
- [ ] 安装 spdlog（header-only）
- [ ] 用完全相同的测试条件运行：
  ```
  我的 AsyncLogger  vs  spdlog::async_logger
  我的 SyncLogger   vs  spdlog::basic_logger_mt
  ```
- [ ] 分析差距来源（缓冲区大小？锁策略？格式化速度？）

**完成标志：** 与 spdlog 差距数据已记录，差距 < 2 倍

---

