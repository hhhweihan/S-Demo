## Day 3（Wed）— 手写 weak_ptr

**预计时间：1 小时**

**任务：**
- [ ] 创建 `smart_ptr/weak_ptr.h`
- [ ] `WeakPtr<T>`：
  - 持有控制块指针，但**不增加 use_count**，只增加 `weak_count`
  - `lock()` 方法：如果 `use_count > 0`，返回一个新的 `SharedPtr<T>`（++use_count），否则返回空
  - 析构时 `--weak_count`，如果 `use_count==0 && weak_count==0`，delete 控制块
- [ ] 验证循环引用被打破：
  ```cpp
  struct Node { SharedPtr<Node> next; };   // 循环引用 → 内存泄漏
  struct Node { WeakPtr<Node> next; };     // 用 WeakPtr → 正常析构
  ```
- [ ] 用 valgrind 或 ASan 确认无内存泄漏

**完成标志：** `lock()` 逻辑正确，循环引用用 WeakPtr 打破后无泄漏

---