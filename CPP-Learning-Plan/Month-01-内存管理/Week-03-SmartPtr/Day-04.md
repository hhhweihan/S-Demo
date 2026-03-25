## Day 4（Thu）— enable_shared_from_this

**预计时间：1 小时**

**任务：**
- [ ] 实现 `EnableSharedFromThis<T>` 基类：
  ```cpp
  template<typename T>
  class EnableSharedFromThis {
      WeakPtr<T> weak_this_;
  public:
      SharedPtr<T> shared_from_this() {
          return weak_this_.lock();
      }
      // SharedPtr 构造时需要初始化 weak_this_
  };
  ```
- [ ] 修改 `SharedPtr` 构造函数：检测 T 是否继承自 `EnableSharedFromThis<T>`，若是则初始化 `weak_this_`
- [ ] 测试：在成员函数里调用 `shared_from_this()` 能拿到有效的 SharedPtr

**完成标志：** `shared_from_this()` 返回的 SharedPtr 与外部那个共享同一控制块

---