#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace demo {

template <typename T, typename U>
struct is_same : std::false_type {};

template <typename T>
struct is_same<T, T> : std::true_type {};

void expect(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "[FAIL] " << message << '\n';
    std::exit(1);
  }
  std::cout << "[PASS] " << message << '\n';
}

template <typename T>
std::string pointer_case(T) {
  return "primary template<T>(T)";
}

template <>
std::string pointer_case<int*>(int*) {
  return "explicit specialization<int*>(int*)";
}

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

std::string non_template_case(int) {
  return "non-template overload(int)";
}

namespace ns {

struct Foo {
  int value;
};

void swap(Foo& lhs, Foo& rhs) noexcept {
  std::cout << "ns::swap triggered by ADL\n";
  std::swap(lhs.value, rhs.value);
}

}  // namespace ns

void run_overload_priority_examples() {
  std::cout << "== overload / specialization examples ==\n";

  int value = 7;
  expect(pointer_case(&value) == "overload<T>(T*)",
    "example 1: pointer overload wins before the primary template can dispatch to its int* specialization");

  expect(array_case("sfinae") == "overload<const char(&)[N]>(const char (&)[N])",
    "example 2: array-reference overload keeps the string literal type and beats the const char* specialization path");

  expect(non_template_case(42) == "non-template overload(int)",
    "example 3: non-template overload beats the primary template and its int specialization");
}

void run_adl_swap_example() {
  std::cout << "== ADL swap example ==\n";

  ns::Foo left{1};
  ns::Foo right{2};

  using std::swap;
  swap(left, right);

  expect(left.value == 2 && right.value == 1,
         "using std::swap; swap(a, b); finds ns::swap by ADL for user-defined type");
}

void run_is_same_example() {
  std::cout << "== is_same example ==\n";

  static_assert(is_same<int, int>::value, "is_same<int, int> should be true");
  static_assert(!is_same<int, double>::value, "is_same<int, double> should be false");

  expect(is_same<int, int>::value,
         "is_same<int, int>::value == true");
  expect(!is_same<int, double>::value,
         "is_same<int, double>::value == false");
}

}  // namespace demo

int main() {
  demo::run_overload_priority_examples();
  std::cout << "----\n";

  demo::run_adl_swap_example();
  std::cout << "----\n";

  demo::run_is_same_example();
  return 0;
}