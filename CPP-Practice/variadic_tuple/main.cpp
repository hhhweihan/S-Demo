#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <cstddef>
#include "templates/my_tuple.h"

namespace demo {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

template <typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 一元右折叠展开为 a0 + (a1 + (a2 + ...))。
}

template <typename... Args>
void print_all(std::ostream& os, Args&&... args) {
    std::size_t index = 0;
    // 逗号折叠依次求值每个子表达式；用 index 判首元素来决定是否补 ", " 分隔符。
    ((os << (index++ == 0 ? "" : ", ") << std::forward<Args>(args)), ...);
}

template <typename... Args>
constexpr std::size_t count() {
    return sizeof...(Args);
}

template <typename... Args>
constexpr bool all_integral = (std::is_integral_v<Args> && ...);

template <std::size_t N, typename... Args>
using nth_type = std::tuple_element_t<N, std::tuple<Args...>>;

template <typename T, typename... Ts>
T max_of(T first, Ts... rest) {
    T result = first;
    // 借 initializer_list 的花括号求值保证参数从左到右顺序处理（折叠表达式对 =
    // 的求值顺序在此更绕）。
    (void)std::initializer_list<int>{((result = result < rest ? rest : result), 0)...};
    return result;
}

template <typename Tuple, std::size_t... Is>
void print_std_tuple_impl(std::ostream& os, const Tuple& tuple, std::index_sequence<Is...>) {
    ((os << (Is == 0 ? "" : " ") << std::get<Is>(tuple)), ...);
}

// 对外接口：生成 0..N-1 的 index_sequence 交给 impl，才能用折叠把编译期索引展开成 get<Is>。
template <typename... Ts>
void print_std_tuple(std::ostream& os, const std::tuple<Ts...>& tuple) {
    print_std_tuple_impl(os, tuple, std::make_index_sequence<sizeof...(Ts)>{});
}

void run_variadic_examples() {
    std::cout << "== variadic template examples ==\n";

    static_assert(count<int, double, std::string>() == 3, "count should use sizeof...");
    static_assert(all_integral<int, long, unsigned>, "all args are integral");
    static_assert(!all_integral<int, double>, "double is not integral");
    static_assert(std::is_same_v<nth_type<1, int, double, std::string>, double>,
                  "nth_type<1> should be double");

    expect(sum(1, 2, 3, 4) == 10, "sum(1, 2, 3, 4) == 10");

    std::ostringstream printed;
    print_all(printed, 1, "two", 3.0);
    expect(printed.str() == "1, two, 3", "print_all prints comma separated values");
}

void run_fold_examples() {
    std::cout << "== fold expression examples ==\n";

    expect(max_of(3, 1, 4, 1, 5, 9) == 9, "max_of(3, 1, 4, 1, 5, 9) == 9");

    std::tuple<int, double, std::string> values{1, 2.5, "tuple"};
    std::ostringstream output;
    print_std_tuple(output, values);
    expect(output.str() == "1 2.5 tuple", "print std::tuple with index_sequence");
}

void run_tuple_examples() {
    std::cout << "== custom Tuple examples ==\n";

    using tuple::get;
    using tuple::Tuple;

    Tuple<int, double, std::string> values(1, 2.0, "hello");

    static_assert(std::is_same_v<tuple::tuple_element_t<0, int, double, std::string>, int>,
                  "TupleElement<0> should be int");
    static_assert(std::is_same_v<tuple::tuple_element_t<2, int, double, std::string>, std::string>,
                  "TupleElement<2> should be string");
    static_assert(sizeof(Tuple<int, double>) >= sizeof(int) + sizeof(double),
                  "Tuple<int, double> stores both values, with possible padding");

    expect(get<0>(values) == 1, "get<0>(tuple) == 1");
    expect(get<2>(values) == "hello", "get<2>(tuple) == hello");

    get<0>(values) = 3;  // 写回验证 get 返回的是内部元素引用而非临时副本。
    expect(get<0>(values) == 3, "get<N> returns a mutable reference");

    const double applied =
        tuple::apply([](int a, double b, const std::string&) { return a + b; }, values);
    expect(applied == 5.0, "apply expands Tuple into function arguments");

    Tuple<int, double, std::string> same(3, 2.0, "hello");
    Tuple<int, double, std::string> different(3, 2.0, "world");
    expect(values == same, "operator== compares every element");
    expect(values != different, "operator!= detects different elements");

    std::ostringstream output;
    tuple::print(output, values);
    expect(output.str() == "(3, 2, hello)", "print(tuple) writes parenthesized values");
}

}  // namespace demo

int main() {
    demo::run_variadic_examples();
    std::cout << "----\n";

    demo::run_fold_examples();
    std::cout << "----\n";

    demo::run_tuple_examples();
    return 0;
}
