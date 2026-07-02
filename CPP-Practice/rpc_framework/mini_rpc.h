#pragma once

#include <cstdint>
#include <cstring>
#include <future>
#include <functional>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mini_rpc {

struct RpcRequest {
  std::uint64_t id = 0;
  std::string service;
  std::string method;
  std::string payload;
};

struct RpcResponse {
  std::uint64_t id = 0;
  bool ok = true;
  std::string payload;
};

inline std::string escape(const std::string& value) {
  std::string out;
  for (char ch : value) {
    if (ch == '\\' || ch == '|' || ch == '=') out.push_back('\\');
    out.push_back(ch);
  }
  return out;
}

inline std::map<std::string, std::string> parse_fields(const std::string& encoded) {
  std::map<std::string, std::string> fields;
  std::string key;
  std::string value;
  bool reading_key = true;
  bool escaped = false;
  for (char ch : encoded) {
    if (escaped) { (reading_key ? key : value).push_back(ch); escaped = false; continue; }
    if (ch == '\\') { escaped = true; continue; }
    if (reading_key && ch == '=') { reading_key = false; continue; }
    if (!reading_key && ch == '|') { fields[key] = value; key.clear(); value.clear(); reading_key = true; continue; }
    (reading_key ? key : value).push_back(ch);
  }
  if (!key.empty()) fields[key] = value;
  return fields;
}

inline std::string encode(const RpcRequest& request) {
  std::ostringstream out;
  out << "type=req|id=" << request.id << "|service=" << escape(request.service)
      << "|method=" << escape(request.method) << "|payload=" << escape(request.payload);
  return out.str();
}

inline std::string encode(const RpcResponse& response) {
  std::ostringstream out;
  out << "type=resp|id=" << response.id << "|ok=" << (response.ok ? 1 : 0) << "|payload=" << escape(response.payload);
  return out.str();
}

inline RpcRequest decode_request(const std::string& bytes) {
  auto fields = parse_fields(bytes);
  return {std::stoull(fields["id"]), fields["service"], fields["method"], fields["payload"]};
}

inline RpcResponse decode_response(const std::string& bytes) {
  auto fields = parse_fields(bytes);
  return {std::stoull(fields["id"]), fields["ok"] == "1", fields["payload"]};
}

class FrameCodec {
 public:
  static std::string encode_frame(const std::string& payload) {
    std::uint32_t length = static_cast<std::uint32_t>(payload.size());
    std::string out(sizeof(length), '\0');
    std::memcpy(out.data(), &length, sizeof(length));
    out += payload;
    return out;
  }
  void append(const std::string& bytes) { buffer_ += bytes; }
  std::optional<std::string> next_frame() {
    if (buffer_.size() < sizeof(std::uint32_t)) return std::nullopt;
    std::uint32_t length = 0;
    std::memcpy(&length, buffer_.data(), sizeof(length));
    if (buffer_.size() < sizeof(length) + length) return std::nullopt;
    buffer_.erase(0, sizeof(length));
    std::string frame = buffer_.substr(0, length);
    buffer_.erase(0, length);
    return frame;
  }
 private:
  std::string buffer_;
};

class RpcServer {
 public:
  using Handler = std::function<std::string(const std::string&)>;
  using Interceptor = std::function<bool(const RpcRequest&, RpcResponse&)>;
  void register_method(std::string service, std::string method, Handler handler) {
    handlers_[service + "." + method] = std::move(handler);
  }
  void add_interceptor(Interceptor interceptor) { interceptors_.push_back(std::move(interceptor)); }
  RpcResponse handle(const RpcRequest& request) const {
    RpcResponse response{request.id, true, {}};
    for (const auto& interceptor : interceptors_) {
      if (!interceptor(request, response)) return response;
    }
    auto it = handlers_.find(request.service + "." + request.method);
    if (it == handlers_.end()) return {request.id, false, "method not found"};
    return {request.id, true, it->second(request.payload)};
  }
 private:
  std::unordered_map<std::string, Handler> handlers_;
  std::vector<Interceptor> interceptors_;
};

class Registry {
 public:
  void register_service(std::string service, std::string endpoint) { endpoints_[std::move(service)].push_back(std::move(endpoint)); }
  std::string discover(const std::string& service) {
    auto& list = endpoints_[service];
    if (list.empty()) throw std::runtime_error("service not found");
    std::string endpoint = list[next_[service]++ % list.size()];
    return endpoint;
  }
 private:
  std::unordered_map<std::string, std::vector<std::string>> endpoints_;
  std::unordered_map<std::string, std::size_t> next_;
};

class RpcClient {
 public:
  explicit RpcClient(const RpcServer& server) : server_(server) {}
  RpcResponse call(const std::string& service, const std::string& method, const std::string& payload) {
    RpcRequest request{++next_id_, service, method, payload};
    return server_.handle(request);
  }
  std::future<RpcResponse> async_call(const std::string& service, const std::string& method, const std::string& payload) {
    return std::async(std::launch::async, [this, service, method, payload] { return call(service, method, payload); });
  }
 private:
  const RpcServer& server_;
  std::uint64_t next_id_ = 0;
};

}  // namespace mini_rpc