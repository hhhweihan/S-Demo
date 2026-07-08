# Week 09 — SFINAE 与 type_traits 知识梳理总结

> 学习周期：Day-057 ~ Day-063  
> 核心主题：函数模板重载决议、SFINAE、`void_t`、`enable_if`、手写 `type_traits`、libc++ 源码对照  
> 代码落点：`CPP-Practice/template_traits/`  
> 核心实现：`CPP-Practice/template_traits/templates/my_type_traits.h`

---

## 一、本周主线

Week 09 是 Month 03 模板泛型的第一周，目标不是追求复杂模板技巧，而是把模板元编程最基础的三件事打通：

1. 编译器如何在多个函数模板、普通函数和特化之间做选择。
2. 如何让“不满足条件的模板”安静地退出候选集。
3. 如何把类型判断和类型变换封装成可复用的 `type_traits`。

本周的知识链路可以压成下面这条：

```text
函数模板与重载决议
  -> 显式特化不是独立重载
  -> ADL 帮助找到用户命名空间函数

SFINAE
  -> 替换失败不是错误
  -> 候选模板被移除
  -> decltype / void_t 表达检测逻辑

enable_if
  -> 把 bool 条件转成 type 是否存在
  -> 控制模板候选是否可见
  -> 和 if constexpr 分工不同

type_traits
  -> 主模板 + 偏特化
  -> 类型变换与类型判断
  -> declval + decltype 推导表达式结果

标准库实现
  -> 教学版重在理解机制
  -> libc++ 大量使用编译器内置减少实例化成本
```

---

## 二、函数模板、特化、重载与 ADL

### 1. 函数模板特化不是新的重载候选

本周 Day 57 最容易混淆的一点是：函数模板显式特化看起来像另一个函数，但它不是重载集里独立参与竞争的成员。

可以这样理解：

```cpp
template <typename T>
void f(T);

template <>
void f<int>(int);

```

`f<int>` 是主模板 `f(T)` 的一个特化版本。调用 `f(1)` 时，编译器先做重载决议，确认主模板是不是被选中；如果主模板被选中，再看是否存在对应特化。

所以如果同时有一个普通函数：

```cpp
void f(int);
```

普通函数重载通常会优先于函数模板，显式特化不一定有机会生效。

### 2. 重载优先级比“特化更具体”更早发生

常见误解是：既然 `f<int>` 看起来更具体，那它一定赢。实际顺序不是这样。

更准确的顺序是：

1. 收集候选函数。
2. 做模板参数推导。
3. 做重载决议，选出最佳候选。
4. 如果最佳候选是某个函数模板，再检查有没有对应显式特化。

这也是为什么本周练习里用了三个例子区分：

- 指针重载优先于主模板再派发到特化
- 字符串字面量可以被数组引用重载保留原始数组类型
- 普通函数重载优先于模板版本

### 3. ADL 的标准用法：`using std::swap; swap(a, b);`

ADL 是 Argument-Dependent Lookup，也就是实参相关查找。对于用户自定义类型：

```cpp
namespace ns {
struct Foo {};
void swap(Foo&, Foo&);
}
```

调用：

```cpp
using std::swap;
swap(a, b);
```

这会同时保留 `std::swap` 作为 fallback，又允许编译器根据 `Foo` 的命名空间找到 `ns::swap`。

这个写法的价值在于：库代码不需要知道所有用户类型的优化交换函数，只要给 ADL 留出空间即可。

---

## 三、SFINAE：Substitution Failure Is Not An Error

### 1. SFINAE 的边界

SFINAE 的完整意思是：模板参数替换失败不是错误。

关键是“替换失败”这几个字。它不是说模板里的任何错误都可以被吞掉。只有发生在函数类型、返回类型、模板参数、默认参数等替换上下文里的失败，才会让该候选模板被移除。

例如：

```cpp
template <typename T>
auto print_size(const T& value) -> decltype(value.size(), void()) {
  std::cout << value.size();
}
```

如果 `T = int`，`value.size()` 在返回类型替换阶段不成立，这个重载会被移除。它不会立刻报错；只有没有任何可行候选时，调用点才报“找不到匹配函数”。

但如果把 `value.size()` 放在函数体里：

```cpp
template <typename T>
void print_size(const T& value) {
  std::cout << value.size();
}
```

当这个模板已经被选中并实例化函数体时，`int` 没有 `size()` 就是硬错误。

### 2. `decltype(expr, void())` 的作用

表达式：

```cpp
decltype(value.size(), void())
```

利用了逗号表达式：

- 先要求 `value.size()` 必须是合法表达式
- 整体类型最终变成 `void`

所以它适合做“检测某个表达式是否存在”的返回类型。

### 3. `void_t` 的检测模式

`void_t` 的最小实现非常简单：

```cpp
template <typename...>
using void_t = void;
```

