#include <algorithm>  // 使用 std::sort、std::reverse、std::is_sorted 验证迭代器兼容性。
#include <cstdlib>  // 使用 std::exit 在测试失败时终止程序。
#include <iostream>  // 使用 std::cout/std::cerr 输出测试结果。
#include <numeric>  // 预留数值算法头，便于后续扩展求和类测试。
#include <string>  // 使用 std::string 保存断言说明文本。
#include <vector>  // 使用 std::vector 作为对照容器收集遍历结果。

#include "mini_stl.h"  // 引入本模块手写的 STL 序列容器和适配器。

namespace {  // 匿名命名空间限制测试函数只在当前文件可见。

void expect(bool condition, const std::string& message) {  // 简单测试断言函数。
  if (!condition) {  // 条件为 false 表示当前验证失败。
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息到标准错误流。
    std::exit(1);  // 立即终止程序，避免继续运行产生误导输出。
  }  // 失败分支结束。
  std::cout << "[PASS] " << message << '\n';  // 条件成立时输出通过信息。
}  // expect 函数结束。

void test_iterator_system() {  // 验证 iterator_traits 和 ReverseIterator 的基础行为。
  using traits = mini_stl::iterator_traits<int*>;  // 对原生指针使用手写 iterator_traits 特化。
  static_assert(std::is_same_v<traits::value_type, int>);  // 编译期验证 int* 的 value_type 是 int。
  mini_stl::MyVector<int> values{1, 2, 3};  // 构造一个手写 vector 作为反向迭代测试数据。
  mini_stl::ReverseIterator reversed(values.end());  // 用 end() 构造反向迭代器，指向最后一个元素的反向位置。
  expect(*reversed == 3, "ReverseIterator reads from the element before base()");  // 验证反向迭代器解引用 base 前一个元素。
}  // 迭代器体系测试结束。

void test_vector() {  // 验证 MyVector 的随机访问迭代器和 range-for 支持。
  mini_stl::MyVector<int> values{5, 1, 4, 2, 3};  // 构造包含乱序整数的 MyVector。
  std::sort(values.begin(), values.end());  // 使用标准库 sort 验证迭代器满足随机访问需求。
  expect(std::is_sorted(values.begin(), values.end()), "MyVector<int> works with std::sort");  // 验证排序结果有序。
  int sum = 0;  // 准备累加器。
  for (int value : values) sum += value;  // 使用 range-for 遍历 MyVector。
  expect(sum == 15, "MyVector supports range-for iteration");  // 验证遍历得到所有元素。
}  // MyVector 测试结束。

void test_deque() {  // 验证 MyDeque 的头尾插入删除和下标访问。
  mini_stl::MyDeque<int> values;  // 构造空双端队列。
  values.push_back(2);  // 尾部插入 2。
  values.push_back(3);  // 尾部插入 3。
  values.push_front(1);  // 头部插入 1。
  values.push_front(0);  // 头部插入 0。
  values.pop_front();  // 删除头部 0，测试头指针移动。
  values.push_back(4);  // 尾部插入 4，形成 1,2,3,4。
  std::vector<int> copied;  // 用标准 vector 收集遍历结果。
  for (int value : values) copied.push_back(value);  // 遍历 MyDeque 并复制到对照容器。
  expect((copied == std::vector<int>{1, 2, 3, 4}), "MyDeque preserves order across front/back operations");  // 验证逻辑顺序正确。
  expect(values.front() == 1 && values.back() == 4 && values[2] == 3, "MyDeque supports indexed access");  // 验证 front/back/operator[]。
}  // MyDeque 测试结束。

void test_list() {  // 验证 MyList 的双向迭代器可被 std::reverse 使用。
  mini_stl::MyList<int> values{1, 2, 3, 4};  // 构造双向链表。
  std::reverse(values.begin(), values.end());  // 使用标准库 reverse 反转链表元素值。
  std::vector<int> copied;  // 用标准 vector 收集遍历结果。
  for (int value : values) copied.push_back(value);  // 遍历 MyList 并复制元素。
  expect((copied == std::vector<int>{4, 3, 2, 1}), "MyList<int> works with std::reverse");  // 验证反转结果正确。
}  // MyList 测试结束。

void test_forward_list() {  // 验证 MyForwardList 的前向遍历。
  mini_stl::MyForwardList<int> values;  // 构造空单向链表。
  values.push_front(3);  // 头插 3。
  values.push_front(2);  // 头插 2。
  values.push_front(1);  // 头插 1，链表逻辑顺序为 1,2,3。
  int sum = 0;  // 准备累加器。
  for (int value : values) sum += value;  // 使用 range-for 前向遍历链表。
  expect(sum == 6 && values.size() == 3, "MyForwardList supports forward range-for traversal");  // 验证遍历总和和大小。
}  // MyForwardList 测试结束。

void test_adapters() {  // 验证栈、队列和优先队列适配器。
  mini_stl::MyStack<int> stack;  // 构造基于 MyVector 的栈。
  stack.push(1);  // 压入 1。
  stack.push(2);  // 压入 2。
  expect(stack.top() == 2, "MyStack exposes LIFO top");  // 验证栈顶是最后压入的值。
  stack.pop();  // 弹出栈顶 2。
  expect(stack.top() == 1, "MyStack pop removes latest value");  // 验证弹出后栈顶回到 1。

  mini_stl::MyQueue<int> queue;  // 构造基于 MyList 的队列。
  queue.push(1);  // 入队 1。
  queue.push(2);  // 入队 2。
  expect(queue.front() == 1, "MyQueue exposes FIFO front");  // 验证队首是最早入队的值。
  queue.pop();  // 出队 1。
  expect(queue.front() == 2, "MyQueue pop removes oldest value");  // 验证出队后队首变为 2。

  mini_stl::MyPriorityQueue<int, std::greater<int>> min_heap;  // 使用 greater 构造小顶堆优先队列。
  min_heap.push(3);  // 插入 3。
  min_heap.push(1);  // 插入 1。
  min_heap.push(2);  // 插入 2。
  expect(min_heap.top() == 1, "MyPriorityQueue supports a custom comparator");  // 验证自定义比较器让最小值位于堆顶。
}  // 容器适配器测试结束。

}  // namespace  // 匿名命名空间结束。

int main() {  // 程序入口，按容器主题依次运行测试。
  std::cout << "== Month 04 STL sequence demo ==\n";  // 输出整体示例标题。
  test_iterator_system();  // 运行迭代器体系测试。
  test_vector();  // 运行 MyVector 测试。
  test_deque();  // 运行 MyDeque 测试。
  test_list();  // 运行 MyList 测试。
  test_forward_list();  // 运行 MyForwardList 测试。
  test_adapters();  // 运行容器适配器测试。
  std::cout << "All Month 04 STL sequence tests passed.\n";  // 输出全部测试通过提示。
  return 0;  // 返回 0 表示程序正常结束。
}  // main 函数结束。