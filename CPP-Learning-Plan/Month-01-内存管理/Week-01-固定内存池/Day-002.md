## Day 2（Tue）— 实现 free list

**预计时间：1 小时**

**任务：**
- [ ] 创建 `fixed_allocator.cpp`
- [ ] `init()`：用 `malloc` 申请一块大内存，把所有 block 串成 free list
  ```cpp
  // 初始化时把每个 block 的前 sizeof(void*) 字节写成指向下一个 block
  for (size_t i = 0; i < block_count - 1; i++) {
      void* current = (char*)chunk + i * block_size;
      void* next    = (char*)chunk + (i + 1) * block_size;
      *reinterpret_cast<void**>(current) = next;
  }
  // 最后一个 block 指向 nullptr
  ```
- [ ] `allocate()`：从 free list 头部取出一个 block 返回
- [ ] `deallocate()`：把 block 头插回 free list
- [ ] `main.cpp` 测试：分配 3 个 block，释放中间那个，再分配 1 个，验证拿到的是刚释放的那个

**完成标志：** 基本分配/释放逻辑跑通，没有段错误

---