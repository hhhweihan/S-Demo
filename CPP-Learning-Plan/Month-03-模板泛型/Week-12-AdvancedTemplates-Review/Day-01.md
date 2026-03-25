## Day 1（Mon）— CRTP 静态多态

**预计时间：1 小时**

**任务：**
- [ ] 理解 CRTP（Curiously Recurring Template Pattern）：
  ```cpp
  template<typename Derived>
  class Shape {
  public:
      double area() const {
          return static_cast<const Derived*>(this)->area_impl();
      }
      void print() const { std::cout << "Area: " << area(); }
  };
  class Circle : public Shape<Circle> {
      double r_;
  public:
      double area_impl() const { return 3.14 * r_ * r_; }
  };
  ```
- [ ] 对比：CRTP（静态多态）vs 虚函数（动态多态）的性能差异
- [ ] 用 benchmark 测量：1000 万次 area() 调用，CRTP vs virtual，各自耗时

**完成标志：** CRTP 版本比 virtual 快（因为无 vtable 查找），有数据支撑

---