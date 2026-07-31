#include "mini_stl.h"

#include <deque>
#include <list>
#include <numeric>
#include <vector>

#include <gtest/gtest.h>
#include <cstddef>
#include <stdexcept>

using mini_stl::MyDeque;
using mini_stl::MyForwardList;
using mini_stl::MyList;
using mini_stl::MyVector;

// ---------------- 生命周期探针类型 ----------------
// 资源持有型元素：持有一块堆内存并维护全局存活计数。它刻意【不提供默认构造函数】——
// 旧实现用 std::make_unique<T[]> / std::unique_ptr<T[]> 存储时会要求元素可默认构造且逐槽
// 值构造，因此对该类型根本无法编译；同时存活计数能确定性地暴露“弹出/析构不销毁元素”
// （泄漏）与“重复释放同一元素”（双删）这类生命周期缺陷，无需依赖 ASan。
struct Tracked {
    static int live;         // 当前存活实例数：每次构造 +1，每次析构 -1，必须成对归零。
    int* payload = nullptr;  // 持有的堆资源，用于让双删/漏删可被侦测。

    explicit Tracked(int v) : payload(new int(v)) { ++live; }
    Tracked(const Tracked& other) : payload(new int(*other.payload)) { ++live; }
    Tracked(Tracked&& other) noexcept : payload(other.payload) {
        other.payload = nullptr;  // 接管资源，来源仍是一个存活对象，其析构稍后再 -1。
        ++live;
    }
    Tracked& operator=(const Tracked& other) {
        if (this != &other) {
            int* fresh = new int(*other.payload);
            delete payload;
            payload = fresh;
        }
        return *this;
    }
    Tracked& operator=(Tracked&& other) noexcept {
        if (this != &other) {
            delete payload;
            payload = other.payload;
            other.payload = nullptr;
        }
        return *this;
    }
    ~Tracked() {
        delete payload;
        --live;
    }
    int value() const { return *payload; }
};
int Tracked::live = 0;

// 只能移动的资源持有类型：验证 MyVector 的裸存储 + placement-new 能就地移动构造，
// 而旧的“预先值构造 + 赋值”实现对不可拷贝、不可默认构造的类型无法工作。
struct MoveOnly {
    static int live;
    int* payload = nullptr;

    explicit MoveOnly(int v) : payload(new int(v)) { ++live; }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&& other) noexcept : payload(other.payload) {
        other.payload = nullptr;
        ++live;
    }
    MoveOnly& operator=(MoveOnly&& other) noexcept {
        if (this != &other) {
            delete payload;
            payload = other.payload;
            other.payload = nullptr;
        }
        return *this;
    }
    ~MoveOnly() {
        delete payload;
        --live;
    }
    int value() const { return *payload; }
};
int MoveOnly::live = 0;

// ---------------- MyVector ----------------

TEST(MyVector, DefaultConstructedIsEmpty) {
    MyVector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
    EXPECT_EQ(v.begin(), v.end());
}

TEST(MyVector, CapacityGrowsFirstToFourThenDoubles) {
    // Growth policy is load-bearing: 0 -> 4 on first insert, then geometric doubling.
    MyVector<int> v;
    v.push_back(0);
    EXPECT_EQ(v.capacity(), 4u);
    for (int i = 1; i < 4; ++i) v.push_back(i);
    EXPECT_EQ(v.capacity(), 4u);
    v.push_back(4);  // fifth element forces the first doubling.
    EXPECT_EQ(v.capacity(), 8u);
    EXPECT_EQ(v.size(), 5u);
}

TEST(MyVector, ReserveDoesNotShrinkAndPreservesElements) {
    MyVector<int> v{1, 2, 3};
    v.reserve(64);
    EXPECT_GE(v.capacity(), 64u);
    v.reserve(2);  // smaller request is a no-op.
    EXPECT_GE(v.capacity(), 64u);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 3);
}

TEST(MyVector, IndexingFrontBack) {
    MyVector<int> v{10, 20, 30};
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
    v[1] = 99;
    EXPECT_EQ(v[1], 99);
}

