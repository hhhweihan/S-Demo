#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "templates/my_type_traits.h"

namespace demo {

template <typename T, typename U>
struct is_same : std::false_type {};

// 偏特化只在两个实参是同一类型时匹配——这正是 is_same 的全部机制。
template <typename T>
struct is_same<T, T> : std::true_type {};

// 尾置返回类型里的 value.size() 走 SFINAE：没有 size() 的类型被静默移出重载集，而非硬报错。
template <typename T>
auto print_size(const T& value) -> decltype(value.size(), void()) {
    std::cout << "size = " << value.size() << '\n';
}

template <typename T, typename = void>
struct has_value_type : std::false_type {};

// void_t 探测 T::value_type 是否存在：存在则命中此偏特化，否则回退到主模板的 false_type。
template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

template <typename T, typename = void>
struct has_iterator : std::false_type {};

// 用 declval<T&> 而非 declval<T>：begin() 往往是非 const 左值成员，必须在左值上探测。
template <typename T>
struct has_iterator<T, std::void_t<decltype(std::declval<T&>().begin())>> : std::true_type {};

// 返回类型里的 enable_if 是 SFINAE 开关：非整数类型没有 ::type，double_it 直接从重载集消失。
template <typename T>
typename traits::enable_if<traits::is_integral<T>::value, T>::type double_it(T value) {
    return value * 2;
}

template <typename T>
auto double_it_if_constexpr(T value) {
    if constexpr (traits::is_integral<T>::value) {
        return value * 2;
    } else {
        // static_assert 依赖 T，只在非整数实例化的 else 分支触发，给出可读错误而非晦涩模板报错。
        static_assert(traits::is_integral<T>::value,
                      "double_it_if_constexpr requires an integral type");
    }
}

template <typename T, typename = void>
struct can_double_it : std::false_type {};

// 探测 double_it(T) 能否成立，间接验证上面 enable_if 的 SFINAE 确实屏蔽了非整数类型。
template <typename T>
struct can_double_it<T, std::void_t<decltype(double_it(std::declval<T>()))>> : std::true_type {};

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

// 主模板 / 显式特化 / 独立重载三者的优先级演示。核心陷阱：函数模板的显式特化不作为独立候选参与
// 重载决议——决议先在主模板与重载之间选,选中主模板后才会去看它有没有匹配的特化。
template <typename T>
std::string pointer_case(T) {
    return "primary template<T>(T)";
}

template <>
std::string pointer_case<int*>(int*) {
    return "explicit specialization<int*>(int*)";
}

// 独立的 (T*) 重载对 int* 比主模板 (T) 更特化，直接胜出，上面的 <int*> 特化永远选不到。
template <typename T>
std::string pointer_case(T*) {
    return "overload<T>(T*)";
}

template <typename T>
std::string array_case(T) {
    return "primary template<T>(T)";
}

template <>
std::string array_case<const char*>(const char*) {
    return "explicit specialization<const char*>(const char*)";
}

// 数组引用重载保留字面量的 const char[N] 类型（不退化成指针），对字符串字面量比主模板更匹配。
template <std::size_t N>
std::string array_case(const char (&)[N]) {
    return "overload<const char(&)[N]>(const char (&)[N])";
}

template <typename T>
std::string non_template_case(T) {
    return "primary template<T>(T)";
}

template <>
std::string non_template_case<int>(int) {
    return "explicit specialization<int>(int)";
}

// 同等匹配时非模板函数优先于任何函数模板，因此它压过上面的主模板及其 int 特化。
std::string non_template_case(int) {
    return "non-template overload(int)";
}

namespace ns {

struct Foo {
    int value;
};

// 与 Foo 同命名空间的 swap，才能被非限定调用经 ADL 找到。
void swap(Foo& lhs, Foo& rhs) noexcept {
    std::cout << "ns::swap triggered by ADL\n";
    std::swap(lhs.value, rhs.value);
}

}  // namespace ns

void run_overload_priority_examples() {
    std::cout << "== overload / specialization examples ==\n";

    int value = 7;
    expect(pointer_case(&value) == "overload<T>(T*)",
           "example 1: pointer overload wins before the primary template can dispatch to its int* "
           "specialization");

    expect(array_case("sfinae") == "overload<const char(&)[N]>(const char (&)[N])",
           "example 2: array-reference overload keeps the string literal type and beats the const "
           "char* specialization path");

    expect(
        non_template_case(42) == "non-template overload(int)",
        "example 3: non-template overload beats the primary template and its int specialization");
}

void run_adl_swap_example() {
    std::cout << "== ADL swap example ==\n";

    ns::Foo left{1};
    ns::Foo right{2};

    // "using std::swap; 再非限定调用 swap" 是标准惯用法：std::swap 作兜底进入候选集，
    // 同时让 ADL 有机会选中用户在 Foo 命名空间里提供的更优 swap。
    using std::swap;
    swap(left, right);

    expect(left.value == 2 && right.value == 1,
           "using std::swap; swap(a, b); finds ns::swap by ADL for user-defined type");
}

void run_is_same_example() {
    std::cout << "== is_same example ==\n";

    static_assert(is_same<int, int>::value, "is_same<int, int> should be true");
    static_assert(!is_same<int, double>::value, "is_same<int, double> should be false");

    expect(is_same<int, int>::value, "is_same<int, int>::value == true");
    expect(!is_same<int, double>::value, "is_same<int, double>::value == false");
}

void run_sfinae_examples() {
    std::cout << "== SFINAE examples ==\n";

    std::vector<int> values{1, 2, 3};
    print_size(values);

    static_assert(has_value_type<std::vector<int>>::value, "vector should expose value_type");
    static_assert(!has_value_type<int>::value, "int should not expose value_type");
    static_assert(has_iterator<std::vector<int>>::value, "vector should expose begin()");
    static_assert(has_iterator<std::list<int>>::value, "list should expose begin()");
    static_assert(!has_iterator<int>::value, "int should not expose begin()");

    expect(has_iterator<std::vector<int>>::value, "has_iterator<std::vector<int>>::value == true");
    expect(!has_iterator<int>::value, "has_iterator<int>::value == false");
}

void run_enable_if_examples() {
    std::cout << "== enable_if examples ==\n";

    static_assert(traits::is_same<traits::enable_if<true, int>::type, int>::value,
                  "enable_if<true, int>::type should be int");
    static_assert(can_double_it<int>::value, "double_it(int) should be viable");
    static_assert(!can_double_it<double>::value, "double_it(double) should be removed by SFINAE");

    expect(double_it(3) == 6, "double_it(3) == 6");
    expect(double_it_if_constexpr(4) == 8, "if constexpr version doubles integral values");
}

int plus_one(int value) {
    return value + 1;
}

struct Widget {
    // explicit + 仅接受 int：这是下面 is_constructible<Widget, std::string> 为 false 的原因。
    explicit Widget(int initial) : value(initial) {}

