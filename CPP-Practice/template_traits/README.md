# template_traits

Week 09（Day 057-063）对应的 SFINAE 和 type_traits 练习代码放在这个目录。

## 覆盖内容

- 函数模板、显式特化、普通函数重载的优先级差异
- 用 3 个最小例子区分“模板特化”和“函数重载”分别在什么时候生效
- ADL（Argument-Dependent Lookup）配合 `using std::swap; swap(a, b);` 的常见写法
- 最简版 `is_same<T, U>` 实现与静态断言验证
- SFINAE 基础：`decltype(expr, void())`、`void_t`、`has_value_type<T>`、`has_iterator<T>`
- 手写 `enable_if`，并对比 C++17 `if constexpr`
- 手写基础 type_traits：`remove_const`、`remove_reference`、`remove_cv`、`add_pointer`、`add_lvalue_reference`
- 手写类型判断：`is_pointer`、`is_reference`、`is_array`、`is_integral`
- 手写进阶 type_traits：`conditional`、`decay`、`is_constructible`、`is_convertible`、`invoke_result`

## 目录

- `main.cpp`：运行示例与 `static_assert` 验证
- `templates/my_type_traits.h`：本周手写 traits 实现

## 构建

```powershell
cmake -S CPP-Practice/template_traits -B CPP-Practice/template_traits/build
cmake --build CPP-Practice/template_traits/build --config Release
.\CPP-Practice\template_traits\build\Release\template_traits_demo.exe
```