它的关键不在最终结果是 `void`，而在模板实参替换过程中会检查里面的类型或表达式是否合法。

典型检测成员类型：

```cpp
template <typename T, typename = void>
struct has_value_type : false_type {};

template <typename T>
struct has_value_type<T, void_t<typename T::value_type>> : true_type {};
```

典型检测成员函数：

```cpp
template <typename T, typename = void>
struct has_iterator : false_type {};

template <typename T>
struct has_iterator<T, void_t<decltype(std::declval<T&>().begin())>> : true_type {};
```

这里的默认主模板表示“检测失败”，偏特化表示“检测成功”。

---

## 四、`enable_if` 与 `if constexpr`

### 1. `enable_if` 的本质

最小实现：

```cpp
template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  using type = T;
};
```

如果条件为 `true`，`enable_if<true, T>::type` 存在；如果条件为 `false`，`type` 不存在。

把它放到函数签名里：

```cpp
template <typename T>
typename enable_if<is_integral<T>::value, T>::type
double_it(T value) {
  return value * 2;
}
```

当 `T` 不是整数类型时，返回类型替换失败，该函数模板从候选集中移除。

### 2. `enable_if` 适合控制“函数是否存在”

`enable_if` 的典型使用位置：

- 返回类型
- 额外模板参数
- 函数参数默认值

核心目的都是一个：让某个重载在条件不满足时不可见。

### 3. `if constexpr` 适合控制“函数体内部走哪条分支”

C++17 后，很多简单分支可以写成：

```cpp
template <typename T>
auto double_it_if_constexpr(T value) {
  if constexpr (is_integral<T>::value) {
    return value * 2;
  } else {
    static_assert(is_integral<T>::value, "requires integral type");
  }
}
```

它和 `enable_if` 的区别：

- `enable_if` 决定这个函数模板是否参与重载决议
- `if constexpr` 决定被选中的模板实例化哪条分支

如果需求是“不同类型选择不同重载”，`enable_if` 更自然；如果需求是“同一个模板内部处理不同类型”，`if constexpr` 更清晰。

---

## 五、手写基础 type_traits

### 1. `integral_constant` 是 trait 的公共底座

`true_type` 和 `false_type` 通常基于 `integral_constant`：

```cpp
template <typename T, T Value>
struct integral_constant {
  static constexpr T value = Value;
  using value_type = T;
  using type = integral_constant;

  constexpr operator value_type() const noexcept { return value; }
};
```

这样每个判断型 trait 都可以统一暴露：

- `::value`
- `::type`
- 到 bool 的 constexpr 转换

### 2. 类型变换 trait：返回 `type`

这类 trait 的目标是从一个类型计算出另一个类型。

典型例子：

```cpp
template <typename T>
struct remove_reference { using type = T; };

template <typename T>
struct remove_reference<T&> { using type = T; };

template <typename T>
struct remove_reference<T&&> { using type = T; };
```

同类还有：

- `remove_const<T>`
- `remove_volatile<T>`
- `remove_cv<T>`
- `add_pointer<T>`
- `add_lvalue_reference<T>`

为了减少噪声，通常会提供 `_t` 别名：

```cpp
template <typename T>
using remove_reference_t = typename remove_reference<T>::type;
```

### 3. 判断型 trait：继承 `true_type` 或 `false_type`

这类 trait 的目标是回答一个编译期布尔问题。

典型例子：

```cpp
template <typename T>
struct is_reference : false_type {};

template <typename T>
struct is_reference<T&> : true_type {};

template <typename T>
struct is_reference<T&&> : true_type {};
```

同类还有：

- `is_pointer<T>`
- `is_array<T>`
- `is_integral<T>`

需要注意的是，`is_pointer<const int*>` 和 `is_pointer<int* const>` 不是同一个 cv 层级问题。前者是“指向 const int 的指针”，后者是“顶层 const 指针”。判断指针时通常要先移除顶层 cv：

```cpp
template <typename T>
struct is_pointer : is_pointer_helper<remove_cv_t<T>> {};
```

### 4. `conditional<B, T, F>` 是类型级 if

最小实现：

```cpp
template <bool B, typename T, typename F>
struct conditional { using type = T; };

template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };
```

它是后续 `decay` 这类组合型 trait 的基础工具。

---

## 六、进阶 type_traits

### 1. `decay<T>`：模拟函数传参时的类型退化

简化版 `decay` 可以先做三步：

1. 移除引用。
2. 如果是数组，退化为指向元素的指针。
3. 否则移除顶层 cv。

示例：

```cpp
decay_t<const int&>  -> int
decay_t<int[3]>      -> int*
```

完整标准库版本还要处理函数类型退化为函数指针，本周实现的是教学版重点路径。

### 2. `is_constructible<T, Args...>`：检测构造表达式是否合法

简化检测方式：

