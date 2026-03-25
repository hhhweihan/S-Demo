# Week 21 — 同步日志基础

## 本周目标

构建同步日志系统骨架：日志级别、格式化、流式接口、线程安全写入。

---

## Day 1（Mon）— 日志级别 + 基础结构

**预计时间：1 小时**

**任务：**
- [ ] 定义日志级别和全局 Logger：
  ```cpp
  enum class LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };

  class Logger {
  public:
      static Logger& instance();
      void set_level(LogLevel lv) { level_ = lv; }
      void log(LogLevel lv, const char* file, int line,
               const std::string& msg);
  private:
      LogLevel level_ = LogLevel::INFO;
      std::mutex mu_;
  };
  ```
- [ ] 实现 `log()` 直接 fprintf 到 stderr（先不做文件输出）
- [ ] 测试：手动调用 `Logger::instance().log(INFO, __FILE__, __LINE__, "hello")`

**完成标志：** 能看到带时间戳和文件位置的日志输出

---

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

## Day 3（Wed）— 流式接口（LOG_INFO << msg）

**预计时间：1 小时**

**任务：**
- [ ] 实现 LogStream（临时对象，析构时 flush）：
  ```cpp
  class LogStream {
      std::ostringstream oss_;
      LogLevel level_;
      const char* file_; int line_;
  public:
      LogStream(LogLevel lv, const char* f, int line)
          : level_(lv), file_(f), line_(line) {}
      ~LogStream() {  // 析构时提交日志
          Logger::instance().log(level_, file_, line_, oss_.str());
      }
      template<typename T>
      LogStream& operator<<(const T& v) { oss_ << v; return *this; }
  };

  #define LOG_INFO  LogStream(LogLevel::INFO,  __FILE__, __LINE__)
  #define LOG_ERROR LogStream(LogLevel::ERROR, __FILE__, __LINE__)
  ```
- [ ] 测试：`LOG_INFO << "count=" << count << " time=" << elapsed;`

**完成标志：** 流式接口正确组合多个值

---

## Day 4（Thu）— 文件输出 + 每日滚动

**预计时间：1 小时**

**任务：**
- [ ] 实现 `FileAppender`：
  - 打开文件 `app_20240115.log`（按天命名）
  - 每次 log 调用检查日期是否变化，变化则关闭旧文件打开新文件
  - `fwrite` 写入（比 fprintf 快）
- [ ] 实现日志文件按大小滚动（超过 100MB 切分）
- [ ] 测试：连续写入跨越午夜，验证文件切分

**完成标志：** 文件滚动正确，旧文件保留完整

---

## Day 5（Fri）— 线程安全 + 性能基准

**预计时间：1 小时**

**任务：**
- [ ] 确认互斥锁保护：多线程同时写日志不出现乱行
- [ ] 同步日志性能压测：
  ```cpp
  // 4 个线程，每线程 25 万条日志 = 100 万条总计
  // 记录总耗时和吞吐量（条/秒）
  ```
- [ ] 分析瓶颈：互斥锁竞争 vs IO 写入速度

**完成标志：** 吞吐量数据已记录（这是 Week 22 异步版本的基准对比）

---

## Day 6（Sat）— 阅读 muduo 日志实现

**预计时间：2 小时**

**任务：**
- [ ] 阅读 muduo `base/Logging.h` / `base/Logging.cc`
- [ ] 回答：
  1. muduo 用 `FixedBuffer` 而不是 `ostringstream`，为什么更快？
  2. muduo `Logger` 为什么使用线程局部的时间戳缓存（`t_errnobuf`）？
  3. `FATAL` 级别日志为什么要 abort()？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理同步日志的接口设计，为 Week 22 双缓冲异步改造做准备
- [ ] `git commit`，推送
