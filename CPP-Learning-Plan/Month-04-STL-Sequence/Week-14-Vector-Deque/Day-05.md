## Day 5（Fri）— MyDeque 设计与实现

**预计时间：1 小时**

**任务：**
- [ ] 理解 deque 的分块存储原理：中控数组（map）+ 固定大小 buffer
  ```
  map:  [ptr0][ptr1][ptr2][ptr3]
         ↓     ↓     ↓     ↓
        [buf] [buf] [buf] [buf]  ← 每个 buf 固定 512 字节
  iterator 需要知道：cur_ptr, buf_start, buf_end, map_node
  ```
- [ ] 实现 `deque` 的 iterator（4 个指针 + 跨 buffer 的 `operator++`）
- [ ] 实现 `push_front(val)` / `push_back(val)` / `front()` / `back()`

**完成标志：** 能交替 push_front 和 push_back 各 100 次，front/back 正确

---

