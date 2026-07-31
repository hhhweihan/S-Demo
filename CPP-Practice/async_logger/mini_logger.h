#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
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

#if !defined(_WIN32)
#include <fcntl.h>
#include <unistd.h>
#endif

namespace mini_log {

enum class Level { Debug, Info, Warn, Error, Fatal };

inline const char* level_name(Level level) {
    switch (level) {
        case Level::Debug:
            return "DEBUG";
        case Level::Info:
            return "INFO";
        case Level::Warn:
            return "WARN";
        case Level::Error:
            return "ERROR";
        case Level::Fatal:
            return "FATAL";
    }
    return "UNKNOWN";
}

inline std::string timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    // localtime 非线程安全（内部静态缓冲），必须用平台各自的可重入版本，否则多线程日志会串。
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
        // sink 会被后台线程与测试线程同时触碰，自带锁保证每个 sink 实现自身线程安全。
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
    ~FileSink() override {
#if !defined(_WIN32)
        if (fd_ >= 0) ::close(fd_);
#endif
    }
    void write(const std::string& line) override {
        std::lock_guard<std::mutex> lock(mutex_);
        open_for_today();  // 跨零点自动滚动到新文件；同一天复用缓存，不再每行重算日期串。
        file_ << line;
    }
    void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);
        // ofstream::flush 只把数据交给 OS page cache，宕机仍会丢；必须再对底层 fd 做
        // fdatasync 强制刷到磁盘，flush() 才真正意味着“已落盘（durable）”。
        file_.flush();
#if !defined(_WIN32)
        if (fd_ >= 0) ::fdatasync(fd_);
#endif
    }
    std::filesystem::path current_path() const { return current_path_; }

 private:
    void open_for_today() {
        const std::time_t now = std::time(nullptr);
        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &now);
#else
        localtime_r(&now, &tm);
#endif
        // 用整数“天号”缓存当前日期：仅当天号变化才重算日期串并重开文件，
        // 避免每行都走 ostringstream + put_time 的昂贵格式化。
        const long day = (tm.tm_year + 1900) * 10000L + (tm.tm_mon + 1) * 100 + tm.tm_mday;
        if (day == current_day_ && file_.is_open()) return;
        current_day_ = day;
        std::ostringstream stamp;  // 仅在换天时执行一次，不再进热路径，可安心用 put_time。
        stamp << std::put_time(&tm, "%Y%m%d");
        current_path_ = directory_ / (prefix_ + "-" + stamp.str() + ".log");
        file_.close();
        file_.open(current_path_, std::ios::app);  // 追加模式：进程重启不覆盖当天已有日志。
#if !defined(_WIN32)
        // 额外持有一个只用于 fsync 的底层 fd（写入仍走 ofstream），跨天滚动时同步重开。
        if (fd_ >= 0) ::close(fd_);
        fd_ = ::open(current_path_.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644);
#endif
    }
    std::filesystem::path directory_;
    std::string prefix_;
    long current_day_ = -1;
    std::filesystem::path current_path_;
    std::ofstream file_;
#if !defined(_WIN32)
    int fd_ = -1;
#endif
    std::mutex mutex_;
};

class SyncLogger {
 public:
    void add_sink(std::shared_ptr<Sink> sink) { sinks_.push_back(std::move(sink)); }
    void log(Level level, const char* file, int line, const std::string& message) {
        // 同步日志在调用线程内直接落盘：实现简单但写入延迟计入热路径。
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
    // 队列满时的溢出策略：Block=背压阻塞生产端（不丢行），Drop=直接丢弃新日志（不阻塞）。
    enum class Overflow { Block, Drop };

    struct Options {
        std::size_t max_pending = 8192;      // 队列上限（待落盘行数）；0 表示无界。
        Overflow overflow = Overflow::Block;  // 默认背压，优先保证不丢行。
    };

    AsyncLogger() : worker_([this] { run(); }) {}
    explicit AsyncLogger(Options options) : options_(options), worker_([this] { run(); }) {}
    ~AsyncLogger() { stop(); }

    void add_sink(std::shared_ptr<Sink> sink) {
        std::lock_guard<std::mutex> lock(sink_mutex_);
        sinks_.push_back(std::move(sink));
    }

    void log(Level level, const char* file, int line, const std::string& message) {
        // 先在锁外完成昂贵的格式化（timestamp→localtime_r+ostringstream），
        // 临界区只保留“入队 + 计数”，避免所有生产端在格式化阶段互相串行。
        std::string formatted = format(level, file, line, message);
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (options_.max_pending != 0 && current_.size() >= options_.max_pending) {
                if (options_.overflow == Overflow::Drop) {
                    ++dropped_;  // 丢弃策略：队列已满则直接丢新日志，不阻塞热路径。
                    return;
                }
                // 阻塞策略：等后台线程腾出空间再入队，代价是生产端被背压、但绝不丢行。
                not_full_.wait(lock, [this] {
                    return current_.size() < options_.max_pending || stopped_.load();
                });
                if (stopped_.load() && current_.size() >= options_.max_pending) {
                    ++dropped_;  // 已在停机途中且仍满：只能丢弃，避免生产端永久阻塞。
                    return;
                }
            }
            current_.push_back(std::move(formatted));
            ++accepted_;
            ++queued_;  // 原子队列深度：既供背压判断，也让 run() 循环条件无需在锁外读 current_。
        }
        cv_.notify_one();
    }

