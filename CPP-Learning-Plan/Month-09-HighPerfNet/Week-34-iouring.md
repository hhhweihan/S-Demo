# Week 34 — io_uring 异步 IO

## 本周目标

掌握 io_uring 的 SQ/CQ 环形队列机制，实现基于 io_uring 的异步文件和网络 IO，
与 epoll 性能对比。

---

## Day 1（Mon）— io_uring 基础原理

**预计时间：1 小时**

**任务：**
- [ ] 理解 io_uring 三个核心：
  ```
  SQ（Submission Queue）：用户态提交 IO 请求
  CQ（Completion Queue）：内核填写完成事件
  SQE/CQE：请求/完成条目（ring buffer，避免系统调用）
  ```
- [ ] 安装 liburing（`sudo apt install liburing-dev`）
- [ ] 实现 Hello io_uring：
  ```c
  struct io_uring ring;
  io_uring_queue_init(256, &ring, 0);
  // 提交一个 read 请求
  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
  io_uring_prep_read(sqe, fd, buf, sizeof(buf), 0);
  io_uring_submit(&ring);
  // 等待完成
  struct io_uring_cqe *cqe;
  io_uring_wait_cqe(&ring, &cqe);
  ```
- [ ] 测试：用 io_uring 异步读文件

**完成标志：** 成功用 io_uring 读写本地文件

---

## Day 2（Tue）— io_uring 网络 IO

**预计时间：1 小时**

**任务：**
- [ ] 用 io_uring 实现 TCP Echo Server：
  - `io_uring_prep_accept`：等待新连接
  - `io_uring_prep_recv`：接收数据
  - `io_uring_prep_send`：发送数据
  - 每个操作完成后提交下一个操作（链式提交）
- [ ] 实现事件循环：
  ```cpp
  while (true) {
      io_uring_wait_cqe(&ring, &cqe);
      // 根据 cqe->user_data 判断操作类型，处理完成事件
      io_uring_cqe_seen(&ring, cqe);
  }
  ```
- [ ] 测试：与 epoll Echo Server 在功能上等价

**完成标志：** io_uring Echo Server 能正确处理并发连接

---

## Day 3（Wed）— 注册固定缓冲区

**预计时间：1 小时**

**任务：**
- [ ] 实现固定缓冲区（减少内核/用户态内存映射开销）：
  ```cpp
  // 预注册缓冲区到内核
  struct iovec iovecs[256];
  // 分配 256 个 4KB 缓冲区
  io_uring_register_buffers(&ring, iovecs, 256);
  // 使用注册缓冲区（不用 io_uring_prep_read，用 io_uring_prep_read_fixed）
  io_uring_prep_read_fixed(sqe, fd, buf, len, offset, buf_index);
  ```
- [ ] 对比：有无固定缓冲区的性能差异

**完成标志：** 固定缓冲区版本比普通版本快

---

## Day 4（Thu）— SQPOLL 内核轮询模式

**预计时间：1 小时**

**任务：**
- [ ] 了解 `IORING_SETUP_SQPOLL`：内核线程轮询 SQ，提交 IO 无需系统调用：
  ```cpp
  struct io_uring_params params = {};
  params.flags |= IORING_SETUP_SQPOLL;
  params.sq_thread_idle = 2000;  // 空闲 2 秒后睡眠
  io_uring_queue_init_params(256, &ring, &params);
  ```
- [ ] 分析适用场景：高 IOPS 场景下系统调用开销 vs SQPOLL CPU 消耗
- [ ] 测试：SQPOLL 模式下的文件读写吞吐量

**完成标志：** 理解 SQPOLL 的 trade-off，数据已记录

---

## Day 5（Fri）— io_uring vs epoll 性能对比

**预计时间：1 小时**

**任务：**
- [ ] 对比测试（同样的 Echo Server）：
  ```
  测试场景 1：10K 并发连接，消息大小 64B
  测试场景 2：100 并发，消息大小 64KB
  指标：QPS、CPU 使用率、延迟
  ```
- [ ] 分析：在什么场景 io_uring 优于 epoll？
  - 答：大量小 IO + 高系统调用频率时（如存储服务）

**完成标志：** 对比数据已记录，适用场景分析完成

---

## Day 6（Sat）— 阅读 liburing 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 liburing `src/lib.c` 和内核 `io_uring/io_uring.c` 关键路径
- [ ] 回答：
  1. io_uring 为什么比 epoll + read/write 少一次系统调用？（共享内存环）
  2. `io_uring_peek_cqe` 和 `io_uring_wait_cqe` 的区别（轮询 vs 阻塞）？
  3. io_uring 的 `multishot accept` 如何减少 accept 系统调用次数？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理 io_uring vs epoll 的核心区别总结
- [ ] `git commit`，推送
