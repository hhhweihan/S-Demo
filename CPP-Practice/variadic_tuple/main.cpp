#include <cstdlib>  // 使用 std::exit 在断言失败时终止程序。
#include <initializer_list>  // 使用 initializer_list 展开参数包并执行副作用表达式。
#include <iostream>  // 使用标准输入输出流打印测试结果。
#include <sstream>  // 使用 std::ostringstream 收集格式化输出用于断言。
#include <string>  // 使用 std::string 参与模板参数和输出比较。
#include <tuple>  // 使用 std::tuple、std::get 和 std::tuple_element_t 做标准库对照。
#include <type_traits>  // 使用 std::is_integral_v、std::is_same_v 等类型萃取。
#include <utility>  // 使用 std::forward 和 std::index_sequence 等工具。

#include "templates/my_tuple.h"  // 引入自定义 Tuple、get、apply 和 print 实现，供示例统一调用。

namespace demo {  // 示例代码放在 demo 命名空间中，避免污染全局命名空间。

void expect(bool condition, const std::string& message) {  // 简单断言辅助函数，用于统一输出通过/失败信息。
  if (!condition) {  // 条件为 false 表示当前示例验证失败。
    std::cerr << "[FAIL] " << message << '\n';  // 把失败信息输出到标准错误流。
    std::exit(1);  // 立即终止程序，避免继续运行产生误导结果。
  }  // 失败分支结束。
  std::cout << "[PASS] " << message << '\n';  // 条件满足时输出通过信息。
}  // expect 辅助函数结束。

template <typename... Args>  // Args 表示任意数量的加法参数类型。
auto sum(Args... args) {  // 对参数包中的所有值求和。
  return (args + ...);  // 使用一元右折叠表达式展开加法。
}  // sum 模板函数结束。

template <typename... Args>  // Args 表示要打印的任意数量参数类型。
void print_all(std::ostream& os, Args&&... args) {  // 把参数包中的值按逗号分隔写入输出流。
  std::size_t index = 0;  // 记录当前是第几个参数，用于决定是否输出分隔符。
  ((os << (index++ == 0 ? "" : ", ") << std::forward<Args>(args)), ...);  // 用逗号折叠表达式依次输出每个参数。
}  // print_all 模板函数结束。

template <typename... Args>  // Args 是待统计的类型参数包。
constexpr std::size_t count() {  // 在编译期返回类型参数的数量。
  return sizeof...(Args);  // sizeof... 直接计算参数包长度。
}  // count 模板函数结束。

template <typename... Args>  // Args 是待检查的类型参数包。
constexpr bool all_integral = (std::is_integral_v<Args> && ...);  // 使用逻辑与折叠判断所有类型是否都是整数类型。

template <std::size_t N, typename... Args>  // N 是类型索引，Args 是类型包。
using nth_type = std::tuple_element_t<N, std::tuple<Args...>>;  // 借助 std::tuple_element_t 取得第 N 个类型。

template <typename T, typename... Ts>  // T 是基准类型，Ts 是剩余参数类型包。
T max_of(T first, Ts... rest) {  // 计算一组值中的最大值。
  T result = first;  // 先把第一个参数作为当前最大值。
  (void)std::initializer_list<int>{((result = result < rest ? rest : result), 0)...};  // 借助 initializer_list 强制从左到右遍历剩余参数。
  return result;  // 返回最终最大值。
}  // max_of 模板函数结束。

template <typename Tuple, std::size_t... Is>  // Tuple 是标准 tuple 类型，Is 是索引包。
void print_std_tuple_impl(std::ostream& os, const Tuple& tuple,  // 使用索引包展开 std::tuple 的元素。
                          std::index_sequence<Is...>) {  // index_sequence 提供编译期索引列表。
  ((os << (Is == 0 ? "" : " ") << std::get<Is>(tuple)), ...);  // 逐项输出元素，并在非首元素前加空格。
}  // 标准 tuple 打印实现结束。

template <typename... Ts>  // Ts 是 std::tuple 的元素类型包。
void print_std_tuple(std::ostream& os, const std::tuple<Ts...>& tuple) {  // 对外提供标准 tuple 打印函数。
  print_std_tuple_impl(os, tuple, std::make_index_sequence<sizeof...(Ts)>{});  // 生成索引序列并调用实现函数。
}  // print_std_tuple 模板函数结束。

void run_variadic_examples() {  // 运行可变参数模板和折叠表达式的基础示例。
  std::cout << "== variadic template examples ==\n";  // 输出当前示例分组标题。

  static_assert(count<int, double, std::string>() == 3, "count should use sizeof...");  // 编译期验证 sizeof... 能正确统计类型个数。
  static_assert(all_integral<int, long, unsigned>, "all args are integral");  // 编译期验证所有整数类型时结果为 true。
  static_assert(!all_integral<int, double>, "double is not integral");  // 编译期验证包含 double 时结果为 false。
  static_assert(std::is_same_v<nth_type<1, int, double, std::string>, double>,  // 编译期验证类型包第 1 个类型是 double。
                "nth_type<1> should be double");  // static_assert 失败时显示的错误信息。

  expect(sum(1, 2, 3, 4) == 10, "sum(1, 2, 3, 4) == 10");  // 运行期验证折叠求和结果。

  std::ostringstream printed;  // 创建字符串输出流，用于捕获 print_all 的输出。
  print_all(printed, 1, "two", 3.0);  // 打印混合类型参数包。
  expect(printed.str() == "1, two, 3", "print_all prints comma separated values");  // 验证输出格式符合预期。
}  // 可变参数基础示例结束。

void run_fold_examples() {  // 运行折叠表达式和索引序列相关示例。
  std::cout << "== fold expression examples ==\n";  // 输出当前示例分组标题。

  expect(max_of(3, 1, 4, 1, 5, 9) == 9,  // 验证 max_of 能遍历参数包并找到最大值。
         "max_of(3, 1, 4, 1, 5, 9) == 9");  // 断言说明文本。

  std::tuple<int, double, std::string> values{1, 2.5, "tuple"};  // 构造标准库 tuple 作为索引序列展开示例。
  std::ostringstream output;  // 创建字符串输出流捕获打印结果。
  print_std_tuple(output, values);  // 使用 index_sequence 逐元素打印 std::tuple。
  expect(output.str() == "1 2.5 tuple", "print std::tuple with index_sequence");  // 验证标准 tuple 打印结果。
}  // 折叠表达式示例结束。

void run_tuple_examples() {  // 运行自定义 Tuple 的构造、访问、比较和打印示例。
  std::cout << "== custom Tuple examples ==\n";  // 输出当前示例分组标题。

  using tuple::Tuple;  // 引入自定义 Tuple 类型，减少后续限定名。
  using tuple::get;  // 引入自定义 get 函数，便于按索引访问元素。

  Tuple<int, double, std::string> values(1, 2.0, "hello");  // 构造包含 int、double、string 的自定义 Tuple。

  static_assert(std::is_same_v<tuple::tuple_element_t<0, int, double, std::string>, int>,  // 验证第 0 个元素类型推导为 int。
                "TupleElement<0> should be int");  // static_assert 失败时显示的错误信息。
  static_assert(std::is_same_v<tuple::tuple_element_t<2, int, double, std::string>, std::string>,  // 验证第 2 个元素类型推导为 string。
                "TupleElement<2> should be string");  // static_assert 失败时显示的错误信息。
  static_assert(sizeof(Tuple<int, double>) >= sizeof(int) + sizeof(double),  // 验证 Tuple 至少保存了两个成员值，允许存在对齐填充。
                "Tuple<int, double> stores both values, with possible padding");  // static_assert 失败时显示的错误信息。

  expect(get<0>(values) == 1, "get<0>(tuple) == 1");  // 验证 get<0> 可以访问第一个元素。
  expect(get<2>(values) == "hello", "get<2>(tuple) == hello");  // 验证 get<2> 可以访问第三个元素。

  get<0>(values) = 3;  // 修改 get 返回的引用，验证它不是临时值。
  expect(get<0>(values) == 3, "get<N> returns a mutable reference");  // 验证修改已经写回 Tuple 内部。

  const double applied = tuple::apply([](int a, double b, const std::string&) {  // 用 lambda 接收 Tuple 展开后的三个元素。
    return a + b;  // 返回前两个元素之和，忽略字符串元素。
  }, values);  // 调用 apply，把 values 展开为 lambda 参数。
  expect(applied == 5.0, "apply expands Tuple into function arguments");  // 验证 apply 展开后的计算结果。

  Tuple<int, double, std::string> same(3, 2.0, "hello");  // 构造与 values 内容相同的 Tuple。
  Tuple<int, double, std::string> different(3, 2.0, "world");  // 构造最后一个元素不同的 Tuple。
  expect(values == same, "operator== compares every element");  // 验证 operator== 会逐元素比较。
  expect(values != different, "operator!= detects different elements");  // 验证 operator!= 能发现元素差异。

  std::ostringstream output;  // 创建字符串输出流捕获自定义 Tuple 打印结果。
  tuple::print(output, values);  // 调用自定义 print 输出 Tuple 内容。
  expect(output.str() == "(3, 2, hello)", "print(tuple) writes parenthesized values");  // 验证打印格式为括号包裹的逗号分隔值。
}  // 自定义 Tuple 示例结束。

}  // namespace demo  // 示例命名空间结束。

int main() {  // 程序入口，按顺序运行所有可变参数模板示例。
  demo::run_variadic_examples();  // 运行基础可变参数模板示例。
  std::cout << "----\n";  // 输出分隔线，区分示例分组。

  demo::run_fold_examples();  // 运行折叠表达式和 index_sequence 示例。
  std::cout << "----\n";  // 输出分隔线，区分示例分组。

  demo::run_tuple_examples();  // 运行自定义 Tuple 示例。
  return 0;  // 返回 0 表示所有示例验证通过。
}  // main 函数结束。