TEST(MyVector, PopBackShrinksAndThrowsWhenEmpty) {
    MyVector<int> v{1, 2};
    v.pop_back();
    EXPECT_EQ(v.size(), 1u);
    v.pop_back();
    EXPECT_TRUE(v.empty());
    EXPECT_THROW(v.pop_back(), std::out_of_range);
}

TEST(MyVector, ForwardIteratorTraversalMatchesStdVector) {
    MyVector<int> mine;
    std::vector<int> ref;
    for (int i = 0; i < 100; ++i) {
        mine.push_back(i * 3);
        ref.push_back(i * 3);
    }
    ASSERT_EQ(mine.size(), ref.size());
    EXPECT_TRUE(std::equal(mine.begin(), mine.end(), ref.begin()));
}

TEST(MyVector, ReverseIteratorTraversal) {
    MyVector<int> v{1, 2, 3, 4};
    std::vector<int> collected;
    for (auto it = v.rbegin(); it != v.rend(); ++it) collected.push_back(*it);
    EXPECT_EQ(collected, (std::vector<int>{4, 3, 2, 1}));
}

TEST(MyVector, CopyConstructIsDeep) {
    MyVector<int> a{1, 2, 3};
    MyVector<int> b(a);
    b[0] = 42;  // mutating the copy must not touch the source.
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(b[0], 42);
    EXPECT_EQ(a.size(), b.size());
}

TEST(MyVector, CopyAndSwapAssignment) {
    MyVector<int> a{1, 2, 3};
    MyVector<int> b{9};
    b = a;
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[2], 3);
}

TEST(MyVector, SwapExchangesContents) {
    MyVector<int> a{1, 2};
    MyVector<int> b{7, 8, 9};
    a.swap(b);
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(a[0], 7);
    EXPECT_EQ(b[0], 1);
}

TEST(MyVector, MatchesStdVectorOverMixedOperationSequence) {
    MyVector<int> mine;
    std::vector<int> ref;
    for (int i = 0; i < 500; ++i) {
        mine.push_back(i);
        ref.push_back(i);
        if (i % 7 == 0 && !ref.empty()) {
            mine.pop_back();
            ref.pop_back();
        }
    }
    ASSERT_EQ(mine.size(), ref.size());
    for (std::size_t i = 0; i < ref.size(); ++i) EXPECT_EQ(mine[i], ref[i]);
}

// ---------------- MyList ----------------

TEST(MyList, EmptyAndSingleElement) {
    MyList<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0u);
    EXPECT_EQ(l.begin(), l.end());
    l.push_back(5);
    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 1u);
    EXPECT_EQ(l.front(), 5);
    EXPECT_EQ(l.back(), 5);
    EXPECT_EQ(*l.begin(), 5);
}

TEST(MyList, PushFrontAndBackOrdering) {
    MyList<int> l;
    l.push_back(2);
    l.push_back(3);
    l.push_front(1);  // sequence should now be 1,2,3.
    std::list<int> ref{1, 2, 3};
    EXPECT_TRUE(std::equal(l.begin(), l.end(), ref.begin()));
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
}

TEST(MyList, PopFrontAndPopBackUntilEmpty) {
    MyList<int> l{1, 2, 3, 4};
    l.pop_front();
    EXPECT_EQ(l.front(), 2);
    l.pop_back();
    EXPECT_EQ(l.back(), 3);
    EXPECT_EQ(l.size(), 2u);
    l.pop_front();
    l.pop_back();
    EXPECT_TRUE(l.empty());
}

TEST(MyList, BidirectionalIteratorWalksBothWays) {
    MyList<int> l{10, 20, 30};
    auto it = l.begin();
    ++it;
    EXPECT_EQ(*it, 20);
    --it;
    EXPECT_EQ(*it, 10);
    auto last = l.end();
    --last;  // end() is the sentinel; stepping back reaches the tail.
    EXPECT_EQ(*last, 30);
}

TEST(MyList, ClearEmptiesContainer) {
    MyList<int> l{1, 2, 3};
    l.clear();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.begin(), l.end());
}

