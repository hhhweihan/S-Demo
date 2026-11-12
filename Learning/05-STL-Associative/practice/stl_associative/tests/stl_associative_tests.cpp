#include "mini_associative.h"

#include <algorithm>
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
using mini_stl::PoolAllocator;
using mini_stl::RBTree;

// 计数 allocator：每种 T 一对静态计数，用来断言节点分配确实经过了自定义 allocator。
// 满足 std::allocator 接口 + 转换 rebind 构造，可直接作为 RBTree 的 Alloc。
template <typename T>
struct CountingAllocator {
    using value_type = T;
    CountingAllocator() = default;
    template <typename U>
    CountingAllocator(const CountingAllocator<U>&) {}  // 转换 rebind 构造：树重绑到 Node 时需要。

    T* allocate(std::size_t n) {
        ++allocations();
        return static_cast<T*>(::operator new(sizeof(T) * n));
    }
    void deallocate(T* p, std::size_t) noexcept {
        ++deallocations();
        ::operator delete(p);
    }
    // 静态计数：无状态 allocator 的所有实例共享，任一实例分配的块可由另一实例释放。
    static std::size_t& allocations() {
        static std::size_t count = 0;
        return count;
    }
    static std::size_t& deallocations() {
        static std::size_t count = 0;
        return count;
    }
    template <typename U>
    bool operator==(const CountingAllocator<U>&) const {
        return true;
    }
    template <typename U>
    bool operator!=(const CountingAllocator<U>&) const {
        return false;
    }
};

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

TEST(RBTree, RealDeleteMatchesStdMapOverHeavyMixedOps) {
    // Centerpiece for the real RB delete: thousands of mixed insert/erase ops with a wide key
    // space so all delete-fixup cases (0/1/2 children, red/black sibling, near/far red nephew and
    // their mirrors) get exercised. At every checkpoint the whole content must equal std::map and
    // validate() (root-black / no red-red / uniform black-height / BST order) must still hold.
    auto full_content = [](const RBTree<int, int>& t) {
        std::vector<std::pair<int, int>> out;
        for (auto it = t.begin(); it != t.end(); ++it) out.emplace_back(it->key, it->value);
        return out;
    };
    for (unsigned seed : {1u, 7u, 42u, 1337u, 90210u}) {
        RBTree<int, int> mine;
        std::map<int, int> ref;
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> key(0, 300);
        for (int step = 0; step < 20000; ++step) {
            int k = key(rng);
            // Bias toward erase (~45%) once populated so deletions dominate and the tree churns.
            if (rng() % 100 < 45) {
                EXPECT_EQ(mine.erase(k), ref.erase(k) != 0);
            } else {
                int v = static_cast<int>(rng() % 100000);
                mine.insert(k, v);
                ref[k] = v;
            }
            ASSERT_EQ(mine.size(), ref.size());
            ASSERT_TRUE(mine.validate()) << "invariants broken at step " << step << " seed " << seed;
            if (step % 250 == 0) {  // periodic full-content diff vs std::map.
                std::vector<std::pair<int, int>> want(ref.begin(), ref.end());
                ASSERT_EQ(full_content(mine), want) << "content diverged at step " << step;
            }
        }
        // Drain the tree by erasing every present key; invariants must survive down to empty.
        std::vector<int> keys;
        for (const auto& [k, v] : ref) keys.push_back(k);
        std::shuffle(keys.begin(), keys.end(), rng);
        for (int k : keys) {
            ASSERT_TRUE(mine.erase(k));
            ASSERT_TRUE(mine.validate());
        }
        EXPECT_TRUE(mine.empty());
        EXPECT_EQ(mine.begin(), mine.end());
    }
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

// ---------------- Allocator-aware（allocator_traits 接入）----------------

// PoolAllocator 作为 Alloc：MySet 语义（插入/去重/erase/有序遍历）必须与默认 allocator 完全一致。
TEST(AllocatorAware, MySetWithPoolAllocatorBehavesCorrectly) {
    MySet<int, std::less<int>, PoolAllocator<int>> s;
    EXPECT_TRUE(s.insert(5));
    EXPECT_TRUE(s.insert(1));
    EXPECT_TRUE(s.insert(3));
    EXPECT_FALSE(s.insert(3));  // 重复插入不新增。
    EXPECT_TRUE(s.contains(1));
    EXPECT_FALSE(s.contains(2));
    EXPECT_EQ(s.size(), 3u);
    std::vector<int> keys(s.begin(), s.end());
    EXPECT_EQ(keys, (std::vector<int>{1, 3, 5}));  // 中序遍历升序。
    EXPECT_TRUE(s.erase(3));
    EXPECT_FALSE(s.erase(3));
    EXPECT_FALSE(s.contains(3));
    EXPECT_EQ(s.size(), 2u);
}

// PoolAllocator 作为 Alloc：MyMap 的 insert/find/operator[]/erase 与默认版一致，且大量增删稳定。
TEST(AllocatorAware, MyMapWithPoolAllocatorMatchesStdMap) {
    MyMap<int, int, std::less<int>, PoolAllocator<std::pair<const int, int>>> mine;
    std::map<int, int> ref;
    std::mt19937 rng(555);
    std::uniform_int_distribution<int> key(0, 80);
    for (int step = 0; step < 3000; ++step) {
        int k = key(rng);
        if (rng() % 3 == 0) {
            EXPECT_EQ(mine.erase(k), ref.erase(k) != 0);
        } else {
            int v = static_cast<int>(rng() % 1000);
            mine[k] = v;
            ref[k] = v;
        }
        ASSERT_EQ(mine.size(), ref.size());
    }
    for (const auto& [k, v] : ref) {
        auto* found = mine.find(k);
        ASSERT_NE(found, nullptr);
        EXPECT_EQ(*found, v);
    }
    // 有序遍历也须与 std::map 完全一致。
    auto it = mine.begin();
    auto rit = ref.begin();
    for (; it != mine.end() && rit != ref.end(); ++it, ++rit) {
        EXPECT_EQ(it->key, rit->first);
        EXPECT_EQ(it->value, rit->second);
    }
    EXPECT_EQ(it, mine.end());
}

// 核心断言：节点分配确实路由到了自定义 allocator——填充后 allocate>0，clear 后 allocate==deallocate。
TEST(AllocatorAware, NodeAllocationRoutedThroughCustomAllocator) {
    using Alloc = CountingAllocator<std::pair<const int, int>>;
    // 计数按重绑后的 Node 类型统计，先归零，隔离其它用例的影响。
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<
        typename RBTree<int, int, std::less<int>, Alloc>::Node>;
    NodeAlloc::allocations() = 0;
    NodeAlloc::deallocations() = 0;
    {
        MyMap<int, int, std::less<int>, Alloc> m;
        for (int i = 0; i < 200; ++i) m.insert(i, i * 2);
        for (int i = 0; i < 50; ++i) EXPECT_TRUE(m.erase(i));  // 触发 destroy_node/deallocate。
        EXPECT_GT(NodeAlloc::allocations(), 0u);  // 确有节点经自定义 allocator 分配。
        EXPECT_EQ(NodeAlloc::allocations(), 200u);
        EXPECT_EQ(NodeAlloc::deallocations(), 50u);
    }  // m 析构 → clear() 回收剩余 150 个节点。
    EXPECT_EQ(NodeAlloc::allocations(), 200u);
    EXPECT_EQ(NodeAlloc::deallocations(), 200u);  // 全部归还：allocate 计数 == deallocate 计数。
}