```cpp
template <typename T, typename... Args>
auto test_constructible(int)
    -> decltype(T(std::declval<Args>()...), true_type{});

template <typename, typename...>
auto test_constructible(...) -> false_type;
```

调用 `test_constructible<T, Args...>(0)` 时，编译器优先尝试 `int` 版本。如果 `T(std::declval<Args>()...)` 合法，返回 `true_type`；否则该候选被 SFINAE 移除，落到 `...` 版本返回 `false_type`。

### 3. `is_convertible<From, To>`：检测隐式转换是否合法

常见技巧是定义一个只声明不实现的接收函数：

```cpp
template <typename To>
void accept(To);

template <typename From, typename To>
auto test_convertible(int)
    -> decltype(accept<To>(std::declval<From>()), true_type{});
```

如果 `From` 可以隐式传给 `To`，表达式合法；否则走失败分支。

### 4. `invoke_result<F, Args...>`：推导调用结果

教学版实现：

```cpp
template <typename F, typename... Args>
struct invoke_result {
  using type = decltype(std::declval<F>()(std::declval<Args>()...));
};
```

它适用于普通函数对象和函数指针的基本调用形式。标准库 `std::invoke_result` 更完整，还要支持成员函数指针、成员对象指针、`std::reference_wrapper` 等 `std::invoke` 规则。

---

## 七、`std::declval` 的正确理解

`std::declval<T>()` 的作用是“在未求值上下文里伪造一个 T 类型表达式”。它通常配合 `decltype` 使用：

```cpp
decltype(std::declval<F>()(std::declval<Args>()...))
```

需要记住两点：

1. `declval` 不能在运行期调用。
2. 它只用于 `decltype`、`sizeof` 这类未求值上下文。

它解决的问题是：不需要真的构造一个对象，也能询问“如果有这个对象，某个表达式的类型是什么”。

---

## 八、libc++ 对照结论

### 1. `is_same` 的工业实现更依赖编译器内置

教学版：

```cpp
template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};
```

当前 libc++ 中，`is_same` 直接继承 `_BoolConstant<__is_same(_Tp, _Up)>`，`is_same_v` 也直接使用 `__is_same`。

这说明标准库实现不只是“把教学版写全”，还会用编译器内置减少实例化数量。

### 2. `remove_reference` 也优先使用内置类型变换

当前 libc++ 的 `remove_reference` 会优先使用 `__remove_reference_t` 或类似内置能力。教学版的三段偏特化适合理解规则，但工业实现会优先考虑编译速度和调试符号表现。

### 3. trivial / nothrow 相关 trait 很多必须依赖编译器

例如：

- `__is_trivially_constructible`
- `__is_trivially_assignable`
- `__is_trivially_destructible`
- `__is_trivially_copyable`

这类性质牵涉编译器对类型布局、构造/析构语义、特殊成员函数的理解，普通模板很难完整准确表达。

---

## 九、常见坑

### 1. 把函数体错误误认为 SFINAE

SFINAE 只发生在替换上下文。函数体里写了非法表达式，模板一旦被选中就会报硬错误。

### 2. 把 `enable_if` 放错位置导致签名冲突

多个模板如果只有默认模板参数不同，可能仍然形成重定义。实际工程里常把 `enable_if_t<cond, int> = 0` 放进非类型模板参数，或放到返回类型/函数参数中区分。

### 3. 忘记引用折叠和 cv 层级

`T&`、`T&&`、`const T`、`T* const`、`const T*` 的层级不同。写 trait 时要先明确要处理的是顶层 cv，还是被指向对象的 cv。

### 4. 过度使用模板技巧

C++20 以后，很多 `enable_if` 场景可以用 concepts 表达得更清楚。本周手写 `enable_if` 是为了理解机制，不是说所有工程代码都应该回到这种写法。

---

## 十、本周代码验证点

本周实现通过以下方式验证：

- `static_assert` 验证编译期 trait 结果
- 运行时 `expect()` 输出关键路径 `[PASS]`
- CMake + MSVC Release 构建验证

验证命令：

```powershell
cmake -S CPP-Practice/template_traits -B CPP-Practice/template_traits/build
cmake --build CPP-Practice/template_traits/build --config Release
.\CPP-Practice\template_traits\build\Release\template_traits_demo.exe
```

关键完成标志：

- `has_iterator<std::vector<int>>::value == true`
- `has_iterator<int>::value == false`
- `double_it(3)` 编译通过
- `double_it(3.14)` 被 SFINAE 移除
- `invoke_result<int(*)(int), int>::type` 是 `int`

---

## 十一、一句话总结

Week 09 真正建立的是“用类型系统做编译期判断和选择”的基础能力：先理解重载候选如何形成，再用 SFINAE 和 `enable_if` 控制候选是否存在，最后把常见判断和变换沉淀成可复用的 `type_traits`。