TEST(MyList, MatchesStdListOverMixedOperationSequence) {
    MyList<int> mine;
    std::list<int> ref;
    for (int i = 0; i < 200; ++i) {
        if (i % 2 == 0) {
            mine.push_back(i);
            ref.push_back(i);
        } else {
            mine.push_front(i);
            ref.push_front(i);
        }
        if (i % 5 == 0 && !ref.empty()) {
            mine.pop_front();
            ref.pop_front();
        }
    }
    ASSERT_EQ(mine.size(), ref.size());
    EXPECT_TRUE(std::equal(mine.begin(), mine.end(), ref.begin()));
}

// ---------------- MyDeque ----------------

TEST(MyDeque, EmptyAndSingleElement) {
    MyDeque<int> d;
    EXPECT_TRUE(d.empty());
    EXPECT_EQ(d.size(), 0u);
    d.push_back(7);
    EXPECT_EQ(d.size(), 1u);
    EXPECT_EQ(d.front(), 7);
    EXPECT_EQ(d.back(), 7);
    EXPECT_EQ(d[0], 7);
}

TEST(MyDeque, PushFrontAndBackIndexing) {
    MyDeque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_front(0);  // logical order 0,1,2 regardless of ring layout.
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[1], 1);
    EXPECT_EQ(d[2], 2);
    EXPECT_EQ(d.front(), 0);
    EXPECT_EQ(d.back(), 2);
}

TEST(MyDeque, PopThrowsWhenEmpty) {
    MyDeque<int> d;
    EXPECT_THROW(d.pop_back(), std::out_of_range);
    EXPECT_THROW(d.pop_front(), std::out_of_range);
}

TEST(MyDeque, RandomAccessIteratorArithmetic) {
    MyDeque<int> d{0, 1, 2, 3, 4};
    auto it = d.begin();
    EXPECT_EQ(*(it + 3), 3);
    EXPECT_EQ(d.end() - d.begin(), 5);
    EXPECT_LT(d.begin(), d.end());
}

TEST(MyDeque, GrowthPreservesLogicalOrderThroughReserve) {
    // Front insertions past the initial capacity (8) force a ring reserve that must
    // relinearize elements; logical order must survive the move.
    MyDeque<int> d;
    for (int i = 0; i < 20; ++i) d.push_front(i);
    for (int i = 0; i < 20; ++i) EXPECT_EQ(d[static_cast<std::size_t>(i)], 19 - i);
}

TEST(MyDeque, MatchesStdDequeOverMixedOperationSequence) {
    MyDeque<int> mine;
    std::deque<int> ref;
    for (int i = 0; i < 300; ++i) {
        if (i % 2 == 0) {
            mine.push_back(i);
            ref.push_back(i);
        } else {
            mine.push_front(i);
            ref.push_front(i);
        }
        if (i % 6 == 0) {
            mine.pop_back();
            ref.pop_back();
        }
    }
    ASSERT_EQ(mine.size(), ref.size());
    for (std::size_t i = 0; i < ref.size(); ++i) EXPECT_EQ(mine[i], ref[i]);
    EXPECT_TRUE(std::equal(mine.begin(), mine.end(), ref.begin()));
}

// ---------------- 生命周期回归测试（针对已修复的缺陷） ----------------

// Bug 2/3：旧 MyVector 用 unique_ptr<T[]> 存储，push_back 是赋值而非就地构造，pop_back 只
// 缩小 size_ 从不析构。该用例要求元素在 pop_back/clear/析构时被真正销毁——旧实现存活计数
// 不会归零；同时 Tracked 无默认构造，旧的 make_unique<T[]> 存储对它根本无法编译。
TEST(MyVectorLifetime, PopBackAndClearDestroyElements) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyVector<Tracked> v;
        v.push_back(Tracked(1));
        v.push_back(Tracked(2));
        v.push_back(Tracked(3));
        EXPECT_EQ(Tracked::live, 3);  // 三个元素就地构造，均存活。

        v.pop_back();
        EXPECT_EQ(Tracked::live, 2);  // 旧实现这里仍是 3（弹出未析构 -> 泄漏）。
        EXPECT_EQ(v.size(), 2u);
        EXPECT_EQ(v.back().value(), 2);

        v.clear();
        EXPECT_EQ(Tracked::live, 0);  // clear 必须销毁全部剩余元素。
        EXPECT_TRUE(v.empty());
    }
    EXPECT_EQ(Tracked::live, 0);  // 作用域结束后不残留、不双删。
}

