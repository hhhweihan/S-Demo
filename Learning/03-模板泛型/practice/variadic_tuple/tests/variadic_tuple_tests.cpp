#include "templates/my_tuple.h"

#include <sstream>
#include <string>
#include <type_traits>

#include <gtest/gtest.h>
#include <utility>

namespace tp = demo::tuple;

// tuple_size_v counts elements at compile time.
static_assert(tp::tuple_size_v<> == 0);
static_assert(tp::tuple_size_v<int, double, char> == 3);

// tuple_element_t maps an index to its element type.
static_assert(std::is_same_v<tp::tuple_element_t<0, int, double, char>, int>);
static_assert(std::is_same_v<tp::tuple_element_t<2, int, double, char>, char>);

// get and operator== are constexpr.
static_assert([] {
    tp::Tuple<int, int> t(1, 2);
    return tp::get<0>(t) == 1 && tp::get<1>(t) == 2;
}());
static_assert(tp::Tuple<int, int>(1, 2) == tp::Tuple<int, int>(1, 2));
static_assert(tp::Tuple<int, int>(1, 2) != tp::Tuple<int, int>(1, 3));

// 回归：单元素 Tuple 的转发构造曾无自类型排除,拷贝/移动非 const 左值会被它抢占而编译失败。
TEST(VariadicTuple, SingleElementIsCopyAndMoveConstructible) {
    tp::Tuple<int> a(1);
    tp::Tuple<int> b(a);  // 拷贝构造：不得被转发构造抢占。
    EXPECT_EQ(tp::get<0>(b), 1);

    tp::Tuple<int> c(std::move(a));  // 移动构造。
    EXPECT_EQ(tp::get<0>(c), 1);

    tp::Tuple<std::string> s(std::string("hi"));
    tp::Tuple<std::string> s_copy(s);  // 非 const 左值拷贝。
    EXPECT_EQ(tp::get<0>(s_copy), "hi");

    tp::Tuple<std::string> s_move(std::move(s));  // 移动构造。
    EXPECT_EQ(tp::get<0>(s_move), "hi");
}

TEST(VariadicTuple, GetReadsAndMutates) {
    tp::Tuple<int, double, std::string> t(1, 2.5, "ok");
    EXPECT_EQ(tp::get<0>(t), 1);
    EXPECT_DOUBLE_EQ(tp::get<1>(t), 2.5);
    EXPECT_EQ(tp::get<2>(t), "ok");

    tp::get<0>(t) = 9;
    EXPECT_EQ(tp::get<0>(t), 9);
}

TEST(VariadicTuple, RvalueGetMovesOut) {
    tp::Tuple<std::string> t(std::string("moved"));
    std::string out = tp::get<0>(std::move(t));
    EXPECT_EQ(out, "moved");
}

TEST(VariadicTuple, Comparison) {
    tp::Tuple<int, std::string> a(1, "x");
    tp::Tuple<int, std::string> b(1, "x");
    tp::Tuple<int, std::string> c(1, "y");
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(VariadicTuple, ApplyExpandsElements) {
    tp::Tuple<int, double> t(3, 0.5);
    double sum = tp::apply([](int a, double b) { return a + b; }, t);
    EXPECT_DOUBLE_EQ(sum, 3.5);
}

TEST(VariadicTuple, PrintFormatsAsParenList) {
    tp::Tuple<int, double, std::string> t(1, 2.5, "ok");
    std::ostringstream os;
    tp::print(os, t);
    EXPECT_EQ(os.str(), "(1, 2.5, ok)");
}
