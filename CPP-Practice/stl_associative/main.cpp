#include <algorithm>  // 使用 std::is_sorted 等标准算法验证自编算法结果。
#include <cmath>  // 使用 std::log2 计算红黑树高度理论边界。
#include <cstdlib>  // 使用 std::exit 在测试失败时终止程序。
#include <iostream>  // 使用 std::cout/std::cerr 输出测试结果。
#include <random>  // 使用 std::mt19937 生成大量随机 key 测试红黑树。
#include <string>  // 使用 std::string 作为 map/unordered_map 的 key。
#include <vector>  // 使用 std::vector 收集遍历结果并测试算法。

#include "mini_associative.h"  // 引入本模块手写关联容器、算法和 allocator。

namespace {  // 匿名命名空间限制测试函数只在当前文件可见。

void expect(bool condition, const std::string& message) {  // 简单测试断言函数。
  if (!condition) {  // 条件为 false 表示当前验证失败。
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息到标准错误流。
    std::exit(1);  // 立即终止程序，避免继续运行产生误导输出。
  }  // 失败分支结束。
  std::cout << "[PASS] " << message << '\n';  // 条件成立时输出通过信息。
}  // expect 函数结束。

void test_rb_tree() {  // 验证红黑树插入、平衡性、删除和中序遍历。
  mini_stl::RBTree<int, int> tree;  // 构造 key/value 都为 int 的红黑树。
  std::mt19937 rng(42);  // 使用固定种子的伪随机数生成器，让测试可复现。
  for (int index = 0; index < 100000; ++index) {  // 插入大量随机值，放大平衡性问题。
    int value = static_cast<int>(rng());  // 生成一个随机 int 作为 key 和 value。
    tree.insert(value, value);  // 插入或更新红黑树节点。
  }  // 随机插入结束。
  const double limit = 2.0 * std::log2(static_cast<double>(tree.size()) + 1.0) + 2.0;  // 计算红黑树高度的宽松理论上界。
  expect(tree.validate(), "RBTree keeps red/black and ordering invariants");  // 验证红黑性质和二叉搜索树顺序。
  expect(tree.height() <= limit, "RBTree height stays within 2*log2(n) bound");  // 验证树高没有退化。
  expect(tree.erase(tree.begin()->key) && tree.validate(), "RBTree erase removes a key and keeps invariants by rebuild");  // 验证教学版删除后仍合法。

  int previous = -2147483648;  // 保存上一个遍历到的 key，用于检查非递减顺序。
  bool sorted = true;  // 记录中序遍历是否保持有序。
  for (auto it = tree.begin(); it != tree.end(); ++it) {  // 使用红黑树迭代器做中序遍历。
    sorted = sorted && previous <= it->key;  // 当前 key 应不小于上一个 key。
    previous = it->key;  // 更新上一个 key。
  }  // 遍历结束。
  expect(sorted, "RBTree iterator yields nondecreasing keys");  // 验证迭代器输出有序 key。
}  // 红黑树测试结束。

void test_set_map() {  // 验证有序 set/map 和 multi 容器封装。
  mini_stl::MySet<int> set;  // 构造基于 RBTree 的 set。
  set.insert(3);  // 插入 key 3。
  set.insert(1);  // 插入 key 1。
  set.insert(2);  // 插入 key 2。
  std::vector<int> values;  // 用标准 vector 收集遍历结果。
  for (int value : set) values.push_back(value);  // 遍历 set，期望按 key 有序。
  expect((values == std::vector<int>{1, 2, 3}), "MySet iterates in sorted order");  // 验证 set 迭代顺序。
  expect(set.erase(2) && !set.contains(2), "MySet supports erase(key)");  // 验证 set 删除和 contains。

  mini_stl::MyMultiSet<int> multiset;  // 构造允许重复 key 的 multiset。
  multiset.insert(1);  // 插入第一个 1。
  multiset.insert(1);  // 插入第二个 1。
  multiset.insert(2);  // 插入 2。
  expect(multiset.count(1) == 2, "MyMultiSet keeps duplicate keys");  // 验证重复 key 计数。

  mini_stl::MyMap<std::string, int> map;  // 构造 string 到 int 的有序 map。
  map["cpp"] = 17;  // 使用 operator[] 插入并赋值。
  map["stl"] = 98;  // 插入另一个 key/value。
  map["map"] = 42;  // 插入用于边界查询的 key/value。
  expect(map.find("stl") && *map.find("stl") == 98, "MyMap supports find and operator[]");  // 验证 find 和 operator[]。
  auto lower = map.lower_bound("m");  // 查找第一个不小于 "m" 的节点。
  auto upper = map.upper_bound("map");  // 查找第一个大于 "map" 的节点。
  auto range = map.equal_range("map");  // 同时取得 map 这个 key 的等价范围。
  expect(lower != map.end() && lower->key == "map", "MyMap supports lower_bound");  // 验证 lower_bound 结果。
  expect(upper != map.end() && upper->key == "stl", "MyMap supports upper_bound");  // 验证 upper_bound 结果。
  expect(range.first->key == "map" && range.second->key == "stl", "MyMap supports equal_range");  // 验证 equal_range 边界。
  expect(map.erase("cpp") && !map.find("cpp"), "MyMap supports erase(key)");  // 验证 map 删除。

  mini_stl::MyMultiMap<std::string, int> multimap;  // 构造允许重复 key 的 multimap。
  multimap.insert("same", 1);  // 插入同 key 的第一个值。
  multimap.insert("same", 2);  // 插入同 key 的第二个值。
  expect(multimap.count("same") == 2, "MyMultiMap keeps duplicate keys");  // 验证重复 key 计数。
}  // set/map 测试结束。

void test_unordered_map() {  // 验证开链哈希表插入、rehash、查找和删除。
  mini_stl::MyUnorderedMap<std::string, int> map(4);  // 构造初始桶数为 4 的哈希表。
  const auto before = map.bucket_count();  // 记录插入前桶数量。
  map["a"] = 1;  // 插入第一个键值。
  map["b"] = 2;  // 插入第二个键值。
  map["c"] = 3;  // 插入第三个键值。
  map["d"] = 4;  // 插入第四个键值，触发负载因子检查。
  expect(map.bucket_count() > before, "MyUnorderedMap rehashes when load factor exceeds 0.75");  // 验证超负载后自动扩桶。
  expect(map.find("c") && *map.find("c") == 3, "MyUnorderedMap keeps values after rehash");  // 验证 rehash 后值仍可查找。
  expect(map.erase("c") && !map.find("c"), "MyUnorderedMap supports erase(key)");  // 验证删除后 key 不存在。
}  // unordered_map 测试结束。

void test_algorithms_allocator() {  // 验证算法封装和 allocator 示例。
  std::vector<int> values{4, 1, 5, 2, 3};  // 构造乱序整数数组。
  mini_stl::heap_sort(values.begin(), values.end());  // 使用手写封装的 heap_sort 排序。
  expect(std::is_sorted(values.begin(), values.end()), "heap_sort sorts a random-access range");  // 验证排序结果有序。
  expect(mini_stl::binary_search(values.begin(), values.end(), 3), "binary_search finds an existing value");  // 验证二分查找找到已有值。

  std::vector<std::pair<int, char>> stable{{2, 'a'}, {1, 'x'}, {2, 'b'}};  // 构造含相等 key 的序列测试稳定排序。
  mini_stl::stable_sort(stable.begin(), stable.end(), [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });  // 按 first 稳定排序。
  expect(stable[1].second == 'a' && stable[2].second == 'b', "stable_sort preserves relative order for equivalent keys");  // 验证相等 key 的相对顺序保持。
  std::vector<int> partial{9, 1, 7, 2, 8};  // 构造部分排序输入。
  mini_stl::partial_sort(partial.begin(), partial.begin() + 3, partial.end());  // 只排序最小的前三个元素。
  expect((std::vector<int>{partial[0], partial[1], partial[2]} == std::vector<int>{1, 2, 7}), "partial_sort selects the smallest prefix");  // 验证最小前缀正确。

  std::vector<int, mini_stl::PoolAllocator<int>> pooled;  // 使用手写 allocator 构造标准 vector。
  for (int value = 0; value < 1000; ++value) pooled.push_back(value);  // 插入一批元素触发多次分配。
  expect(pooled.size() == 1000 && pooled.back() == 999, "PoolAllocator works with std::vector allocator_traits");  // 验证 allocator_traits 能使用该 allocator。
}  // 算法和 allocator 测试结束。

}  // namespace  // 匿名命名空间结束。

int main() {  // 程序入口，按主题依次运行关联容器测试。
  std::cout << "== Month 05 STL associative demo ==\n";  // 输出整体示例标题。
  test_rb_tree();  // 运行红黑树测试。
  test_set_map();  // 运行 set/map/multi 容器测试。
  test_unordered_map();  // 运行 unordered_map 测试。
  test_algorithms_allocator();  // 运行算法和 allocator 测试。
  std::cout << "All Month 05 STL associative tests passed.\n";  // 输出全部测试通过提示。
  return 0;  // 返回 0 表示程序正常结束。
}  // main 函数结束。