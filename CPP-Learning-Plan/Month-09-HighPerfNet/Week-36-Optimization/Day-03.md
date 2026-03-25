## Day 3（Wed）— SO_REUSEPORT 多线程 accept

**预计时间：1 小时**

**任务：**
- [ ] 实现多线程 accept（避免 accept 串行成为瓶颈）：
  ```cpp
  // 每个工作线程创建自己的监听 socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));
  bind(fd, addr, addrlen);
  listen(fd, backlog);
  // 内核将新连接均匀分配给各监听 fd
  ```
- [ ] 对比：单线程 accept vs SO_REUSEPORT 多线程 accept 的 QPS 差距

**完成标志：** SO_REUSEPORT 版本 QPS 随核数线性增长

---

