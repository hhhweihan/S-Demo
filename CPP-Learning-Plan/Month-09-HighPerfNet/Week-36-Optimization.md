# Week 36 — 整合 + 性能优化

## 本周目标

整合 Month 9 所有组件，实现零拷贝文件传输和多线程 accept，完成月度验收。

---

## Day 1（Mon）— 零拷贝 sendfile

**预计时间：1 小时**

**任务：**
- [ ] 实现基于 `sendfile(2)` 的文件传输（内核直接传输，不经用户态）：
  ```cpp
  // 普通方式（两次拷贝）：
  // disk → kernel buffer → user buffer → kernel socket buffer → NIC
  // sendfile 方式（一次拷贝）：
  // disk → kernel buffer → kernel socket buffer → NIC
  ssize_t ret = sendfile(sock_fd, file_fd, &offset, file_size);
  ```
- [ ] 对比：100MB 文件，read/write vs sendfile 耗时和 CPU 使用率

**完成标志：** sendfile 比 read/write 快，CPU 使用率更低

---

## Day 2（Tue）— splice + 管道零拷贝

**预计时间：1 小时**

**任务：**
- [ ] 实现 `splice` 管道转发（两个 fd 之间，无需经过用户态）：
  ```cpp
  int pipefd[2];
  pipe(pipefd);
  // 从 src_fd 移动数据到管道（无拷贝）
  splice(src_fd, nullptr, pipefd[1], nullptr, len, SPLICE_F_MOVE);
  // 从管道移动到 dst_fd（无拷贝）
  splice(pipefd[0], nullptr, dst_fd, nullptr, len, SPLICE_F_MOVE);
  ```
- [ ] 实现一个 TCP 代理（双向 splice 转发）
- [ ] 测试：代理 100MB 数据传输性能

**完成标志：** TCP 代理正确转发数据，吞吐量接近网卡上限

---

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

## Day 4（Thu）— Buffer 优化（scatter-gather IO）

**预计时间：1 小时**

**任务：**
- [ ] 实现 `writev` 散布写（一次系统调用发送多块不连续内存）：
  ```cpp
  struct iovec iov[2];
  iov[0] = {header_buf, header_len};
  iov[1] = {body_buf,   body_len};
  writev(sock_fd, iov, 2);
  ```
- [ ] 避免为每个 TCP 包分配完整 header+body 的连续内存
- [ ] 对比：memcpy+write vs writev 的性能差距（大量小包场景）

**完成标志：** writev 减少了内存拷贝次数

---

## Day 5（Fri）— 月度综合压测

**预计时间：1 小时**

**任务：**
- [ ] 验证月度验收标准：
  - [ ] HTTP Server QPS > 10 万（4 核机器，4线程 Reactor + SO_REUSEPORT）
  - [ ] io_uring 大文件传输比 epoll 快 20%+
  - [ ] 连接空闲 60 秒自动清理（时间轮）
  - [ ] 10 万并发连接稳定 1 分钟
- [ ] 用 wrk 压测最终版本 HTTP Server

**完成标志：** 所有验收标准通过

---

## Day 6（Sat）— 阅读 nginx 事件模型

**预计时间：2 小时**

**任务：**
- [ ] 阅读 nginx `src/event/ngx_event.c` 和 `src/event/modules/ngx_epoll_module.c`
- [ ] 回答：
  1. nginx 的 master + worker 进程模型与 one-loop-per-thread 有何不同？
  2. nginx 如何实现惊群问题的避免（accept_mutex）？
  3. nginx 的 ngx_event_t 如何与 fd 关联（类似 Channel）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘 + 规划 Month 10

**预计时间：1 小时**

- [ ] 整理 Month 9 高性能网络库的技术栈图
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**Month 9 验收：**
| 验收项 | 结果 | 备注 |
|--------|------|------|
| HTTP Server QPS | — | — |
| io_uring vs epoll | — | — |
| 10万连接稳定性 | — | — |