    int value;
};

void run_type_traits_examples() {
    std::cout << "== custom type_traits examples ==\n";

    static_assert(traits::is_same<traits::remove_const_t<const int>, int>::value,
                  "remove_const<const int> -> int");
    static_assert(traits::is_same<traits::remove_reference_t<int&>, int>::value,
                  "remove_reference<int&> -> int");
    static_assert(traits::is_same<traits::remove_cv_t<const volatile int>, int>::value,
                  "remove_cv<const volatile int> -> int");
    static_assert(traits::is_same<traits::add_pointer_t<int&>, int*>::value,
                  "add_pointer<int&> -> int*");
    static_assert(traits::is_same<traits::add_lvalue_reference_t<int>, int&>::value,
                  "add_lvalue_reference<int> -> int&");
    static_assert(traits::is_pointer<int*>::value, "is_pointer<int*> == true");
    static_assert(!traits::is_pointer<int>::value, "is_pointer<int> == false");
    static_assert(traits::is_reference<int&&>::value, "is_reference<int&&> == true");
    static_assert(traits::is_array<int[3]>::value, "is_array<int[3]> == true");
    static_assert(traits::is_same<traits::conditional<true, int, double>::type, int>::value,
                  "conditional<true, int, double> -> int");
    static_assert(traits::is_same<traits::conditional<false, int, double>::type, double>::value,
                  "conditional<false, int, double> -> double");
    static_assert(traits::is_same<traits::decay_t<const int&>, int>::value,
                  "decay<const int&> -> int");
    static_assert(traits::is_same<traits::decay_t<int[3]>, int*>::value, "decay<int[3]> -> int*");
    static_assert(traits::is_constructible<Widget, int>::value,
                  "Widget should be constructible from int");
    static_assert(!traits::is_constructible<Widget, std::string>::value,
                  "Widget should not be constructible from string");
    static_assert(traits::is_convertible<int, double>::value,
                  "int should be convertible to double");
    static_assert(!traits::is_convertible<std::string, int>::value,
                  "string should not be convertible to int");
    static_assert(traits::is_same<traits::invoke_result_t<decltype(&plus_one), int>, int>::value,
                  "invoke_result<int(*)(int), int>::type should be int");

    expect(traits::is_same<traits::invoke_result_t<decltype(&plus_one), int>, int>::value,
           "invoke_result<int(*)(int), int>::type is int");
}

}  // namespace demo

int main() {
    demo::run_overload_priority_examples();
    std::cout << "----\n";

    demo::run_adl_swap_example();
    std::cout << "----\n";

    demo::run_is_same_example();
    std::cout << "----\n";

    demo::run_sfinae_examples();
    std::cout << "----\n";

    demo::run_enable_if_examples();
    std::cout << "----\n";

    demo::run_type_traits_examples();
    return 0;
}
