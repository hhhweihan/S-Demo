## Day 3（Wed）— 后台线程写入逻辑

**预计时间：1 小时**

**任务：**
- [ ] 实现后台线程 `backend_thread()`：
  ```cpp
  void AsyncLogger::backend_thread() {
      while (running_) {
          {
              std::unique_lock lock(mu_);
              cv_.wait_for(lock, 3s, [this]{ return !buffers_.empty(); });
              buffers_.push_back(std::move(current_));
              current_ = std::make_unique<Buffer>();
          }
          // 在锁外批量写入文件（IO 不持锁）
          for (auto& buf : buffers_to_write)
              file_.write(buf->data(), buf->used());
          file_.flush();
          buffers_to_write.clear();
      }
  }
  ```
- [ ] 测试：前台 4 线程连续写，后台线程正确 flush

**完成标志：** 无日志丢失，后台线程定时 flush 有效

---

