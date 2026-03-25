## Day 2（Tue）— SFINAE 入门

**预计时间：1 小时**

**任务：**
- [ ] 理解 SFINAE 的含义：Substitution Failure Is Not An Error
- [ ] 写 3 个例子演示 SFINAE：
  ```cpp
  // 例 1：只对有 .size() 方法的类型生效
  template<typename T>
  auto print_size(T t) -> decltype(t.size(), void()) {
      std::cout << t.size();
  }
  // 对 int 调用：SFINAE，编译期不报错，只是找不到这个重载

  // 例 2：用 void_t 检测成员
  template<typename T, typename = void>
  struct has_value_type : std::false_type {};
  template<typename T>
  struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};
  ```
- [ ] 手写 `has_iterator<T>`：检测 T 是否有 `begin()` 方法

**完成标志：** `has_iterator<std::vector<int>>::value == true`，`has_iterator<int>::value == false`

---