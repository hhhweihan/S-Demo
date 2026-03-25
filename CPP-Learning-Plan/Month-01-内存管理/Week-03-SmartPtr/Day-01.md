## Day 1（Mon）— 手写 unique_ptr

**预计时间：1 小时**

**任务：**
- [ ] 创建 `smart_ptr/unique_ptr.h`，实现完整的 `UniquePtr<T>`：
  ```cpp
  template<typename T>
  class UniquePtr {
      T* ptr_ = nullptr;
  public:
      explicit UniquePtr(T* p = nullptr);
      ~UniquePtr();
      UniquePtr(UniquePtr&& other) noexcept;         // 移动构造
      UniquePtr& operator=(UniquePtr&& other) noexcept; // 移动赋值
      UniquePtr(const UniquePtr&) = delete;           // 禁止拷贝
      UniquePtr& operator=(const UniquePtr&) = delete;
      T* get() const;
      T& operator*() const;
      T* operator->() const;
      T* release();          // 放弃所有权，返回裸指针
      void reset(T* p = nullptr);
      explicit operator bool() const;
  };
  ```
- [ ] 测试：
  - 离开作用域自动析构（加打印确认）
  - `std::move` 转移所有权后原指针变为 nullptr
  - 禁止拷贝（编译报错即正确）

**完成标志：** 移动语义正常，离开作用域自动析构，拷贝被禁止

---