## Day 2（Tue）— 实现 PoolAllocator 核心

**预计时间：1 小时**

**任务：**
- [ ] `PoolAllocator` 内部持有 16 个 `FixedAllocator` 数组
- [ ] 构造时初始化每个 FixedAllocator（block_size 对应 size class 大小，初始 64 块）
- [ ] `allocate(n)`：调用 `size_to_class(n)` 路由，返回对应 FixedAllocator 的块
- [ ] `deallocate(ptr, n)`：同样路由回对应的 FixedAllocator 释放
- [ ] 测试：分配 20 字节 10 次，分配 200 字节 10 次，stats 显示两个 class 各用了 10 块

**完成标志：** 多 size class 分配/释放正常，stats 数字正确

---