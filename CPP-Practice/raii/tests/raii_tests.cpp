#include "file_guard.h"
#include "scope_guard.h"

#include <cstdio>
#include <string>

#include <gtest/gtest.h>
#include <algorithm>
#include <stdexcept>
#include <utility>

TEST(ScopeGuard, RunsActionOnScopeExit) {
    bool ran = false;
    {
        auto guard = make_scope_guard([&ran] { ran = true; });
        EXPECT_TRUE(guard.active());
        EXPECT_FALSE(ran);  // action must not fire until the scope ends
    }
    EXPECT_TRUE(ran);
}

TEST(ScopeGuard, DismissPreventsAction) {
    bool ran = false;
    {
        auto guard = make_scope_guard([&ran] { ran = true; });
        guard.dismiss();
        EXPECT_FALSE(guard.active());
    }
    // A dismissed guard must skip its cleanup entirely.
    EXPECT_FALSE(ran);
}

TEST(ScopeGuard, MoveTransfersCleanupResponsibility) {
    int runs = 0;
    {
        auto outer = make_scope_guard([&runs] { ++runs; });
        {
            // Moving must hand the single cleanup to the new guard, not duplicate it.
            auto inner = std::move(outer);
            EXPECT_TRUE(inner.active());
            EXPECT_FALSE(outer.active());
        }
        EXPECT_EQ(runs, 1);  // inner ran once on its scope exit
    }
    // The moved-from outer guard must not run the action again.
    EXPECT_EQ(runs, 1);
}

TEST(ScopeGuard, DeferMacroRunsAtScopeExit) {
    int order = 0;
    int deferred_at = -1;
    {
        DEFER(deferred_at = order);
        order = 5;  // DEFER captures by reference and runs after this line, at scope end
    }
    EXPECT_EQ(deferred_at, 5);
}

namespace {

std::string temp_path(const char* name) {
    return std::string(P_tmpdir ? P_tmpdir : "/tmp") + "/s_demo_raii_" + name;
}

}  // namespace

TEST(FileGuard, OpensWritesAndClosesFile) {
    const std::string path = temp_path("write.txt");
    std::remove(path.c_str());
    {
        FileGuard guard(path.c_str(), "w");
        ASSERT_TRUE(static_cast<bool>(guard));
        ASSERT_NE(guard.get(), nullptr);
        EXPECT_EQ(guard.path(), path);
        std::fputs("hello", guard.get());
    }
    // After the guard's scope the file must be flushed/closed and readable.
    FILE* verify = std::fopen(path.c_str(), "r");
    ASSERT_NE(verify, nullptr);
    char buf[16] = {};
    std::fgets(buf, sizeof(buf), verify);
    std::fclose(verify);
    EXPECT_STREQ(buf, "hello");
    std::remove(path.c_str());
}

TEST(FileGuard, ThrowsWhenOpenFails) {
    // Opening a nonexistent path for reading must surface as an exception, not a null guard.
    EXPECT_THROW(FileGuard("/nonexistent_dir_s_demo/should_fail.txt", "r"), std::runtime_error);
}

TEST(FileGuard, MoveTransfersOwnership) {
    const std::string path = temp_path("move.txt");
    std::remove(path.c_str());
    FileGuard src(path.c_str(), "w");
    FILE* raw = src.get();
    FileGuard dst(std::move(src));
    // Ownership moves; the source must be emptied so it will not double-close.
    EXPECT_EQ(dst.get(), raw);
    EXPECT_EQ(src.get(), nullptr);
    EXPECT_FALSE(static_cast<bool>(src));
    std::remove(path.c_str());
}
