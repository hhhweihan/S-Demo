## Day 5（Fri）— 手写 type_traits（二）

**预计时间：1 小时**

**任务：**
- [ ] 继续实现：
  - `decay<T>`：移除引用和 cv，数组退化为指针
  - `is_constructible<T, Args...>`：T 能否用 Args... 构造
  - `is_convertible<From, To>`：From 能否隐式转换为 To
  - `invoke_result<F, Args...>`：等价于 `decltype(std::declval<F>()(std::declval<Args>()...))`
- [ ] `invoke_result` 用 `decltype` + `std::declval` 实现

**完成标志：** `invoke_result<int(*)(int), int>::type` 是 `int`

---