// Bug 2：reserve 必须逐元素搬移（move_if_noexcept）到新裸缓冲，且不泄漏、不重复构造。
TEST(MyVectorLifetime, ReserveRelocatesWithoutLeakOrDoubleConstruct) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyVector<Tracked> v;
        for (int i = 0; i < 10; ++i) v.push_back(Tracked(i));  // 跨越 4->8->16 多次扩容。
        EXPECT_EQ(Tracked::live, 10);                          // 恰好 size 个存活，扩容无残留。
        EXPECT_GE(v.capacity(), 10u);
        for (int i = 0; i < 10; ++i) EXPECT_EQ(v[static_cast<std::size_t>(i)].value(), i);
    }
    EXPECT_EQ(Tracked::live, 0);
}

// Bug 2：拷贝构造须深拷贝裸存储，两份互不影响且各自独立销毁。
TEST(MyVectorLifetime, CopyIsDeepAndIndependent) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyVector<Tracked> a;
        a.push_back(Tracked(1));
        a.push_back(Tracked(2));
        MyVector<Tracked> b(a);
        EXPECT_EQ(Tracked::live, 4);  // 深拷贝 -> 2 + 2；浅拷贝只会是 2。
        b[0] = Tracked(99);
        EXPECT_EQ(a[0].value(), 1);
        EXPECT_EQ(b[0].value(), 99);
    }
    EXPECT_EQ(Tracked::live, 0);
}

// Bug 2：裸存储 + placement-new 必须支持只能移动、不可默认构造的元素类型。
TEST(MyVectorLifetime, SupportsMoveOnlyElements) {
    ASSERT_EQ(MoveOnly::live, 0);
    {
        MyVector<MoveOnly> v;
        for (int i = 0; i < 6; ++i) v.push_back(MoveOnly(i));  // push_back(T&&) 就地移动构造。
        EXPECT_EQ(v.size(), 6u);
        EXPECT_EQ(MoveOnly::live, 6);
        for (int i = 0; i < 6; ++i) EXPECT_EQ(v[static_cast<std::size_t>(i)].value(), i);

        v.pop_back();
        EXPECT_EQ(MoveOnly::live, 5);  // 弹出的移动型元素被销毁。

        MyVector<MoveOnly> moved(std::move(v));  // 移动构造须转移所有权，不得双删。
        EXPECT_EQ(moved.size(), 5u);
        EXPECT_EQ(MoveOnly::live, 5);
    }
    EXPECT_EQ(MoveOnly::live, 0);
}

// ---------------- MyForwardList ----------------

// Bug 1：旧 MyForwardList 有析构却无拷贝构造（违反 rule-of-3）——`b = a` 会浅拷贝 head_，
// 两个析构函数删除同一条节点链造成双删。深拷贝后两条链各自持有独立节点：存活计数应为
// 2N（浅拷贝只会是 N），且两次析构不重复释放。
TEST(MyForwardListLifetime, CopyConstructIsDeepNoDoubleFree) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyForwardList<Tracked> a;
        a.push_front(Tracked(1));
        a.push_front(Tracked(2));
        a.push_front(Tracked(3));
        ASSERT_EQ(Tracked::live, 3);

        MyForwardList<Tracked> b = a;   // 旧代码：浅拷贝 -> live 仍为 3，且离开作用域时双删。
        EXPECT_EQ(Tracked::live, 6);    // 深拷贝：3 + 3。
        EXPECT_EQ(b.size(), a.size());

        // 顺序保持一致（头插得到 3,2,1）。
        auto ia = a.begin();
        auto ib = b.begin();
        for (; ia != a.end() && ib != b.end(); ++ia, ++ib) EXPECT_EQ((*ia).value(), (*ib).value());
    }
    EXPECT_EQ(Tracked::live, 0);  // 两条链独立释放，计数干净归零。
}

