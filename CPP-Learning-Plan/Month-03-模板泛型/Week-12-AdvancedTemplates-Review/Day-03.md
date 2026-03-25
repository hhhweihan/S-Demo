## Day 3（Wed）— C++20 Concepts 入门

**预计时间：1 小时**

**任务：**
- [ ] 用 concepts 替代 SFINAE：
  ```cpp
  // 旧写法（SFINAE）
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  T double_it(T x) { return x * 2; }

  // 新写法（concepts）
  template<std::integral T>
  T double_it(T x) { return x * 2; }
  ```
- [ ] 定义自定义 concept：
  ```cpp
  template<typename T>
  concept Printable = requires(T t) { std::cout << t; };

  template<typename T>
  concept Container = requires(T c) {
      c.begin(); c.end(); c.size();
      typename T::value_type;
  };
  ```
- [ ] 给线程池的 `submit` 加 concept 约束（只接受可调用对象）

**完成标志：** 传错类型时编译报错，错误信息比 SFINAE 更清晰

---