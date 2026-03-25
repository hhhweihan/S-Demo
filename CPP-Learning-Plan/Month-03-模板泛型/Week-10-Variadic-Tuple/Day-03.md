## Day 3（Wed）— 手写 tuple（类型存储）

**预计时间：1 小时**

**任务：**
- [ ] 用递归模板实现 `Tuple` 的存储：
  ```cpp
  // 基础：空 Tuple
  template<typename... Ts> struct Tuple {};

  // 递归：Tuple<Head, Tail...> 继承自 Tuple<Tail...>
  template<typename Head, typename... Tail>
  struct Tuple<Head, Tail...> : Tuple<Tail...> {
      Head value;
      Tuple(Head h, Tail... t) : Tuple<Tail...>(t...), value(h) {}
  };
  ```
- [ ] 测试：`Tuple<int, double, std::string> t(1, 2.0, "hello")` 能构造
- [ ] 用 `sizeof(Tuple<int, double>)` 验证大小符合预期

**完成标志：** Tuple 能构造，大小正确（注意内存对齐）

---