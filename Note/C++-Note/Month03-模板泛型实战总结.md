# Month 03 — C++ 模板泛型实战总结

> 学习周期：Week 09 ~ Week 12  
> 核心产出：`type_traits`、`Tuple`、`Optional`、`Variant`、`Function`、CRTP / policy / concepts / constexpr 综合练习  
> 代码位置：`CPP-Practice/template_traits/`、`CPP-Practice/variadic_tuple/`、`CPP-Practice/type_erasure/`、`CPP-Practice/advanced_templates/`

---

## 一、这个月到底完成了什么

Month 03 的目标是把 C++ 模板从语法学习推进到工程化使用。四周内容形成了一条完整链路：

```text
Week 09: SFINAE + type_traits
  -> 模板候选控制
  -> 编译期类型判断和变换

Week 10: 变参模板 + Tuple
  -> 参数包展开
  -> index_sequence
  -> 编译期索引访问

Week 11: 类型擦除
  -> Optional
  -> Variant
  -> Function

Week 12: 高级技巧 + 整合
  -> CRTP
  -> Policy-Based Design
  -> concepts
  -> constexpr
```

这个月真正建立的是一种思维方式：把一部分原本运行时才发现的问题，提前放到编译期表达、检查和组合。

---

## 二、SFINAE 到 concepts：从隐式失败到显式约束

Week 09 的 SFINAE 让模板可以根据表达式是否合法自动移除候选。它很强，但也有明显问题：代码可读性差，错误信息长，接口意图不够直接。

concepts 的进步在于把约束写成接口的一部分：

```cpp
template <std::integral T>
T double_it(T value);
```

或者：

```cpp
template <typename F, typename... Args>
  requires std::invocable<F, Args...>
auto submit(F&& f, Args&&... args);
```

这比把 `enable_if_t` 塞进返回类型更接近自然语言，也更容易维护。SFINAE 仍然是理解标准库旧实现的基础，但新接口设计应优先考虑 concepts。

---

## 三、变参模板最实用的三个模式

### 1. 折叠表达式

适合对参数包做同构操作，例如求和、逻辑合取、批量输出。

### 2. `index_sequence`

适合需要“第 N 个元素”的场景。`Tuple::apply`、`print(tuple)`、逐元素比较都依赖它。

### 3. 完美转发构造

变参模板经常和 `std::forward<Args>(args)...` 一起出现，用于把外层接口收到的参数原样转交给内部对象或 callable。

这三个模式串起来，就是许多现代 C++ 工具的骨架。

---

## 四、类型擦除的三种取舍

### 1. 虚函数类型擦除

`Function<Ret(Args...)>` 用虚基类保存统一调用接口，派生模板保存真实 callable。这种方式开放、直观，但有一次虚调用和可能的堆分配成本。

### 2. `variant`

`Variant<Ts...>` 的类型集合在编译期固定，运行时只保存下标。它比 `any` 更类型安全，也更容易优化，但不适合未知类型集合。

### 3. `any`

`any` 可以保存任意合规类型，最灵活，但访问时需要类型检查，运行时成本和误用风险更高。

简单判断：

- 类型集合固定：优先 `variant`
- 只关心调用签名：优先 `function`
- 类型集合开放且只是临时承载：考虑 `any`

---

## 五、CRTP、Policy 和 constexpr 的位置

CRTP 解决的是静态多态问题，适合追求内联、类型集合已知的场景。虚函数解决的是运行时扩展问题，适合插件式、基类指针统一管理等场景。

Policy-Based Design 解决的是策略组合问题。把线程策略、扩容策略、比较策略、分配策略拆成模板参数，可以让代码在编译期生成不同组合，避免运行时分支。

constexpr 解决的是把确定性小计算前移到编译期的问题。C++20 后 constexpr 能做的事明显更多，但仍要控制复杂度，避免把编译时间变成新的运行时成本。

---

## 六、工程验证结果

本月新增和验证的工程：

- `CPP-Practice/template_traits`：通过构建和运行，覆盖 Day057-Day063
- `CPP-Practice/variadic_tuple`：通过构建和运行，覆盖 Day064-Day070
- `CPP-Practice/type_erasure`：通过构建和运行，覆盖 Day071-Day077
- `CPP-Practice/advanced_templates`：通过构建和运行，覆盖 Day078-Day084 的主要模板任务
- `CPP-Practice/thread_pool`：重新构建运行通过，验证 C++20 concepts 兼容约束未破坏 C++17 工程
- `TestProject/leveldb`：已构建 `db_bench.exe` 并完成小规模 benchmark

---

## 七、这个月最重要的一句话

模板不是为了把代码写得更玄，而是为了把类型约束、结构组合和一部分错误检查前移到编译期；真正成熟的模板代码，应该让接口更清楚、错误更早暴露、运行时分发更少，而不是让维护者更难读懂。