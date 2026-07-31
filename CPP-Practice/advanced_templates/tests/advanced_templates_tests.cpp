// advanced_templates keeps its techniques inside main.cpp (no reusable header),
// so this suite re-implements the same small CRTP / concept / constexpr shapes
// and also exercises the Month 3 headers this module depends on.
#include <array>
#include <concepts>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include <algorithm>
#include "templates/my_optional.h"
#include "templates/my_tuple.h"
#include "templates/my_type_traits.h"

namespace {

// CRTP: static dispatch to the derived implementation, no vtable.
template <typename Derived>
struct ShapeCRTP {
    constexpr double area() const { return static_cast<const Derived*>(this)->area_impl(); }
};

struct SquareCRTP : ShapeCRTP<SquareCRTP> {
    double side;
    constexpr explicit SquareCRTP(double s) : side(s) {}
    constexpr double area_impl() const { return side * side; }
};

// C++20 concept mirroring the demo's Printable/Container style.
template <typename T>
concept Container = requires(T c) {
    c.begin();
    c.end();
    c.size();
    typename T::value_type;
};

template <std::integral T>
constexpr T double_it(T value) {
    return value * 2;
}

constexpr std::size_t fibonacci(std::size_t n) {
    return n < 2 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

template <std::size_t N>
constexpr std::array<int, N> sorted_copy(std::array<int, N> values) {
    for (std::size_t i = 0; i < N; ++i)
        for (std::size_t j = i + 1; j < N; ++j)
            if (values[j] < values[i]) std::swap(values[i], values[j]);
    return values;
}

// Compile-time guarantees.
static_assert(SquareCRTP(3.0).area() == 9.0);
static_assert(double_it(21) == 42);
static_assert(fibonacci(10) == 55);
static_assert(Container<std::vector<int>>);
static_assert(!Container<int>);
constexpr auto kSorted = sorted_copy(std::array<int, 5>{5, 1, 4, 1, 3});
static_assert(kSorted[0] == 1 && kSorted[4] == 5);

// Depends-on headers still resolve under cxx_std_20 with this module's include dirs.
static_assert(demo::traits::is_same<demo::traits::decay_t<int[3]>, int*>::value);

}  // namespace

TEST(AdvancedTemplates, CrtpStaticDispatch) {
    SquareCRTP sq(4.0);
    EXPECT_DOUBLE_EQ(sq.area(), 16.0);
}

TEST(AdvancedTemplates, ConstexprFunctionsRunAtRuntime) {
    EXPECT_EQ(double_it(5), 10);
    EXPECT_EQ(fibonacci(10), 55u);
    auto s = sorted_copy(std::array<int, 3>{3, 2, 1});
    EXPECT_EQ(s[0], 1);
    EXPECT_EQ(s[2], 3);
}

TEST(AdvancedTemplates, ConceptConstrainedContainer) {
    std::vector<int> v{1, 2, 3};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_TRUE(Container<std::vector<int>>);
}

TEST(AdvancedTemplates, DependentHeadersIntegrate) {
    demo::tuple::Tuple<int, std::string> t(1, "ok");
    EXPECT_EQ(demo::tuple::get<0>(t), 1);
    demo::erasure::Optional<int> o(7);
    EXPECT_EQ(o.value_or(0), 7);
}