// Bug 1：copy-and-swap 赋值同样必须深拷贝，且先释放被覆盖链避免泄漏。
TEST(MyForwardListLifetime, CopyAssignIsDeepAndReleasesOldChain) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyForwardList<Tracked> a;
        a.push_front(Tracked(1));
        a.push_front(Tracked(2));

        MyForwardList<Tracked> b;
        b.push_front(Tracked(7));
        EXPECT_EQ(Tracked::live, 3);

        b = a;  // b 原有的 1 个节点被释放，再深拷贝 a 的 2 个。
        EXPECT_EQ(Tracked::live, 4);  // a:2 + b:2；若泄漏旧链会是 5，若浅拷贝会是 3。
        EXPECT_EQ(b.size(), 2u);
    }
    EXPECT_EQ(Tracked::live, 0);
}

// Bug 1：移动构造转移节点链，来源置空，无双删。
TEST(MyForwardListLifetime, MoveConstructTransfersOwnership) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyForwardList<Tracked> a;
        a.push_front(Tracked(1));
        a.push_front(Tracked(2));
        ASSERT_EQ(Tracked::live, 2);

        MyForwardList<Tracked> b = std::move(a);
        EXPECT_EQ(Tracked::live, 2);  // 未新建节点：只是转移。
        EXPECT_EQ(b.size(), 2u);
        EXPECT_TRUE(a.empty());
    }
    EXPECT_EQ(Tracked::live, 0);
}

TEST(MyForwardListBehavior, PushFrontOrderAndSizeWithTrackedType) {
    MyForwardList<Tracked> l;
    l.push_front(Tracked(3));
    l.push_front(Tracked(2));
    l.push_front(Tracked(1));
    std::vector<int> seen;
    for (auto it = l.begin(); it != l.end(); ++it) seen.push_back((*it).value());
    EXPECT_EQ(seen, (std::vector<int>{1, 2, 3}));
    EXPECT_EQ(l.size(), 3u);
}

// ---------------- MyDeque 生命周期 ----------------

// Bug 3：旧 MyDeque 的 pop_front/pop_back 只移动 head_ / 缩小 size_，从不析构被弹元素。
// 裸存储改造后弹出必须真正销毁；Tracked 无默认构造也证明旧的 unique_ptr<T[]> 存储不适用。
TEST(MyDequeLifetime, PopFrontAndPopBackDestroyElements) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyDeque<Tracked> d;
        for (int i = 0; i < 5; ++i) d.push_back(Tracked(i));  // 0,1,2,3,4
        d.push_front(Tracked(-1));                            // -1,0,1,2,3,4
        EXPECT_EQ(Tracked::live, 6);

        d.pop_front();
        EXPECT_EQ(Tracked::live, 5);  // 旧实现仍为 6（弹出未析构）。
        EXPECT_EQ(d.front().value(), 0);

        d.pop_back();
        EXPECT_EQ(Tracked::live, 4);
        EXPECT_EQ(d.back().value(), 3);
    }
    EXPECT_EQ(Tracked::live, 0);  // 析构销毁全部剩余环形元素，无泄漏无双删。
}

// Bug 3：环形扩容（reserve 摊平布局）必须逐元素搬移且不泄漏，跨越初始容量 8。
TEST(MyDequeLifetime, GrowthRelocatesRingWithoutLeak) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyDeque<Tracked> d;
        for (int i = 0; i < 20; ++i) d.push_front(Tracked(i));  // 触发多次环形 reserve。
        EXPECT_EQ(Tracked::live, 20);
        for (int i = 0; i < 20; ++i) EXPECT_EQ(d[static_cast<std::size_t>(i)].value(), 19 - i);
    }
    EXPECT_EQ(Tracked::live, 0);
}

// MyDeque 拷贝构造须深拷贝并保持逻辑顺序，两份独立销毁。
TEST(MyDequeLifetime, CopyIsDeepAndIndependent) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyDeque<Tracked> a;
        a.push_back(Tracked(1));
        a.push_back(Tracked(2));
        a.push_front(Tracked(0));  // 0,1,2
        MyDeque<Tracked> b(a);
        EXPECT_EQ(Tracked::live, 6);  // 深拷贝：3 + 3。
        for (std::size_t i = 0; i < 3; ++i) EXPECT_EQ(a[i].value(), b[i].value());
        b[0] = Tracked(99);
        EXPECT_EQ(a[0].value(), 0);
    }
    EXPECT_EQ(Tracked::live, 0);
}

// ---------------- MyVector insert / erase / emplace / resize ----------------

