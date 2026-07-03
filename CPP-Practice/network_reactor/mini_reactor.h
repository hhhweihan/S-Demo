#pragma once  // 防止头文件重复包含

#include <algorithm>  // 提供 std::min
#include <cstdint>  // 提供固定宽度整数类型
#include <cstring>  // 提供 std::memcpy
#include <deque>  // 保留双端队列容器支持
#include <functional>  // 提供 std::function
#include <map>  // 提供有序映射容器
#include <optional>  // 提供 std::optional
#include <queue>  // 提供队列容器
#include <sstream>  // 提供字符串流
#include <stdexcept>  // 提供标准异常类型
#include <string>  // 提供字符串类型
#include <unordered_map>  // 提供哈希映射容器
#include <utility>  // 提供 std::move
#include <vector>  // 提供动态数组容器

namespace mini_net {  // 迷你网络库命名空间

class Buffer {  // 长度前缀协议缓冲区
 public:  // 对外提供缓冲区操作接口
  void append(const std::string& bytes) { data_ += bytes; }  // 追加接收到的字节
  std::size_t readable_bytes() const { return data_.size(); }  // 返回当前可读字节数
  std::string retrieve(std::size_t count) {  // 取出指定数量的字节
    count = std::min(count, data_.size());  // 避免读取超过缓冲区长度
    std::string result = data_.substr(0, count);  // 拷贝待返回的数据片段
    data_.erase(0, count);  // 从缓冲区移除已读数据
    return result;  // 返回取出的数据
  }  // 结束取出操作
  std::optional<std::string> read_frame() {  // 尝试读取一个完整帧
    if (data_.size() < sizeof(std::uint32_t)) return std::nullopt;  // 长度字段不足时等待更多数据
    std::uint32_t length = 0;  // 保存帧体长度
    std::memcpy(&length, data_.data(), sizeof(length));  // 从缓冲区拷贝长度字段
    if (data_.size() < sizeof(length) + length) return std::nullopt;  // 帧体不足时等待更多数据
    data_.erase(0, sizeof(length));  // 移除已解析的长度字段
    return retrieve(length);  // 取出并返回完整帧体
  }  // 结束读帧操作
  static std::string frame(const std::string& payload) {  // 将载荷编码为长度前缀帧
    std::uint32_t length = static_cast<std::uint32_t>(payload.size());  // 计算载荷长度
    std::string out(sizeof(length), '\0');  // 预留长度字段空间
    std::memcpy(out.data(), &length, sizeof(length));  // 写入长度字段
    out += payload;  // 拼接载荷内容
    return out;  // 返回完整帧
  }  // 结束帧编码
 private:  // 内部保存缓冲区状态
  std::string data_;  // 保存尚未消费的字节数据
};  // 结束 Buffer 类

class Channel {  // 封装一个可分发事件的通道
 public:  // 对外提供通道操作接口
  using Callback = std::function<void()>;  // 定义无参事件回调类型
  explicit Channel(int fd) : fd_(fd) {}  // 保存通道对应的描述符
  int fd() const { return fd_; }  // 返回通道描述符
  void set_read_callback(Callback cb) { read_callback_ = std::move(cb); }  // 设置读事件回调
  void handle_read() { if (read_callback_) read_callback_(); }  // 有回调时执行读事件处理
 private:  // 内部保存通道状态
  int fd_;  // 通道标识描述符
  Callback read_callback_;  // 读事件回调函数
};  // 结束 Channel 类

class FakePoller {  // 用队列模拟事件轮询器
 public:  // 对外提供轮询器操作接口
  void update(Channel* channel) { channels_[channel->fd()] = channel; }  // 注册或更新通道
  void activate(int fd) { active_.push(fd); }  // 标记描述符为活跃
  std::vector<Channel*> poll() {  // 拉取当前活跃通道
    std::vector<Channel*> ready;  // 保存就绪通道列表
    while (!active_.empty()) {  // 逐个消费活跃描述符
      int fd = active_.front();  // 读取队首描述符
      active_.pop();  // 移除已处理描述符
      if (channels_.count(fd)) ready.push_back(channels_[fd]);  // 注册过的描述符才加入就绪列表
    }  // 结束活跃队列消费
    return ready;  // 返回本轮就绪通道
  }  // 结束轮询操作
 private:  // 内部保存注册和活跃事件
  std::unordered_map<int, Channel*> channels_;  // 描述符到通道的映射
  std::queue<int> active_;  // 待分发的活跃描述符队列
};  // 结束 FakePoller 类

class EventLoop {  // 简化事件循环
 public:  // 对外提供事件循环接口
  void update(Channel* channel) { poller_.update(channel); }  // 向轮询器注册通道
  void activate(int fd) { poller_.activate(fd); }  // 模拟激活某个描述符
  int loop_once() {  // 执行一轮事件分发
    auto active = poller_.poll();  // 获取当前就绪通道
    for (auto* channel : active) channel->handle_read();  // 逐个触发读回调
    return static_cast<int>(active.size());  // 返回处理的通道数量
  }  // 结束单轮循环
 private:  // 内部持有轮询器
  FakePoller poller_;  // 内部模拟轮询器
};  // 结束 EventLoop 类

class EventLoopThreadPool {  // 简化的一线程一循环池模型
 public:  // 对外提供事件循环池接口
  explicit EventLoopThreadPool(std::size_t count) : loops_(count) {}  // 创建指定数量事件循环
  EventLoop& next_loop() {  // 按轮转策略返回下一个事件循环
    EventLoop& loop = loops_[next_++ % loops_.size()];  // 通过取模选择循环
    return loop;  // 返回选中的事件循环
  }  // 结束轮转选择
  std::size_t size() const { return loops_.size(); }  // 返回事件循环数量
 private:  // 内部保存轮转状态
  std::vector<EventLoop> loops_;  // 保存所有事件循环
  std::size_t next_ = 0;  // 下一次选择的轮转下标
};  // 结束 EventLoopThreadPool 类

struct HttpRequest {  // 保存解析后的 HTTP 请求信息
  std::string method;  // 请求方法
  std::string path;  // 请求路径
  std::map<std::string, std::string> headers;  // 请求头集合
};  // 结束 HTTP 请求结构

class HttpParser {  // 简化 HTTP 请求解析器
 public:  // 对外提供解析入口
  static HttpRequest parse(const std::string& raw) {  // 从原始文本解析请求
    std::istringstream input(raw);  // 使用字符串流逐段读取
    HttpRequest request;  // 存放解析结果
    std::string version;  // 临时保存 HTTP 版本
    input >> request.method >> request.path >> version;  // 解析请求行
    std::string line;  // 保存当前头部行
    std::getline(input, line);  // 消费请求行末尾换行
    while (std::getline(input, line) && line != "\r") {  // 逐行读取头部直到空行
      if (!line.empty() && line.back() == '\r') line.pop_back();  // 去掉 Windows 风格回车
      const auto colon = line.find(':');  // 查找头部键值分隔符
      if (colon != std::string::npos) request.headers[line.substr(0, colon)] = trim(line.substr(colon + 1));  // 保存合法头部
    }  // 结束头部解析循环
    return request;  // 返回解析后的请求
  }  // 结束请求解析
 private:  // 内部提供解析辅助函数
  static std::string trim(std::string value) {  // 去掉字符串左侧空格
    while (!value.empty() && value.front() == ' ') value.erase(value.begin());  // 循环删除前导空格
    return value;  // 返回清理后的字符串
  }  // 结束修剪函数
};  // 结束 HttpParser 类

class HttpServerCore {  // 简化 HTTP 服务核心
 public:  // 对外提供路由注册和处理接口
  using Handler = std::function<std::string(const HttpRequest&)>;  // 定义请求处理函数类型
  void get(std::string path, Handler handler) { handlers_[std::move(path)] = std::move(handler); }  // 注册 GET 路由
  std::string handle(const std::string& raw) const {  // 处理原始 HTTP 请求
    HttpRequest request = HttpParser::parse(raw);  // 解析请求文本
    auto it = handlers_.find(request.path);  // 查找路径对应处理器
    if (request.method != "GET" || it == handlers_.end()) return response(404, "Not Found", "not found");  // 未命中时返回 404
    return response(200, "OK", it->second(request));  // 命中时返回处理器结果
  }  // 结束请求处理
 private:  // 内部提供响应构造和路由表
  static std::string response(int code, const std::string& reason, const std::string& body) {  // 构造 HTTP 响应文本
    std::ostringstream out;  // 使用字符串流拼接响应
    out << "HTTP/1.1 " << code << ' ' << reason << "\r\nContent-Length: " << body.size()  // 写入状态行和长度头
        << "\r\nConnection: close\r\n\r\n" << body;  // 写入连接头、空行和响应体
    return out.str();  // 返回响应字符串
  }  // 结束响应构造
  std::unordered_map<std::string, Handler> handlers_;  // 路径到处理器的映射
};  // 结束 HttpServerCore 类

class RpcServerCore {  // 简化 RPC 服务核心
 public:  // 对外提供方法注册和帧处理接口
  using Method = std::function<std::string(const std::string&)>;  // 定义 RPC 方法函数类型
  void register_method(std::string name, Method method) { methods_[std::move(name)] = std::move(method); }  // 注册 RPC 方法
  std::string handle_frame(const std::string& payload) const {  // 处理一帧 RPC 请求载荷
    const auto colon = payload.find(':');  // 查找方法名与正文分隔符
    if (colon == std::string::npos) return "ERR:bad request";  // 缺少分隔符时返回错误
    const std::string method = payload.substr(0, colon);  // 提取方法名
    const std::string body = payload.substr(colon + 1);  // 提取请求正文
    auto it = methods_.find(method);  // 查找注册方法
    if (it == methods_.end()) return "ERR:not found";  // 方法不存在时返回错误
    return "OK:" + it->second(body);  // 调用方法并包装成功响应
  }  // 结束 RPC 帧处理
 private:  // 内部保存 RPC 方法表
  std::unordered_map<std::string, Method> methods_;  // 方法名到处理函数的映射
};  // 结束 RpcServerCore 类

class RpcClientCore {  // 简化 RPC 客户端核心
 public:  // 对外提供客户端调用接口
  explicit RpcClientCore(const RpcServerCore& server) : server_(server) {}  // 绑定一个服务端核心
  std::string call(const std::string& method, const std::string& body) const {  // 发起一次 RPC 调用
    Buffer inbound;  // 创建本地入站缓冲区模拟网络收包
    inbound.append(Buffer::frame(method + ':' + body));  // 编码并写入请求帧
    auto frame = inbound.read_frame();  // 从缓冲区读取完整请求帧
    if (!frame) throw std::runtime_error("missing request frame");  // 读不到完整帧时抛出异常
    return server_.handle_frame(*frame);  // 交给服务端处理并返回结果
  }  // 结束 RPC 调用
 private:  // 内部保存绑定服务端引用
  const RpcServerCore& server_;  // 引用绑定的服务端核心
};  // 结束 RpcClientCore 类

}  // namespace mini_net