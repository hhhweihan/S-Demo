# Week 40 — 协程 + 网络 IO 整合

## 本周目标

将协程与 epoll 事件循环整合：hook 阻塞系统调用，实现协程式异步编程（
写同步代码，实际执行异步 IO）。

---

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

## Day 2（Tue）— 协程式 epoll 等待

**预计时间：1 小时**

**任务：**
- [ ] 实现 `wait_readable(fd)` 和 `wait_writable(fd)`：
  ```cpp
  void Scheduler::wait_readable(int fd) {
      // 将 fd 注册到 epoll，并关联当前 Fiber
      epoll_event ev;
      ev.data.ptr = current_fiber_;
      epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
      // yield 当前 Fiber（让出 CPU）
      yield_current();
  }

  // EventLoop 中：
  void event_loop() {
      epoll_event events[1024];
      while (running_) {
          int n = epoll_wait(epoll_fd_, events, 1024, timeout);
          for (int i = 0; i < n; i++) {
              auto* fiber = (Fiber*)events[i].data.ptr;
              scheduler.resume(fiber);  // 唤醒等待的协程
          }
          // 调度就绪协程
          scheduler.run_pending();
      }
  }
  ```
- [ ] 测试：协程异步 TCP 连接（connect 不阻塞线程）

**完成标志：** 协程在 IO 等待期间不占用线程

---

## Day 3（Wed）— 协程式 sleep

**预计时间：1 小时**

**任务：**
- [ ] hook `sleep` / `usleep` / `nanosleep`，实现协程级 sleep：
  ```cpp
  // 普通 sleep 会阻塞整个线程（所有协程都停！）
  // 协程 sleep：把当前 Fiber 加入定时器，yield 出去
  void co_sleep(int ms) {
      timer_wheel_.add(ms, [this, f = current_fiber_]() {
          scheduler_.resume(f);
      });
      yield_current();
  }
  ```
- [ ] 测试：1000 个协程各 sleep 1 秒，全部在 ~1 秒内完成（而非 1000 秒）

**完成标志：** 协程并发 sleep 而非串行 sleep

---

## Day 4（Thu）— 协程式 HTTP 客户端

**预计时间：1 小时**

**任务：**
- [ ] 实现协程式 HTTP GET 客户端（使用协程 + hook 的 read/write）：
  ```cpp
  void co_http_get(const string& url, function<void(string)> callback) {
      // 在协程中：TCP connect → send request → recv response
      // 全程非阻塞，但代码写起来像同步的
      int fd = co_connect(host, port);
      co_write(fd, request.data(), request.size());
      string response = co_read_all(fd);
      callback(response);
  }
  ```
- [ ] 测试：1000 个协程并发 HTTP GET，对比 1000 个线程的内存占用

**完成标志：** 1000 并发协程 HTTP 请求，内存 < 100MB

---

## Day 5（Fri）— 月度验收 + 性能测试

**预计时间：1 小时**

**任务：**
- [ ] 验证月度验收标准：
  - [ ] 10 万协程并发，内存 < 4GB
  - [ ] Channel 通信 Go 风格正常工作
  - [ ] 协程 HTTP 客户端并发 1000 请求
  - [ ] 协程切换 < 100ns
- [ ] 压测：协程 HTTP 服务器 vs 多线程 HTTP 服务器（QPS 对比）

**完成标志：** 所有验收标准通过

---

## Day 6（Sat）— 阅读 libco / sylar 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 sylar（C++ 协程框架）的 `scheduler.cc` 和 `iomanager.cc`
- [ ] 回答：
  1. sylar 如何将 IOManager 与 Scheduler 结合（EventLoop + 协程）？
  2. 为什么 hook sleep 后的协程比线程池 sleep 内存效率高 100 倍以上？
  3. 协程与 C++20 coroutine（co_await）的核心区别是什么（有栈 vs 无栈）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘 + 规划 Month 11

**预计时间：1 小时**

- [ ] 整理协程库完整架构图（Fiber + Scheduler + Channel + IO Hook）
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**Month 10 验收：**
| 验收项 | 结果 | 备注 |
|--------|------|------|
| 10万协程内存 | — | — |
| 协程切换延迟 | — | — |
| 并发HTTP请求 | — | — |
