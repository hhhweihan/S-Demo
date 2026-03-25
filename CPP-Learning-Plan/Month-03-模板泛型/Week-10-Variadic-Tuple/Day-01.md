## Day 1（Mon）— 变参模板基础

**预计时间：1 小时**

**任务：**
- [ ] 写 5 个变参模板函数练习：
  ```cpp
  // 1. 求所有参数之和
  template<typename... Args>
  auto sum(Args... args) { return (args + ...); }  // 折叠表达式

  // 2. 打印所有参数（用逗号分隔）
  template<typename... Args>
  void print_all(Args&&... args) {
      ((std::cout << args << " "), ...);
  }

  // 3. 统计参数个数
  template<typename... Args>
  constexpr size_t count() { return sizeof...(Args); }

  // 4. 检查是否所有参数都是整数
  template<typename... Args>
  constexpr bool all_integral = (std::is_integral_v<Args> && ...);

  // 5. 找出第 N 个类型
  template<size_t N, typename... Args>
  using nth_type = std::tuple_element_t<N, std::tuple<Args...>>;  // 先用标准库
  ```

**完成标志：** 5 个都能正确编译运行

---