#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mini_net {

class Buffer {
 public:
  void append(const std::string& bytes) { data_ += bytes; }
  std::size_t readable_bytes() const { return data_.size(); }
  std::string retrieve(std::size_t count) {
    count = std::min(count, data_.size());
    std::string result = data_.substr(0, count);
    data_.erase(0, count);
    return result;
  }
  std::optional<std::string> read_frame() {
    if (data_.size() < sizeof(std::uint32_t)) return std::nullopt;
    std::uint32_t length = 0;
    std::memcpy(&length, data_.data(), sizeof(length));
    if (data_.size() < sizeof(length) + length) return std::nullopt;
    data_.erase(0, sizeof(length));
    return retrieve(length);
  }
  static std::string frame(const std::string& payload) {
    std::uint32_t length = static_cast<std::uint32_t>(payload.size());
    std::string out(sizeof(length), '\0');
    std::memcpy(out.data(), &length, sizeof(length));
    out += payload;
    return out;
  }
 private:
  std::string data_;
};

class Channel {
 public:
  using Callback = std::function<void()>;
  explicit Channel(int fd) : fd_(fd) {}
  int fd() const { return fd_; }
  void set_read_callback(Callback cb) { read_callback_ = std::move(cb); }
  void handle_read() { if (read_callback_) read_callback_(); }
 private:
  int fd_;
  Callback read_callback_;
};

class FakePoller {
 public:
  void update(Channel* channel) { channels_[channel->fd()] = channel; }
  void activate(int fd) { active_.push(fd); }
  std::vector<Channel*> poll() {
    std::vector<Channel*> ready;
    while (!active_.empty()) {
      int fd = active_.front();
      active_.pop();
      if (channels_.count(fd)) ready.push_back(channels_[fd]);
    }
    return ready;
  }
 private:
  std::unordered_map<int, Channel*> channels_;
  std::queue<int> active_;
};

class EventLoop {
 public:
  void update(Channel* channel) { poller_.update(channel); }
  void activate(int fd) { poller_.activate(fd); }
  int loop_once() {
    auto active = poller_.poll();
    for (auto* channel : active) channel->handle_read();
    return static_cast<int>(active.size());
  }
 private:
  FakePoller poller_;
};

class EventLoopThreadPool {
 public:
  explicit EventLoopThreadPool(std::size_t count) : loops_(count) {}
  EventLoop& next_loop() {
    EventLoop& loop = loops_[next_++ % loops_.size()];
    return loop;
  }
  std::size_t size() const { return loops_.size(); }
 private:
  std::vector<EventLoop> loops_;
  std::size_t next_ = 0;
};

struct HttpRequest {
  std::string method;
  std::string path;
  std::map<std::string, std::string> headers;
};

class HttpParser {
 public:
  static HttpRequest parse(const std::string& raw) {
    std::istringstream input(raw);
    HttpRequest request;
    std::string version;
    input >> request.method >> request.path >> version;
    std::string line;
    std::getline(input, line);
    while (std::getline(input, line) && line != "\r") {
      if (!line.empty() && line.back() == '\r') line.pop_back();
      const auto colon = line.find(':');
      if (colon != std::string::npos) request.headers[line.substr(0, colon)] = trim(line.substr(colon + 1));
    }
    return request;
  }
 private:
  static std::string trim(std::string value) {
    while (!value.empty() && value.front() == ' ') value.erase(value.begin());
    return value;
  }
};

class HttpServerCore {
 public:
  using Handler = std::function<std::string(const HttpRequest&)>;
  void get(std::string path, Handler handler) { handlers_[std::move(path)] = std::move(handler); }
  std::string handle(const std::string& raw) const {
    HttpRequest request = HttpParser::parse(raw);
    auto it = handlers_.find(request.path);
    if (request.method != "GET" || it == handlers_.end()) return response(404, "Not Found", "not found");
    return response(200, "OK", it->second(request));
  }
 private:
  static std::string response(int code, const std::string& reason, const std::string& body) {
    std::ostringstream out;
    out << "HTTP/1.1 " << code << ' ' << reason << "\r\nContent-Length: " << body.size()
        << "\r\nConnection: close\r\n\r\n" << body;
    return out.str();
  }
  std::unordered_map<std::string, Handler> handlers_;
};

class RpcServerCore {
 public:
  using Method = std::function<std::string(const std::string&)>;
  void register_method(std::string name, Method method) { methods_[std::move(name)] = std::move(method); }
  std::string handle_frame(const std::string& payload) const {
    const auto colon = payload.find(':');
    if (colon == std::string::npos) return "ERR:bad request";
    const std::string method = payload.substr(0, colon);
    const std::string body = payload.substr(colon + 1);
    auto it = methods_.find(method);
    if (it == methods_.end()) return "ERR:not found";
    return "OK:" + it->second(body);
  }
 private:
  std::unordered_map<std::string, Method> methods_;
};

class RpcClientCore {
 public:
  explicit RpcClientCore(const RpcServerCore& server) : server_(server) {}
  std::string call(const std::string& method, const std::string& body) const {
    Buffer inbound;
    inbound.append(Buffer::frame(method + ':' + body));
    auto frame = inbound.read_frame();
    if (!frame) throw std::runtime_error("missing request frame");
    return server_.handle_frame(*frame);
  }
 private:
  const RpcServerCore& server_;
};

}  // namespace mini_net