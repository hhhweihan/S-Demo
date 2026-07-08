#pragma once  // 防止该 type_traits 头文件在同一编译单元中被重复包含。

#include <cstddef>  // 使用 std::size_t 表示数组长度等编译期大小。
#include <utility>  // 使用 std::declval 模拟表达式类型推导，不真正构造对象。

namespace demo::traits {  // 自定义 traits 放在 demo::traits 命名空间中，避免和 std::traits 冲突。

template <typename T, T Value>  // T 是常量类型，Value 是编译期常量值。
struct integral_constant {  // 用类型包装一个编译期常量，是 true_type/false_type 的基础。
  static constexpr T value = Value;  // 暴露编译期常量值。
  using value_type = T;  // 暴露常量值的类型。
  using type = integral_constant;  // 暴露自身类型，模仿标准库 integral_constant 接口。

  constexpr operator value_type() const noexcept { return value; }  // 允许对象在常量表达式中隐式转换为其值。
};  // integral_constant 定义结束。

using true_type = integral_constant<bool, true>;  // 表示编译期 true 的类型。
using false_type = integral_constant<bool, false>;  // 表示编译期 false 的类型。

template <typename T, typename U>  // T/U 是待比较的两个类型。
struct is_same : false_type {};  // 默认认为两个类型不同。

template <typename T>  // 当两个模板参数都是同一个 T 时匹配该特化。
struct is_same<T, T> : true_type {};  // 相同类型时继承 true_type。

template <bool B, typename T = void>  // B 控制是否暴露 type，T 是成功时返回的类型。
struct enable_if {};  // 默认不提供 type，让依赖它的模板替换失败。

template <typename T>  // 当条件为 true 时匹配该特化。
struct enable_if<true, T> {  // enable_if 成功分支。
  using type = T;  // 暴露 type，使模板替换继续成功。
};  // enable_if<true> 定义结束。

template <typename...>  // 接收任意类型参数包。
using void_t = void;  // 只要参数替换成功，结果类型就是 void，常用于检测表达式是否有效。

template <typename T>  // T 是待移除 const 的类型。
struct remove_const {  // 默认类型没有顶层 const。
  using type = T;  // 原样返回类型。
};  // remove_const 主模板结束。

template <typename T>  // 匹配带顶层 const 的类型。
struct remove_const<const T> {  // const 特化分支。
  using type = T;  // 去掉顶层 const 后返回原始 T。
};  // remove_const<const T> 特化结束。

template <typename T>  // T 是待移除 volatile 的类型。
struct remove_volatile {  // 默认类型没有顶层 volatile。
  using type = T;  // 原样返回类型。
};  // remove_volatile 主模板结束。

template <typename T>  // 匹配带顶层 volatile 的类型。
struct remove_volatile<volatile T> {  // volatile 特化分支。
  using type = T;  // 去掉顶层 volatile 后返回原始 T。
};  // remove_volatile<volatile T> 特化结束。

template <typename T>  // T 是待移除 const/volatile 的类型。
struct remove_cv {  // 组合 remove_const 和 remove_volatile。
  using type = typename remove_volatile<typename remove_const<T>::type>::type;  // 先移除 const，再移除 volatile。
};  // remove_cv 定义结束。

template <typename T>  // T 是待移除引用的类型。
struct remove_reference {  // 默认类型不是引用。
  using type = T;  // 原样返回类型。
};  // remove_reference 主模板结束。

template <typename T>  // 匹配左值引用类型。
struct remove_reference<T&> {  // 左值引用特化分支。
  using type = T;  // 去掉 & 后返回被引用类型。
};  // remove_reference<T&> 特化结束。

template <typename T>  // 匹配右值引用类型。
struct remove_reference<T&&> {  // 右值引用特化分支。
  using type = T;  // 去掉 && 后返回被引用类型。
};  // remove_reference<T&&> 特化结束。

template <typename T>  // T 是待处理类型。
using remove_const_t = typename remove_const<T>::type;  // remove_const 的别名模板，减少 typename 书写。

template <typename T>  // T 是待处理类型。
using remove_cv_t = typename remove_cv<T>::type;  // remove_cv 的别名模板。

template <typename T>  // T 是待处理类型。
using remove_reference_t = typename remove_reference<T>::type;  // remove_reference 的别名模板。

template <typename T>  // T 是待添加指针的类型。
struct add_pointer {  // 模拟 std::add_pointer 的基础行为。
  using type = remove_reference_t<T>*;  // 先移除引用，再添加指针符号。
};  // add_pointer 定义结束。

namespace detail {  // detail 命名空间保存实现细节，避免暴露给示例调用者。

template <typename T>  // T 是尝试添加左值引用的类型。
auto try_add_lvalue_reference(int) -> T&;  // 优先重载：如果 T& 合法，就返回 T&。

template <typename T>  // T 是尝试添加左值引用的类型。
auto try_add_lvalue_reference(...) -> T;  // 兜底重载：T& 不合法时保留 T。

}  // namespace detail  // add_lvalue_reference 的检测细节结束。

template <typename T>  // T 是待添加左值引用的类型。
struct add_lvalue_reference {  // 模拟 std::add_lvalue_reference。
  using type = decltype(detail::try_add_lvalue_reference<T>(0));  // 通过重载决议得到 T& 或 T。
};  // add_lvalue_reference 定义结束。

template <typename T>  // T 是待处理类型。
using add_pointer_t = typename add_pointer<T>::type;  // add_pointer 的别名模板。

template <typename T>  // T 是待处理类型。
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;  // add_lvalue_reference 的别名模板。

namespace detail {  // 保存 is_pointer 的辅助特化。

template <typename T>  // T 是移除 cv 后的待判断类型。
struct is_pointer_helper : false_type {};  // 默认不是指针。

template <typename T>  // 匹配任意对象/函数指针类型。
struct is_pointer_helper<T*> : true_type {};  // T* 形式说明它是指针。

}  // namespace detail  // is_pointer 辅助实现结束。

template <typename T>  // T 是待判断类型。
struct is_pointer : detail::is_pointer_helper<remove_cv_t<T>> {};  // 去掉顶层 cv 后复用指针特化判断。

template <typename T>  // T 是待判断类型。
struct is_reference : false_type {};  // 默认不是引用。

template <typename T>  // 匹配左值引用。
struct is_reference<T&> : true_type {};  // T& 是引用类型。

template <typename T>  // 匹配右值引用。
struct is_reference<T&&> : true_type {};  // T&& 是引用类型。

template <typename T>  // T 是待判断类型。
struct is_array : false_type {};  // 默认不是数组。

template <typename T>  // 匹配未知边界数组。
struct is_array<T[]> : true_type {};  // T[] 是数组类型。

template <typename T, std::size_t N>  // T 是元素类型，N 是数组长度。
struct is_array<T[N]> : true_type {};  // T[N] 是已知边界数组类型。

template <bool B, typename T, typename F>  // B 是条件，T/F 是两个候选类型。
struct conditional {  // 条件为 true 的默认分支。
  using type = T;  // 选择 T。
};  // conditional 主模板结束。

template <typename T, typename F>  // 条件为 false 时匹配该特化。
struct conditional<false, T, F> {  // conditional 的 false 分支。
  using type = F;  // 选择 F。
};  // conditional<false> 特化结束。

template <typename T>  // T 是待移除数组维度的类型。
struct remove_extent {  // 默认不是数组。
  using type = T;  // 原样返回类型。
};  // remove_extent 主模板结束。

template <typename T>  // 匹配未知边界数组。
struct remove_extent<T[]> {  // T[] 特化分支。
  using type = T;  // 去掉一层数组维度，得到元素类型。
};  // remove_extent<T[]> 特化结束。

template <typename T, std::size_t N>  // 匹配已知边界数组。
struct remove_extent<T[N]> {  // T[N] 特化分支。
  using type = T;  // 去掉一层数组维度，得到元素类型。
};  // remove_extent<T[N]> 特化结束。

template <typename T>  // T 是待处理类型。
using remove_extent_t = typename remove_extent<T>::type;  // remove_extent 的别名模板。

template <typename T>  // T 是待退化的类型。
struct decay {  // 简化版 std::decay，处理引用、数组和 cv 限定。
 private:  // 中间类型只供 decay 内部使用。
  using U = remove_reference_t<T>;  // 先移除引用，得到后续判断的基础类型。

