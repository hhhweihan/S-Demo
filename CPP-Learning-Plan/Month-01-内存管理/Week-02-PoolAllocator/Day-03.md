## Day 3（Wed）— 大对象 fallback + header 标记

**预计时间：1 小时**

**任务：**
- [ ] 大对象（>1024）直接走 `malloc`，但要记录这是大对象（在指针前加 8 字节 header）
  ```cpp
  struct Header { bool is_large; size_t size; };
  void* p = malloc(sizeof(Header) + n);
  new(p) Header{true, n};
  return (char*)p + sizeof(Header);
  ```
- [ ] `deallocate` 时回退 8 字节读 header，判断走 pool 还是 `free`
- [ ] 同样给 pool 分配的块加 header（记录 class index），这样 `deallocate` 不再需要传 size
  ```cpp
  void deallocate(void* ptr);  // 不需要 size 参数！
  ```
- [ ] 更新 `allocate/deallocate` 接口，测试混合大小分配/释放不崩溃

**完成标志：** `deallocate` 只需要 `ptr` 一个参数，大小对象混合分配正常

---