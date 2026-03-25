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

