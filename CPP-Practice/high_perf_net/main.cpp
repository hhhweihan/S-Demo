#include <cstdlib>  // 使用进程退出函数
#include <iostream>  // 使用标准输入输出流
#include <string>  // 使用字符串类型
#include <vector>  // 使用动态数组容器

#include "mini_high_perf_net.h"  // 引入高性能网络示例组件

namespace {  // 限定本文件内部测试工具

void expect(bool condition, const std::string& message) {  // 校验条件并输出测试结果
  if (!condition) {  // 条件失败时进入错误分支
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息
    std::exit(1);  // 立即结束进程
  }  // 失败分支结束
  std::cout << "[PASS] " << message << '\n';  // 输出通过信息
}  // expect 函数结束

void test_timer_wheel() {  // 测试时间轮刷新与过期逻辑
  std::vector<int> expired;  // 保存已过期连接编号
  mini_hpn::TimerWheel wheel(8, [&](int id) { expired.push_back(id); });  // 创建 8 槽时间轮并记录回调
  wheel.add_or_refresh(1, 3);  // 添加连接 1 的初始超时
  wheel.add_or_refresh(2, 5);  // 添加连接 2 的超时
  wheel.add_or_refresh(1, 6);  // 刷新连接 1 的超时
  for (int i = 0; i < 5; ++i) wheel.tick();  // 推进 5 个时钟刻度
  expect(expired == std::vector<int>{2}, "TimerWheel expires idle connections and honors refresh");  // 校验仅连接 2 过期
  wheel.tick();  // 再推进一个刻度
  expect((expired == std::vector<int>{2, 1}), "TimerWheel expires refreshed connection at the latest tick");  // 校验连接 1 在刷新后过期
}  // test_timer_wheel 结束

void test_precision_heap() {  // 测试精确定时器堆
  mini_hpn::PrecisionTimerHeap heap;  // 创建定时器小根堆
  heap.add(1, 10);  // 添加较晚过期的定时器
  heap.add(2, 3);  // 添加最早过期的定时器
  heap.add(3, 7);  // 添加当前截止的定时器
  expect((heap.pop_expired(7) == std::vector<int>{2, 3}), "PrecisionTimerHeap pops timers by exact deadline");  // 校验按截止时间弹出
}  // test_precision_heap 结束

void test_io_uring_simulator() {  // 测试 io_uring 模拟器
  mini_hpn::IoUringSimulator ring;  // 创建提交/完成队列模拟器
  ring.submit({1, "read", "file"});  // 提交读请求
  ring.submit({2, "send", "socket"});  // 提交发送请求
  expect(ring.pending() == 2, "IoUringSimulator queues SQ entries");  // 校验提交队列长度
  ring.process();  // 处理所有提交请求
  auto first = ring.peek_completion();  // 取出第一个完成项
  auto second = ring.peek_completion();  // 取出第二个完成项
  expect(first && first->result == "read:file", "IoUringSimulator produces CQ completion for read");  // 校验读完成结果
  expect(second && second->result == "send:socket", "IoUringSimulator produces CQ completion for send");  // 校验发送完成结果
}  // test_io_uring_simulator 结束

void test_connection_pool() {  // 测试连接池与负载均衡
  mini_hpn::ConnectionPool pool({{"a"}, {"b"}, {"c"}});  // 创建包含三个后端的连接池
  expect(pool.acquire_round_robin() == "a", "RoundRobin picks first backend");  // 校验轮询选中第一个后端
  expect(pool.acquire_round_robin() == "b", "RoundRobin rotates backend");  // 校验轮询切换到下一个后端
  pool.release("a");  // 释放后端 a 的连接
  pool.mark_unhealthy("b");  // 将后端 b 标记为不可用
  std::string chosen = pool.acquire_least_conn();  // 使用最少连接策略选择后端
  expect(chosen != "b", "LeastConn skips unhealthy backend");  // 校验跳过不可用后端
}  // test_connection_pool 结束

void test_optimization_report() {  // 测试平台优化建议
#if defined(__linux__)  // Linux 平台分支
  constexpr bool linux_like = true;  // Linux 平台支持相关优化
#else  // 非 Linux 平台分支
  constexpr bool linux_like = false;  // 非 Linux 平台关闭相关优化
#endif  // 平台判断结束
  auto report = mini_hpn::analyze_platform(linux_like);  // 生成平台分析报告
  expect(report.recommendation.find(linux_like ? "sendfile" : "WSL2") != std::string::npos, "OptimizationReport reflects platform benchmark limits");  // 校验建议内容匹配平台
}  // test_optimization_report 结束

}  // namespace

int main() {  // 程序入口
  std::cout << "== Month 09 high performance networking demo ==\n";  // 输出测试标题
  test_timer_wheel();  // 运行时间轮测试
  test_precision_heap();  // 运行精确定时堆测试
  test_io_uring_simulator();  // 运行 io_uring 模拟器测试
  test_connection_pool();  // 运行连接池测试
  test_optimization_report();  // 运行平台优化报告测试
  std::cout << "All Month 09 high performance networking tests passed.\n";  // 输出全部通过提示
  return 0;  // 返回成功状态码
}  // main 结束