// 由多个参数构造的小类型：用于验证 emplace / emplace_back 是就地转发多参构造，
// 而非“先建临时再拷贝”。counted 记录默认构造次数以侦测 resize 是否多造了元素。
struct Point {
    int x = 0;
    int y = 0;
    Point() = default;
    Point(int x_, int y_) : x(x_), y(y_) {}
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

TEST(MyVectorModify, InsertAtBeginMiddleEnd) {
    MyVector<int> v{1, 2, 3};
    auto it = v.insert(v.begin(), 0);  // 头插：0,1,2,3
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(v[0], 0);
    v.insert(v.begin() + 2, 99);  // 中插：0,1,99,2,3
    EXPECT_EQ(v[2], 99);
    v.insert(v.end(), 4);  // 尾插：0,1,99,2,3,4
    std::vector<int> ref{0, 1, 99, 2, 3, 4};
    EXPECT_EQ(v.size(), ref.size());
    for (std::size_t i = 0; i < ref.size(); ++i) EXPECT_EQ(v[i], ref[i]);
}

TEST(MyVectorModify, InsertRvalueMoveOnly) {
    MyVector<MoveOnly> v;
    v.push_back(MoveOnly(1));
    v.push_back(MoveOnly(3));
    v.insert(v.begin() + 1, MoveOnly(2));  // 只能移动类型也能中插：1,2,3
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0].value(), 1);
    EXPECT_EQ(v[1].value(), 2);
    EXPECT_EQ(v[2].value(), 3);
}

TEST(MyVectorModify, EraseSingleAndRange) {
    MyVector<int> v{0, 1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 2);  // 删单个 -> 0,1,3,4,5
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(v.size(), 5u);
    v.erase(v.begin() + 1, v.begin() + 3);  // 删区间 [1,3) -> 0,4,5
    std::vector<int> ref{0, 4, 5};
    EXPECT_EQ(v.size(), ref.size());
    for (std::size_t i = 0; i < ref.size(); ++i) EXPECT_EQ(v[i], ref[i]);
}

TEST(MyVectorModify, EraseDestroysRemovedElements) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyVector<Tracked> v;
        for (int i = 0; i < 5; ++i) v.push_back(Tracked(i));  // 0..4
        EXPECT_EQ(Tracked::live, 5);
        v.erase(v.begin() + 1, v.begin() + 4);  // 删 1,2,3 -> 只剩 0,4
        EXPECT_EQ(v.size(), 2u);
        EXPECT_EQ(Tracked::live, 2);  // 被删元素必须真正析构。
        EXPECT_EQ(v[0].value(), 0);
        EXPECT_EQ(v[1].value(), 4);
    }
    EXPECT_EQ(Tracked::live, 0);
}

TEST(MyVectorModify, EmplaceBackAndEmplaceConstructInPlace) {
    MyVector<Point> v;
    Point& ref = v.emplace_back(1, 2);  // 多参就地构造并返回引用。
    EXPECT_EQ(ref, (Point{1, 2}));
    v.emplace_back(3, 4);
    auto it = v.emplace(v.begin() + 1, 9, 9);  // 中间就地构造 -> {1,2},{9,9},{3,4}
    EXPECT_EQ(*it, (Point{9, 9}));
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], (Point{1, 2}));
    EXPECT_EQ(v[1], (Point{9, 9}));
    EXPECT_EQ(v[2], (Point{3, 4}));
}

TEST(MyVectorModify, ResizeGrowAndShrink) {
    MyVector<int> v{1, 2, 3};
    v.resize(5, 7);  // 增大用 value 填充 -> 1,2,3,7,7
    ASSERT_EQ(v.size(), 5u);
    EXPECT_EQ(v[3], 7);
    EXPECT_EQ(v[4], 7);
    v.resize(2);  // 缩小截断 -> 1,2
    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    v.resize(4);  // 增大用默认构造(0) 填充 -> 1,2,0,0
    ASSERT_EQ(v.size(), 4u);
    EXPECT_EQ(v[2], 0);
    EXPECT_EQ(v[3], 0);
}

