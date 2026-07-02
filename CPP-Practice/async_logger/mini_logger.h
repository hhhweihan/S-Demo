#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace mini_log {

enum class Level { Debug, Info, Warn, Error, Fatal };

inline const char* level_name(Level level) {
  switch (level) {
    case Level::Debug: return "DEBUG";
    case Level::Info: return "INFO";
    case Level::Warn: return "WARN";
    case Level::Error: return "ERROR";
    case Level::Fatal: return "FATAL";
  }
  return "UNKNOWN";
}

inline std::string timestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);
  std::tm tm{};
#if defined(_WIN32)
  localtime_s(&tm, &time);
#else
  localtime_r(&time, &tm);
#endif
  std::ostringstream out;
  out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return out.str();
}

inline std::string date_stamp() {
  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);
  std::tm tm{};
#if defined(_WIN32)
  localtime_s(&tm, &time);
#else
  localtime_r(&time, &tm);
#endif
  std::ostringstream out;
  out << std::put_time(&tm, "%Y%m%d");
  return out.str();
}

inline std::string format(Level level, const char* file, int line, const std::string& message) {
  std::ostringstream out;
  out << timestamp() << " [" << level_name(level) << "] [tid=" << std::this_thread::get_id()
      << "] " << file << ':' << line << " - " << message << '\n';
  return out.str();
}

class Sink {
 public:
  virtual ~Sink() = default;
  virtual void write(const std::string& line) = 0;
  virtual void flush() = 0;
};

class MemorySink : public Sink {
 public:
  void write(const std::string& line) override {
    std::lock_guard<std::mutex> lock(mutex_);
    lines_.push_back(line);
  }
  void flush() override {}
  std::size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return lines_.size();
  }
 private:
  mutable std::mutex mutex_;
  std::vector<std::string> lines_;
};

class ConsoleSink : public Sink {
 public:
  explicit ConsoleSink(std::ostream& out) : out_(out) {}
  void write(const std::string& line) override {
    std::lock_guard<std::mutex> lock(mutex_);
    out_ << line;
  }
  void flush() override {
    std::lock_guard<std::mutex> lock(mutex_);
    out_.flush();
  }
 private:
  std::ostream& out_;
  std::mutex mutex_;
};

class FileSink : public Sink {
 public:
  explicit FileSink(std::filesystem::path directory, std::string prefix = "app")
      : directory_(std::move(directory)), prefix_(std::move(prefix)) {
    std::filesystem::create_directories(directory_);
    open_for_today();
  }
  void write(const std::string& line) override {
    std::lock_guard<std::mutex> lock(mutex_);
    open_for_today();
    file_ << line;
  }
  void flush() override {
    std::lock_guard<std::mutex> lock(mutex_);
    file_.flush();
  }
  std::filesystem::path current_path() const { return current_path_; }
 private:
  void open_for_today() {
    const std::string today = date_stamp();
    if (today == current_date_ && file_.is_open()) return;
    current_date_ = today;
    current_path_ = directory_ / (prefix_ + "-" + today + ".log");
    file_.close();
    file_.open(current_path_, std::ios::app);
  }
  std::filesystem::path directory_;
  std::string prefix_;
  std::string current_date_;
  std::filesystem::path current_path_;
  std::ofstream file_;
  std::mutex mutex_;
};

class SyncLogger {
 public:
  void add_sink(std::shared_ptr<Sink> sink) { sinks_.push_back(std::move(sink)); }
  void log(Level level, const char* file, int line, const std::string& message) {
    const std::string formatted = format(level, file, line, message);
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& sink : sinks_) sink->write(formatted);
  }
  void flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& sink : sinks_) sink->flush();
  }
 private:
  std::mutex mutex_;
  std::vector<std::shared_ptr<Sink>> sinks_;
};

class AsyncLogger {
 public:
  AsyncLogger() : worker_([this] { run(); }) {}
  ~AsyncLogger() { stop(); }

  void add_sink(std::shared_ptr<Sink> sink) {
    std::lock_guard<std::mutex> lock(sink_mutex_);
    sinks_.push_back(std::move(sink));
  }

  void log(Level level, const char* file, int line, const std::string& message) {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      current_.push_back(format(level, file, line, message));
      ++accepted_;
    }
    cv_.notify_one();
  }

  void flush() {
    cv_.notify_one();
    while (written_.load() < accepted_.load()) {
      std::this_thread::yield();
    }
    flush_sinks();
  }

  void stop() {
    bool expected = false;
    if (!stopped_.compare_exchange_strong(expected, true)) return;
    cv_.notify_one();
    if (worker_.joinable()) worker_.join();
  }

  std::size_t accepted() const { return accepted_.load(); }
  std::size_t written() const { return written_.load(); }

 private:
  void run() {
    std::vector<std::string> pending;
    while (!stopped_.load() || !current_.empty()) {
      {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait_for(lock, std::chrono::milliseconds(20), [this] { return stopped_.load() || !current_.empty(); });
        current_.swap(pending);
      }
      write_batch(pending);
      pending.clear();
    }
    flush_sinks();
  }

  void write_batch(const std::vector<std::string>& lines) {
    if (lines.empty()) return;
    std::lock_guard<std::mutex> lock(sink_mutex_);
    for (const auto& line : lines) {
      for (auto& sink : sinks_) sink->write(line);
      ++written_;
    }
  }

  void flush_sinks() {
    std::lock_guard<std::mutex> lock(sink_mutex_);
    for (auto& sink : sinks_) sink->flush();
  }

  std::mutex mutex_;
  std::condition_variable cv_;
  std::vector<std::string> current_;
  std::mutex sink_mutex_;
  std::vector<std::shared_ptr<Sink>> sinks_;
  std::thread worker_;
  std::atomic<bool> stopped_{false};
  std::atomic<std::size_t> accepted_{0};
  std::atomic<std::size_t> written_{0};
};

class LogLine {
 public:
  LogLine(AsyncLogger& logger, Level level, const char* file, int line)
      : logger_(&logger), level_(level), file_(file), line_(line) {}
  LogLine(SyncLogger& logger, Level level, const char* file, int line)
      : sync_logger_(&logger), level_(level), file_(file), line_(line) {}
  ~LogLine() {
    if (logger_) logger_->log(level_, file_, line_, stream_.str());
    if (sync_logger_) sync_logger_->log(level_, file_, line_, stream_.str());
  }
  template <typename T>
  LogLine& operator<<(const T& value) { stream_ << value; return *this; }
 private:
  AsyncLogger* logger_ = nullptr;
  SyncLogger* sync_logger_ = nullptr;
  Level level_;
  const char* file_;
  int line_;
  std::ostringstream stream_;
};

}  // namespace mini_log

#define MINI_LOG_INFO(logger) mini_log::LogLine((logger), mini_log::Level::Info, __FILE__, __LINE__)
#define MINI_LOG_WARN(logger) mini_log::LogLine((logger), mini_log::Level::Warn, __FILE__, __LINE__)