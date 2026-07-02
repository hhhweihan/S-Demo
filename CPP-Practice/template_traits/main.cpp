#include <cstdlib>  // 使用 std::exit 在测试失败时终止程序。
#include <iostream>  // 使用 std::cout/std::cerr 输出示例结果。
#include <list>  // 使用 std::list 验证迭代器检测 trait。
#include <string>  // 使用 std::string 作为示例类型和断言消息。
#include <type_traits>  // 使用标准库 true_type、false_type、void_t 等进行对照。
#include <utility>  // 使用 std::declval 和 std::swap。
#include <vector>  // 使用 std::vector 验证容器相关 SFINAE。

#include "templates/my_type_traits.h"  // 引入本练习手写的 type_traits 实现。

namespace demo {  // 示例函数放入 demo 命名空间，避免污染全局命名空间。

template <typename T, typename U>  // T/U 是待比较的两个类型。
struct is_same : std::false_type {};  // 默认分支表示两个类型不同。

template <typename T>  // 两个模板参数完全相同时匹配该特化。
struct is_same<T, T> : std::true_type {};  // 相同类型时继承 true_type。

template <typename T>  // T 是待打印 size 的类型。
auto print_size(const T& value) -> decltype(value.size(), void()) {  // 只有 value.size() 合法时该模板才参与重载。
  std::cout << "size = " << value.size() << '\n';  // 输出容器或对象的 size() 返回值。
}  // print_size 示例函数结束。

template <typename T, typename = void>  // 第二个模板参数用于 SFINAE 检测。
struct has_value_type : std::false_type {};  // 默认认为 T 没有 value_type。

template <typename T>  // T 是待检测类型。
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};  // T::value_type 存在时选择该特化。

template <typename T, typename = void>  // 第二个模板参数用于检测 begin() 表达式。
struct has_iterator : std::false_type {};  // 默认认为 T 没有可用 begin()。

template <typename T>  // T 是待检测类型。
struct has_iterator<T, std::void_t<decltype(std::declval<T&>().begin())>> : std::true_type {};  // begin() 表达式合法时选择该特化。

template <typename T>  // T 是输入值类型。
typename traits::enable_if<traits::is_integral<T>::value, T>::type double_it(T value) {  // 只有 T 是整数类型时函数才存在。
  return value * 2;  // 对整数值执行翻倍操作。
}  // enable_if 版本 double_it 结束。

template <typename T>  // T 是输入值类型。
auto double_it_if_constexpr(T value) {  // 使用 if constexpr 实现整数限定的翻倍示例。
  if constexpr (traits::is_integral<T>::value) {  // 编译期判断 T 是否为整数类型。
    return value * 2;  // 整数分支返回翻倍值。
  } else {  // 非整数分支只在 T 不是整数时参与编译。
    static_assert(traits::is_integral<T>::value, "double_it_if_constexpr requires an integral type");  // 给非整数调用提供清晰编译错误。
  }  // if constexpr 结束。
}  // if constexpr 版本 double_it 结束。

template <typename T, typename = void>  // 第二个模板参数用于检测 double_it(T) 是否可用。
struct can_double_it : std::false_type {};  // 默认认为不能调用 double_it。

template <typename T>  // T 是待检测类型。
struct can_double_it<T, std::void_t<decltype(double_it(std::declval<T>()))>> : std::true_type {};  // double_it(T) 替换成功时选择该特化。

void expect(bool condition, const std::string& message) {  // 简单测试断言函数。
  if (!condition) {  // 条件不成立表示示例验证失败。
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息到标准错误流。
    std::exit(1);  // 立即退出程序，避免继续输出误导结果。
  }  // 失败分支结束。
  std::cout << "[PASS] " << message << '\n';  // 条件成立时输出通过信息。
}  // expect 函数结束。

template <typename T>  // T 是普通模板参数。
std::string pointer_case(T) {  // 指针例子的主模板。
  return "primary template<T>(T)";  // 返回主模板被选中的标记字符串。
}  // pointer_case 主模板结束。

