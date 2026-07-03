#pragma once  // 防止头文件被重复包含

#include <atomic>  // 使用原子计数和停止标记
#include <chrono>  // 使用时间戳和等待时间
#include <condition_variable>  // 使用条件变量唤醒后台线程
#include <ctime>  // 使用本地时间转换结构
#include <filesystem>  // 使用日志文件路径和目录创建
#include <fstream>  // 使用文件输出流
#include <iomanip>  // 使用时间格式化输出
#include <memory>  // 使用共享指针管理输出目标
#include <mutex>  // 使用互斥锁保护共享状态
#include <sstream>  // 使用字符串流拼接日志
#include <string>  // 使用字符串保存日志内容
#include <thread>  // 使用后台写日志线程
#include <utility>  // 使用移动语义转移资源
#include <vector>  // 使用动态数组保存消息和输出目标

namespace mini_log {  // 定义迷你日志库命名空间

enum class Level { Debug, Info, Warn, Error, Fatal };  // 定义日志级别枚举

inline const char* level_name(Level level) {  // 将日志级别转换为文本
  switch (level) {  // 按级别选择对应名称
    case Level::Debug: return "DEBUG";  // 返回调试级别名称
    case Level::Info: return "INFO";  // 返回信息级别名称
    case Level::Warn: return "WARN";  // 返回警告级别名称
    case Level::Error: return "ERROR";  // 返回错误级别名称
    case Level::Fatal: return "FATAL";  // 返回致命级别名称
  }  // 结束级别分支
  return "UNKNOWN";  // 返回兜底级别名称
}  // 结束级别名称函数

inline std::string timestamp() {  // 生成秒级时间戳字符串
  const auto now = std::chrono::system_clock::now();  // 获取当前系统时间点
  const auto time = std::chrono::system_clock::to_time_t(now);  // 转换为 time_t
  std::tm tm{};  // 准备本地时间结构
#if defined(_WIN32)  // Windows 使用安全版本本地时间转换
  localtime_s(&tm, &time);  // 转换为 Windows 本地时间
#else  // 非 Windows 使用 POSIX 本地时间转换
  localtime_r(&time, &tm);  // 转换为 POSIX 本地时间
#endif  // 结束平台分支
  std::ostringstream out;  // 准备格式化缓冲
  out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");  // 写入可读时间格式
  return out.str();  // 返回时间戳文本
}  // 结束时间戳函数

inline std::string date_stamp() {  // 生成日志文件日期戳
  const auto now = std::chrono::system_clock::now();  // 获取当前系统时间点
  const auto time = std::chrono::system_clock::to_time_t(now);  // 转换为 time_t
  std::tm tm{};  // 准备本地时间结构
#if defined(_WIN32)  // Windows 使用安全版本本地时间转换
  localtime_s(&tm, &time);  // 转换为 Windows 本地时间
#else  // 非 Windows 使用 POSIX 本地时间转换
  localtime_r(&time, &tm);  // 转换为 POSIX 本地时间
#endif  // 结束平台分支
  std::ostringstream out;  // 准备格式化缓冲
  out << std::put_time(&tm, "%Y%m%d");  // 写入紧凑日期格式
  return out.str();  // 返回日期戳文本
}  // 结束日期戳函数

inline std::string format(Level level, const char* file, int line, const std::string& message) {  // 格式化单条日志
  std::ostringstream out;  // 准备日志拼接缓冲
  out << timestamp() << " [" << level_name(level) << "] [tid=" << std::this_thread::get_id()  // 拼接时间级别和线程号
      << "] " << file << ':' << line << " - " << message << '\n';  // 拼接位置和消息内容
  return out.str();  // 返回完整日志行
}  // 结束格式化函数

class Sink {  // 定义日志输出目标接口
 public:  // 暴露输出目标公共接口
  virtual ~Sink() = default;  // 允许通过基类指针析构
  virtual void write(const std::string& line) = 0;  // 写入一行日志
  virtual void flush() = 0;  // 刷新输出目标
};  // 结束输出目标接口

class MemorySink : public Sink {  // 定义内存日志输出目标
 public:  // 暴露内存输出目标接口
  void write(const std::string& line) override {  // 写入日志到内存数组
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护日志数组
    lines_.push_back(line);  // 保存日志行
  }  // 结束内存写入
  void flush() override {}  // 内存输出无需刷新
  std::size_t size() const {  // 返回已保存日志数量
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取日志数组
    return lines_.size();  // 返回日志行数量
  }  // 结束数量查询
 private:  // 隐藏内部存储成员
  mutable std::mutex mutex_;  // 保护内存日志数组
  std::vector<std::string> lines_;  // 保存日志行内容
};  // 结束内存输出目标

class ConsoleSink : public Sink {  // 定义流式控制台输出目标
 public:  // 暴露控制台输出目标接口
  explicit ConsoleSink(std::ostream& out) : out_(out) {}  // 绑定外部输出流
  void write(const std::string& line) override {  // 写入日志到输出流
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护输出流
    out_ << line;  // 写入日志行
  }  // 结束流写入
  void flush() override {  // 刷新输出流
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护输出流
    out_.flush();  // 刷新底层流缓冲
  }  // 结束流刷新
 private:  // 隐藏输出流成员
  std::ostream& out_;  // 保存外部输出流引用
  std::mutex mutex_;  // 保护输出流并发访问
};  // 结束控制台输出目标

class FileSink : public Sink {  // 定义按日期写入文件的输出目标
 public:  // 暴露文件输出目标接口
  explicit FileSink(std::filesystem::path directory, std::string prefix = "app")  // 构造文件输出目标
      : directory_(std::move(directory)), prefix_(std::move(prefix)) {  // 移动保存目录和前缀
    std::filesystem::create_directories(directory_);  // 确保日志目录存在
    open_for_today();  // 打开当天日志文件
  }  // 结束文件输出目标构造
  void write(const std::string& line) override {  // 写入日志到文件
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护文件流
    open_for_today();  // 确保当前日期文件已打开
    file_ << line;  // 写入日志行
  }  // 结束文件写入
  void flush() override {  // 刷新文件输出
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护文件流
    file_.flush();  // 刷新文件缓冲
  }  // 结束文件刷新
  std::filesystem::path current_path() const { return current_path_; }  // 返回当前日志文件路径
 private:  // 隐藏文件输出内部状态
  void open_for_today() {  // 打开当前日期对应的日志文件
    const std::string today = date_stamp();  // 获取当天日期戳
    if (today == current_date_ && file_.is_open()) return;  // 当前文件可用时直接返回
    current_date_ = today;  // 更新当前日期
    current_path_ = directory_ / (prefix_ + "-" + today + ".log");  // 生成当前日志文件路径
    file_.close();  // 关闭旧文件流
    file_.open(current_path_, std::ios::app);  // 以追加模式打开新文件
  }  // 结束打开文件逻辑
  std::filesystem::path directory_;  // 保存日志目录
  std::string prefix_;  // 保存日志文件名前缀
  std::string current_date_;  // 记录已打开文件的日期
  std::filesystem::path current_path_;  // 记录当前日志文件路径
  std::ofstream file_;  // 保存文件输出流
  std::mutex mutex_;  // 保护文件输出状态
};  // 结束文件输出目标

class SyncLogger {  // 定义同步日志器
 public:  // 暴露同步日志器接口
  void add_sink(std::shared_ptr<Sink> sink) { sinks_.push_back(std::move(sink)); }  // 添加输出目标
  void log(Level level, const char* file, int line, const std::string& message) {  // 同步写入一条日志
    const std::string formatted = format(level, file, line, message);  // 格式化日志行
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护输出目标列表
    for (auto& sink : sinks_) sink->write(formatted);  // 写入所有输出目标
  }  // 结束同步写日志
  void flush() {  // 刷新所有同步输出目标
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护输出目标列表
    for (auto& sink : sinks_) sink->flush();  // 刷新所有输出目标
  }  // 结束同步刷新
 private:  // 隐藏同步日志器状态
  std::mutex mutex_;  // 保护输出目标列表
  std::vector<std::shared_ptr<Sink>> sinks_;  // 保存输出目标集合
};  // 结束同步日志器

class AsyncLogger {  // 定义异步日志器
 public:  // 暴露异步日志器接口
  AsyncLogger() : worker_([this] { run(); }) {}  // 构造时启动后台线程
  ~AsyncLogger() { stop(); }  // 析构时停止后台线程

