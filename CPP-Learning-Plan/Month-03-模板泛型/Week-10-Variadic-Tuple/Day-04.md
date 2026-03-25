## Day 4（Thu）— 手写 get<N>

**预计时间：1 小时**

**任务：**
- [ ] 实现 `get<N>(tuple)`：
  ```cpp
  // 辅助：获取第 N 层基类的类型
  template<size_t N, typename Head, typename... Tail>
  struct TupleElement {
      using type = typename TupleElement<N-1, Tail...>::type;
  };
  template<typename Head, typename... Tail>
  struct TupleElement<0, Head, Tail...> {
      using type = Head;
  };

  // get 函数：通过向上转型到第 N 层基类
  template<size_t N, typename... Ts>
  auto& get(Tuple<Ts...>& t) {
      using Base = Tuple</* 第 N 层 */ ...>;
      return static_cast<Base&>(t).value;
  }
  ```
- [ ] 测试：`get<0>(t) == 1`，`get<2>(t) == "hello"`

**完成标志：** get<N> 能正确获取对应元素，返回引用（可修改）

---