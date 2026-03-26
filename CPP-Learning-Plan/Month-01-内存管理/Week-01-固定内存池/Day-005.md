## Day 5（Fri）— Benchmark 对比

**预计时间：1 小时**

**任务：**
- [ ] 分别测量以下三种方式分配/释放 **100 万次** 的总耗时：
  ```cpp
  // 方案 A：new/delete
  int* p = new int; *p = 42; delete p;

  // 方案 B：FixedAllocator
  void* p = alloc.allocate(); *(int*)p = 42; alloc.deallocate(p);

  // 方案 C：malloc/free
  int* p = (int*)malloc(sizeof(int)); *p = 42; free(p);
  ```
- [ ] 用 `std::chrono::high_resolution_clock` 计时
- [ ] 把结果记录在本文件底部的「本周结果」区域
- [ ] 分析：为什么内存池更快？（省去了 malloc 内部的 mutex 和碎片处理）

**完成标志：** FixedAllocator 至少比 new 快 2x

---