## Day 3（Wed）— Buffer（环形缓冲）

**预计时间：1 小时**

**任务：**
- [ ] 实现 `Buffer`：
  ```cpp
  // 布局：[prependable bytes][readable bytes][writable bytes]
  //       ↑read_index_       ↑write_index_
  class Buffer {
      std::vector<char> buf_;
      size_t read_idx_ = 8;   // 留 8 字节给 prepend（用于帧头）
      size_t write_idx_ = 8;
  public:
      void append(const char* data, size_t len);
      size_t readable() const;
      const char* peek() const;     // 返回可读数据起始指针
      void retrieve(size_t len);    // 消费 len 字节
      ssize_t read_fd(int fd);      // 从 fd 读数据，用 readv 避免多次 syscall
  };
  ```
- [ ] `read_fd` 用 `readv` + 栈上临时缓冲区，一次读完所有数据（避免多次 read syscall）

**完成标志：** Buffer 能正确管理读写索引，无越界

---