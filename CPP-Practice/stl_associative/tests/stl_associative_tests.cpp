#include "mini_associative.h"

#include <map>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>
#include <cstddef>
#include <utility>

using mini_stl::MyMap;
using mini_stl::MySet;
using mini_stl::MyUnorderedMap;
using mini_stl::RBTree;

// ---------------- RBTree ----------------

TEST(RBTree, EmptyTreeInvariants) {
    RBTree<int, int> t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0u);
    EXPECT_EQ(t.begin(), t.end());
    EXPECT_EQ(t.find_node(1), nullptr);
    EXPECT_TRUE(t.validate());
    EXPECT_FALSE(t.erase(1));  // erasing a missing key reports no removal.
}

TEST(RBTree, SingleInsertFindErase) {
    RBTree<int, int> t;
    auto [it, inserted] = t.insert(42, 100);
    EXPECT_TRUE(inserted);
    EXPECT_EQ(it->key, 42);
    EXPECT_EQ(it->value, 100);
    EXPECT_EQ(t.size(), 1u);
    ASSERT_NE(t.find_node(42), nullptr);
    EXPECT_EQ(t.find_node(42)->value, 100);
    EXPECT_TRUE(t.erase(42));
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.find_node(42), nullptr);
}

TEST(RBTree, DuplicateInsertUpdatesValueNotSize) {
    RBTree<int, int> t;
    t.insert(1, 10);
    auto [it, inserted] = t.insert(1, 999);  // same key: update value, no new node.
    EXPECT_FALSE(inserted);
    EXPECT_EQ(it->value, 999);
    EXPECT_EQ(t.size(), 1u);
}

TEST(RBTree, InorderTraversalIsSorted) {
    RBTree<int, int> t;
    for (int k : {5, 3, 8, 1, 4, 7, 9, 2, 6, 0}) t.insert(k, k);
    std::vector<int> keys;
    for (auto it = t.begin(); it != t.end(); ++it) keys.push_back(it->key);
    EXPECT_TRUE(std::is_sorted(keys.begin(), keys.end()));
    EXPECT_EQ(keys.size(), 10u);
}

TEST(RBTree, StaysValidAndHeightBoundedUnderManyInserts) {
    // The whole point of the RB fixups: height stays O(log n), invariants hold.
    RBTree<int, int> t;
    constexpr int kN = 2000;
    for (int i = 0; i < kN; ++i)
        t.insert(i, i);  // ascending insert is the worst case for a plain BST.
    EXPECT_TRUE(t.validate());
    EXPECT_EQ(t.size(), static_cast<std::size_t>(kN));
    EXPECT_LE(t.height(), 2 * 11 + 2);  // 2*log2(2000) upper bound on RB height.
}

TEST(RBTree, LowerUpperBoundAndEqualRange) {
    RBTree<int, int> t;
    for (int k : {10, 20, 30, 40}) t.insert(k, k);
    EXPECT_EQ(t.lower_bound(20)->key, 20);
    EXPECT_EQ(t.upper_bound(20)->key, 30);
    EXPECT_EQ(t.lower_bound(25)->key, 30);  // no exact match rounds up to next key.
    EXPECT_EQ(t.upper_bound(40), t.end());  // nothing greater than the max.
    auto [lo, hi] = t.equal_range(30);
    EXPECT_EQ(lo->key, 30);
    EXPECT_EQ(hi->key, 40);
}

TEST(RBTree, EraseUntilEmptyKeepsInvariants) {
    RBTree<int, int> t;
    for (int i = 0; i < 100; ++i) t.insert(i, i);
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(t.erase(i));
        EXPECT_TRUE(t.validate());
    }
    EXPECT_TRUE(t.empty());
}

TEST(RBTree, MatchesStdMapOverRandomizedOperationSequence) {
    RBTree<int, int> mine;
    std::map<int, int> ref;
    std::mt19937 rng(1234);
    std::uniform_int_distribution<int> key(0, 50);
    for (int step = 0; step < 5000; ++step) {
        int k = key(rng);
        if (rng() % 3 == 0) {
            bool a = mine.erase(k);
            bool b = ref.erase(k) != 0;
            EXPECT_EQ(a, b);
        } else {
            int v = static_cast<int>(rng() % 1000);
            mine.insert(k, v);  // RBTree::insert overwrites, matching std::map::operator[].
            ref[k] = v;
        }
        ASSERT_EQ(mine.size(), ref.size());
        ASSERT_TRUE(mine.validate());
    }
    // Full content and ordering must agree at the end.
    std::vector<std::pair<int, int>> got;
    for (auto it = mine.begin(); it != mine.end(); ++it) got.emplace_back(it->key, it->value);
    std::vector<std::pair<int, int>> want(ref.begin(), ref.end());
    EXPECT_EQ(got, want);
}

// ---------------- MySet ----------------

TEST(MySet, InsertContainsEraseReturnValues) {
    MySet<int> s;
    EXPECT_TRUE(s.insert(1));
    EXPECT_FALSE(s.insert(1));  // duplicate insert reports no new element.
    EXPECT_TRUE(s.contains(1));
    EXPECT_FALSE(s.contains(2));
    EXPECT_TRUE(s.erase(1));
    EXPECT_FALSE(s.erase(1));
    EXPECT_EQ(s.size(), 0u);
}