  void add_sink(std::shared_ptr<Sink> sink) {  // 添加输出目标
    std::lock_guard<std::mutex> lock(sink_mutex_);  // 加锁保护输出目标列表
    sinks_.push_back(std::move(sink));  // 保存输出目标
  }  // 结束添加输出目标

  void log(Level level, const char* file, int line, const std::string& message) {  // 异步提交一条日志
    {  // 限定队列锁作用域
      std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护当前批次
      current_.push_back(format(level, file, line, message));  // 将格式化日志放入当前批次
      ++accepted_;  // 增加已接收计数
    }  // 释放队列锁
    cv_.notify_one();  // 唤醒后台线程
  }  // 结束异步提交日志

  void flush() {  // 等待已接收日志写出并刷新
    cv_.notify_one();  // 主动唤醒后台线程
    while (written_.load() < accepted_.load()) {  // 等待写出数量追上接收数量
      std::this_thread::yield();  // 让出 CPU 等待后台写入
    }  // 结束等待循环
    flush_sinks();  // 刷新所有输出目标
  }  // 结束异步刷新

  void stop() {  // 停止后台日志线程
    bool expected = false;  // 准备原子状态期望值
    if (!stopped_.compare_exchange_strong(expected, true)) return;  // 已停止时直接返回
    cv_.notify_one();  // 唤醒后台线程退出
    if (worker_.joinable()) worker_.join();  // 等待后台线程结束
  }  // 结束停止逻辑