template <>  // 显式特化主模板的 int* 版本。
std::string pointer_case<int*>(int*) {  // 只有主模板被选中且 T 推导为 int* 后才可能使用该特化。
  return "explicit specialization<int*>(int*)";  // 返回显式特化被使用的标记字符串。
}  // pointer_case<int*> 显式特化结束。

template <typename T>  // T 是指针指向的元素类型。
std::string pointer_case(T*) {  // 指针参数重载，比主模板更匹配 int* 实参。
  return "overload<T>(T*)";  // 返回指针重载被选中的标记字符串。
}  // pointer_case 指针重载结束。

template <typename T>  // T 是普通模板参数。
std::string array_case(T) {  // 字符串字面量例子的主模板。
  return "primary template<T>(T)";  // 返回主模板被选中的标记字符串。
}  // array_case 主模板结束。

template <>  // 显式特化主模板的 const char* 版本。
std::string array_case<const char*>(const char*) {  // 当主模板参数退化为 const char* 时可能使用该特化。
  return "explicit specialization<const char*>(const char*)";  // 返回显式特化被使用的标记字符串。
}  // array_case<const char*> 显式特化结束。

template <std::size_t N>  // N 捕获字符串字面量数组长度。
std::string array_case(const char (&)[N]) {  // 数组引用重载保留字符串字面量的数组类型。
  return "overload<const char(&)[N]>(const char (&)[N])";  // 返回数组引用重载被选中的标记字符串。
}  // array_case 数组引用重载结束。

template <typename T>  // T 是普通模板参数。
std::string non_template_case(T) {  // 普通函数重载例子的主模板。
  return "primary template<T>(T)";  // 返回主模板被选中的标记字符串。
}  // non_template_case 主模板结束。

template <>  // 显式特化主模板的 int 版本。
std::string non_template_case<int>(int) {  // 主模板被选中且 T 为 int 时可能使用该特化。
  return "explicit specialization<int>(int)";  // 返回显式特化被使用的标记字符串。
}  // non_template_case<int> 显式特化结束。

std::string non_template_case(int) {  // 非模板普通函数重载。
  return "non-template overload(int)";  // 返回普通函数重载被选中的标记字符串。
}  // non_template_case 普通函数重载结束。

namespace ns {  // 定义用户类型所在命名空间，用于演示 ADL。

struct Foo {  // 简单用户自定义类型。
  int value;  // 保存一个整数值，便于验证 swap 结果。
};  // Foo 定义结束。

void swap(Foo& lhs, Foo& rhs) noexcept {  // 与 Foo 位于同一命名空间的 swap，可被 ADL 找到。
  std::cout << "ns::swap triggered by ADL\n";  // 输出提示，证明命中的是 ns::swap。
  std::swap(lhs.value, rhs.value);  // 交换两个 Foo 内部的 value。
}  // ns::swap 结束。

}  // namespace ns  // ADL 示例命名空间结束。

void run_overload_priority_examples() {  // 运行函数模板、显式特化和普通重载优先级示例。
  std::cout << "== overload / specialization examples ==\n";  // 输出当前示例分组标题。

  int value = 7;  // 准备一个 int 变量，用它的地址触发指针重载候选。
  expect(pointer_case(&value) == "overload<T>(T*)",  // 验证 int* 实参优先选择指针重载，而不是主模板特化。
    "example 1: pointer overload wins before the primary template can dispatch to its int* specialization");  // 断言说明文本。

  expect(array_case("sfinae") == "overload<const char(&)[N]>(const char (&)[N])",  // 验证字符串字面量优先匹配数组引用重载。
    "example 2: array-reference overload keeps the string literal type and beats the const char* specialization path");  // 断言说明文本。

  expect(non_template_case(42) == "non-template overload(int)",  // 验证普通函数重载优先于函数模板及其显式特化。
    "example 3: non-template overload beats the primary template and its int specialization");  // 断言说明文本。
}  // 重载优先级示例结束。

