#include <algorithm>  // 使用 std::max 等通用算法。
#include <array>  // 使用 std::array 演示 constexpr 排序和质数筛。
#include <chrono>  // 使用 steady_clock 和 duration 进行简单 benchmark。
#include <cmath>  // 使用 std::abs 比较浮点计算结果。
#include <concepts>  // 使用 C++20 concept 和 std::integral 约束模板。
#include <cstdlib>  // 使用 std::exit 在测试失败时终止程序。
#include <functional>  // 使用 std::invoke 调用被提交的任务。
#include <future>  // 使用 std::packaged_task 和 std::future 模拟线程池提交结果。
#include <iostream>  // 使用 std::cout/std::cerr 输出示例结果。
#include <memory>  // 使用 std::unique_ptr 管理分配器 chunk。
#include <mutex>  // 使用 std::mutex 和 std::lock_guard 实现多线程策略。
#include <numeric>  // 预留数值算法头，便于综合示例扩展。
#include <sstream>  // 使用 std::ostringstream 收集格式化输出。
#include <string>  // 使用 std::string 参与综合测试。
#include <type_traits>  // 使用 std::is_arithmetic_v、std::invoke_result_t 等类型工具。
#include <utility>  // 使用 std::move 和 std::forward。
#include <vector>  // 使用 std::vector 验证 Container concept 和分配器存储。

#include "templates/my_function.h"  // 引入 Month 3 手写 Function。
#include "templates/my_optional.h"  // 引入 Month 3 手写 Optional。
#include "templates/my_tuple.h"  // 引入 Month 3 手写 Tuple。
#include "templates/my_type_traits.h"  // 引入 Month 3 手写 type_traits。
#include "templates/my_variant.h"  // 引入 Month 3 手写 Variant。

namespace demo {  // 示例代码放在 demo 命名空间中，避免污染全局命名空间。

using erasure::Function;  // 引入手写 Function，减少后续限定名。
using erasure::Optional;  // 引入手写 Optional，减少后续限定名。
using erasure::Variant;  // 引入手写 Variant，减少后续限定名。
using erasure::visit;  // 引入 Variant 的 visit 分发函数。
using tuple::Tuple;  // 引入手写 Tuple 类型。
using tuple::get;  // 引入 Tuple 的下标访问函数。

void expect(bool condition, const std::string& message) {  // 简单测试断言函数。
  if (!condition) {  // 条件不成立表示当前示例失败。
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息到标准错误流。
    std::exit(1);  // 立即退出程序，避免继续运行产生误导输出。
  }  // 失败分支结束。
  std::cout << "[PASS] " << message << '\n';  // 条件成立时输出通过信息。
}  // expect 函数结束。

template <typename Derived>  // Derived 是实际派生类类型，用于 CRTP 静态派发。
class ShapeCRTP {  // CRTP 基类，通过模板参数获得派生类接口。
 public:  // 暴露统一的 area 调用入口。
  double area() const {  // 对外提供面积计算接口。
    return static_cast<const Derived*>(this)->area_impl();  // 静态转型到派生类并调用 area_impl，避免虚调用。
  }  // area 结束。
};  // ShapeCRTP 定义结束。

class CircleCRTP : public ShapeCRTP<CircleCRTP> {  // 使用 CRTP 的圆形实现。
 public:  // 暴露构造函数和 CRTP 调用的实现函数。
  explicit CircleCRTP(double radius) : radius_(radius) {}  // 保存圆的半径。

  double area_impl() const {  // CRTP 基类会静态调用该实现函数。
    return 3.14159265358979323846 * radius_ * radius_;  // 使用 pi*r*r 计算面积。
  }  // area_impl 结束。

 private:  // 内部状态只由 CircleCRTP 自己维护。
  double radius_;  // 圆的半径。
};  // CircleCRTP 定义结束。

class ShapeVirtual {  // 使用传统虚函数动态多态的形状基类。
 public:  // 暴露虚析构和纯虚面积接口。
  virtual ~ShapeVirtual() = default;  // 虚析构保证通过基类引用/指针销毁派生类安全。
  virtual double area() const = 0;  // 纯虚接口，由派生类实现面积计算。
};  // ShapeVirtual 定义结束。

class CircleVirtual final : public ShapeVirtual {  // 使用虚函数多态的圆形实现。
 public:  // 暴露构造函数和重写的面积接口。
  explicit CircleVirtual(double radius) : radius_(radius) {}  // 保存圆的半径。

