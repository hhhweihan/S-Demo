#pragma once  // 防止头文件重复包含

#include <cstdint>  // 提供固定宽度整数类型
#include <cstring>  // 提供内存复制函数
#include <future>  // 提供异步结果类型
#include <functional>  // 提供函数对象封装
#include <map>  // 提供有序映射容器
#include <optional>  // 提供可选值类型
#include <sstream>  // 提供字符串流
#include <stdexcept>  // 提供标准异常类型
#include <string>  // 提供字符串类型
#include <unordered_map>  // 提供哈希映射容器
#include <utility>  // 提供移动语义工具
#include <vector>  // 提供动态数组容器

namespace mini_rpc {  // 定义迷你 RPC 命名空间

struct RpcRequest {  // 表示一次 RPC 请求
  std::uint64_t id = 0;  // 请求编号
  std::string service;  // 服务名
  std::string method;  // 方法名
  std::string payload;  // 请求载荷
};  // 结束请求结构体

struct RpcResponse {  // 表示一次 RPC 响应
  std::uint64_t id = 0;  // 响应对应的请求编号
  bool ok = true;  // 调用是否成功
  std::string payload;  // 响应载荷
};  // 结束响应结构体

inline std::string escape(const std::string& value) {  // 转义字段分隔相关字符
  std::string out;  // 保存转义后的结果
  for (char ch : value) {  // 遍历输入字符
    if (ch == '\\' || ch == '|' || ch == '=') out.push_back('\\');  // 特殊字符前添加反斜杠
    out.push_back(ch);  // 写入当前字符
  }  // 结束字符遍历
  return out;  // 返回转义结果
}  // 结束转义函数

inline std::map<std::string, std::string> parse_fields(const std::string& encoded) {  // 解析键值字段串
  std::map<std::string, std::string> fields;  // 保存解析出的字段
  std::string key;  // 当前字段名
  std::string value;  // 当前字段值
  bool reading_key = true;  // 标记当前是否读取字段名
  bool escaped = false;  // 标记当前字符是否被转义
  for (char ch : encoded) {  // 遍历编码字符串
    if (escaped) { (reading_key ? key : value).push_back(ch); escaped = false; continue; }  // 处理已转义字符
    if (ch == '\\') { escaped = true; continue; }  // 遇到反斜杠后转入转义状态
    if (reading_key && ch == '=') { reading_key = false; continue; }  // 等号结束字段名
    if (!reading_key && ch == '|') { fields[key] = value; key.clear(); value.clear(); reading_key = true; continue; }  // 竖线结束字段
    (reading_key ? key : value).push_back(ch);  // 追加普通字符到当前部分
  }  // 结束字段解析循环
  if (!key.empty()) fields[key] = value;  // 保存最后一个字段
  return fields;  // 返回字段映射
}  // 结束字段解析函数

inline std::string encode(const RpcRequest& request) {  // 编码 RPC 请求
  std::ostringstream out;  // 创建输出字符串流
  out << "type=req|id=" << request.id << "|service=" << escape(request.service)  // 写入请求类型、编号和服务名
      << "|method=" << escape(request.method) << "|payload=" << escape(request.payload);  // 写入方法名和载荷
  return out.str();  // 返回编码后的字符串
}  // 结束请求编码函数

inline std::string encode(const RpcResponse& response) {  // 编码 RPC 响应
  std::ostringstream out;  // 创建输出字符串流
  out << "type=resp|id=" << response.id << "|ok=" << (response.ok ? 1 : 0) << "|payload=" << escape(response.payload);  // 写入响应字段
  return out.str();  // 返回编码后的字符串
}  // 结束响应编码函数

inline RpcRequest decode_request(const std::string& bytes) {  // 解码 RPC 请求
  auto fields = parse_fields(bytes);  // 解析请求字段
  return {std::stoull(fields["id"]), fields["service"], fields["method"], fields["payload"]};  // 构造请求对象
}  // 结束请求解码函数

inline RpcResponse decode_response(const std::string& bytes) {  // 解码 RPC 响应
  auto fields = parse_fields(bytes);  // 解析响应字段
  return {std::stoull(fields["id"]), fields["ok"] == "1", fields["payload"]};  // 构造响应对象
}  // 结束响应解码函数

class FrameCodec {  // 提供长度前缀帧编解码
 public:  // 公共接口
  static std::string encode_frame(const std::string& payload) {  // 将载荷编码为长度前缀帧
    std::uint32_t length = static_cast<std::uint32_t>(payload.size());  // 计算载荷长度
    std::string out(sizeof(length), '\0');  // 为长度前缀预留空间
    std::memcpy(out.data(), &length, sizeof(length));  // 写入二进制长度
    out += payload;  // 追加载荷内容
    return out;  // 返回完整帧
  }  // 结束帧编码函数
  void append(const std::string& bytes) { buffer_ += bytes; }  // 追加接收到的字节
  std::optional<std::string> next_frame() {  // 尝试取出下一帧载荷
    if (buffer_.size() < sizeof(std::uint32_t)) return std::nullopt;  // 长度前缀不足则等待更多数据
    std::uint32_t length = 0;  // 保存解析出的帧长度
    std::memcpy(&length, buffer_.data(), sizeof(length));  // 读取长度前缀
    if (buffer_.size() < sizeof(length) + length) return std::nullopt;  // 完整帧不足则等待更多数据
    buffer_.erase(0, sizeof(length));  // 移除长度前缀
    std::string frame = buffer_.substr(0, length);  // 取出帧载荷
    buffer_.erase(0, length);  // 从缓冲区移除已消费载荷
    return frame;  // 返回帧载荷
  }  // 结束取帧函数
 private:  // 私有成员
  std::string buffer_;  // 保存尚未解码的字节缓冲
};  // 结束帧编解码器

class RpcServer {  // 提供本地 RPC 服务端调度
 public:  // 公共接口
  using Handler = std::function<std::string(const std::string&)>;  // 定义业务处理函数类型
  using Interceptor = std::function<bool(const RpcRequest&, RpcResponse&)>;  // 定义请求拦截器类型
  void register_method(std::string service, std::string method, Handler handler) {  // 注册服务方法
    handlers_[service + "." + method] = std::move(handler);  // 以服务名和方法名作为处理器键
  }  // 结束方法注册函数
  void add_interceptor(Interceptor interceptor) { interceptors_.push_back(std::move(interceptor)); }  // 添加请求拦截器
  RpcResponse handle(const RpcRequest& request) const {  // 处理一次 RPC 请求
    RpcResponse response{request.id, true, {}};  // 初始化成功响应
    for (const auto& interceptor : interceptors_) {  // 依次执行拦截器
      if (!interceptor(request, response)) return response;  // 拦截器拒绝时直接返回响应
    }  // 结束拦截器循环
    auto it = handlers_.find(request.service + "." + request.method);  // 查找目标方法处理器
    if (it == handlers_.end()) return {request.id, false, "method not found"};  // 方法不存在时返回失败
    return {request.id, true, it->second(request.payload)};  // 调用处理器并返回成功响应
  }  // 结束请求处理函数
 private:  // 私有成员
  std::unordered_map<std::string, Handler> handlers_;  // 保存方法处理器表
  std::vector<Interceptor> interceptors_;  // 保存请求拦截器列表
};  // 结束 RPC 服务端

class Registry {  // 提供简单服务注册与发现
 public:  // 公共接口
  void register_service(std::string service, std::string endpoint) { endpoints_[std::move(service)].push_back(std::move(endpoint)); }  // 注册服务端点
  std::string discover(const std::string& service) {  // 发现指定服务的端点
    auto& list = endpoints_[service];  // 获取服务端点列表
    if (list.empty()) throw std::runtime_error("service not found");  // 无端点时抛出异常
    std::string endpoint = list[next_[service]++ % list.size()];  // 轮询选择一个端点
    return endpoint;  // 返回发现到的端点
  }  // 结束服务发现函数
 private:  // 私有成员
  std::unordered_map<std::string, std::vector<std::string>> endpoints_;  // 保存服务到端点列表的映射
  std::unordered_map<std::string, std::size_t> next_;  // 保存每个服务的轮询位置
};  // 结束注册中心

class RpcClient {  // 提供面向服务端的 RPC 客户端
 public:  // 公共接口
  explicit RpcClient(const RpcServer& server) : server_(server) {}  // 绑定目标服务端
  RpcResponse call(const std::string& service, const std::string& method, const std::string& payload) {  // 发起同步调用
    RpcRequest request{++next_id_, service, method, payload};  // 构造递增编号请求
    return server_.handle(request);  // 直接交给服务端处理
  }  // 结束同步调用函数
  std::future<RpcResponse> async_call(const std::string& service, const std::string& method, const std::string& payload) {  // 发起异步调用
    return std::async(std::launch::async, [this, service, method, payload] { return call(service, method, payload); });  // 在线程任务中执行同步调用
  }  // 结束异步调用函数
 private:  // 私有成员
  const RpcServer& server_;  // 引用绑定的服务端
  std::uint64_t next_id_ = 0;  // 保存下一个请求编号
};  // 结束 RPC 客户端

}  // namespace mini_rpc