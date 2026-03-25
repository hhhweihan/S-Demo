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