  double area() const override {  // 重写 ShapeVirtual 的虚函数接口。
    return 3.14159265358979323846 * radius_ * radius_;  // 使用 pi*r*r 计算面积。
  }  // area 重写结束。

 private:  // 内部状态只由 CircleVirtual 自己维护。
  double radius_;  // 圆的半径。
};  // CircleVirtual 定义结束。

template <typename F>  // F 是需要测量耗时的可调用对象类型。
long long measure_ns(F&& function) {  // 执行函数并返回纳秒级耗时。
  const auto start = std::chrono::steady_clock::now();  // 记录开始时间点。
  volatile double sink = function();  // 保存结果到 volatile，降低循环被优化掉的概率。
  (void)sink;  // 显式标记 sink 已使用，避免未使用警告。
  const auto end = std::chrono::steady_clock::now();  // 记录结束时间点。
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();  // 返回持续时间的纳秒数。
}  // measure_ns 结束。

void run_crtp_benchmark() {  // 运行 CRTP 静态多态和虚函数动态多态的简单对比。
  std::cout << "== CRTP benchmark ==\n";  // 输出当前示例分组标题。

  constexpr int iterations = 10'000'000;  // 设置重复调用次数，让耗时更容易观察。
  CircleCRTP crtp(3.0);  // 构造 CRTP 版本圆形。
  CircleVirtual virtual_circle(3.0);  // 构造虚函数版本圆形。
  const ShapeVirtual& virtual_shape = virtual_circle;  // 通过基类引用触发虚调用。

  double crtp_sum = 0.0;  // 保存 CRTP 循环计算总和，供后续结果比较。
  const auto crtp_ns = measure_ns([&] {  // 测量 CRTP area 调用循环耗时。
    double sum = 0.0;  // 局部累加值，避免每次写外部变量。
    for (int index = 0; index < iterations; ++index) {  // 重复调用 CRTP area。
      sum += crtp.area();  // 静态派发到 CircleCRTP::area_impl。
    }  // 循环结束。
    crtp_sum = sum;  // 保存总和供 benchmark 之后比较。
    return sum;  // 返回总和给 measure_ns 的 sink。
  });  // CRTP 测量结束。

  double virtual_sum = 0.0;  // 保存虚函数循环计算总和，供后续结果比较。
  const auto virtual_ns = measure_ns([&] {  // 测量虚函数 area 调用循环耗时。
    double sum = 0.0;  // 局部累加值，避免每次写外部变量。
    for (int index = 0; index < iterations; ++index) {  // 重复通过基类引用调用 area。
      sum += virtual_shape.area();  // 动态派发到 CircleVirtual::area。
    }  // 循环结束。
    virtual_sum = sum;  // 保存总和供 benchmark 之后比较。
    return sum;  // 返回总和给 measure_ns 的 sink。
  });  // 虚函数测量结束。

  std::cout << "CRTP area loop: " << crtp_ns / 1'000'000.0 << " ms\n";  // 输出 CRTP 循环毫秒耗时。
  std::cout << "virtual area loop: " << virtual_ns / 1'000'000.0 << " ms\n";  // 输出虚函数循环毫秒耗时。
  expect(std::abs(crtp_sum - virtual_sum) < 0.001,  // 验证两种实现计算结果一致。
         "CRTP and virtual versions compute the same total area");  // 断言说明文本。
}  // CRTP benchmark 示例结束。

struct SingleThreaded {  // 单线程策略，不做实际同步。
  void lock() {}  // lock 接口为空实现，满足 lock_guard 需要的 BasicLockable 形状。
  void unlock() {}  // unlock 接口为空实现，满足 lock_guard 需要的 BasicLockable 形状。
};  // SingleThreaded 策略结束。

struct MultiThreaded {  // 多线程策略，内部使用 std::mutex 同步。
  void lock() { mutex.lock(); }  // 加锁接口转发给 mutex。
  void unlock() { mutex.unlock(); }  // 解锁接口转发给 mutex。

