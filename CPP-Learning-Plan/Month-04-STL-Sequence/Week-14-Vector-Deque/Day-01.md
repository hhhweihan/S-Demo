## Day 1（Mon）— MyVector 骨架 + 内存管理

**预计时间：1 小时**

**任务：**
- [ ] 创建 `mystl/vector.h`，实现基础框架：
  ```cpp
  template<typename T, typename Alloc = std::allocator<T>>
  class vector {
      T*     begin_    = nullptr;  // 数据起始
      T*     end_      = nullptr;  // 已用结束
      T*     cap_end_  = nullptr;  // 分配结束
  public:
      // 类型别名、构造/析构、begin/end/size/capacity
  };
  ```
- [ ] 实现构造：`vector()` / `vector(n, val)` / `vector(first, last)`
- [ ] 实现析构：逐元素 `~T()` + 释放内存
- [ ] 实现 `begin() / end() / size() / capacity() / empty()`

**完成标志：** 能用范围 for 遍历，size/capacity 正确

---