void run_adl_swap_example() {  // 运行 ADL 查找 swap 的示例。
  std::cout << "== ADL swap example ==\n";  // 输出当前示例分组标题。

  ns::Foo left{1};  // 构造左侧 Foo，初始值为 1。
  ns::Foo right{2};  // 构造右侧 Foo，初始值为 2。

  using std::swap;  // 先把 std::swap 放入候选集，作为通用兜底。
  swap(left, right);  // 非限定调用触发 ADL，优先找到 ns::swap(Foo&, Foo&)。

  expect(left.value == 2 && right.value == 1,  // 验证两个 Foo 的值已经交换。
         "using std::swap; swap(a, b); finds ns::swap by ADL for user-defined type");  // 断言说明文本。
}  // ADL swap 示例结束。

void run_is_same_example() {  // 运行最小 is_same 实现示例。
  std::cout << "== is_same example ==\n";  // 输出当前示例分组标题。

  static_assert(is_same<int, int>::value, "is_same<int, int> should be true");  // 编译期验证相同类型返回 true。
  static_assert(!is_same<int, double>::value, "is_same<int, double> should be false");  // 编译期验证不同类型返回 false。

  expect(is_same<int, int>::value,  // 运行期输出相同类型验证结果。
         "is_same<int, int>::value == true");  // 断言说明文本。
  expect(!is_same<int, double>::value,  // 运行期输出不同类型验证结果。
         "is_same<int, double>::value == false");  // 断言说明文本。
}  // is_same 示例结束。

void run_sfinae_examples() {  // 运行 SFINAE 检测成员类型和成员函数的示例。
  std::cout << "== SFINAE examples ==\n";  // 输出当前示例分组标题。

  std::vector<int> values{1, 2, 3};  // 构造 vector，用于演示 size() 检测成功路径。
  print_size(values);  // 调用只有 size() 存在时才可用的 print_size。

  static_assert(has_value_type<std::vector<int>>::value,  // 编译期验证 vector 暴露 value_type。
                "vector should expose value_type");  // static_assert 失败时显示的错误信息。
  static_assert(!has_value_type<int>::value,  // 编译期验证 int 没有 value_type。
                "int should not expose value_type");  // static_assert 失败时显示的错误信息。
  static_assert(has_iterator<std::vector<int>>::value,  // 编译期验证 vector 有 begin()。
                "vector should expose begin()");  // static_assert 失败时显示的错误信息。
  static_assert(has_iterator<std::list<int>>::value,  // 编译期验证 list 有 begin()。
                "list should expose begin()");  // static_assert 失败时显示的错误信息。
  static_assert(!has_iterator<int>::value,  // 编译期验证 int 没有 begin()。
                "int should not expose begin()");  // static_assert 失败时显示的错误信息。

  expect(has_iterator<std::vector<int>>::value,  // 运行期输出 vector 迭代器检测结果。
         "has_iterator<std::vector<int>>::value == true");  // 断言说明文本。
  expect(!has_iterator<int>::value,  // 运行期输出 int 迭代器检测结果。
         "has_iterator<int>::value == false");  // 断言说明文本。
}  // SFINAE 示例结束。

void run_enable_if_examples() {  // 运行 enable_if 和 if constexpr 对比示例。
  std::cout << "== enable_if examples ==\n";  // 输出当前示例分组标题。

  static_assert(traits::is_same<traits::enable_if<true, int>::type, int>::value,  // 验证 enable_if<true> 会暴露 type。
                "enable_if<true, int>::type should be int");  // static_assert 失败时显示的错误信息。
  static_assert(can_double_it<int>::value,  // 验证整数类型可以调用 double_it。
                "double_it(int) should be viable");  // static_assert 失败时显示的错误信息。
  static_assert(!can_double_it<double>::value,  // 验证 double 类型调用被 SFINAE 移除。
                "double_it(double) should be removed by SFINAE");  // static_assert 失败时显示的错误信息。

  expect(double_it(3) == 6, "double_it(3) == 6");  // 运行期验证 enable_if 版本能翻倍整数。
  expect(double_it_if_constexpr(4) == 8,  // 运行期验证 if constexpr 版本能翻倍整数。
         "if constexpr version doubles integral values");  // 断言说明文本。
}  // enable_if 示例结束。

int plus_one(int value) {  // 普通函数，用于测试 invoke_result 的返回类型推导。
  return value + 1;  // 返回输入值加一。
}  // plus_one 函数结束。

