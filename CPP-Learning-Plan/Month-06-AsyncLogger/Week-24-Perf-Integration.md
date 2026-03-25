# Week 24 — 性能压测 + 日志库整合

## 本周目标

完成日志库的性能压测、与 spdlog/glog 对比，整合所有功能并发布为可复用库。

---

## Day 1（Mon）— 全功能集成测试

**预计时间：1 小时**

**任务：**
- [ ] 集成测试：同时启用 FileSink + ConsoleSink + AsyncSinkWrapper
  ```cpp
  auto logger = Logger::create("app");
  logger->add_sink(make_shared<AsyncSinkWrapper>(
      make_shared<RotatingFileSink>("app.log", 100_MB, 5)));
  logger->add_sink(make_shared<FilterSink>(
      make_shared<ConsoleSink>(), LogLevel::WARN));  // 终端只显示 WARN+
  logger->set_formatter(make_shared<JsonFormatter>());
  ```
- [ ] 测试：多线程混合级别写入，验证文件和终端输出符合预期

**完成标志：** 所有 Sink 协同工作，无乱序、无丢失

---

## Day 2（Tue）— 极限吞吐量压测

**预计时间：1 小时**

**任务：**
- [ ] 压测矩阵（每个场景 100 万条日志）：
  ```
  线程数 × 日志大小：
  [1, 4, 8, 16 线程] × [20B, 100B, 500B 消息]
  ```
- [ ] 测量指标：
  - 总吞吐量（条/秒、MB/秒）
  - 业务线程平均延迟（锁持有时间）
  - P99 延迟
- [ ] 在不同缓冲区大小下（1MB / 4MB / 16MB）测量性能差异

**完成标志：** 压测数据表格已记录

---

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

## Day 4（Thu）— 内存使用分析

**预计时间：1 小时**

**任务：**
- [ ] 使用 Valgrind massif 或 /proc/self/status 追踪日志库内存使用：
  - 空闲时（无日志写入）的内存占用
  - 高压下（缓冲区满）的峰值内存
- [ ] 实现内存上限控制：缓冲队列超过 X 个未处理则丢弃或阻塞
- [ ] 测试：模拟后台线程卡死场景，验证内存不无限增长

**完成标志：** 内存上限机制有效，无 OOM

---

## Day 5（Fri）— 公开 API 设计 + 文档

**预计时间：1 小时**

**任务：**
- [ ] 整理公开头文件（只暴露必要接口）：
  ```
  include/mylog/logger.h      — Logger + LogLevel
  include/mylog/sinks.h       — Sink 基类 + 常用 Sink
  include/mylog/formatters.h  — Formatter 接口
  ```
- [ ] 写使用示例 `examples/basic.cpp` 和 `examples/async.cpp`
- [ ] 写 CMakeLists.txt，支持 `find_package(MyLog)` 引入

**完成标志：** 外部项目能通过 CMake 引入使用

---

## Day 6（Sat）— 月度验收 + 对比 glog

**预计时间：2 小时**

**任务：**
- [ ] 验证月度验收标准：
  - [ ] 异步吞吐量 > 同步 5 倍
  - [ ] 文件按天滚动正常
  - [ ] 与 spdlog 差距 < 2 倍
  - [ ] 业务线程平均延迟 < 1μs
- [ ] 阅读 glog 代码：了解 Google 如何处理 `LOG(FATAL)` 时的栈回溯

**完成标志：** 所有验收标准通过

---

## Day 7（Sun）— 复盘 + 规划 Month 7

**预计时间：1 小时**

- [ ] 写「异步日志库设计总结」（重点：双缓冲设计决策）
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**Month 6 验收：**
| 场景 | 我的实现 | spdlog | 差距 |
|------|---------|--------|------|
| 4线程异步日志 100万条 | — | — | — |
| 同步日志 100万条 | — | — | — |
| 业务线程平均延迟 | — | — | — |
