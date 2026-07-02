# Week 9 — SFINAE + type_traits

## 本周目标

彻底搞清 SFINAE 原理，手写常用 type_traits，理解 `enable_if` 条件编译。

## 本周完成情况

- [x] Day 57：复习函数模板特化、函数重载优先级和 ADL，手写最简 `is_same<T, U>`
- [x] Day 58：理解 SFINAE，完成 `decltype(expr, void())`、`void_t`、`has_iterator<T>` 示例
- [x] Day 59：手写 `enable_if`，用条件编译限制 `double_it(T)`，并用 `if constexpr` 对照
- [x] Day 60：创建 `templates/my_type_traits.h`，实现基础类型变换和类型判断 trait
- [x] Day 61：继续实现 `decay`、`is_constructible`、`is_convertible`、`invoke_result`
- [x] Day 62：对照 libc++ `type_traits` 源码，记录编译器内置和库实现宏的作用
- [x] Day 63：整理本周实现和复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：[CPP-Practice/template_traits](../../../CPP-Practice/template_traits)
- 核心实现：[templates/my_type_traits.h](../../../CPP-Practice/template_traits/templates/my_type_traits.h)
- 运行入口：[main.cpp](../../../CPP-Practice/template_traits/main.cpp)
- 知识总结：[Note/C++-Note/Week09-SFINAE-TypeTraits-知识梳理总结.md](../../../Note/C++-Note/Week09-SFINAE-TypeTraits-知识梳理总结.md)

## 七天路线

### Day 57：先把模板重载决议边界踩稳

本周没有一上来写 `type_traits`，而是先复习函数模板、显式特化、普通函数重载和 ADL。这个顺序是有必要的：SFINAE 本质上也发生在候选集形成和重载决议附近，如果不先知道“谁会进入候选集、谁会被优先选择”，后面很容易把模板特化、重载和替换失败混成一团。

本日最重要的结论是：显式特化不是独立重载集成员；普通函数和函数模板同时可行时，普通函数通常优先；`using std::swap; swap(a, b);` 能让 ADL 找到用户类型命名空间里的 `swap`。

### Day 58：SFINAE 是候选移除机制

Day 58 把重点放到 SFINAE：模板参数替换失败不是硬错误，而是让当前模板候选退出重载集。`print_size()` 用 `decltype(value.size(), void())` 检测表达式是否成立，`has_value_type<T>` 和 `has_iterator<T>` 用 `void_t` 把“成员是否存在”转换成偏特化选择问题。

这一天真正需要记住的不是某个写法，而是判断位置：只有发生在替换阶段的失败才是 SFINAE；函数体内部普通语句报错不是 SFINAE。

### Day 59：`enable_if` 是把布尔条件塞进类型系统

Day 59 手写了最小版 `enable_if`：条件为 `true` 时提供 `type`，条件为 `false` 时不提供 `type`。把它放在函数签名中，就可以让不满足条件的模板在替换阶段被移除。

同一天也用 C++17 `if constexpr` 写了对照版本。两者的分工不同：`enable_if` 更适合控制某个重载是否存在，`if constexpr` 更适合已经进入函数体之后按类型分支。

### Day 60：基础 trait 的主线是主模板 + 偏特化

Day 60 的实现集中在 `my_type_traits.h`：`remove_const`、`remove_reference`、`remove_cv`、`add_pointer`、`add_lvalue_reference`、`is_pointer`、`is_reference`、`is_array`、`conditional`。

这些 trait 大多很短，但它们提供了类型计算的基础积木。比如先 `remove_reference_t<T>` 再加 `*`，就能处理 `add_pointer<int&>`；`is_pointer` 先移除顶层 cv，再判断是否匹配 `T*`。

### Day 61：进阶 trait 开始依赖表达式探测

Day 61 的 `decay<T>`、`is_constructible<T, Args...>`、`is_convertible<From, To>`、`invoke_result<F, Args...>` 更接近标准库 trait 的真实味道。

其中 `decay` 是类型变换组合；`is_constructible` 和 `is_convertible` 用两个探测函数配合 `decltype` 选择 `true_type` 或 `false_type`；`invoke_result` 则用 `std::declval` 在未求值上下文里推导调用表达式结果。

### Day 62：标准库实现大量借助编译器内置

对照 libc++ 后可以看到，工业实现并不只是“把教学版偏特化补全”。当前 libc++ 的 `is_same` 直接继承 `_BoolConstant<__is_same(_Tp, _Up)>`，`remove_reference` 也优先使用 `__remove_reference_t` 这类编译器内置。

这说明标准库 trait 的目标不仅是语义正确，还包括减少模板实例化、降低编译成本，并表达普通 C++ 模板难以完整表达的编译器语义。

### Day 63：把语法点收束成类型系统工具链

Day 63 的整理重点是把本周内容串起来：SFINAE 解决“候选是否存在”，`enable_if` 把布尔条件接到候选选择上，`type_traits` 提供可复用的类型计算和判断，`static_assert` 则把验证前移到编译期。

## 本周关键结论

1. SFINAE 不是“吞掉所有模板错误”，它只处理替换阶段的失败。
2. `void_t` 的本质是把检测表达式折叠成 `void`，再借助偏特化判断检测是否成功。
3. `enable_if` 适合控制重载可见性，`if constexpr` 适合函数体内部类型分支。
4. 基础 `type_traits` 大多是主模板和偏特化；进阶 trait 往往需要 `decltype`、`declval` 和表达式探测。
5. 标准库实现会优先使用编译器内置，教学版实现重在理解机制，工业版实现还要考虑编译性能和 ABI/debug 细节。

## 验证命令

```powershell
cmake -S CPP-Practice/template_traits -B CPP-Practice/template_traits/build
cmake --build CPP-Practice/template_traits/build --config Release
.\CPP-Practice\template_traits\build\Release\template_traits_demo.exe
```

---