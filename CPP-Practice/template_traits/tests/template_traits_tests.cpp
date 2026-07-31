#include "templates/my_type_traits.h"

#include <string>

#include <gtest/gtest.h>

namespace tr = demo::traits;

// integral_constant / true_type / false_type
static_assert(tr::true_type::value);
static_assert(!tr::false_type::value);
static_assert(tr::integral_constant<int, 7>::value == 7);

// is_same
static_assert(tr::is_same<int, int>::value);
static_assert(!tr::is_same<int, const int>::value);
static_assert(!tr::is_same<int, long>::value);

// enable_if: type present only when condition holds.
static_assert(tr::is_same<tr::enable_if<true, double>::type, double>::value);

// remove_const / remove_cv / remove_reference
static_assert(tr::is_same<tr::remove_const_t<const int>, int>::value);
static_assert(tr::is_same<tr::remove_cv_t<const volatile int>, int>::value);
static_assert(tr::is_same<tr::remove_reference_t<int&>, int>::value);
static_assert(tr::is_same<tr::remove_reference_t<int&&>, int>::value);

// add_pointer / add_lvalue_reference
static_assert(tr::is_same<tr::add_pointer_t<int>, int*>::value);
static_assert(tr::is_same<tr::add_pointer_t<int&>, int*>::value);
static_assert(tr::is_same<tr::add_lvalue_reference_t<int>, int&>::value);
static_assert(tr::is_same<tr::add_lvalue_reference_t<void>,
                          void>::value);  // void& is ill-formed, must fall back to void.

// is_pointer / is_reference / is_array
static_assert(tr::is_pointer<int*>::value);
static_assert(tr::is_pointer<const int* const>::value);  // top-level cv stripped before check.
static_assert(!tr::is_pointer<int>::value);
static_assert(tr::is_reference<int&>::value);
static_assert(tr::is_reference<int&&>::value);
static_assert(!tr::is_reference<int>::value);
static_assert(tr::is_array<int[]>::value);
static_assert(tr::is_array<int[4]>::value);
static_assert(!tr::is_array<int*>::value);

// conditional / remove_extent
static_assert(tr::is_same<tr::conditional<true, int, long>::type, int>::value);
static_assert(tr::is_same<tr::conditional<false, int, long>::type, long>::value);
static_assert(tr::is_same<tr::remove_extent_t<int[5]>, int>::value);

// decay: arrays -> pointers, references and cv removed.
static_assert(tr::is_same<tr::decay_t<int[3]>, int*>::value);
static_assert(tr::is_same<tr::decay_t<const int&>, int>::value);

// is_constructible / is_convertible via SFINAE
static_assert(tr::is_constructible<std::string, const char*>::value);
static_assert(!tr::is_constructible<int, std::string>::value);
static_assert(tr::is_convertible<int, double>::value);
static_assert(!tr::is_convertible<std::string, int>::value);

// invoke_result
static_assert(tr::is_same<tr::invoke_result_t<int (*)(double), double>, int>::value);

// is_integral incl. cv qualified forms
static_assert(tr::is_integral<int>::value);
static_assert(tr::is_integral<const unsigned long long>::value);
static_assert(tr::is_integral<bool>::value);
static_assert(!tr::is_integral<double>::value);
static_assert(!tr::is_integral<std::string>::value);

// Runtime smoke so ctest has an executable case to run; the traits are compile-time,
// so we surface a few values through EXPECT to prove they link and evaluate.
TEST(TemplateTraits, ValuesResolveAtRuntime) {
    EXPECT_TRUE((tr::is_same<int, int>::value));
    EXPECT_FALSE((tr::is_same<int, long>::value));
    EXPECT_TRUE(tr::is_integral<const int>::value);
    EXPECT_FALSE(tr::is_integral<double>::value);
    EXPECT_TRUE((tr::is_convertible<int, double>::value));

    // integral_constant is implicitly convertible to its value_type.
    constexpr tr::integral_constant<int, 42> c{};
    EXPECT_EQ(static_cast<int>(c), 42);
}
