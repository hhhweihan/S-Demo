#include <cstdlib>
#include <iostream>
#include <string>

#include "mini_reactor.h"

namespace {

void expect(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "[FAIL] " << message << '\n';
    std::exit(1);
  }
  std::cout << "[PASS] " << message << '\n';
}

void test_sticky_packet_buffer() {
  mini_net::Buffer buffer;
  std::string framed = mini_net::Buffer::frame("hello") + mini_net::Buffer::frame("world");
  buffer.append(framed.substr(0, 6));
  expect(!buffer.read_frame().has_value(), "Buffer waits for a complete length-prefixed frame");
  buffer.append(framed.substr(6));
  expect(buffer.read_frame().value_or("") == "hello", "Buffer decodes the first complete frame");
  expect(buffer.read_frame().value_or("") == "world", "Buffer decodes the second complete frame");
}

void test_reactor_core() {
  mini_net::EventLoop loop;
  int reads = 0;
  mini_net::Channel channel(10);
  channel.set_read_callback([&] { ++reads; });
  loop.update(&channel);
  loop.activate(10);
  expect(loop.loop_once() == 1 && reads == 1, "EventLoop dispatches active Channel read callbacks");

  mini_net::EventLoopThreadPool pool(2);
  auto& first = pool.next_loop();
  auto& second = pool.next_loop();
  expect(&first != &second && pool.size() == 2, "EventLoopThreadPool rotates loops for one-loop-per-thread design");
}

void test_http_core() {
  mini_net::HttpServerCore server;
  server.get("/hello", [](const mini_net::HttpRequest& request) { return "hello " + request.path; });
  const std::string response = server.handle("GET /hello HTTP/1.1\r\nHost: local\r\n\r\n");
  expect(response.find("HTTP/1.1 200 OK") == 0, "HTTP server core responds to GET request");
  expect(response.find("hello /hello") != std::string::npos, "HTTP response contains handler body");
}

void test_rpc_core() {
  mini_net::RpcServerCore server;
  server.register_method("echo", [](const std::string& body) { return body; });
  mini_net::RpcClientCore client(server);
  expect(client.call("echo", "payload") == "OK:payload", "RPC core completes one request-response round trip");
}

}  // namespace

int main() {
  std::cout << "== Month 08 network reactor demo ==\n";
  test_sticky_packet_buffer();
  test_reactor_core();
  test_http_core();
  test_rpc_core();
  std::cout << "All Month 08 network reactor tests passed.\n";
  return 0;
}