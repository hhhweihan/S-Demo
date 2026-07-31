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

// ---------------------------------------------------------------------------
// WeakPtr: 观察者不拥有对象，只是弱引用；用来打破循环引用。
// ---------------------------------------------------------------------------

TEST(WeakPtr, DoesNotExtendObjectLifetime) {
    Tracked::alive = 0;
    WeakPtr<Tracked> weak;
    {
        SharedPtr<Tracked> owner(new Tracked(11));
        weak = WeakPtr<Tracked>(owner);
        // A weak ref must never bump the strong count.
        EXPECT_EQ(owner.use_count(), 1);
        EXPECT_EQ(weak.use_count(), 1);
        EXPECT_FALSE(weak.expired());
        EXPECT_EQ(Tracked::alive, 1);
    }
    // Object dies with the last SharedPtr, even though a WeakPtr still points at it.
    EXPECT_EQ(Tracked::alive, 0);
    EXPECT_TRUE(weak.expired());
    EXPECT_EQ(weak.use_count(), 0);
}

TEST(WeakPtr, ExpiredAndLockAfterStrongCountHitsZero) {
    Tracked::alive = 0;
    WeakPtr<Tracked> weak;
    {
        SharedPtr<Tracked> owner(new Tracked(22));
        weak = WeakPtr<Tracked>(owner);

        // While the object is alive, lock() yields a valid, sharing SharedPtr.
        SharedPtr<Tracked> locked = weak.lock();
        ASSERT_TRUE(static_cast<bool>(locked));
        EXPECT_EQ(locked->value, 22);
        EXPECT_EQ(owner.use_count(), 2);  // owner + locked
    }
    // After every strong reference is gone the weak ref is expired.
    EXPECT_TRUE(weak.expired());
    SharedPtr<Tracked> revived = weak.lock();
    EXPECT_EQ(revived.get(), nullptr);
    EXPECT_EQ(revived.use_count(), 0);
    EXPECT_EQ(Tracked::alive, 0);
}

TEST(WeakPtr, ControlBlockOutlivesObject) {
    // Two-phase lifetime: the object is destroyed at strong==0, but the control
    // block must survive so a lingering WeakPtr can safely report expired().
    Tracked::alive = 0;
    WeakPtr<Tracked> weak;
    {
        SharedPtr<Tracked> owner(new Tracked(33));
        weak = WeakPtr<Tracked>(owner);
    }
    // Object gone; querying through the still-alive weak ref must not touch freed memory.
    EXPECT_EQ(Tracked::alive, 0);
    EXPECT_TRUE(weak.expired());
    EXPECT_EQ(weak.use_count(), 0);
    weak.reset();  // Dropping the last weak ref now frees the control block.
    EXPECT_TRUE(weak.expired());
}

// ---------------------------------------------------------------------------
// 循环引用：全用 SharedPtr 会泄漏，把一条边换成 WeakPtr 即可正常析构。
// ---------------------------------------------------------------------------

namespace {

// A node that counts destructions so we can prove a cycle actually gets freed.
struct Node {
    static int destroyed;
    SharedPtr<Node> strong_next;  // strong edge
    WeakPtr<Node> weak_next;      // weak edge (breaks the cycle)
    ~Node() { ++destroyed; }
};
int Node::destroyed = 0;

}  // namespace

TEST(WeakPtr, BreaksReferenceCycleSoObjectsAreDestroyed) {
    Node::destroyed = 0;
    {
        SharedPtr<Node> a(new Node());
        SharedPtr<Node> b(new Node());
        // a -> b via a strong edge; b -> a via a weak edge.
        a->strong_next = b;
        b->weak_next = WeakPtr<Node>(a);

        EXPECT_EQ(a.use_count(), 1);  // only the local `a` counts, weak edge does not
        EXPECT_EQ(b.use_count(), 2);  // local `b` + a->strong_next
    }
    // Both nodes must be reclaimed: no leak thanks to the weak edge.
    EXPECT_EQ(Node::destroyed, 2);
}

// ---------------------------------------------------------------------------
// EnableSharedFromThis: 对象自身交出的 SharedPtr 必须共享同一控制块。
// ---------------------------------------------------------------------------

namespace {

struct Widget : EnableSharedFromThis<Widget> {
    static int alive;
    int value;
    explicit Widget(int v = 0) : value(v) { ++alive; }
    ~Widget() { --alive; }
};
int Widget::alive = 0;

}  // namespace

TEST(EnableSharedFromThis, SharesSameControlBlock) {
    Widget::alive = 0;
    {
        SharedPtr<Widget> owner(new Widget(99));
        EXPECT_EQ(owner.use_count(), 1);

        SharedPtr<Widget> self = owner->shared_from_this();
        // shared_from_this must reuse the existing control block, not make a new one.
        ASSERT_TRUE(static_cast<bool>(self));
        EXPECT_EQ(self.get(), owner.get());
        EXPECT_EQ(owner.use_count(), 2);
        EXPECT_EQ(self.use_count(), 2);
        EXPECT_EQ(self->value, 99);
    }
    // Exactly one object, destroyed exactly once (no double-free from two blocks).
    EXPECT_EQ(Widget::alive, 0);
}

TEST(EnableSharedFromThis, WeakFromThisTracksLifetime) {
    Widget::alive = 0;
    WeakPtr<Widget> weak;
    {
        SharedPtr<Widget> owner(new Widget(7));
        weak = owner->weak_from_this();
        EXPECT_FALSE(weak.expired());
        EXPECT_EQ(weak.lock().get(), owner.get());
    }
    EXPECT_TRUE(weak.expired());
    EXPECT_EQ(Widget::alive, 0);
}
