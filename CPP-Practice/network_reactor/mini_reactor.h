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

// 长度前缀协议缓冲区：解决 TCP 粘包/半包。帧格式 = uint32 长度 + 载荷。
class Buffer {
 public:
    void append(const std::string& bytes) { data_ += bytes; }
    std::size_t readable_bytes() const { return data_.size(); }
    std::string retrieve(std::size_t count) {
        count = std::min(count, data_.size());  // 夹取防止越界读
        std::string result = data_.substr(0, count);
        data_.erase(0, count);
        return result;
    }
    // 返回 nullopt 表示数据未攒齐，调用方应保留缓冲等待下一次 append，不能丢弃。
    std::optional<std::string> read_frame() {
        if (data_.size() < sizeof(std::uint32_t)) return std::nullopt;  // 连长度字段都没到齐
        std::uint32_t length = 0;
        std::memcpy(&length, data_.data(), sizeof(length));
        if (data_.size() < sizeof(length) + length) return std::nullopt;  // 帧体还没到齐
        data_.erase(0, sizeof(length));
        return retrieve(length);
    }
    static std::string frame(const std::string& payload) {
        std::uint32_t length = static_cast<std::uint32_t>(payload.size());
        std::string out(sizeof(length), '\0');
        std::memcpy(out.data(), &length,
                    sizeof(length));  // 按主机字节序写长度（教学简化，未处理网络序）
        out += payload;
        return out;
    }

 private:
    std::string data_;
};

// Reactor 中一个 fd 的事件与回调的绑定单元。
class Channel {
 public:
    using Callback = std::function<void()>;
    explicit Channel(int fd) : fd_(fd) {}
    int fd() const { return fd_; }
    void set_read_callback(Callback cb) { read_callback_ = std::move(cb); }
    void handle_read() {
        if (read_callback_) read_callback_();
    }

 private:
    int fd_;
    Callback read_callback_;
};

// 用队列替代真正的 epoll/kqueue，让核心调度逻辑可在无内核依赖下单测。
class FakePoller {
 public:
    void update(Channel* channel) { channels_[channel->fd()] = channel; }
    void activate(int fd) { active_.push(fd); }
    std::vector<Channel*> poll() {
        std::vector<Channel*> ready;
        while (!active_.empty()) {
            int fd = active_.front();
            active_.pop();
            if (channels_.count(fd)) ready.push_back(channels_[fd]);  // 只分发已注册的 fd
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

// one-loop-per-thread 模型：每个 loop 归属一个线程，连接落到某个 loop 后就固定在该线程处理，
// 因此单个 loop 内部无需加锁。next_loop 用轮转做负载均衡。
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

// 只解析请求行 + 头部的极简 HTTP 解析器（不处理 body/分块编码）。
class HttpParser {
 public:
    static HttpRequest parse(const std::string& raw) {
        std::istringstream input(raw);
        HttpRequest request;
        std::string version;
        input >> request.method >> request.path >> version;
        std::string line;
        std::getline(input, line);                                      // 吃掉请求行末尾换行
        while (std::getline(input, line) && line != "\r") {             // 空行（仅 \r）标志头部结束
            if (!line.empty() && line.back() == '\r') line.pop_back();  // 兼容 CRLF 行尾
            const auto colon = line.find(':');
            if (colon != std::string::npos)
                request.headers[line.substr(0, colon)] = trim(line.substr(colon + 1));
        }
        return request;
    }

 private:
    static std::string trim(std::string value) {
        while (!value.empty() && value.front() == ' ')
            value.erase(value.begin());  // 去掉 ": " 后的前导空格
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
        if (request.method != "GET" || it == handlers_.end())
            return response(404, "Not Found", "not found");
        return response(200, "OK", it->second(request));
    }

 private:
    static std::string response(int code, const std::string& reason, const std::string& body) {
        std::ostringstream out;
        // Content-Length 必填，否则短连接下客户端无法判断 body 边界；Connection: close
        // 表示用完即关。
        out << "HTTP/1.1 " << code << ' ' << reason << "\r\nContent-Length: " << body.size()
            << "\r\nConnection: close\r\n\r\n"
            << body;
        return out.str();
    }
    std::unordered_map<std::string, Handler> handlers_;
};

// RPC 载荷约定为 "method:body"，响应加 "OK:"/"ERR:" 前缀区分成败。
class RpcServerCore {
 public:
    using Method = std::function<std::string(const std::string&)>;
    void register_method(std::string name, Method method) {
        methods_[std::move(name)] = std::move(method);
    }
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
        // 走一遍 Buffer 编帧/读帧，模拟真实网络收发时的粘包处理路径。
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
