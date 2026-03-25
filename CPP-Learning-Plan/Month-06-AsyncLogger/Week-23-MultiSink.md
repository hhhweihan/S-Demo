# Week 23 — 多 Sink 支持

## 本周目标

将日志库扩展为多目标输出（Sink）架构，支持文件、控制台、网络等多种后端，
并实现 Sink 的热插拔。

---

## Day 1（Mon）— Sink 抽象接口

**预计时间：1 小时**

**任务：**
- [ ] 定义 Sink 抽象基类：
  ```cpp
  class Sink {
  public:
      virtual ~Sink() = default;
      virtual void write(LogLevel lv, const char* data, size_t len) = 0;
      virtual void flush() = 0;
  };
  ```
- [ ] 重构 Logger：持有 `vector<shared_ptr<Sink>>` 代替直接写文件
- [ ] 实现 `Logger::add_sink(sink)` / `Logger::remove_sink(id)` 热插拔
- [ ] 测试：同时注册两个 sink，验证都收到日志

**完成标志：** Logger 可以同时输出到多个目标

---

## Day 2（Tue）— FileSink（滚动文件）

**预计时间：1 小时**

**任务：**
- [ ] 实现 `FileSink`，支持两种滚动策略：
  - **按大小**：`RotatingFileSink`（达到 maxBytes 则重命名旧文件，打开新文件）
  - **按时间**：`DailyFileSink`（每天 00:00 切换新文件）
  ```cpp
  class RotatingFileSink : public Sink {
      std::string base_name_;
      size_t max_bytes_;   // e.g., 100MB
      int    max_files_;   // 保留最近 N 个文件
      // 实现 rotate()：重命名 app.log → app.log.1, app.log.1 → app.log.2 ...
  };
  ```
- [ ] 测试：连续写入超过大小限制，验证文件滚动

**完成标志：** 日志文件正确滚动，历史文件数量限制有效

---

## Day 3（Wed）— ConsoleSink（彩色终端）

**预计时间：1 小时**

**任务：**
- [ ] 实现 `ConsoleSink`：不同级别用 ANSI 颜色区分：
  ```cpp
  // DEBUG: 灰色  INFO: 绿色  WARN: 黄色  ERROR: 红色  FATAL: 红底白字
  static const char* level_colors[] = {
      "\033[90m", "\033[32m", "\033[33m", "\033[31m", "\033[41;37m"
  };
  void write(LogLevel lv, const char* data, size_t len) override {
      fputs(level_colors[int(lv)], stderr);
      fwrite(data, 1, len, stderr);
      fputs("\033[0m\n", stderr);
  }
  ```
- [ ] 检测是否为 TTY（`isatty(fileno(stderr))`），非 TTY 不输出颜色
- [ ] 测试：不同级别日志输出不同颜色

**完成标志：** 终端彩色输出正确，重定向时无乱码

---

## Day 4（Thu）— 过滤器 + 格式化器

**预计时间：1 小时**

**任务：**
- [ ] 为每个 Sink 添加独立级别过滤：
  ```cpp
  class FilterSink : public Sink {
      shared_ptr<Sink> inner_;
      LogLevel min_level_;
  public:
      void write(LogLevel lv, const char* data, size_t len) override {
          if (lv >= min_level_) inner_->write(lv, data, len);
      }
  };
  ```
- [ ] 实现 Formatter 接口，允许自定义输出格式（JSON 格式日志）：
  ```cpp
  class JsonFormatter : public Formatter {
      std::string format(LogLevel lv, const char* file, int line,
                         const std::string& msg) override;
  };
  ```
- [ ] 测试：FileSink 用 JSON 格式，ConsoleSink 用普通格式

**完成标志：** 同一条日志在文件中是 JSON，在控制台是人读格式

---

## Day 5（Fri）— 异步 Sink 包装

**预计时间：1 小时**

**任务：**
- [ ] 实现 `AsyncSinkWrapper`：将任何同步 Sink 包装成异步（内部有后台线程和缓冲队列）
  ```cpp
  class AsyncSinkWrapper : public Sink {
      shared_ptr<Sink> inner_;
      BlockingQueue<LogEntry> queue_;
      std::thread worker_;
  public:
      void write(...) override { queue_.push(entry); }  // 非阻塞
  };
  ```
- [ ] 这与 Week 22 的 AsyncLogger 不同：这是 Sink 级别的异步，更灵活
- [ ] 性能对比：AsyncSinkWrapper vs 直接 AsyncLogger

**完成标志：** AsyncSinkWrapper 包装 FileSink 后性能提升

---

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

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理 Sink 设计的类图（UML）
- [ ] `git commit`，推送
