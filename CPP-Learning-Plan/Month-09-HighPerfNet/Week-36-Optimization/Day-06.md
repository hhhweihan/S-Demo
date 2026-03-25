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