  std::mutex mutex;  // 保存互斥量状态。
};  // MultiThreaded 策略结束。

struct DoubleGrowth {  // 倍增扩容策略。
  std::size_t next(std::size_t current) const {  // 根据当前容量计算下一次容量。
    return current == 0 ? 8 : current * 2;  // 初次扩到 8，之后容量翻倍。
  }  // next 结束。
};  // DoubleGrowth 策略结束。

struct LinearGrowth {  // 线性扩容策略。
  std::size_t next(std::size_t current) const {  // 根据当前容量计算下一次容量。
    return current + 8;  // 每次固定增加 8 个块。
  }  // next 结束。
};  // LinearGrowth 策略结束。

template <typename ThreadingPolicy = SingleThreaded, typename GrowthPolicy = DoubleGrowth>  // 两个策略参数分别控制同步和扩容。
class PolicyFixedAllocator {  // 固定块分配器，通过策略类组合行为。
 public:  // 暴露构造、分配、释放和容量查询接口。
  explicit PolicyFixedAllocator(std::size_t block_size)  // 构造时指定每个块大小。
      : block_size_(std::max(block_size, sizeof(Node))) {}  // 块大小至少能容纳空闲链表节点。

  void* allocate() {  // 从分配器中取出一个固定大小的块。
    std::lock_guard<PolicyFixedAllocator> lock(*this);  // 通过当前对象的 lock/unlock 接口应用线程策略。
    if (free_list_ == nullptr) {  // 空闲链表为空时需要扩容。
      grow();  // 按扩容策略新增一批块。
    }  // 扩容检查结束。
    Node* node = free_list_;  // 取出空闲链表头节点。
    free_list_ = free_list_->next;  // 头指针后移到下一个空闲块。
    return node;  // 返回可用内存块地址。
  }  // allocate 结束。

  void deallocate(void* pointer) {  // 把一个块归还给分配器。
    if (pointer == nullptr) {  // 空指针释放是无操作。
      return;  // 直接返回。
    }  // 空指针检查结束。
    std::lock_guard<PolicyFixedAllocator> lock(*this);  // 通过线程策略保护空闲链表修改。
    auto* node = static_cast<Node*>(pointer);  // 把用户块重新解释为空闲链表节点。
    node->next = free_list_;  // 将归还块链接到当前链表头之前。
    free_list_ = node;  // 更新空闲链表头。
  }  // deallocate 结束。

  std::size_t capacity() const noexcept {  // 查询当前总块容量。
    return capacity_;  // 返回已经扩容出来的块数量。
  }  // capacity 结束。

  void lock() { threading_.lock(); }  // 供 lock_guard 调用，转发到线程策略。
  void unlock() { threading_.unlock(); }  // 供 lock_guard 调用，转发到线程策略。

 private:  // 以下成员维护空闲链表、策略对象和 chunk 生命周期。
  struct Node {  // 空闲块被挂入链表时使用的节点视图。
    Node* next;  // 指向下一个空闲块。
  };  // Node 定义结束。

  void grow() {  // 按扩容策略新增一批固定大小块。
    const std::size_t next_capacity = growth_.next(capacity_);  // 计算扩容后的目标容量。
    const std::size_t add_count = next_capacity - capacity_;  // 计算这次需要新增的块数量。
    auto chunk = std::make_unique<char[]>(add_count * block_size_);  // 分配一整段连续字节存储。

    for (std::size_t index = 0; index < add_count; ++index) {  // 遍历这次新增的每个块。
      auto* node = reinterpret_cast<Node*>(chunk.get() + index * block_size_);  // 计算块地址并当作 Node 使用。
      node->next = free_list_;  // 把新块插到空闲链表头。
      free_list_ = node;  // 更新空闲链表头为新块。
    }  // 新块挂链结束。

    chunks_.push_back(std::move(chunk));  // 保存 chunk 所有权，保证块内存生命周期覆盖分配器。
    capacity_ = next_capacity;  // 更新总容量。
  }  // grow 结束。

