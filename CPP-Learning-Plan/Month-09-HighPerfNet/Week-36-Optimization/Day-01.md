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