    void flush() {
        cv_.notify_one();
        // 忙等到 written 追平 accepted，确保调用者看到所有已提交日志都已落盘（无丢行）。
        while (written_.load() < accepted_.load()) {
            std::this_thread::yield();
        }
        flush_sinks();
    }

    void stop() {
        // CAS 保证 stop 幂等：析构与显式 stop 都可能调用，只有第一次真正 join 后台线程。
        bool expected = false;
        if (!stopped_.compare_exchange_strong(expected, true)) return;
        cv_.notify_one();
        not_full_.notify_all();  // 唤醒可能因背压阻塞的生产端，避免停机时死等。
        if (worker_.joinable()) worker_.join();
    }

    std::size_t accepted() const { return accepted_.load(); }
    std::size_t written() const { return written_.load(); }
    std::size_t dropped() const { return dropped_.load(); }

 private:
    void run() {
        std::vector<std::string> pending;
        // 停止后仍要把队列里剩余的日志排空再退出，否则关机时会丢尾部日志。
        // 循环条件只读原子 stopped_/queued_，不再在锁外触碰 current_（消除数据竞争）。
        while (!stopped_.load() || queued_.load() != 0) {
            std::size_t swapped = 0;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                // 谓词在锁内求值：入队(log)与停止(stop)改变谓词后都会 notify，故无需超时轮询，
                // 也不会丢唤醒。谓词只读 current_/stopped_，全程持锁，消除对 current_ 的竞争。
                cv_.wait(lock, [this] { return stopped_.load() || !current_.empty(); });
                current_.swap(
                    pending);  // 双缓冲：交换出整批日志，让生产端立刻能继续往空 current_ 写。
                swapped = pending.size();
                queued_.fetch_sub(swapped);  // 与 push 同在 mutex_ 下，队列深度保持精确。
            }
            if (swapped > 0)
                not_full_.notify_all();  // 腾出了队列空间，唤醒被背压阻塞的生产端。
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
            ++written_;  // 逐行累加，flush 的忙等据此判断是否已全部落盘。
        }
    }

    void flush_sinks() {
        std::lock_guard<std::mutex> lock(sink_mutex_);
        for (auto& sink : sinks_) sink->flush();
    }

    Options options_{};
    std::mutex mutex_;  // 保护 current_ 批次，与 sink_mutex_ 分离以缩短生产端持锁时间。
    std::condition_variable cv_;        // 生产端 → 后台线程：有新日志可处理。
    std::condition_variable not_full_;  // 后台线程 → 生产端：队列已腾出空间（背压）。
    std::vector<std::string> current_;
    std::mutex sink_mutex_;  // 独立锁保护 sink 列表，写盘期间不阻塞新日志入队。
    std::vector<std::shared_ptr<Sink>> sinks_;
    std::atomic<bool> stopped_{false};
    std::atomic<std::size_t> accepted_{0};
    std::atomic<std::size_t> written_{0};
    std::atomic<std::size_t> queued_{0};   // 当前待落盘行数（队列深度）。
    std::atomic<std::size_t> dropped_{0};  // Drop 策略下累计丢弃的行数。
    // worker_ 必须是最后一个成员：线程在构造时立即执行 run()，只有排在最后才能保证
    // 上面所有状态（mutex_/atomics/current_ 等）都已初始化完毕，避免启动即数据竞争。
    std::thread worker_;
};

class LogLine {
 public:
    // 同一个 LogLine 用两个互斥指针分别指向异步/同步 logger，让 MINI_LOG_* 宏对两种 logger 通用。
    LogLine(AsyncLogger& logger, Level level, const char* file, int line)
        : logger_(&logger), level_(level), file_(file), line_(line) {}
    LogLine(SyncLogger& logger, Level level, const char* file, int line)
        : sync_logger_(&logger), level_(level), file_(file), line_(line) {}
    ~LogLine() {
        // 关键设计：日志在析构时才提交，因此整条 operator<< 链拼完才作为一行原子写出。
        if (logger_) logger_->log(level_, file_, line_, stream_.str());
        if (sync_logger_) sync_logger_->log(level_, file_, line_, stream_.str());
    }
    template <typename T>
    LogLine& operator<<(const T& value) {
        stream_ << value;
        return *this;
    }

 private:
    AsyncLogger* logger_ = nullptr;
    SyncLogger* sync_logger_ = nullptr;
    Level level_;
    const char* file_;
    int line_;
    std::ostringstream stream_;
};

}  // namespace mini_log

// __FILE__/__LINE__ 必须在宏展开处取值才能记录调用点，所以位置捕获放在宏而非函数里。
#define MINI_LOG_INFO(logger) mini_log::LogLine((logger), mini_log::Level::Info, __FILE__, __LINE__)
#define MINI_LOG_WARN(logger) mini_log::LogLine((logger), mini_log::Level::Warn, __FILE__, __LINE__)