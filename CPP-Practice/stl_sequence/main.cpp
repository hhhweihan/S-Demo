#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "mini_stl.h"

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

void test_iterator_system() {
    using traits = mini_stl::iterator_traits<int*>;
    static_assert(std::is_same_v<traits::value_type, int>);
    mini_stl::MyVector<int> values{1, 2, 3};
    mini_stl::ReverseIterator reversed(values.end());
    // 反向迭代器解引用的是 base() 的前一个元素——这是 std::reverse_iterator 的核心约定。
    expect(*reversed == 3, "ReverseIterator reads from the element before base()");
}

void test_vector() {
    mini_stl::MyVector<int> values{5, 1, 4, 2, 3};
    std::sort(values.begin(),
              values.end());  // 能被 std::sort 使用即证明迭代器满足 random-access 要求。
    expect(std::is_sorted(values.begin(), values.end()), "MyVector<int> works with std::sort");
    int sum = 0;
    for (int value : values) sum += value;
    expect(sum == 15, "MyVector supports range-for iteration");
}

void test_deque() {
    mini_stl::MyDeque<int> values;
    values.push_back(2);
    values.push_back(3);
    values.push_front(1);
    values.push_front(0);
    values.pop_front();  // 触发环形缓冲区 head 移动，验证逻辑顺序不受物理位置影响。
    values.push_back(4);
    std::vector<int> copied;
    for (int value : values) copied.push_back(value);
    expect((copied == std::vector<int>{1, 2, 3, 4}),
           "MyDeque preserves order across front/back operations");
    expect(values.front() == 1 && values.back() == 4 && values[2] == 3,
           "MyDeque supports indexed access");
}

void test_list() {
    mini_stl::MyList<int> values{1, 2, 3, 4};
    std::reverse(
        values.begin(),
        values.end());  // std::reverse 只需双向迭代器，用它验证 MyList 的 bidirectional 语义。
    std::vector<int> copied;
    for (int value : values) copied.push_back(value);
    expect((copied == std::vector<int>{4, 3, 2, 1}), "MyList<int> works with std::reverse");
}

void test_forward_list() {
    mini_stl::MyForwardList<int> values;
    values.push_front(3);
    values.push_front(2);
    values.push_front(1);  // 头插使逻辑顺序与插入顺序相反，得到 1,2,3。
    int sum = 0;
    for (int value : values) sum += value;
    expect(sum == 6 && values.size() == 3, "MyForwardList supports forward range-for traversal");
}

void test_adapters() {
    mini_stl::MyStack<int> stack;
    stack.push(1);
    stack.push(2);
    expect(stack.top() == 2, "MyStack exposes LIFO top");
    stack.pop();
    expect(stack.top() == 1, "MyStack pop removes latest value");

    mini_stl::MyQueue<int> queue;
    queue.push(1);
    queue.push(2);
    expect(queue.front() == 1, "MyQueue exposes FIFO front");
    queue.pop();
    expect(queue.front() == 2, "MyQueue pop removes oldest value");

    mini_stl::MyPriorityQueue<int, std::greater<int>>
        min_heap;  // greater 比较器把默认大顶堆翻转成小顶堆。
    min_heap.push(3);
    min_heap.push(1);
    min_heap.push(2);
    expect(min_heap.top() == 1, "MyPriorityQueue supports a custom comparator");
}

}  // namespace

int main() {
    std::cout << "== Month 04 STL sequence demo ==\n";
    test_iterator_system();
    test_vector();
    test_deque();
    test_list();
    test_forward_list();
    test_adapters();
    std::cout << "All Month 04 STL sequence tests passed.\n";
    return 0;
}