struct Widget {  // 简单用户类型，用于测试 is_constructible。
  explicit Widget(int initial) : value(initial) {}  // 只允许从 int 显式构造 Widget。

  int value;  // 保存构造传入的整数值。
};  // Widget 定义结束。

void run_type_traits_examples() {  // 运行手写 type_traits 的综合验证示例。
  std::cout << "== custom type_traits examples ==\n";  // 输出当前示例分组标题。

  static_assert(traits::is_same<traits::remove_const_t<const int>, int>::value,  // 验证 remove_const 能移除顶层 const。
                "remove_const<const int> -> int");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::remove_reference_t<int&>, int>::value,  // 验证 remove_reference 能移除左值引用。
                "remove_reference<int&> -> int");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::remove_cv_t<const volatile int>, int>::value,  // 验证 remove_cv 能移除 const 和 volatile。
                "remove_cv<const volatile int> -> int");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::add_pointer_t<int&>, int*>::value,  // 验证 add_pointer 会先移除引用再加指针。
                "add_pointer<int&> -> int*");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::add_lvalue_reference_t<int>, int&>::value,  // 验证 add_lvalue_reference<int> 得到 int&。
                "add_lvalue_reference<int> -> int&");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_pointer<int*>::value,  // 验证 int* 被识别为指针。
                "is_pointer<int*> == true");  // static_assert 失败时显示的错误信息。
  static_assert(!traits::is_pointer<int>::value,  // 验证 int 不是指针。
                "is_pointer<int> == false");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_reference<int&&>::value,  // 验证右值引用被识别为引用。
                "is_reference<int&&> == true");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_array<int[3]>::value,  // 验证定长数组被识别为数组。
                "is_array<int[3]> == true");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::conditional<true, int, double>::type, int>::value,  // 验证 conditional<true> 选择第一个类型。
                "conditional<true, int, double> -> int");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::conditional<false, int, double>::type, double>::value,  // 验证 conditional<false> 选择第二个类型。
                "conditional<false, int, double> -> double");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::decay_t<const int&>, int>::value,  // 验证 decay 会移除引用和顶层 const。
                "decay<const int&> -> int");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::decay_t<int[3]>, int*>::value,  // 验证数组类型会退化为指针。
                "decay<int[3]> -> int*");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_constructible<Widget, int>::value,  // 验证 Widget 可以从 int 构造。
                "Widget should be constructible from int");  // static_assert 失败时显示的错误信息。
  static_assert(!traits::is_constructible<Widget, std::string>::value,  // 验证 Widget 不能从 string 构造。
                "Widget should not be constructible from string");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_convertible<int, double>::value,  // 验证 int 可以隐式转换为 double。
                "int should be convertible to double");  // static_assert 失败时显示的错误信息。
  static_assert(!traits::is_convertible<std::string, int>::value,  // 验证 string 不能隐式转换为 int。
                "string should not be convertible to int");  // static_assert 失败时显示的错误信息。
  static_assert(traits::is_same<traits::invoke_result_t<decltype(&plus_one), int>, int>::value,  // 验证 plus_one(int) 的调用结果类型是 int。
                "invoke_result<int(*)(int), int>::type should be int");  // static_assert 失败时显示的错误信息。

  expect(traits::is_same<traits::invoke_result_t<decltype(&plus_one), int>, int>::value,  // 运行期输出 invoke_result 验证结果。
         "invoke_result<int(*)(int), int>::type is int");  // 断言说明文本。
}  // 手写 type_traits 综合示例结束。

}  // namespace demo  // 示例命名空间结束。

int main() {  // 程序入口，按主题依次运行模板和 traits 示例。
  demo::run_overload_priority_examples();  // 运行重载优先级示例。
  std::cout << "----\n";  // 输出分隔线，便于阅读不同示例分组。

  demo::run_adl_swap_example();  // 运行 ADL swap 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_is_same_example();  // 运行 is_same 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_sfinae_examples();  // 运行 SFINAE 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_enable_if_examples();  // 运行 enable_if 和 if constexpr 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_type_traits_examples();  // 运行手写 type_traits 综合示例。
  return 0;  // 返回 0 表示所有示例通过。
}  // main 函数结束。