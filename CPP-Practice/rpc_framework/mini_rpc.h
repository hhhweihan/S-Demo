#pragma once

#include <cstdint>
#include <cstring>
#include <functional>
#include <future>
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

// 文本编码用 '|' 分隔字段、'=' 分隔键值，故载荷里的 \ | = 必须转义，
// 否则会被误当作分隔符导致解析错位。
inline std::string escape(const std::string& value) {
    std::string out;
    for (char ch : value) {
        if (ch == '\\' || ch == '|' || ch == '=') out.push_back('\\');
        out.push_back(ch);
    }
    return out;
}

// 手写状态机解析，逐字符处理转义：escaped 标志确保被转义的分隔符按普通字符收入当前字段。
inline std::map<std::string, std::string> parse_fields(const std::string& encoded) {
    std::map<std::string, std::string> fields;
    std::string key;
    std::string value;
    bool reading_key = true;
    bool escaped = false;
    for (char ch : encoded) {
        if (escaped) {
            (reading_key ? key : value).push_back(ch);
            escaped = false;
            continue;
        }  // 转义后一律当字面量
        if (ch == '\\') {
            escaped = true;
            continue;
        }
        if (reading_key && ch == '=') {
            reading_key = false;
            continue;
        }  // 未转义的 = 切到读值
        if (!reading_key && ch == '|') {
            fields[key] = value;
            key.clear();
            value.clear();
            reading_key = true;
            continue;
        }  // 未转义的 | 收一字段
        (reading_key ? key : value).push_back(ch);
    }
    if (!key.empty()) fields[key] = value;  // 末字段后无分隔符，循环外补收
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
    out << "type=resp|id=" << response.id << "|ok=" << (response.ok ? 1 : 0)
        << "|payload=" << escape(response.payload);
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

// 长度前缀帧：区分传输层分帧与应用层字段编码两层职责，解决 TCP 粘包/半包。
class FrameCodec {
 public:
    static std::string encode_frame(const std::string& payload) {
        std::uint32_t length = static_cast<std::uint32_t>(payload.size());
        std::string out(sizeof(length), '\0');
        std::memcpy(out.data(), &length, sizeof(length));  // 主机字节序写入长度（教学简化）
        out += payload;
        return out;
    }
    void append(const std::string& bytes) { buffer_ += bytes; }
    // nullopt = 数据未攒齐，保留缓冲等待后续 append，绝不能丢弃已收字节。
    std::optional<std::string> next_frame() {
        if (buffer_.size() < sizeof(std::uint32_t)) return std::nullopt;  // 长度前缀未到齐
        std::uint32_t length = 0;
        std::memcpy(&length, buffer_.data(), sizeof(length));
        if (buffer_.size() < sizeof(length) + length) return std::nullopt;  // 帧体未到齐
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
        handlers_[service + "." + method] = std::move(handler);  // "service.method" 作复合键
    }
    void add_interceptor(Interceptor interceptor) {
        interceptors_.push_back(std::move(interceptor));
    }
    RpcResponse handle(const RpcRequest& request) const {
        RpcResponse response{request.id, true, {}};
        // 拦截器链：任一返回 false 即短路（鉴权/限流），处理器不再执行。
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
    void register_service(std::string service, std::string endpoint) {
        endpoints_[std::move(service)].push_back(std::move(endpoint));
    }
    std::string discover(const std::string& service) {
        auto& list = endpoints_[service];
        if (list.empty()) throw std::runtime_error("service not found");
        std::string endpoint = list[next_[service]++ % list.size()];  // 每服务独立游标，轮询摊负载
        return endpoint;
    }

 private:
    std::unordered_map<std::string, std::vector<std::string>> endpoints_;
    std::unordered_map<std::string, std::size_t> next_;  // 各服务的轮询位置
};

class RpcClient {
 public:
    explicit RpcClient(const RpcServer& server) : server_(server) {}
    RpcResponse call(const std::string& service, const std::string& method,
                     const std::string& payload) {
        RpcRequest request{++next_id_, service, method,
                           payload};     // 单调递增 id 便于将来匹配请求/响应
        return server_.handle(request);  // 同进程直调，省去真实网络往返
    }
    std::future<RpcResponse> async_call(const std::string& service, const std::string& method,
                                        const std::string& payload) {
        return std::async(std::launch::async, [this, service, method, payload] {
            return call(service, method, payload);
        });  // launch::async 强制新线程，得到真异步
    }

 private:
    const RpcServer& server_;
    std::uint64_t next_id_ = 0;
};

}  // namespace mini_rpc
