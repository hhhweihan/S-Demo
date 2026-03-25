## Day 1（Mon）— Hook 系统调用原理

**预计时间：1 小时**

**任务：**
- [ ] 理解 hook 原理（LD_PRELOAD 或编译时链接替换）：
  ```cpp
  // 用 dlsym 获取原始系统调用：
  static auto real_read = (ssize_t(*)(int,void*,size_t))dlsym(RTLD_NEXT, "read");

  // 替换 read：
  ssize_t read(int fd, void* buf, size_t len) {
      if (is_in_fiber() && is_nonblocking(fd)) {
          // 注册到 epoll，yield 当前协程
          scheduler.wait_readable(fd);
          // 唤醒后执行真正的 read
      }
      return real_read(fd, buf, len);
  }
  ```
- [ ] 实现 `is_in_fiber()` 检查（通过 TLS 判断当前是否在协程中执行）
- [ ] 测试：hook 后，协程中调用阻塞 read 变为非阻塞

**完成标志：** hook 机制不影响非协程场景，协程中 read 不阻塞线程

---

