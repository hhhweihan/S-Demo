#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "mini_associative.h"

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

void test_rb_tree() {
    mini_stl::RBTree<int, int> tree;
    std::mt19937 rng(42);  // 固定种子让插入序列可复现，红黑树平衡问题才能稳定重现。
    for (int index = 0; index < 100000; ++index) {
        int value = static_cast<int>(rng());
        tree.insert(value, value);
    }
    // 红黑树保证树高 <= 2*log2(n+1)：用它验证大量随机插入后树没有退化成链。
    const double limit = 2.0 * std::log2(static_cast<double>(tree.size()) + 1.0) + 2.0;
    expect(tree.validate(), "RBTree keeps red/black and ordering invariants");
    expect(tree.height() <= limit, "RBTree height stays within 2*log2(n) bound");
    expect(tree.erase(tree.begin()->key) && tree.validate(),
           "RBTree erase removes a key and keeps invariants by rebuild");

    int previous = -2147483648;  // 从 int 下界起步，保证首个 key 一定 >= previous。
    bool sorted = true;
    // 中序遍历红黑树必然产出非递减 key——这是 BST 有序性的对外可观测保证。
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        sorted = sorted && previous <= it->key;
        previous = it->key;
    }
    expect(sorted, "RBTree iterator yields nondecreasing keys");
}

void test_set_map() {
    mini_stl::MySet<int> set;
    set.insert(3);
    set.insert(1);
    set.insert(2);
    std::vector<int> values;
    for (int value : set) values.push_back(value);
    expect((values == std::vector<int>{1, 2, 3}), "MySet iterates in sorted order");
    expect(set.erase(2) && !set.contains(2), "MySet supports erase(key)");

    mini_stl::MyMultiSet<int> multiset;
    multiset.insert(1);
    multiset.insert(1);
    multiset.insert(2);
    expect(multiset.count(1) == 2,
           "MyMultiSet keeps duplicate keys");  // multiset 保留重复 key，count 应等于插入次数。

    mini_stl::MyMap<std::string, int> map;
    map["cpp"] = 17;
    map["stl"] = 98;
    map["map"] = 42;
    expect(map.find("stl") && *map.find("stl") == 98, "MyMap supports find and operator[]");
    auto lower = map.lower_bound("m");    // 第一个 >= "m" 的 key。
    auto upper = map.upper_bound("map");  // 第一个 > "map" 的 key。
    auto range = map.equal_range("map");
    expect(lower != map.end() && lower->key == "map", "MyMap supports lower_bound");
    expect(upper != map.end() && upper->key == "stl", "MyMap supports upper_bound");
    expect(range.first->key == "map" && range.second->key == "stl", "MyMap supports equal_range");
    expect(map.erase("cpp") && !map.find("cpp"), "MyMap supports erase(key)");

    mini_stl::MyMultiMap<std::string, int> multimap;
    multimap.insert("same", 1);
    multimap.insert("same", 2);
    expect(multimap.count("same") == 2, "MyMultiMap keeps duplicate keys");
}

void test_unordered_map() {
    mini_stl::MyUnorderedMap<std::string, int> map(4);
    const auto before = map.bucket_count();
    map["a"] = 1;
    map["b"] = 2;
    map["c"] = 3;
    map["d"] = 4;
    // 负载因子超过 0.75 触发 rehash：桶数应增长，且 rehash 迁移不能丢数据。
    expect(map.bucket_count() > before, "MyUnorderedMap rehashes when load factor exceeds 0.75");
    expect(map.find("c") && *map.find("c") == 3, "MyUnorderedMap keeps values after rehash");
    expect(map.erase("c") && !map.find("c"), "MyUnorderedMap supports erase(key)");
}

void test_algorithms_allocator() {
    std::vector<int> values{4, 1, 5, 2, 3};
    mini_stl::heap_sort(values.begin(), values.end());
    expect(std::is_sorted(values.begin(), values.end()), "heap_sort sorts a random-access range");
    expect(mini_stl::binary_search(values.begin(), values.end(), 3),
           "binary_search finds an existing value");

    std::vector<std::pair<int, char>> stable{
        {2, 'a'}, {1, 'x'}, {2, 'b'}};  // 两个 key=2 的元素，用来检验稳定性。
    mini_stl::stable_sort(stable.begin(), stable.end(),
                          [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
    // 稳定排序的定义：等价 key 保持输入相对顺序，故 'a' 仍在 'b' 前。
    expect(stable[1].second == 'a' && stable[2].second == 'b',
           "stable_sort preserves relative order for equivalent keys");
    std::vector<int> partial{9, 1, 7, 2, 8};
    mini_stl::partial_sort(partial.begin(), partial.begin() + 3,
                           partial.end());  // 只保证前 3 名有序，其余不排。
    expect((std::vector<int>{partial[0], partial[1], partial[2]} == std::vector<int>{1, 2, 7}),
           "partial_sort selects the smallest prefix");

    std::vector<int, mini_stl::PoolAllocator<int>>
        pooled;  // 把自定义 allocator 塞进标准 vector，验证 allocator_traits 适配。
    for (int value = 0; value < 1000; ++value)
        pooled.push_back(value);  // 大量插入触发多次 allocate/扩容。
    expect(pooled.size() == 1000 && pooled.back() == 999,
           "PoolAllocator works with std::vector allocator_traits");
}

}  // namespace

int main() {
    std::cout << "== Month 05 STL associative demo ==\n";
    test_rb_tree();
    test_set_map();
    test_unordered_map();
    test_algorithms_allocator();
    std::cout << "All Month 05 STL associative tests passed.\n";
    return 0;
}