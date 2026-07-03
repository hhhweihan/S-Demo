#include <cstdlib>  // 提供 exit 退出函数
#include <iostream>  // 提供标准输入输出流
#include <string>  // 提供 std::string

#include "mini_reactor.h"  // 引入迷你 Reactor 实现

namespace {  // 限定本文件内部测试函数

void expect(bool condition, const std::string& message) {  // 校验测试条件并输出结果
  if (!condition) {  // 条件失败时进入错误路径
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息
    std::exit(1);  // 立即结束进程
  }  // 结束失败分支
  std::cout << "[PASS] " << message << '\n';  // 输出通过信息
}  // 结束断言辅助函数

void test_sticky_packet_buffer() {  // 测试粘包拆包缓冲区
  mini_net::Buffer buffer;  // 创建接收缓冲区
  std::string framed = mini_net::Buffer::frame("hello") + mini_net::Buffer::frame("world");  // 拼接两个长度前缀帧
  buffer.append(framed.substr(0, 6));  // 先写入不完整帧数据
  expect(!buffer.read_frame().has_value(), "Buffer waits for a complete length-prefixed frame");  // 验证半包不会被读出
  buffer.append(framed.substr(6));  // 写入剩余帧数据
  expect(buffer.read_frame().value_or("") == "hello", "Buffer decodes the first complete frame");  // 验证首帧内容
  expect(buffer.read_frame().value_or("") == "world", "Buffer decodes the second complete frame");  // 验证第二帧内容
}  // 结束缓冲区测试

void test_reactor_core() {  // 测试 Reactor 核心调度
  mini_net::EventLoop loop;  // 创建事件循环
  int reads = 0;  // 记录读回调次数
  mini_net::Channel channel(10);  // 创建文件描述符为 10 的通道
  channel.set_read_callback([&] { ++reads; });  // 设置读事件回调
  loop.update(&channel);  // 将通道注册到事件循环
  loop.activate(10);  // 激活对应描述符事件
  expect(loop.loop_once() == 1 && reads == 1, "EventLoop dispatches active Channel read callbacks");  // 验证事件被分发一次

  mini_net::EventLoopThreadPool pool(2);  // 创建包含两个事件循环的线程池模型
  auto& first = pool.next_loop();  // 获取第一个轮询事件循环
  auto& second = pool.next_loop();  // 获取第二个轮询事件循环
  expect(&first != &second && pool.size() == 2, "EventLoopThreadPool rotates loops for one-loop-per-thread design");  // 验证轮转分配
}  // 结束 Reactor 核心测试

void test_http_core() {  // 测试 HTTP 核心处理
  mini_net::HttpServerCore server;  // 创建 HTTP 服务核心
  server.get("/hello", [](const mini_net::HttpRequest& request) { return "hello " + request.path; });  // 注册 GET 路由处理器
  const std::string response = server.handle("GET /hello HTTP/1.1\r\nHost: local\r\n\r\n");  // 处理一条原始 HTTP 请求
  expect(response.find("HTTP/1.1 200 OK") == 0, "HTTP server core responds to GET request");  // 验证响应状态行
  expect(response.find("hello /hello") != std::string::npos, "HTTP response contains handler body");  // 验证响应体内容
}  // 结束 HTTP 核心测试

void test_rpc_core() {  // 测试 RPC 核心处理
  mini_net::RpcServerCore server;  // 创建 RPC 服务核心
  server.register_method("echo", [](const std::string& body) { return body; });  // 注册回显方法
  mini_net::RpcClientCore client(server);  // 创建绑定服务端的客户端核心
  expect(client.call("echo", "payload") == "OK:payload", "RPC core completes one request-response round trip");  // 验证一次请求响应
}  // 结束 RPC 核心测试

}  // namespace

int main() {  // 程序入口
  std::cout << "== Month 08 network reactor demo ==\n";  // 输出演示标题
  test_sticky_packet_buffer();  // 运行缓冲区测试
  test_reactor_core();  // 运行 Reactor 测试
  test_http_core();  // 运行 HTTP 测试
  test_rpc_core();  // 运行 RPC 测试
  std::cout << "All Month 08 network reactor tests passed.\n";  // 输出全部通过信息
  return 0;  // 返回成功状态
}  // 结束主函数