## Day 2（Tue）— Policy-Based Design

**预计时间：1 小时**

**任务：**
- [ ] 用策略模板改造 Month 1 的 PoolAllocator：
  ```cpp
  // ThreadingPolicy：决定是否加锁
  struct SingleThreaded { void lock() {} void unlock() {} };
  struct MultiThreaded   { std::mutex m; void lock() { m.lock(); } void unlock() { m.unlock(); } };

  // GrowthPolicy：决定扩容策略
  struct DoubleGrowth   { size_t next(size_t n) { return n * 2; } };
  struct LinearGrowth   { size_t next(size_t n) { return n + 64; } };

  template<typename ThreadPolicy = SingleThreaded, typename GrowthPolicy = DoubleGrowth>
  class FixedAllocator { ... };
  ```
- [ ] 测试：`FixedAllocator<SingleThreaded, DoubleGrowth>` 和 `FixedAllocator<MultiThreaded>` 都能正确工作

**完成标志：** 两种策略组合都能通过测试

---