 public:  // 暴露最终退化后的 type。
  using type = typename conditional<  // 根据 U 是否为数组选择不同退化规则。
      is_array<U>::value,  // 数组类型需要退化为指向首元素的指针。
      add_pointer_t<remove_extent_t<U>>,  // 数组分支：去掉一层数组维度后添加指针。
      remove_cv_t<U>>::type;  // 非数组分支：去掉顶层 const/volatile。
};  // decay 定义结束。

template <typename T>  // T 是待处理类型。
using decay_t = typename decay<T>::type;  // decay 的别名模板。

namespace detail {  // 保存 constructible/convertible 的表达式检测细节。

template <typename T, typename... Args>  // T 是目标类型，Args 是构造实参类型包。
auto test_constructible(int) -> decltype(T(std::declval<Args>()...), true_type{});  // 构造表达式合法时返回 true_type。

template <typename, typename...>  // 兜底重载不关心具体类型。
auto test_constructible(...) -> false_type;  // 构造表达式不合法时通过 SFINAE 选择 false_type。

template <typename To>  // To 是转换目标类型。
void accept(To);  // 声明一个接收 To 的函数，用于检测 From 能否传给 To。

template <typename From, typename To>  // From 是源类型，To 是目标类型。
auto test_convertible(int) -> decltype(accept<To>(std::declval<From>()), true_type{});  // 如果 From 可传给 To，则返回 true_type。

template <typename, typename>  // 兜底重载不关心具体类型。
auto test_convertible(...) -> false_type;  // 转换表达式不合法时通过 SFINAE 选择 false_type。

}  // namespace detail  // 可构造/可转换检测细节结束。

template <typename T, typename... Args>  // T 是目标类型，Args 是构造参数类型包。
struct is_constructible : decltype(detail::test_constructible<T, Args...>(0)) {};  // 继承检测函数返回的 true_type 或 false_type。

template <typename From, typename To>  // From 是源类型，To 是目标类型。
struct is_convertible : decltype(detail::test_convertible<From, To>(0)) {};  // 继承检测函数返回的 true_type 或 false_type。

template <typename F, typename... Args>  // F 是可调用对象类型，Args 是调用参数类型包。
struct invoke_result {  // 推导调用 F(Args...) 后的返回类型。
  using type = decltype(std::declval<F>()(std::declval<Args>()...));  // 用 declval 构造未求值调用表达式并取其类型。
};  // invoke_result 定义结束。

template <typename F, typename... Args>  // F 是可调用对象类型，Args 是调用参数类型包。
using invoke_result_t = typename invoke_result<F, Args...>::type;  // invoke_result 的别名模板。

template <typename T>  // T 是待判断类型。
struct is_integral : false_type {};  // 默认不是整数类型。

template <>  // bool 是整数类型之一。
struct is_integral<bool> : true_type {};  // bool 判断为 true。

template <>  // char 是整数类型之一。
struct is_integral<char> : true_type {};  // char 判断为 true。

template <>  // signed char 是整数类型之一。
struct is_integral<signed char> : true_type {};  // signed char 判断为 true。

template <>  // unsigned char 是整数类型之一。
struct is_integral<unsigned char> : true_type {};  // unsigned char 判断为 true。

template <>  // short 是整数类型之一。
struct is_integral<short> : true_type {};  // short 判断为 true。

template <>  // unsigned short 是整数类型之一。
struct is_integral<unsigned short> : true_type {};  // unsigned short 判断为 true。

template <>  // int 是整数类型之一。
struct is_integral<int> : true_type {};  // int 判断为 true。

template <>  // unsigned int 是整数类型之一。
struct is_integral<unsigned int> : true_type {};  // unsigned int 判断为 true。

template <>  // long 是整数类型之一。
struct is_integral<long> : true_type {};  // long 判断为 true。

template <>  // unsigned long 是整数类型之一。
struct is_integral<unsigned long> : true_type {};  // unsigned long 判断为 true。

template <>  // long long 是整数类型之一。
struct is_integral<long long> : true_type {};  // long long 判断为 true。

template <>  // unsigned long long 是整数类型之一。
struct is_integral<unsigned long long> : true_type {};  // unsigned long long 判断为 true。

template <typename T>  // T 是去掉 const 后的类型。
struct is_integral<const T> : is_integral<T> {};  // const 限定不影响是否为整数类型。

template <typename T>  // T 是去掉 volatile 后的类型。
struct is_integral<volatile T> : is_integral<T> {};  // volatile 限定不影响是否为整数类型。

template <typename T>  // T 是去掉 const volatile 后的类型。
struct is_integral<const volatile T> : is_integral<T> {};  // const volatile 限定不影响是否为整数类型。

}  // namespace demo::traits  // 自定义 type_traits 命名空间结束。