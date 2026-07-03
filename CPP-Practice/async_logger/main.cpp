#include <chrono>  // 使用时间点和耗时统计
#include <cstdlib>  // 使用进程退出接口
#include <filesystem>  // 使用文件路径和存在性检查
#include <iostream>  // 使用标准输入输出流
#include <sstream>  // 使用字符串流缓冲输出
#include <string>  // 使用字符串类型
#include <thread>  // 使用线程并发测试
#include <vector>  // 使用动态数组保存线程

#include "mini_logger.h"  // 引入迷你日志器实现

namespace {  // 限定测试辅助函数的内部链接

void expect(bool condition, const std::string& message) {  // 校验测试条件并打印结果
  if (!condition) {  // 条件失败时进入失败分支
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息
    std::exit(1);  // 立即结束进程
  }  // 结束失败分支
  std::cout << "[PASS] " << message << '\n';  // 输出通过信息
}  // 结束断言辅助函数

void test_sync_logger() {  // 测试同步日志器基础写入
  mini_log::SyncLogger logger;  // 创建同步日志器
  auto memory = std::make_shared<mini_log::MemorySink>();  // 创建内存输出目标
  logger.add_sink(memory);  // 注册内存输出目标
  MINI_LOG_INFO(logger) << "sync value=" << 42;  // 写入一条信息日志
  logger.flush();  // 刷新同步日志输出
  expect(memory->size() == 1, "SyncLogger writes a formatted line through stream interface");  // 校验写入一行
}  // 结束同步日志器测试

void test_async_logger_multithread() {  // 测试异步日志器多线程写入
  mini_log::AsyncLogger logger;  // 创建异步日志器
  auto memory = std::make_shared<mini_log::MemorySink>();  // 创建内存输出目标
  logger.add_sink(memory);  // 注册内存输出目标
  constexpr int thread_count = 4;  // 设置线程数量
  constexpr int per_thread = 2000;  // 设置每个线程写入条数
  std::vector<std::thread> threads;  // 保存工作线程对象
  for (int thread_index = 0; thread_index < thread_count; ++thread_index) {  // 启动多个工作线程
    threads.emplace_back([&logger, thread_index, per_thread] {  // 构造并加入一个写日志线程
      for (int index = 0; index < per_thread; ++index) {  // 在线程内循环写入日志
        MINI_LOG_INFO(logger) << "worker=" << thread_index << " message=" << index;  // 写入带线程和序号的日志
      }  // 结束单线程写入循环
    });  // 结束线程创建
  }  // 结束线程启动循环
  for (auto& thread : threads) thread.join();  // 等待所有线程结束
  logger.flush();  // 等待异步日志全部落入输出目标
  expect(logger.accepted() == thread_count * per_thread, "AsyncLogger accepts all multi-thread messages");  // 校验接收总数
  expect(logger.written() == logger.accepted() && memory->size() == logger.accepted(), "AsyncLogger flushes all messages to sinks");  // 校验写出总数
}  // 结束异步多线程测试

void test_multi_sink_file_roll() {  // 测试多输出目标和文件创建
  const auto directory = std::filesystem::path("CPP-Practice/async_logger/build/logs");  // 设置日志目录
  mini_log::AsyncLogger logger;  // 创建异步日志器
  auto file = std::make_shared<mini_log::FileSink>(directory, "demo");  // 创建文件输出目标
  std::ostringstream console_buffer;  // 创建控制台输出缓冲
  auto console = std::make_shared<mini_log::ConsoleSink>(console_buffer);  // 创建控制台输出目标
  logger.add_sink(file);  // 注册文件输出目标
  logger.add_sink(console);  // 注册控制台输出目标
  MINI_LOG_WARN(logger) << "multi sink message";  // 写入一条警告日志
  logger.flush();  // 刷新所有输出目标
  expect(std::filesystem::exists(file->current_path()), "FileSink creates a date-stamped log file");  // 校验日志文件已创建
  expect(console_buffer.str().find("multi sink message") != std::string::npos, "ConsoleSink receives the same log line");  // 校验控制台缓冲收到日志
}  // 结束多输出目标测试

void test_enqueue_latency_smoke() {  // 测试异步入队耗时烟测
  mini_log::AsyncLogger logger;  // 创建异步日志器
  logger.add_sink(std::make_shared<mini_log::MemorySink>());  // 注册内存输出目标
  constexpr int messages = 10000;  // 设置日志消息数量
  const auto begin = std::chrono::steady_clock::now();  // 记录入队起始时间
  for (int index = 0; index < messages; ++index) {  // 循环写入烟测日志
    MINI_LOG_INFO(logger) << "latency-smoke " << index;  // 写入带序号的日志
  }  // 结束烟测写入循环
  const auto enqueue_end = std::chrono::steady_clock::now();  // 记录入队结束时间
  logger.flush();  // 等待所有日志写出
  const auto enqueue_us = std::chrono::duration_cast<std::chrono::microseconds>(enqueue_end - begin).count();  // 计算入队耗时微秒数
  std::cout << "Async enqueue avg: " << (static_cast<double>(enqueue_us) / messages) << " us/message\n";  // 输出平均入队耗时
  expect(logger.written() == messages, "AsyncLogger performance smoke writes every message");  // 校验所有消息已写出
}  // 结束入队耗时烟测

}  // namespace

int main() {  // 程序入口
  std::cout << "== Month 06 async logger demo ==\n";  // 输出测试标题
  test_sync_logger();  // 执行同步日志器测试
  test_async_logger_multithread();  // 执行异步多线程测试
  test_multi_sink_file_roll();  // 执行多输出目标测试
  test_enqueue_latency_smoke();  // 执行入队耗时烟测
  std::cout << "All Month 06 async logger tests passed.\n";  // 输出全部通过提示
  return 0;  // 返回成功退出码
}  // 结束程序入口