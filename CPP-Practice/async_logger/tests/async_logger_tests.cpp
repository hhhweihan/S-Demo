#include "mini_logger.h"

#include <atomic>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>
#include <cstddef>
#include <memory>
#include <mutex>

namespace {

using mini_log::AsyncLogger;
using mini_log::Level;
using mini_log::MemorySink;
using mini_log::Sink;
using mini_log::SyncLogger;

// Captures full formatted lines so tests can assert on content, not just count.
// MemorySink only exposes size(); torn/lost-line detection needs the payloads.
class CaptureSink : public Sink {
 public:
    void write(const std::string& line) override {
        std::lock_guard<std::mutex> lock(mutex_);
        lines_.push_back(line);
    }
    void flush() override {}
    std::vector<std::string> snapshot() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return lines_;
    }

 private:
    mutable std::mutex mutex_;
    std::vector<std::string> lines_;
};

// Everything after the " - " separator up to the trailing newline is the message.
std::string extract_payload(const std::string& line) {
    const auto pos = line.find(" - ");
    if (pos == std::string::npos) return {};
    std::string rest = line.substr(pos + 3);
    if (!rest.empty() && rest.back() == '\n') rest.pop_back();
    return rest;
}

}  // namespace

TEST(SyncLogger, DeliversEveryLineToSink) {
    SyncLogger logger;
    auto sink = std::make_shared<MemorySink>();
    logger.add_sink(sink);
    for (int i = 0; i < 50; ++i) logger.log(Level::Info, "t.cpp", i, "line");
    EXPECT_EQ(sink->size(), 50u);
}

TEST(SyncLogger, FormatsLevelAndMessage) {
    SyncLogger logger;
    auto sink = std::make_shared<CaptureSink>();
    logger.add_sink(sink);
    logger.log(Level::Warn, "file.cpp", 42, "hello world");
    const auto lines = sink->snapshot();
    ASSERT_EQ(lines.size(), 1u);
    EXPECT_NE(lines[0].find("[WARN]"), std::string::npos);
    EXPECT_NE(lines[0].find("file.cpp:42"), std::string::npos);
    EXPECT_EQ(extract_payload(lines[0]), "hello world");
    EXPECT_EQ(lines[0].back(), '\n');  // lines must not be torn: each ends complete
}

TEST(SyncLogger, FansOutToEverySink) {
    SyncLogger logger;
    auto a = std::make_shared<MemorySink>();
    auto b = std::make_shared<MemorySink>();
    logger.add_sink(a);
    logger.add_sink(b);
    for (int i = 0; i < 10; ++i) logger.log(Level::Info, "t.cpp", i, "x");
    EXPECT_EQ(a->size(), 10u);
    EXPECT_EQ(b->size(), 10u);
}

TEST(AsyncLogger, FlushDeliversAllAcceptedLines) {
    auto sink = std::make_shared<CaptureSink>();
    AsyncLogger logger;
    logger.add_sink(sink);
    constexpr int kLines = 500;
    for (int i = 0; i < kLines; ++i) logger.log(Level::Info, "t.cpp", i, "msg");
    logger.flush();
    // flush() must block until written catches up to accepted.
    EXPECT_EQ(logger.accepted(), static_cast<std::size_t>(kLines));
    EXPECT_EQ(logger.written(), static_cast<std::size_t>(kLines));
    EXPECT_EQ(sink->snapshot().size(), static_cast<std::size_t>(kLines));
}

TEST(AsyncLogger, DeliversRemainingLinesOnDestruction) {
    auto sink = std::make_shared<CaptureSink>();
    {
        AsyncLogger logger;
        logger.add_sink(sink);
        for (int i = 0; i < 200; ++i) logger.log(Level::Info, "t.cpp", i, "m");
        // No explicit flush: the destructor's stop() must drain the queue.
    }
    EXPECT_EQ(sink->snapshot().size(), 200u);
}

TEST(AsyncLogger, FansOutToEverySink) {
    auto a = std::make_shared<MemorySink>();
    auto b = std::make_shared<MemorySink>();
    AsyncLogger logger;
    logger.add_sink(a);
    logger.add_sink(b);
    for (int i = 0; i < 100; ++i) logger.log(Level::Info, "t.cpp", i, "y");
    logger.flush();
    EXPECT_EQ(a->size(), 100u);
    EXPECT_EQ(b->size(), 100u);
}

TEST(AsyncLogger, StopIsIdempotent) {
    AsyncLogger logger;
    auto sink = std::make_shared<MemorySink>();
    logger.add_sink(sink);
    logger.log(Level::Info, "t.cpp", 1, "z");
    logger.stop();
    logger.stop();  // second call must be a harmless no-op, not a double-join
    EXPECT_EQ(sink->size(), 1u);
}

// The TSan target: N producers race on the queue; every one of N*M lines must
// arrive exactly once and intact (no lost, duplicated, or torn lines).
TEST(AsyncLogger, ConcurrentProducersLoseNoLines) {
    constexpr int kThreads = 8;
    constexpr int kPerThread = 2000;
    auto sink = std::make_shared<CaptureSink>();
    AsyncLogger logger;
    logger.add_sink(sink);

    std::vector<std::thread> producers;
    for (int t = 0; t < kThreads; ++t) {
        producers.emplace_back([&logger, t] {
            for (int i = 0; i < kPerThread; ++i) {
                logger.log(Level::Info, "t.cpp", i,
                           "thr" + std::to_string(t) + "_msg" + std::to_string(i));
            }
        });
    }
    for (auto& p : producers) p.join();
    logger.flush();

    const auto lines = sink->snapshot();
    ASSERT_EQ(lines.size(), static_cast<std::size_t>(kThreads * kPerThread));

    std::map<std::string, int> seen;
    for (const auto& line : lines) {
        EXPECT_EQ(line.back(), '\n');  // intact line ends with its newline
        ++seen[extract_payload(line)];
    }
    EXPECT_EQ(seen.size(), static_cast<std::size_t>(kThreads * kPerThread));
    for (const auto& [payload, count] : seen) {
        EXPECT_EQ(count, 1) << "duplicated or torn: " << payload;
    }
}
