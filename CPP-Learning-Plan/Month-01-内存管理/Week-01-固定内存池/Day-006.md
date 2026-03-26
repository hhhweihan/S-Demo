## Day 6（Sat）— 读 nginx 内存池

**预计时间：2 小时**

**任务：**
- [ ] 下载 nginx 源码：`git clone https://github.com/nginx/nginx`
- [ ] 只读这两个文件（共约 250 行）：
  - `src/core/ngx_palloc.h`
  - `src/core/ngx_palloc.c`
- [ ] 边读边回答以下问题（写在下方的「读源码笔记」里）：
  1. nginx 如何区分「小块」和「大块」分配？阈值是多少？
  2. `ngx_pool_t` 结构体里 `d.last`、`d.end`、`d.next` 分别是什么？
  3. nginx 的内存池为什么不支持释放单个 block（只能整池销毁）？
  4. 你的 FixedAllocator 和 nginx 内存池的核心设计差异是什么？

**完成标志：** 4 个问题都有答案，写了笔记

---