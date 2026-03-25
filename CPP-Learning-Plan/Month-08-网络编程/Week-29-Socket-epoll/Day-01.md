## Day 1（Mon）— TCP 回显服务器

**预计时间：1 小时**

**任务：**
- [ ] 用原始 POSIX API（不用任何框架）写一个阻塞式 TCP Echo Server：
  ```cpp
  // server: socket → bind → listen → accept → read/write loop
  // client: socket → connect → write/read
  ```
- [ ] 同时开一个 server 和一个 client，client 发 "hello"，server 回显 "hello"
- [ ] 理解每个系统调用的作用（写注释）

**完成标志：** 能收发数据，无内存泄漏

---