TEST(MyVectorModify, ResizeShrinkDestroysElements) {
    ASSERT_EQ(Tracked::live, 0);
    {
        MyVector<Tracked> v;
        for (int i = 0; i < 6; ++i) v.push_back(Tracked(i));
        EXPECT_EQ(Tracked::live, 6);
        v.resize(2, Tracked(0));  // 缩小必须析构被截掉的 4 个元素（value 在缩小分支不使用）。
        EXPECT_EQ(v.size(), 2u);
        EXPECT_EQ(Tracked::live, 2);
        v.resize(4, Tracked(42));  // 增大用给定值拷贝构造。
        EXPECT_EQ(v.size(), 4u);
        EXPECT_EQ(Tracked::live, 4);
        EXPECT_EQ(v[3].value(), 42);
    }
    EXPECT_EQ(Tracked::live, 0);
}

// ---------------- 分配器感知（allocator_traits 路由）----------------

// 跨所有 rebind 实例共享的计数器：MyVector<int,CA> 用 CA<int>，MyList<int,CA> 改绑成 CA<Node>，
// 二者都累加到这里，从而能断言分配确实经过了自定义分配器而非全局 ::operator new。
struct AllocStats {
    static inline int allocate_calls = 0;
    static inline int deallocate_calls = 0;
    static inline std::size_t objects_allocated = 0;
    static inline std::size_t objects_deallocated = 0;
    static void reset() {
        allocate_calls = 0;
        deallocate_calls = 0;
        objects_allocated = 0;
        objects_deallocated = 0;
    }
};

// 最小 std::allocator 兼容的计数分配器：无状态，故容器默认构造即可使用；带模板转换构造函数，
// 使 allocator_traits 的默认 rebind_alloc<U> 能正常改绑到节点类型。
template <class T>
struct CountingAllocator {
    using value_type = T;
    CountingAllocator() = default;
    template <class U>
    CountingAllocator(const CountingAllocator<U>&) noexcept {}
    T* allocate(std::size_t n) {
        ++AllocStats::allocate_calls;
        AllocStats::objects_allocated += n;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t n) noexcept {
        ++AllocStats::deallocate_calls;
        AllocStats::objects_deallocated += n;
        ::operator delete(p);
    }
    template <class U>
    bool operator==(const CountingAllocator<U>&) const noexcept {
        return true;
    }
    template <class U>
    bool operator!=(const CountingAllocator<U>&) const noexcept {
        return false;
    }
};

TEST(AllocatorAware, MyVectorRoutesThroughCustomAllocator) {
    AllocStats::reset();
    {
        MyVector<int, CountingAllocator<int>> v;
        for (int i = 0; i < 10; ++i) v.push_back(i);  // 触发多次扩容分配。
        EXPECT_GT(AllocStats::allocate_calls, 0);
        EXPECT_EQ(v.size(), 10u);
        for (int i = 0; i < 10; ++i) EXPECT_EQ(v[static_cast<std::size_t>(i)], i);
    }
    // 离开作用域后申请与释放必须收支平衡，证明 deallocate 也走了同一分配器。
    EXPECT_EQ(AllocStats::allocate_calls, AllocStats::deallocate_calls);
    EXPECT_EQ(AllocStats::objects_allocated, AllocStats::objects_deallocated);
    EXPECT_GT(AllocStats::objects_allocated, 0u);
}

TEST(AllocatorAware, MyListRoutesThroughRebindAllocator) {
    AllocStats::reset();
    {
        MyList<int, CountingAllocator<int>> l;
        // 每个节点一次 allocate(1)：N 个元素 + 1 个哨兵。
        for (int i = 0; i < 8; ++i) l.push_back(i);
        EXPECT_EQ(l.size(), 8u);
        EXPECT_EQ(AllocStats::objects_allocated, 9u);  // 8 节点 + 1 哨兵。
        int expected = 0;
        for (int value : l) EXPECT_EQ(value, expected++);
        l.pop_front();
        l.pop_back();
        EXPECT_EQ(l.size(), 6u);
    }
    // 全部节点（含哨兵）都经改绑分配器释放，收支平衡。
    EXPECT_EQ(AllocStats::allocate_calls, AllocStats::deallocate_calls);
    EXPECT_EQ(AllocStats::objects_allocated, AllocStats::objects_deallocated);
}
