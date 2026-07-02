#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "mini_logger.h"

namespace {

void expect(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "[FAIL] " << message << '\n';
    std::exit(1);
  }
  std::cout << "[PASS] " << message << '\n';
}

void test_sync_logger() {
  mini_log::SyncLogger logger;
  auto memory = std::make_shared<mini_log::MemorySink>();
  logger.add_sink(memory);
  MINI_LOG_INFO(logger) << "sync value=" << 42;
  logger.flush();
  expect(memory->size() == 1, "SyncLogger writes a formatted line through stream interface");
}

void test_async_logger_multithread() {
  mini_log::AsyncLogger logger;
  auto memory = std::make_shared<mini_log::MemorySink>();
  logger.add_sink(memory);
  constexpr int thread_count = 4;
  constexpr int per_thread = 2000;
  std::vector<std::thread> threads;
  for (int thread_index = 0; thread_index < thread_count; ++thread_index) {
    threads.emplace_back([&logger, thread_index, per_thread] {
      for (int index = 0; index < per_thread; ++index) {
        MINI_LOG_INFO(logger) << "worker=" << thread_index << " message=" << index;
      }
    });
  }
  for (auto& thread : threads) thread.join();
  logger.flush();
  expect(logger.accepted() == thread_count * per_thread, "AsyncLogger accepts all multi-thread messages");
  expect(logger.written() == logger.accepted() && memory->size() == logger.accepted(), "AsyncLogger flushes all messages to sinks");
}

void test_multi_sink_file_roll() {
  const auto directory = std::filesystem::path("CPP-Practice/async_logger/build/logs");
  mini_log::AsyncLogger logger;
  auto file = std::make_shared<mini_log::FileSink>(directory, "demo");
  std::ostringstream console_buffer;
  auto console = std::make_shared<mini_log::ConsoleSink>(console_buffer);
  logger.add_sink(file);
  logger.add_sink(console);
  MINI_LOG_WARN(logger) << "multi sink message";
  logger.flush();
  expect(std::filesystem::exists(file->current_path()), "FileSink creates a date-stamped log file");
  expect(console_buffer.str().find("multi sink message") != std::string::npos, "ConsoleSink receives the same log line");
}

void test_enqueue_latency_smoke() {
  mini_log::AsyncLogger logger;
  logger.add_sink(std::make_shared<mini_log::MemorySink>());
  constexpr int messages = 10000;
  const auto begin = std::chrono::steady_clock::now();
  for (int index = 0; index < messages; ++index) {
    MINI_LOG_INFO(logger) << "latency-smoke " << index;
  }
  const auto enqueue_end = std::chrono::steady_clock::now();
  logger.flush();
  const auto enqueue_us = std::chrono::duration_cast<std::chrono::microseconds>(enqueue_end - begin).count();
  std::cout << "Async enqueue avg: " << (static_cast<double>(enqueue_us) / messages) << " us/message\n";
  expect(logger.written() == messages, "AsyncLogger performance smoke writes every message");
}

}  // namespace

int main() {
  std::cout << "== Month 06 async logger demo ==\n";
  test_sync_logger();
  test_async_logger_multithread();
  test_multi_sink_file_roll();
  test_enqueue_latency_smoke();
  std::cout << "All Month 06 async logger tests passed.\n";
  return 0;
}