  std::size_t block_size_;  // 每个固定块的字节大小。
  std::size_t capacity_ = 0;  // 当前总块数量。
  Node* free_list_ = nullptr;  // 空闲块链表头指针。
  ThreadingPolicy threading_;  // 线程同步策略对象。
  GrowthPolicy growth_;  // 扩容策略对象。
  std::vector<std::unique_ptr<char[]>> chunks_;  // 保存所有已分配 chunk，负责释放内存。
};  // PolicyFixedAllocator 定义结束。

void run_policy_examples() {  // 运行策略式固定块分配器示例。
  std::cout << "== policy-based allocator examples ==\n";  // 输出当前示例分组标题。

  PolicyFixedAllocator<SingleThreaded, DoubleGrowth> single(sizeof(int));  // 组合单线程策略和倍增扩容策略。
  void* first = single.allocate();  // 分配第一个 int 大小块。
  void* second = single.allocate();  // 分配第二个 int 大小块。
  expect(first != nullptr && second != nullptr, "single-threaded policy allocator allocates blocks");  // 验证分配成功。
  single.deallocate(first);  // 归还第一个块。
  single.deallocate(second);  // 归还第二个块。
  expect(single.capacity() >= 8, "double growth policy expands initial capacity");  // 验证初次扩容至少到 8。

  PolicyFixedAllocator<MultiThreaded, LinearGrowth> multi(sizeof(double));  // 组合多线程策略和线性扩容策略。
  void* block = multi.allocate();  // 分配一个 double 大小块。
  expect(block != nullptr, "multi-threaded policy allocator allocates blocks");  // 验证多线程策略版本分配成功。
  multi.deallocate(block);  // 归还该块。
  expect(multi.capacity() == 8, "linear growth policy expands by a fixed step first");  // 验证线性策略初次扩容为 8。
}  // policy allocator 示例结束。

template <typename T>  // T 是待判断是否可打印的类型。
concept Printable = requires(std::ostream& os, T value) {  // Printable 要求 T 能写入 ostream。
  os << value;  // 表达式合法则说明 T 支持流式输出。
};  // Printable concept 定义结束。

template <typename T>  // T 是待判断是否像容器的类型。
concept Container = requires(T container) {  // Container 要求具备基本容器接口。
  container.begin();  // 要求可以取得起始迭代器。
  container.end();  // 要求可以取得结束迭代器。
  container.size();  // 要求可以查询元素数量。
  typename T::value_type;  // 要求类型内部暴露 value_type。
};  // Container concept 定义结束。

template <typename F, typename... Args>  // F 是可调用对象类型，Args 是调用参数类型包。
concept CallableTask = std::invocable<F, Args...>;  // CallableTask 复用标准 invocable 判断任务是否可调用。

template <std::integral T>  // T 被约束为标准整数类型。
T double_it(T value) {  // concepts 版本的整数翻倍函数。
  return value * 2;  // 返回输入值的两倍。
}  // double_it 结束。

template <Printable T>  // T 必须满足 Printable concept。
std::string to_printable_string(const T& value) {  // 把可打印对象转换为字符串。
  std::ostringstream output;  // 创建字符串输出流。
  output << value;  // 把值写入输出流。
  return output.str();  // 返回输出流中的字符串。
}  // to_printable_string 结束。

template <Container C>  // C 必须满足 Container concept。
std::size_t container_size(const C& container) {  // 返回容器大小。
  return container.size();  // 调用 concept 保证存在的 size() 接口。
}  // container_size 结束。

class MiniThreadPool {  // 最小线程池外形示例，用同步执行模拟 submit 接口。
 public:  // 暴露任务提交接口。
  template <typename F, typename... Args>  // F 是任务函数类型，Args 是绑定参数类型包。
    requires CallableTask<F, Args...>  // 只有 F 能用 Args... 调用时 submit 才可用。
  auto submit(F&& function, Args&&... args) {  // 提交一个任务并返回 future。
    using Ret = std::invoke_result_t<F, Args...>;  // 推导任务调用后的返回类型。
    std::packaged_task<Ret()> task(  // 把任务包装成无参 packaged_task。
        [func = std::forward<F>(function), ... bound_args = std::forward<Args>(args)]() mutable {  // C++20 pack init-capture 绑定函数和参数。
          return std::invoke(std::move(func), std::move(bound_args)...);  // 调用绑定后的任务并返回结果。
        });  // packaged_task 构造结束。
    auto future = task.get_future();  // 取得用于接收结果的 future。
    task();  // 示例里同步执行任务，不启动真实工作线程。
    return future;  // 返回已经可获取结果的 future。
  }  // submit 结束。
};  // MiniThreadPool 定义结束。

void run_concepts_examples() {  // 运行 C++20 concepts 约束示例。
  std::cout << "== concepts examples ==\n";  // 输出当前示例分组标题。

  static_assert(Printable<int>);  // 编译期验证 int 可打印。
  static_assert(Container<std::vector<int>>);  // 编译期验证 vector 满足容器形状。
  static_assert(!Container<int>);  // 编译期验证 int 不满足容器形状。
  static_assert(CallableTask<decltype([](int value) { return value + 1; }), int>);  // 编译期验证 lambda 可用 int 调用。

  expect(double_it(21) == 42, "concept-constrained double_it accepts integral values");  // 运行期验证 integral 约束函数。
  expect(to_printable_string(3.5) == "3.5", "Printable concept accepts streamable values");  // 运行期验证 Printable 转字符串。

  std::vector<int> values{1, 2, 3};  // 构造 vector，用于 Container 示例。
  expect(container_size(values) == 3, "Container concept accepts vector-like values");  // 验证 container_size 能读取 vector 大小。

  MiniThreadPool pool;  // 构造最小线程池示例对象。
  auto future = pool.submit([](int left, int right) { return left + right; }, 20, 22);  // 提交一个可调用任务。
  expect(future.get() == 42, "concept-constrained submit accepts callable tasks");  // 验证 future 中保存任务结果。
}  // concepts 示例结束。

constexpr std::size_t fibonacci(std::size_t n) {  // constexpr 递归计算斐波那契数。
  return n < 2 ? n : fibonacci(n - 1) + fibonacci(n - 2);  // 基例返回 n，递归例返回前两项之和。
}  // fibonacci 结束。

template <std::size_t N>  // N 是数组长度。
constexpr std::array<int, N> constexpr_sort(std::array<int, N> values) {  // 在编译期对数组副本排序。
  for (std::size_t i = 0; i < N; ++i) {  // 外层循环选择当前位置。
    for (std::size_t j = i + 1; j < N; ++j) {  // 内层循环查找后续更小元素。
      if (values[j] < values[i]) {  // 如果后续元素更小则交换。
        const int tmp = values[i];  // 暂存当前位置的值。
        values[i] = values[j];  // 把更小值放到当前位置。
        values[j] = tmp;  // 把原值放到后续位置。
      }  // 交换分支结束。
    }  // 内层循环结束。
  }  // 外层循环结束。
  return values;  // 返回排序后的数组副本。
}  // constexpr_sort 结束。

template <std::size_t Limit>  // Limit 是筛法上界。
constexpr auto prime_sieve() {  // 在编译期生成 0..Limit 的质数标记表。
  std::array<bool, Limit + 1> is_prime{};  // 创建布尔数组，索引表示数字本身。
  is_prime.fill(true);  // 初始假设所有数字都是质数。
  if constexpr (Limit >= 0) {  // 对无符号 Limit 来说该条件恒真，用于保留教学结构。
    is_prime[0] = false;  // 0 不是质数。
  }  // 0 的标记结束。
  if constexpr (Limit >= 1) {  // 上界至少包含 1 时才访问 is_prime[1]。
    is_prime[1] = false;  // 1 不是质数。
  }  // 1 的标记结束。
  for (std::size_t p = 2; p * p <= Limit; ++p) {  // 枚举可能的质因子。
    if (is_prime[p]) {  // 只有当前 p 仍为质数时才筛掉倍数。
      for (std::size_t multiple = p * p; multiple <= Limit; multiple += p) {  // 从 p*p 开始标记 p 的倍数。
        is_prime[multiple] = false;  // p 的倍数不是质数。
      }  // 倍数标记循环结束。
    }  // 当前 p 处理结束。
  }  // 筛法循环结束。
  return is_prime;  // 返回质数标记表。
}  // prime_sieve 结束。

constexpr std::size_t hash(const char* text) {  // constexpr FNV-1a 风格字符串哈希。
  std::size_t value = 1469598103934665603ull;  // 初始化 64 位偏移基准值。
  while (*text != '\0') {  // 遍历直到 C 字符串结尾。
    value ^= static_cast<unsigned char>(*text++);  // 将当前字节混入哈希值并前进指针。
    value *= 1099511628211ull;  // 乘以 FNV prime 扩散位模式。
  }  // 字符遍历结束。
  return value;  // 返回最终哈希值。
}  // hash 结束。

void run_constexpr_examples() {  // 运行 constexpr 编译期计算示例。
  std::cout << "== constexpr examples ==\n";  // 输出当前示例分组标题。

  static_assert(fibonacci(10) == 55);  // 编译期验证斐波那契结果。
  constexpr auto sorted = constexpr_sort(std::array<int, 5>{5, 1, 4, 1, 3});  // 编译期排序数组。
  static_assert(sorted[0] == 1 && sorted[4] == 5);  // 编译期验证排序边界元素。
  constexpr auto primes = prime_sieve<20>();  // 编译期生成 20 以内质数表。
  static_assert(primes[2] && primes[3] && primes[19]);  // 编译期验证若干质数标记。
  static_assert(!primes[1] && !primes[4] && !primes[20]);  // 编译期验证若干非质数标记。
  static_assert(hash("hello") != hash("world"));  // 编译期验证不同字符串哈希不同。

  expect(fibonacci(10) == 55, "constexpr fibonacci result is available at compile time");  // 运行期输出 fibonacci 验证结果。
  expect(sorted[2] == 3, "constexpr sort orders the array");  // 运行期输出排序验证结果。
  expect(primes[19], "constexpr sieve marks 19 as prime");  // 运行期输出质数表验证结果。
  expect(hash("hello") != hash("world"), "constexpr hash distinguishes strings");  // 运行期输出哈希验证结果。
}  // constexpr 示例结束。

void run_month3_integrated_tests() {  // 运行 Month 3 各手写组件的综合验证。
  std::cout << "== Month 3 integrated tests ==\n";  // 输出当前示例分组标题。

  static_assert(traits::is_same<traits::remove_cv_t<const volatile int>, int>::value);  // 编译期验证 remove_cv。
  static_assert(traits::is_same<traits::decay_t<int[3]>, int*>::value);  // 编译期验证数组 decay 为指针。
  static_assert(traits::is_convertible<int, double>::value);  // 编译期验证 int 可转换为 double。
  expect(traits::is_integral<const int>::value, "my_type_traits detects integral cv types");  // 运行期验证 cv 整数类型识别。

  Tuple<int, double, std::string> tuple_value(1, 2.5, "ok");  // 构造手写 Tuple 综合测试值。
  expect(get<0>(tuple_value) == 1, "Tuple get<0> works in integrated tests");  // 验证 Tuple 按下标访问。
  expect(tuple::apply([](int a, double b, const std::string&) { return a + b; }, tuple_value) == 3.5,  // 验证 Tuple 展开调用 lambda。
         "Tuple apply works in integrated tests");  // 断言说明文本。
  std::ostringstream tuple_output;  // 创建输出流收集 Tuple 打印结果。
  tuple::print(tuple_output, tuple_value);  // 调用手写 Tuple 打印函数。
  expect(tuple_output.str() == "(1, 2.5, ok)", "Tuple print works in integrated tests");  // 验证 Tuple 打印格式。

  Optional<std::string> optional_value("value");  // 构造保存字符串的 Optional。
  expect(optional_value.has_value(), "Optional stores values in integrated tests");  // 验证 Optional 有值。
  expect(optional_value.value_or("fallback") == "value", "Optional value_or returns stored value");  // 验证有值时返回内部值。
  optional_value.reset();  // 清空 Optional。
  expect(!optional_value, "Optional reset clears value");  // 验证 reset 后为空。

  Variant<int, std::string> variant_value(7);  // 构造保存 int 的 Variant。
  expect(variant_value.index() == 0, "Variant stores int in integrated tests");  // 验证当前 alternative 下标为 int。
  variant_value = std::string("seven");  // 切换 Variant 为 string alternative。
  expect(variant_value.index() == 1, "Variant switches alternatives in integrated tests");  // 验证下标已切换为 string。
  expect(visit([](const auto& item) { return to_printable_string(item); }, variant_value) == "seven",  // 验证 visit 能访问当前 string。
         "Variant visit works in integrated tests");  // 断言说明文本。

  Function<int(int, int)> function_value = [](int left, int right) { return left - right; };  // 构造保存 lambda 的 Function。
  expect(function_value(10, 3) == 7, "Function stores lambda in integrated tests");  // 验证 Function 调用 lambda。
  Function<int(int, int)> copied = function_value;  // 拷贝 Function，触发擦除对象 clone。
  expect(copied(12, 5) == 7, "Function copy works in integrated tests");  // 验证拷贝后的 Function 可调用。
  Function<int(int, int)> pointer = [](int left, int right) { return left + right; };  // 构造另一个 callable 示例。
  expect(pointer(3, 4) == 7, "Function stores another callable in integrated tests");  // 验证另一个 callable 调用结果。
}  // Month 3 综合测试结束。

}  // namespace demo  // 示例命名空间结束。

int main() {  // 程序入口，按主题依次运行高级模板示例。
  demo::run_crtp_benchmark();  // 运行 CRTP benchmark 示例。
  std::cout << "----\n";  // 输出分隔线，便于阅读不同示例分组。

  demo::run_policy_examples();  // 运行策略式分配器示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_concepts_examples();  // 运行 concepts 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_constexpr_examples();  // 运行 constexpr 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_month3_integrated_tests();  // 运行 Month 3 综合测试。
  return 0;  // 返回 0 表示所有示例通过。
}  // main 函数结束。