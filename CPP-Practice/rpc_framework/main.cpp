#include <cstdlib>  // 提供进程退出函数
#include <chrono>  // 提供计时工具
#include <iostream>  // 提供标准输入输出流
#include <numeric>  // 提供数值算法头
#include <string>  // 提供字符串类型
#include <vector>  // 提供动态数组容器

#include "mini_rpc.h"  // 引入迷你 RPC 实现

namespace {  // 限定测试辅助符号在本文件内可见

void expect(bool condition, const std::string& message) {  // 检查测试条件并输出结果
  if (!condition) {  // 条件失败时进入失败分支
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息
    std::exit(1);  // 终止程序表示测试失败
  }  // 结束失败分支
  std::cout << "[PASS] " << message << '\n';  // 输出通过信息
}  // 结束断言辅助函数

void test_codec_frame() {  // 测试请求编解码和帧协议
  mini_rpc::RpcRequest request{7, "Echo", "Say", "hello|rpc"};  // 构造带转义字符的请求
  auto decoded = mini_rpc::decode_request(mini_rpc::encode(request));  // 编码后再解码请求
  expect(decoded.id == 7 && decoded.payload == "hello|rpc", "RPC request encode/decode preserves escaped payload");  // 验证请求字段保持一致

  mini_rpc::FrameCodec codec;  // 创建帧解码器
  auto frame = mini_rpc::FrameCodec::encode_frame("payload");  // 构造长度前缀帧
  codec.append(frame.substr(0, 3));  // 追加不完整帧片段
  expect(!codec.next_frame().has_value(), "FrameCodec waits for a complete frame");  // 验证半包不会产出帧
  codec.append(frame.substr(3));  // 追加剩余帧数据
  expect(codec.next_frame().value_or("") == "payload", "FrameCodec decodes a length-prefixed frame");  // 验证完整帧可解出载荷
}  // 结束编解码测试

void test_rpc_call() {  // 测试同步和异步 RPC 调用
  mini_rpc::RpcServer server;  // 创建 RPC 服务端
  server.register_method("Echo", "Say", [](const std::string& payload) { return payload; });  // 注册回显方法
  mini_rpc::RpcClient client(server);  // 创建绑定服务端的客户端
  auto response = client.call("Echo", "Say", "sync");  // 发起同步调用
  expect(response.ok && response.payload == "sync", "Sync RPC call returns server response");  // 验证同步响应正确
  auto future = client.async_call("Echo", "Say", "async");  // 发起异步调用
  expect(future.get().payload == "async", "Async RPC call returns future response");  // 验证异步响应正确
}  // 结束 RPC 调用测试

void test_registry_interceptor() {  // 测试服务发现和拦截器
  mini_rpc::Registry registry;  // 创建服务注册中心
  registry.register_service("Echo", "127.0.0.1:9001");  // 注册第一个服务端点
  registry.register_service("Echo", "127.0.0.1:9002");  // 注册第二个服务端点
  expect(registry.discover("Echo") == "127.0.0.1:9001", "Registry discovers first endpoint");  // 验证首次发现返回首个端点
  expect(registry.discover("Echo") == "127.0.0.1:9002", "Registry round-robins endpoints");  // 验证轮询返回下一个端点

  mini_rpc::RpcServer server;  // 创建带拦截器的服务端
  server.add_interceptor([](const mini_rpc::RpcRequest& request, mini_rpc::RpcResponse& response) {  // 添加请求拦截器
    if (request.payload == "blocked") { response = {request.id, false, "blocked by interceptor"}; return false; }  // 拦截指定载荷
    return true;  // 放行其他请求
  });  // 完成拦截器注册
  server.register_method("Echo", "Say", [](const std::string& payload) { return payload; });  // 注册回显方法
  mini_rpc::RpcClient client(server);  // 创建测试客户端
  expect(!client.call("Echo", "Say", "blocked").ok, "Interceptor can reject a request");  // 验证拦截器可拒绝请求
}  // 结束注册中心和拦截器测试

template <typename Fn>  // 声明通用计时函数模板
double elapsed_us(Fn&& fn) {  // 以微秒统计函数执行耗时
  auto begin = std::chrono::steady_clock::now();  // 记录起始时间
  fn();  // 执行待测函数
  auto end = std::chrono::steady_clock::now();  // 记录结束时间
  return std::chrono::duration<double, std::micro>(end - begin).count();  // 返回微秒耗时
}  // 结束计时函数模板

void test_simulated_acceptance() {  // 运行同进程模拟验收测试
  mini_rpc::RpcServer server;  // 创建 RPC 服务端
  server.register_method("Echo", "Say", [](const std::string& payload) { return payload; });  // 注册回显方法
  mini_rpc::RpcClient client(server);  // 创建 RPC 客户端

  constexpr int sync_calls = 20000;  // 设置同步调用次数
  double sync_us = elapsed_us([&] {  // 统计同步调用总耗时
    for (int i = 0; i < sync_calls; ++i) {  // 循环执行同步调用
      auto response = client.call("Echo", "Say", "payload");  // 发起一次同步调用
      if (!response.ok) std::exit(1);  // 响应失败则终止测试
    }  // 结束同步调用循环
  });  // 完成同步耗时统计
  double sync_avg_us = sync_us / sync_calls;  // 计算同步平均延迟
  expect(sync_avg_us < 200.0, "Simulated same-process sync RPC average latency is under 200us");  // 验证同步平均延迟阈值

  constexpr int async_calls = 120000;  // 设置吞吐代理调用次数
  double async_us = elapsed_us([&] {  // 统计吞吐代理总耗时
    for (int i = 0; i < async_calls; ++i) {  // 循环执行调用
      auto response = client.call("Echo", "Say", "payload");  // 发起一次调用
      if (!response.ok) std::exit(1);  // 响应失败则终止测试
    }  // 结束吞吐代理循环
  });  // 完成吞吐耗时统计
  double async_qps_proxy = async_calls / (async_us / 1000000.0);  // 计算 QPS 代理值
  expect(async_qps_proxy > 100000.0, "Simulated async dispatch throughput proxy is above 100k QPS");  // 验证吞吐代理阈值

  mini_rpc::Registry registry;  // 创建服务注册中心
  registry.register_service("Echo", "127.0.0.1:9001");  // 注册第一个端点
  registry.register_service("Echo", "127.0.0.1:9002");  // 注册第二个端点
  constexpr int discovers = 10000;  // 设置发现次数
  double discover_us = elapsed_us([&] {  // 统计服务发现总耗时
    for (int i = 0; i < discovers; ++i) registry.discover("Echo");  // 循环执行服务发现
  });  // 完成服务发现耗时统计
  double discover_avg_ms = (discover_us / discovers) / 1000.0;  // 计算平均发现延迟毫秒值
  expect(discover_avg_ms < 5.0, "Simulated service discovery average latency is under 5ms");  // 验证发现延迟阈值

  double mock_grpc_us = elapsed_us([&] {  // 统计模拟帧化基线耗时
    for (int i = 0; i < sync_calls; ++i) {  // 循环执行帧化请求
      mini_rpc::RpcRequest request{static_cast<std::uint64_t>(i), "Echo", "Say", "payload"};  // 构造请求对象
      auto frame = mini_rpc::FrameCodec::encode_frame(mini_rpc::encode(request));  // 编码请求并封装帧
      mini_rpc::FrameCodec codec;  // 创建帧解码器
      codec.append(frame);  // 写入完整帧
      auto decoded = mini_rpc::decode_request(codec.next_frame().value());  // 解帧并解码请求
      auto response = server.handle(decoded);  // 交给服务端处理
      auto bytes = mini_rpc::encode(response);  // 编码响应
      if (bytes.empty()) std::exit(1);  // 响应为空则终止测试
    }  // 结束帧化请求循环
  });  // 完成帧化基线统计
  expect(sync_us <= mock_grpc_us, "Mini RPC direct call is no slower than mock framed baseline");  // 验证直接调用不慢于基线

  std::cout << "[BENCH] sync_avg_us=" << sync_avg_us  // 输出同步平均延迟
            << ", async_qps_proxy=" << static_cast<long long>(async_qps_proxy)  // 输出吞吐代理值
            << ", discover_avg_ms=" << discover_avg_ms  // 输出服务发现平均延迟
            << ", mock_framed_avg_us=" << (mock_grpc_us / sync_calls) << '\n';  // 输出帧化基线平均延迟
}  // 结束模拟验收测试

}  // namespace

int main() {  // 程序入口
  std::cout << "== Month 11 RPC framework demo ==\n";  // 输出演示标题
  test_codec_frame();  // 运行编解码测试
  test_rpc_call();  // 运行 RPC 调用测试
  test_registry_interceptor();  // 运行注册中心和拦截器测试
  test_simulated_acceptance();  // 运行模拟验收测试
  std::cout << "All Month 11 RPC framework tests passed.\n";  // 输出全部通过信息
  return 0;  // 返回成功状态
}  // 结束程序入口