TEST(MySet, IterationIsSortedAndUnique) {
    MySet<int> s;
    for (int k : {5, 1, 5, 3, 1, 9, 3}) s.insert(k);
    std::vector<int> keys(s.begin(), s.end());
    EXPECT_EQ(keys, (std::vector<int>{1, 3, 5, 9}));
}

TEST(MySet, MatchesStdSetOverRandomizedOperationSequence) {
    MySet<int> mine;
    std::set<int> ref;
    std::mt19937 rng(77);
    std::uniform_int_distribution<int> key(0, 40);
    for (int step = 0; step < 4000; ++step) {
        int k = key(rng);
        if (rng() % 2 == 0) {
            EXPECT_EQ(mine.insert(k), ref.insert(k).second);
        } else {
            EXPECT_EQ(mine.erase(k), ref.erase(k) != 0);
        }
        ASSERT_EQ(mine.size(), ref.size());
    }
    std::vector<int> got(mine.begin(), mine.end());
    std::vector<int> want(ref.begin(), ref.end());
    EXPECT_EQ(got, want);
}

// ---------------- MyMap ----------------

TEST(MyMap, OperatorBracketInsertsDefaultThenReads) {
    MyMap<std::string, int> m;
    EXPECT_EQ(m["missing"], 0);  // default-inserts a value-initialized int.
    EXPECT_EQ(m.size(), 1u);
    m["a"] = 5;
    m["a"] += 2;
    EXPECT_EQ(m["a"], 7);
}

TEST(MyMap, InsertAndFind) {
    MyMap<int, int> m;
    EXPECT_TRUE(m.insert(1, 100));
    EXPECT_FALSE(m.insert(1, 200));  // existing key: reports no new node.
    ASSERT_NE(m.find(1), nullptr);
    EXPECT_EQ(*m.find(1), 200);  // ...but value is updated.
    EXPECT_EQ(m.find(2), nullptr);
    EXPECT_TRUE(m.erase(1));
    EXPECT_EQ(m.find(1), nullptr);
}

TEST(MyMap, OrderedIterationMatchesStdMap) {
    MyMap<int, int> mine;
    std::map<int, int> ref;
    std::mt19937 rng(2024);
    for (int i = 0; i < 500; ++i) {
        int k = static_cast<int>(rng() % 100);
        int v = static_cast<int>(rng() % 100);
        mine[k] = v;
        ref[k] = v;
    }
    ASSERT_EQ(mine.size(), ref.size());
    auto it = mine.begin();
    auto rit = ref.begin();
    for (; it != mine.end() && rit != ref.end(); ++it, ++rit) {
        EXPECT_EQ(it->key, rit->first);
        EXPECT_EQ(it->value, rit->second);
    }
    EXPECT_EQ(it, mine.end());
}

// ---------------- MyUnorderedMap ----------------

TEST(MyUnorderedMap, InsertFindErase) {
    MyUnorderedMap<int, int> m;
    m[1] = 10;
    m[2] = 20;
    ASSERT_NE(m.find(1), nullptr);
    EXPECT_EQ(*m.find(1), 10);
    EXPECT_EQ(m.find(999), nullptr);
    EXPECT_EQ(m.size(), 2u);
    EXPECT_TRUE(m.erase(1));
    EXPECT_FALSE(m.erase(1));
    EXPECT_EQ(m.find(1), nullptr);
    EXPECT_EQ(m.size(), 1u);
}

TEST(MyUnorderedMap, RehashKeepsEntriesAndBoundsLoadFactor) {
    MyUnorderedMap<int, int> m(4);
    for (int i = 0; i < 200; ++i) m[i] = i * 2;
    EXPECT_GT(m.bucket_count(), 4u);  // auto-rehash grew the table.
    EXPECT_LE(m.load_factor(), m.max_load_factor());
    for (int i = 0; i < 200; ++i) {
        ASSERT_NE(m.find(i), nullptr);
        EXPECT_EQ(*m.find(i), i * 2);
    }
    m.rehash(1024);  // manual rehash must not lose or corrupt entries.
    EXPECT_EQ(m.bucket_count(), 1024u);
    EXPECT_EQ(m.size(), 200u);
    for (int i = 0; i < 200; ++i) EXPECT_EQ(*m.find(i), i * 2);
}

TEST(MyUnorderedMap, MatchesStdUnorderedMapOverRandomizedOperationSequence) {
    MyUnorderedMap<int, int> mine;
    std::unordered_map<int, int> ref;
    std::mt19937 rng(99);
    std::uniform_int_distribution<int> key(0, 100);
    for (int step = 0; step < 5000; ++step) {
        int k = key(rng);
        int op = static_cast<int>(rng() % 3);
        if (op == 0) {
            EXPECT_EQ(mine.erase(k), ref.erase(k) != 0);
        } else {
            int v = static_cast<int>(rng() % 1000);
            mine[k] = v;
            ref[k] = v;
        }
        ASSERT_EQ(mine.size(), ref.size());
    }
    // Every reference key must resolve to the same value; membership must agree exactly.
    for (const auto& [k, v] : ref) {
        auto* found = mine.find(k);
        ASSERT_NE(found, nullptr);
        EXPECT_EQ(*found, v);
    }
    for (int k = 0; k <= 100; ++k) {
        EXPECT_EQ(mine.find(k) != nullptr, ref.count(k) != 0);
    }
}
