## Day 4（Thu）— 过滤器 + 格式化器

**预计时间：1 小时**

**任务：**
- [ ] 为每个 Sink 添加独立级别过滤：
  ```cpp
  class FilterSink : public Sink {
      shared_ptr<Sink> inner_;
      LogLevel min_level_;
  public:
      void write(LogLevel lv, const char* data, size_t len) override {
          if (lv >= min_level_) inner_->write(lv, data, len);
      }
  };
  ```
- [ ] 实现 Formatter 接口，允许自定义输出格式（JSON 格式日志）：
  ```cpp
  class JsonFormatter : public Formatter {
      std::string format(LogLevel lv, const char* file, int line,
                         const std::string& msg) override;
  };
  ```
- [ ] 测试：FileSink 用 JSON 格式，ConsoleSink 用普通格式

**完成标志：** 同一条日志在文件中是 JSON，在控制台是人读格式

---

