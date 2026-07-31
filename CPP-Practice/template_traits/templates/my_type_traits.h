#pragma once

#include <cstddef>
#include <utility>  // std::declval: 在未求值上下文里"造出"一个 T 值，无需 T 可默认构造。

namespace demo::traits {

template <typename T, T Value>
struct integral_constant {
    static constexpr T value = Value;
    using value_type = T;
    using type = integral_constant;

    // 隐式转换到 value_type，使对象能直接用在常量表达式/需要值的位置，贴合标准库接口。
    constexpr operator value_type() const noexcept { return value; }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <typename T, typename U>
struct is_same : false_type {};

// 仅当两个实参是同一类型时匹配的偏特化。
template <typename T>
struct is_same<T, T> : true_type {};

// 主模板不提供 ::type，让依赖它的模板在条件为假时替换失败（SFINAE 的开关本体）。
template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
    using type = T;
};

// 只要参数包全部替换成功结果就是 void；配合 decltype 用来探测表达式/类型是否合法。
template <typename...>
using void_t = void;

template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
struct remove_volatile {
    using type = T;
};

template <typename T>
struct remove_volatile<volatile T> {
    using type = T;
};

template <typename T>
struct remove_cv {
    using type = typename remove_volatile<typename remove_const<T>::type>::type;
};

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct add_pointer {
    // 先去引用再加指针：引用没有"指向自身的指针"，直接 T* 对 int& 会失败。
    using type = remove_reference_t<T>*;
};

namespace detail {

// 重载技巧探测 T& 是否合法：能形成引用时优先选中 int 版返回 T&；
// 对 void 之类无法引用的类型，T& 在此重载上 SFINAE 失败，回退到 ... 版保留 T。
template <typename T>
auto try_add_lvalue_reference(int) -> T&;

template <typename T>
auto try_add_lvalue_reference(...) -> T;

}  // namespace detail

template <typename T>
struct add_lvalue_reference {
    using type = decltype(detail::try_add_lvalue_reference<T>(0));
};

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

namespace detail {

template <typename T>
struct is_pointer_helper : false_type {};

template <typename T>
struct is_pointer_helper<T*> : true_type {};

}  // namespace detail

// 先剥掉顶层 cv，才能让 int* const、int* volatile 这类也被判为指针。
template <typename T>
struct is_pointer : detail::is_pointer_helper<remove_cv_t<T>> {};

template <typename T>
struct is_reference : false_type {};

template <typename T>
struct is_reference<T&> : true_type {};

template <typename T>
struct is_reference<T&&> : true_type {};

template <typename T>
struct is_array : false_type {};

// 未知边界 T[] 与已知边界 T[N] 需要各自一个偏特化才能都命中。
template <typename T>
struct is_array<T[]> : true_type {};

template <typename T, std::size_t N>
struct is_array<T[N]> : true_type {};

template <bool B, typename T, typename F>
struct conditional {
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};

template <typename T>
struct remove_extent {
    using type = T;
};

template <typename T>
struct remove_extent<T[]> {
    using type = T;
};

template <typename T, std::size_t N>
struct remove_extent<T[N]> {
    using type = T;
};

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

template <typename T>
struct decay {
 private:
    using U = remove_reference_t<T>;

 public:
    // 复刻按值传参的类型退化：数组→指向首元素的指针，否则剥掉顶层 cv（函数类型此简化版不处理）。
    using type = typename conditional<is_array<U>::value, add_pointer_t<remove_extent_t<U>>,
                                      remove_cv_t<U>>::type;
};

template <typename T>
using decay_t = typename decay<T>::type;

namespace detail {

// 经典 SFINAE 探测：构造表达式合法时 int 版胜出返回 true_type；不合法时该版被剔除，
// 只剩 ... 版返回 false_type。第一实参 0 用来在两个重载间制造优先级。
template <typename T, typename... Args>
auto test_constructible(int) -> decltype(T(std::declval<Args>()...), true_type{});

template <typename, typename...>
auto test_constructible(...) -> false_type;

// accept<To> 只声明不定义：借"能否把 From 实参传给形参 To"来检测隐式可转换性。
template <typename To>
void accept(To);

template <typename From, typename To>
auto test_convertible(int) -> decltype(accept<To>(std::declval<From>()), true_type{});

template <typename, typename>
auto test_convertible(...) -> false_type;

}  // namespace detail

template <typename T, typename... Args>
struct is_constructible : decltype(detail::test_constructible<T, Args...>(0)) {};

template <typename From, typename To>
struct is_convertible : decltype(detail::test_convertible<From, To>(0)) {};

template <typename F, typename... Args>
struct invoke_result {
    // declval 造出未求值的 F(Args...) 调用表达式,decltype 取其结果类型而不真正调用。
    using type = decltype(std::declval<F>()(std::declval<Args>()...));
};

template <typename F, typename... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

// 逐个整数类型做全特化,标准库正是这样以"白名单"定义 is_integral 而非靠某种通用判据。
template <typename T>
struct is_integral : false_type {};

template <>
struct is_integral<bool> : true_type {};

template <>
struct is_integral<char> : true_type {};

template <>
struct is_integral<signed char> : true_type {};

template <>
struct is_integral<unsigned char> : true_type {};

template <>
struct is_integral<short> : true_type {};

template <>
struct is_integral<unsigned short> : true_type {};

template <>
struct is_integral<int> : true_type {};

template <>
struct is_integral<unsigned int> : true_type {};

template <>
struct is_integral<long> : true_type {};

template <>
struct is_integral<unsigned long> : true_type {};

template <>
struct is_integral<long long> : true_type {};

template <>
struct is_integral<unsigned long long> : true_type {};

// cv 限定不改变"是否整数",转发给无 cv 版本，省去为每个整数类型再写一遍 cv 特化。
template <typename T>
struct is_integral<const T> : is_integral<T> {};

template <typename T>
struct is_integral<volatile T> : is_integral<T> {};

template <typename T>
struct is_integral<const volatile T> : is_integral<T> {};

}  // namespace demo::traits
