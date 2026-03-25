# Week 32 — HTTP 服务器 + 整月复盘

## 本周目标

在 Reactor 框架上实现完整 HTTP/1.1 服务器，完成整月网络编程综合验收。

---

## Day 1（Mon）— HTTP 协议解析

**预计时间：1 小时**

**任务：**
- [ ] 实现 HTTP 请求解析器（状态机方式）：
  ```cpp
  class HttpRequest {
      enum ParseState { REQUEST_LINE, HEADERS, BODY, DONE };
      ParseState state_ = REQUEST_LINE;
  public:
      bool parse(Buffer& buf);  // 增量解析，返回 true 表示完整请求
      std::string method, path, version;
      std::map<std::string, std::string> headers;
      std::string body;
  };
  ```
- [ ] 处理 `Content-Length` 和 `Transfer-Encoding: chunked`
- [ ] 测试：用原始 TCP 发送 HTTP 请求，验证解析正确

**完成标志：** GET / POST 请求解析正确

---

## Day 2（Tue）— HTTP 响应构造 + 连接管理

**预计时间：1 小时**

**任务：**
- [ ] 实现 `HttpResponse`（状态码、Header、Body 组装）：
  ```cpp
  class HttpResponse {
  public:
      void set_status(int code, const std::string& msg);
      void add_header(const std::string& k, const std::string& v);
      void set_body(const std::string& body, const std::string& mime);
      std::string serialize() const;
  };
  ```
- [ ] 实现 Keep-Alive 连接管理（复用 TCP 连接处理多个请求）
- [ ] 测试：实现 `/echo` 接口（把请求体原样返回）

**完成标志：** curl 能成功访问 /echo 接口

---

## Day 3（Wed）— 路由 + 静态文件服务

**预计时间：1 小时**

**任务：**
- [ ] 实现 Router（前缀树或 map 匹配）：
  ```cpp
  class Router {
      using Handler = function<void(const HttpRequest&, HttpResponse&)>;
      map<string, Handler> routes_;
  public:
      void GET(const string& path, Handler h);
      void POST(const string& path, Handler h);
      bool dispatch(const HttpRequest& req, HttpResponse& resp);
  };
  ```
- [ ] 实现静态文件服务（读取本地文件，设置 MIME 类型）
- [ ] 测试：浏览器访问静态 HTML 文件

**完成标志：** 浏览器能加载包含 CSS/JS 的静态页面

---

## Day 4（Thu）— 连接池压测

**预计时间：1 小时**

**任务：**
- [ ] 用 wrk 或 ab 工具对自己的 HTTP 服务器进行压测：
  ```bash
  wrk -t4 -c100 -d30s http://localhost:8080/echo
  # 关注：QPS、延迟分布、错误率
  ```
- [ ] 对比单线程 Reactor vs 多线程 Reactor（one-loop-per-thread）的 QPS 差距
- [ ] 记录性能数据

**完成标志：** QPS 和延迟数据已记录

---

## Day 5（Fri）— 整月综合 benchmark

**预计时间：1 小时**

**任务：**
- [ ] 验证整月验收标准：
  - [ ] epoll ET 模式正确处理 EAGAIN
  - [ ] Reactor 多线程下无数据竞争（ThreadSanitizer 验证）
  - [ ] HTTP 服务器能处理 10K 并发连接
  - [ ] 压测 QPS 数据已记录
- [ ] ASan + TSan 全套测试通过

**完成标志：** 所有验收标准通过

---

## Day 6（Sat）— 阅读 muduo 网络库源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 muduo `net/TcpServer.cc` / `net/TcpConnection.cc`
- [ ] 回答：
  1. muduo 为什么用 `weak_ptr<TcpConnection>` 而不是 `shared_ptr` 在 Channel 回调中？
  2. muduo 的 `runInLoop` + `queueInLoop` 如何保证跨线程调用的安全？
  3. muduo 如何实现优雅关闭（halfClose）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘 + 规划 Month 9

**预计时间：1 小时**

- [ ] 整理 Reactor 模式的完整设计图（EventLoop/Channel/Poller/TcpConnection）
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**HTTP Server 压测：**
| 模式 | QPS | P99 延迟 | 错误率 |
|------|-----|---------|--------|
| 单线程 Reactor | — | — | — |
| 4线程 Reactor | — | — | — |
