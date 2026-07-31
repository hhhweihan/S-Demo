#include "templates/my_function.h"
#include "templates/my_optional.h"
#include "templates/my_variant.h"

#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>

#include <gtest/gtest.h>

namespace er = demo::erasure;

// Variant type<->index mapping is a compile-time computation.
static_assert(er::variant_npos == static_cast<std::size_t>(-1));

TEST(Optional, StartsEmpty) {
    er::Optional<int> o;
    EXPECT_FALSE(o.has_value());
    EXPECT_FALSE(static_cast<bool>(o));
    EXPECT_THROW(o.value(), std::bad_optional_access);
}

TEST(Optional, HoldsValueAndResets) {
    er::Optional<std::string> o("hi");
    EXPECT_TRUE(o.has_value());
    EXPECT_EQ(o.value(), "hi");
    o.value() += "!";
    EXPECT_EQ(o.value(), "hi!");
    o.reset();
    EXPECT_FALSE(o.has_value());
}

TEST(Optional, ValueOr) {
    er::Optional<int> empty;
    EXPECT_EQ(empty.value_or(5), 5);
    er::Optional<int> full(9);
    EXPECT_EQ(full.value_or(5), 9);
}

TEST(Optional, MoveLeavesSourceEmpty) {
    er::Optional<std::string> src("data");
    er::Optional<std::string> dst(std::move(src));
    EXPECT_TRUE(dst.has_value());
    EXPECT_EQ(dst.value(), "data");
    EXPECT_FALSE(src.has_value());  // move construction resets the source.
}

TEST(Variant, IndexAndGetByTypeAndIndex) {
    er::Variant<int, std::string> v(7);
    EXPECT_EQ(v.index(), 0u);
    EXPECT_EQ(v.get<int>(), 7);
    EXPECT_EQ(v.get<0>(), 7);
    EXPECT_THROW(v.get<std::string>(), std::bad_cast);
}

TEST(Variant, ReassignSwitchesAlternative) {
    er::Variant<int, std::string> v(7);
    v = std::string("seven");
    EXPECT_EQ(v.index(), 1u);
    EXPECT_EQ(v.get<std::string>(), "seven");
}

TEST(Variant, EmplaceAndReset) {
    er::Variant<int, std::string> v;
    EXPECT_TRUE(v.valueless_by_exception());
    v.emplace<std::string>("x");
    EXPECT_EQ(v.get<std::string>(), "x");
    v.reset();
    EXPECT_TRUE(v.valueless_by_exception());
}

TEST(Variant, VisitDispatchesOnActiveAlternative) {
    er::Variant<int, std::string> v(std::string("seven"));
    std::string seen = er::visit(
        [](const auto& item) {
            std::ostringstream os;
            os << item;
            return os.str();
        },
        v);
    EXPECT_EQ(seen, "seven");
}

TEST(Variant, VisitEmptyThrows) {
    er::Variant<int, std::string> v;
    EXPECT_THROW(er::visit([](const auto&) { return 0; }, v), std::bad_cast);
}

TEST(Function, EmptyIsFalseAndThrowsOnCall) {
    er::Function<int(int)> f;
    EXPECT_FALSE(static_cast<bool>(f));
    EXPECT_THROW(f(1), std::bad_function_call);
}

TEST(Function, CallsStoredCallable) {
    er::Function<int(int, int)> f = [](int a, int b) { return a - b; };
    EXPECT_TRUE(static_cast<bool>(f));
    EXPECT_EQ(f(10, 3), 7);
}

TEST(Function, CopyClonesTarget) {
    er::Function<int(int, int)> f = [](int a, int b) { return a + b; };
    er::Function<int(int, int)> copy = f;
    EXPECT_EQ(copy(2, 3), 5);
    EXPECT_EQ(f(4, 4), 8);
}

TEST(Function, VoidReturnSignature) {
    int side = 0;
    er::Function<void(int)> f = [&side](int v) { side = v; };
    f(42);
    EXPECT_EQ(side, 42);
}
