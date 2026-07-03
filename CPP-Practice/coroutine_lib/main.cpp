#include <chrono>  // 引入时间相关类型
#include <cstdlib>  // 引入进程退出函数
#include <iostream>  // 引入标准输入输出流
#include <string>  // 引入字符串类型
#include <vector>  // 引入动态数组容器

#include "mini_coroutine.h"  // 引入迷你协程库接口

namespace {  // 使用匿名命名空间限制本文件符号可见性

void expect(bool condition, const std::string& message) {  // 检查测试条件并输出结果
  if (!condition) {  // 条件失败时进入错误处理
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息
    std::exit(1);  // 以失败状态终止程序
  }  // 结束失败分支
  std::cout << "[PASS] " << message << '\n';  // 输出通过信息
}  // 结束测试断言函数

mini_coro::Task increment_twice(int& value) {  // 创建将整数递增两次的协程任务
  ++value;  // 第一次递增共享整数
  co_await mini_coro::yield();  // 主动让出执行权等待调度器恢复
  ++value;  // 恢复后第二次递增共享整数
}  // 结束递增协程任务

mini_coro::Task produce(mini_coro::Channel<int>& channel) {  // 创建向通道写入两个整数的生产者协程
  channel.send(1);  // 发送第一个整数
  co_await mini_coro::yield();  // 让出执行权以模拟异步切换
  channel.send(2);  // 恢复后发送第二个整数
}  // 结束生产者协程任务

void test_task_scheduler() {  // 测试调度器能恢复多个协程任务
  mini_coro::Scheduler scheduler;  // 创建本次测试使用的调度器
  mini_coro::SchedulerScope scope(scheduler);  // 将调度器绑定到当前线程作用域
  int value = 0;  // 初始化共享计数值
  auto task1 = increment_twice(value);  // 创建第一个递增任务
  auto task2 = increment_twice(value);  // 创建第二个递增任务
  task1.start(scheduler);  // 将第一个任务加入调度队列
  task2.start(scheduler);  // 将第二个任务加入调度队列
  scheduler.run();  // 运行调度器直到队列清空
  expect(value == 4, "Scheduler resumes multiple coroutine Tasks through yield");  // 校验两个任务共完成四次递增
}  // 结束任务调度测试

void test_channel_select() {  // 测试通道选择和缓冲传递
  mini_coro::Scheduler scheduler;  // 创建本次测试使用的调度器
  mini_coro::SchedulerScope scope(scheduler);  // 将调度器设为当前线程调度器
  mini_coro::Channel<int> first(2);  // 创建容量为二的第一个整数通道
  mini_coro::Channel<int> second(2);  // 创建容量为二的第二个整数通道
  second.send(42);  // 预先向第二个通道写入测试值
  std::vector<mini_coro::Channel<int>*> channels{&first, &second};  // 按顺序保存待选择的通道指针
  expect(mini_coro::select_recv(channels).value_or(0) == 42, "select_recv receives from the first ready Channel");  // 校验选择函数取到首个可读通道值

  auto task = produce(first);  // 创建向第一个通道生产数据的任务
  task.start(scheduler);  // 将生产者任务加入调度器
  scheduler.run();  // 执行生产者任务直到完成
  expect(first.recv().value_or(0) == 1 && first.recv().value_or(0) == 2, "Buffered Channel transports coroutine messages");  // 校验缓冲通道按顺序传输消息
}  // 结束通道选择测试

void test_async_http_core() {  // 测试异步 HTTP 核心模拟请求
  mini_coro::Scheduler scheduler;  // 创建本次测试使用的调度器
  mini_coro::SchedulerScope scope(scheduler);  // 将调度器绑定到当前线程
  mini_coro::AsyncHttpClientCore client;  // 创建异步 HTTP 客户端核心对象
  mini_coro::Channel<std::string> results(4);  // 创建用于收集响应文本的通道
  auto a = client.get("/a", results);  // 创建第一个 GET 请求任务
  auto b = client.get("/b", results);  // 创建第二个 GET 请求任务
  a.start(scheduler);  // 启动第一个请求任务
  b.start(scheduler);  // 启动第二个请求任务
  scheduler.run();  // 运行调度器处理两个请求
  expect(results.size() == 2, "AsyncHttpClientCore completes multiple coroutine requests");  // 校验两个请求都写入了结果
}  // 结束异步 HTTP 核心测试

}  // namespace

int main() {  // 程序入口，运行协程库演示测试
  std::cout << "== Month 10 coroutine library demo ==\n";  // 输出演示标题
  test_task_scheduler();  // 执行任务调度测试
  test_channel_select();  // 执行通道选择测试
  test_async_http_core();  // 执行异步 HTTP 核心测试
  std::cout << "All Month 10 coroutine library tests passed.\n";  // 输出全部测试通过提示
  return 0;  // 返回成功退出码
}  // 结束程序入口函数