  std::size_t accepted() const { return accepted_.load(); }  // 返回已接收日志数量
  std::size_t written() const { return written_.load(); }  // 返回已写出日志数量

 private:  // 隐藏异步日志器内部实现
  void run() {  // 后台线程主循环
    std::vector<std::string> pending;  // 保存待写出批次
    while (!stopped_.load() || !current_.empty()) {  // 未停止或仍有日志时继续工作
      {  // 限定队列锁作用域
        std::unique_lock<std::mutex> lock(mutex_);  // 加锁等待当前批次
        cv_.wait_for(lock, std::chrono::milliseconds(20), [this] { return stopped_.load() || !current_.empty(); });  // 等待停止或新日志
        current_.swap(pending);  // 交换出待写批次
      }  // 释放队列锁
      write_batch(pending);  // 写出当前批次
      pending.clear();  // 清空待写批次
    }  // 结束后台循环
    flush_sinks();  // 退出前刷新所有输出目标
  }  // 结束后台线程主循环

  void write_batch(const std::vector<std::string>& lines) {  // 批量写出日志行
    if (lines.empty()) return;  // 空批次直接返回
    std::lock_guard<std::mutex> lock(sink_mutex_);  // 加锁保护输出目标列表
    for (const auto& line : lines) {  // 遍历待写日志行
      for (auto& sink : sinks_) sink->write(line);  // 写入每个输出目标
      ++written_;  // 增加已写出计数
    }  // 结束日志行遍历
  }  // 结束批量写出

  void flush_sinks() {  // 刷新所有输出目标
    std::lock_guard<std::mutex> lock(sink_mutex_);  // 加锁保护输出目标列表
    for (auto& sink : sinks_) sink->flush();  // 逐个刷新输出目标
  }  // 结束输出目标刷新

  std::mutex mutex_;  // 保护当前批次队列
  std::condition_variable cv_;  // 通知后台线程有新日志
  std::vector<std::string> current_;  // 保存生产者当前批次
  std::mutex sink_mutex_;  // 保护输出目标集合
  std::vector<std::shared_ptr<Sink>> sinks_;  // 保存输出目标集合
  std::thread worker_;  // 后台写日志线程
  std::atomic<bool> stopped_{false};  // 标记后台线程是否停止
  std::atomic<std::size_t> accepted_{0};  // 记录已接收日志数量
  std::atomic<std::size_t> written_{0};  // 记录已写出日志数量
};  // 结束异步日志器

class LogLine {  // 定义流式日志构造辅助对象
 public:  // 暴露流式日志接口
  LogLine(AsyncLogger& logger, Level level, const char* file, int line)  // 绑定异步日志器
      : logger_(&logger), level_(level), file_(file), line_(line) {}  // 保存异步日志上下文
  LogLine(SyncLogger& logger, Level level, const char* file, int line)  // 绑定同步日志器
      : sync_logger_(&logger), level_(level), file_(file), line_(line) {}  // 保存同步日志上下文
  ~LogLine() {  // 析构时提交拼好的日志
    if (logger_) logger_->log(level_, file_, line_, stream_.str());  // 向异步日志器提交日志
    if (sync_logger_) sync_logger_->log(level_, file_, line_, stream_.str());  // 向同步日志器提交日志
  }  // 结束流式日志提交
  template <typename T>  // 支持任意可流式输出类型
  LogLine& operator<<(const T& value) { stream_ << value; return *this; }  // 追加值到日志流
 private:  // 隐藏流式日志内部状态
  AsyncLogger* logger_ = nullptr;  // 保存异步日志器指针
  SyncLogger* sync_logger_ = nullptr;  // 保存同步日志器指针
  Level level_;  // 保存日志级别
  const char* file_;  // 保存调用处文件名
  int line_;  // 保存调用处行号
  std::ostringstream stream_;  // 保存日志消息拼接流
};  // 结束流式日志辅助对象

}  // namespace mini_log

#define MINI_LOG_INFO(logger) mini_log::LogLine((logger), mini_log::Level::Info, __FILE__, __LINE__)  // 创建信息级别日志行
#define MINI_LOG_WARN(logger) mini_log::LogLine((logger), mini_log::Level::Warn, __FILE__, __LINE__)  // 创建警告级别日志行