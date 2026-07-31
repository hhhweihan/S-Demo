#include "mini_stl.h"

#include <deque>
#include <list>
#include <numeric>
#include <vector>

#include <gtest/gtest.h>

using mini_stl::MyDeque;
using mini_stl::MyList;
using mini_stl::MyVector;

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
