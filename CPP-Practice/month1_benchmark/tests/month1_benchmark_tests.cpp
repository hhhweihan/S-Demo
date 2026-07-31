#include "smart_ptr.h"

#include <utility>

#include <gtest/gtest.h>

namespace {

// Counts live instances so tests can prove the smart pointers actually destroy their objects.
struct Tracked {
    static int alive;
    int value;
    explicit Tracked(int v = 0) : value(v) { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

}  // namespace

TEST(UniquePtr, OwnsAndReleasesObject) {
    Tracked::alive = 0;
    {
        UniquePtr<Tracked> p(new Tracked(42));
        ASSERT_NE(p.get(), nullptr);
        EXPECT_EQ(p->value, 42);
        EXPECT_EQ((*p).value, 42);
        EXPECT_EQ(Tracked::alive, 1);
    }
    // Leaving scope must delete the managed object exactly once.
    EXPECT_EQ(Tracked::alive, 0);
}

TEST(UniquePtr, DefaultConstructedIsNull) {
    UniquePtr<Tracked> p;
    EXPECT_EQ(p.get(), nullptr);
}

TEST(UniquePtr, MoveTransfersOwnership) {
    Tracked::alive = 0;
    UniquePtr<Tracked> src(new Tracked(7));
    Tracked* raw = src.get();
    UniquePtr<Tracked> dst(std::move(src));
    // Ownership moves to dst; the source must be emptied so it cannot double-delete.
    EXPECT_EQ(src.get(), nullptr);
    EXPECT_EQ(dst.get(), raw);
    EXPECT_EQ(Tracked::alive, 1);
}

TEST(UniquePtr, MoveAssignReleasesPreviousObject) {
    Tracked::alive = 0;
    UniquePtr<Tracked> a(new Tracked(1));
    UniquePtr<Tracked> b(new Tracked(2));
    EXPECT_EQ(Tracked::alive, 2);
    a = std::move(b);
    // a's original object must be freed before it takes over b's object.
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(a->value, 2);
    EXPECT_EQ(b.get(), nullptr);
}

TEST(SharedPtr, DefaultAndNullHaveZeroCount) {
    SharedPtr<Tracked> empty;
    EXPECT_EQ(empty.get(), nullptr);
    EXPECT_EQ(empty.use_count(), 0);

    // Constructing from a null pointer must not create a control block.
    SharedPtr<Tracked> from_null(static_cast<Tracked*>(nullptr));
    EXPECT_EQ(from_null.get(), nullptr);
    EXPECT_EQ(from_null.use_count(), 0);
}

TEST(SharedPtr, CopyIncrementsAndScopeDecrementsCount) {
    Tracked::alive = 0;
    SharedPtr<Tracked> a(new Tracked(5));
    EXPECT_EQ(a.use_count(), 1);
    {
        SharedPtr<Tracked> b = a;
        EXPECT_EQ(a.use_count(), 2);
        EXPECT_EQ(b.use_count(), 2);
        EXPECT_EQ(b.get(), a.get());
    }
    // b's destruction drops the count but the object survives while a holds it.
    EXPECT_EQ(a.use_count(), 1);
    EXPECT_EQ(Tracked::alive, 1);
}

TEST(SharedPtr, DestroysObjectOnLastReference) {
    Tracked::alive = 0;
    {
        SharedPtr<Tracked> a(new Tracked(9));
        SharedPtr<Tracked> b = a;
        EXPECT_EQ(Tracked::alive, 1);
    }
    // Object must be destroyed only after both references are gone.
    EXPECT_EQ(Tracked::alive, 0);
}

TEST(SharedPtr, CopyAssignReleasesOldReference) {
    Tracked::alive = 0;
    SharedPtr<Tracked> a(new Tracked(1));
    {
        SharedPtr<Tracked> b(new Tracked(2));
        EXPECT_EQ(Tracked::alive, 2);
        b = a;  // b's original object loses its only reference and is destroyed
        EXPECT_EQ(Tracked::alive, 1);
        EXPECT_EQ(a.use_count(), 2);
        EXPECT_EQ(b->value, 1);
    }
    EXPECT_EQ(a.use_count(), 1);
}

TEST(SharedPtr, MoveTransfersWithoutChangingCount) {
    Tracked::alive = 0;
    SharedPtr<Tracked> a(new Tracked(3));
    EXPECT_EQ(a.use_count(), 1);
    SharedPtr<Tracked> b(std::move(a));
    // Moving steals the reference: count stays 1 and the source is emptied.
    EXPECT_EQ(b.use_count(), 1);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(a.use_count(), 0);
    EXPECT_EQ(b->value, 3);
}

TEST(SharedPtr, MoveAssignReleasesTargetThenTakesSource) {
    Tracked::alive = 0;
    SharedPtr<Tracked> a(new Tracked(1));
    SharedPtr<Tracked> b(new Tracked(2));
    EXPECT_EQ(Tracked::alive, 2);
    b = std::move(a);
    // b's old object is freed; a's reference is transferred, not copied.
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(b->value, 1);
    EXPECT_EQ(b.use_count(), 1);
    EXPECT_EQ(a.get(), nullptr);
}
