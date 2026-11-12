#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <cstddef>
#include "templates/my_function.h"
#include "templates/my_optional.h"
#include "templates/my_tuple.h"
#include "templates/my_type_traits.h"
#include "templates/my_variant.h"

namespace demo {

using erasure::Function;
using erasure::Optional;
using erasure::Variant;
using erasure::visit;
using tuple::get;
using tuple::Tuple;

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

// CRTP：基类经模板参数拿到派生类型，把接口调用在编译期解析成对 Derived 的直接调用，无虚表开销。
template <typename Derived>
class ShapeCRTP {
 public:
    double area() const {
        return static_cast<const Derived*>(this)->area_impl();  // 静态派发，非虚调用。
    }
};

class CircleCRTP : public ShapeCRTP<CircleCRTP> {
 public:
    explicit CircleCRTP(double radius) : radius_(radius) {}

    double area_impl() const { return 3.14159265358979323846 * radius_ * radius_; }

 private:
    double radius_;
};

// 对照组：传统虚函数动态多态，area 每次经虚表分发。
class ShapeVirtual {
 public:
    virtual ~ShapeVirtual() = default;
    virtual double area() const = 0;
};

class CircleVirtual final : public ShapeVirtual {
 public:
    explicit CircleVirtual(double radius) : radius_(radius) {}

    double area() const override { return 3.14159265358979323846 * radius_ * radius_; }

 private:
    double radius_;
};

template <typename F>
long long measure_ns(F&& function) {
    const auto start = std::chrono::steady_clock::now();
    volatile double sink = function();  // volatile 阻止优化器把整个计算/循环判定为无副作用而删掉。
    (void)sink;
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

void run_crtp_benchmark() {
    std::cout << "== CRTP benchmark ==\n";

    constexpr int iterations = 10'000'000;
    CircleCRTP crtp(3.0);
    CircleVirtual virtual_circle(3.0);
    const ShapeVirtual& virtual_shape = virtual_circle;  // 经基类引用才会触发虚调用。

    double crtp_sum = 0.0;
    const auto crtp_ns = measure_ns([&] {
        double sum = 0.0;
        for (int index = 0; index < iterations; ++index) {
            sum += crtp.area();
        }
        crtp_sum = sum;
        return sum;
    });

    double virtual_sum = 0.0;
    const auto virtual_ns = measure_ns([&] {
        double sum = 0.0;
        for (int index = 0; index < iterations; ++index) {
            sum += virtual_shape.area();
        }
        virtual_sum = sum;
        return sum;
    });

    std::cout << "CRTP area loop: " << crtp_ns / 1'000'000.0 << " ms\n";
    std::cout << "virtual area loop: " << virtual_ns / 1'000'000.0 << " ms\n";
    expect(std::abs(crtp_sum - virtual_sum) < 0.001,
           "CRTP and virtual versions compute the same total area");
}

// 以下四个策略类是 policy-based design 的构件：以空的 lock/unlock 满足 BasicLockable，
// 让分配器在编译期组合"要不要加锁""怎么扩容"，零运行时分支。
struct SingleThreaded {
    void lock() {}
    void unlock() {}
};

struct MultiThreaded {
    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }

    std::mutex mutex;
};

struct DoubleGrowth {
    std::size_t next(std::size_t current) const { return current == 0 ? 8 : current * 2; }
};

struct LinearGrowth {
    std::size_t next(std::size_t current) const { return current + 8; }
};

template <typename ThreadingPolicy = SingleThreaded, typename GrowthPolicy = DoubleGrowth>
class PolicyFixedAllocator {
 public:
    // 块大小至少 sizeof(Node)：空闲块被复用为侵入式链表节点，必须放得下一个 next 指针。
    explicit PolicyFixedAllocator(std::size_t block_size)
        : block_size_(std::max(block_size, sizeof(Node))) {}

    void* allocate() {
        std::lock_guard<PolicyFixedAllocator> lock(*this);  // 经本对象的 lock/unlock 套用线程策略。
        if (free_list_ == nullptr) {
            grow();
        }
        Node* node = free_list_;
        free_list_ = free_list_->next;
        return node;
    }

    void deallocate(void* pointer) {
        if (pointer == nullptr) {
            return;
        }
        std::lock_guard<PolicyFixedAllocator> lock(*this);
        auto* node = static_cast<Node*>(pointer);  // 归还的用户块重新当作链表节点挂回。
        node->next = free_list_;
        free_list_ = node;
    }

    std::size_t capacity() const noexcept { return capacity_; }

    void lock() { threading_.lock(); }
    void unlock() { threading_.unlock(); }

 private:
    // 空闲块与用户数据共用同一块内存：占用时是用户数据，空闲时被解释成 Node（故需 sizeof(Node)
    // 下限）。
    struct Node {
        Node* next;
    };

    void grow() {
        const std::size_t next_capacity = growth_.next(capacity_);
        const std::size_t add_count = next_capacity - capacity_;
        auto chunk = std::make_unique<char[]>(add_count * block_size_);

        for (std::size_t index = 0; index < add_count; ++index) {
            auto* node = reinterpret_cast<Node*>(chunk.get() + index * block_size_);
            node->next = free_list_;
            free_list_ = node;
        }

        chunks_.push_back(
            std::move(chunk));  // 持有 chunk 所有权，保证块内存活得比分配器发出的指针久。
        capacity_ = next_capacity;
    }

    std::size_t block_size_;
    std::size_t capacity_ = 0;
    Node* free_list_ = nullptr;
    ThreadingPolicy threading_;
    GrowthPolicy growth_;
    std::vector<std::unique_ptr<char[]>> chunks_;
};

void run_policy_examples() {
    std::cout << "== policy-based allocator examples ==\n";

    PolicyFixedAllocator<SingleThreaded, DoubleGrowth> single(sizeof(int));
    void* first = single.allocate();
    void* second = single.allocate();
    expect(first != nullptr && second != nullptr,
           "single-threaded policy allocator allocates blocks");
    single.deallocate(first);
    single.deallocate(second);
    expect(single.capacity() >= 8, "double growth policy expands initial capacity");

    PolicyFixedAllocator<MultiThreaded, LinearGrowth> multi(sizeof(double));
    void* block = multi.allocate();
    expect(block != nullptr, "multi-threaded policy allocator allocates blocks");
    multi.deallocate(block);
    expect(multi.capacity() == 8, "linear growth policy expands by a fixed step first");
}

// concept 用 requires 描述类型必须支持的表达式，作为模板形参的编译期约束。
template <typename T>
concept Printable = requires(std::ostream& os, T value) { os << value; };

template <typename T>
concept Container = requires(T container) {
    container.begin();
    container.end();
    container.size();
    typename T::value_type;
};

template <typename F, typename... Args>
concept CallableTask = std::invocable<F, Args...>;

template <std::integral T>
T double_it(T value) {
    return value * 2;
}

template <Printable T>
std::string to_printable_string(const T& value) {
    std::ostringstream output;
    output << value;
    return output.str();
}

template <Container C>
std::size_t container_size(const C& container) {
    return container.size();
}

// 最小线程池外形：只演示 submit 的模板/future 机制，任务实际是同步执行的。
class MiniThreadPool {
 public:
    template <typename F, typename... Args>
        requires CallableTask<F, Args...>
    auto submit(F&& function, Args&&... args) {
        using Ret = std::invoke_result_t<F, Args...>;
        std::packaged_task<Ret()> task(
            // C++20 pack init-capture：把函数和各参数按值捕获进无参 task，规避悬垂引用。
            [func = std::forward<F>(function),
             ... bound_args = std::forward<Args>(args)]() mutable {
                return std::invoke(std::move(func), std::move(bound_args)...);
            });
        auto future = task.get_future();
        task();  // 同步跑完，不起真实工作线程。
        return future;
    }
};

void run_concepts_examples() {
    std::cout << "== concepts examples ==\n";

    static_assert(Printable<int>);
    static_assert(Container<std::vector<int>>);
    static_assert(!Container<int>);
    static_assert(CallableTask<decltype([](int value) { return value + 1; }), int>);

    expect(double_it(21) == 42, "concept-constrained double_it accepts integral values");
    expect(to_printable_string(3.5) == "3.5", "Printable concept accepts streamable values");

    std::vector<int> values{1, 2, 3};
    expect(container_size(values) == 3, "Container concept accepts vector-like values");

    MiniThreadPool pool;
    auto future = pool.submit([](int left, int right) { return left + right; }, 20, 22);
    expect(future.get() == 42, "concept-constrained submit accepts callable tasks");
}

constexpr std::size_t fibonacci(std::size_t n) {
    return n < 2 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

template <std::size_t N>
constexpr std::array<int, N> constexpr_sort(std::array<int, N> values) {
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            if (values[j] < values[i]) {
                const int tmp = values[i];
                values[i] = values[j];
                values[j] = tmp;
            }
        }
    }
    return values;
}

template <std::size_t Limit>
constexpr auto prime_sieve() {
    std::array<bool, Limit + 1> is_prime{};
    is_prime.fill(true);
    // 无符号 Limit 下 >=0 恒真；保留这个 if constexpr 只为对齐 >=1 分支的教学结构。
    if constexpr (Limit >= 0) {
        is_prime[0] = false;
    }
    if constexpr (Limit >= 1) {  // 上界含 1 时才有 is_prime[1] 可写。
        is_prime[1] = false;
    }
    for (std::size_t p = 2; p * p <= Limit; ++p) {
        if (is_prime[p]) {
            for (std::size_t multiple = p * p; multiple <= Limit;
                 multiple += p) {  // 从 p*p 起筛，更小的倍数已被更小质因子处理。
                is_prime[multiple] = false;
            }
        }
    }
    return is_prime;
}

// constexpr FNV-1a 字符串哈希；两个魔数是 64 位 FNV 的 offset basis 与 prime。
constexpr std::size_t hash(const char* text) {
    std::size_t value = 1469598103934665603ull;
    while (*text != '\0') {
        value ^= static_cast<unsigned char>(*text++);
        value *= 1099511628211ull;
    }
    return value;
}

void run_constexpr_examples() {
    std::cout << "== constexpr examples ==\n";

    static_assert(fibonacci(10) == 55);
    constexpr auto sorted = constexpr_sort(std::array<int, 5>{5, 1, 4, 1, 3});
    static_assert(sorted[0] == 1 && sorted[4] == 5);
    constexpr auto primes = prime_sieve<20>();
    static_assert(primes[2] && primes[3] && primes[19]);
    static_assert(!primes[1] && !primes[4] && !primes[20]);
    static_assert(hash("hello") != hash("world"));

    expect(fibonacci(10) == 55, "constexpr fibonacci result is available at compile time");
    expect(sorted[2] == 3, "constexpr sort orders the array");
    expect(primes[19], "constexpr sieve marks 19 as prime");
    expect(hash("hello") != hash("world"), "constexpr hash distinguishes strings");
}

void run_month3_integrated_tests() {
    std::cout << "== Month 3 integrated tests ==\n";

    static_assert(traits::is_same<traits::remove_cv_t<const volatile int>, int>::value);
    static_assert(traits::is_same<traits::decay_t<int[3]>, int*>::value);
    static_assert(traits::is_convertible<int, double>::value);
    expect(traits::is_integral<const int>::value, "my_type_traits detects integral cv types");

    Tuple<int, double, std::string> tuple_value(1, 2.5, "ok");
    expect(get<0>(tuple_value) == 1, "Tuple get<0> works in integrated tests");
    expect(
        tuple::apply([](int a, double b, const std::string&) { return a + b; }, tuple_value) == 3.5,
        "Tuple apply works in integrated tests");
    std::ostringstream tuple_output;
    tuple::print(tuple_output, tuple_value);
    expect(tuple_output.str() == "(1, 2.5, ok)", "Tuple print works in integrated tests");

    Optional<std::string> optional_value("value");
    expect(optional_value.has_value(), "Optional stores values in integrated tests");
    expect(optional_value.value_or("fallback") == "value",
           "Optional value_or returns stored value");
    optional_value.reset();
    expect(!optional_value, "Optional reset clears value");

    Variant<int, std::string> variant_value(7);
    expect(variant_value.index() == 0, "Variant stores int in integrated tests");
    variant_value = std::string("seven");
    expect(variant_value.index() == 1, "Variant switches alternatives in integrated tests");
    expect(
        visit([](const auto& item) { return to_printable_string(item); }, variant_value) == "seven",
        "Variant visit works in integrated tests");

    Function<int(int, int)> function_value = [](int left, int right) { return left - right; };
    expect(function_value(10, 3) == 7, "Function stores lambda in integrated tests");
    Function<int(int, int)> copied = function_value;
    expect(copied(12, 5) == 7, "Function copy works in integrated tests");
    Function<int(int, int)> pointer = [](int left, int right) { return left + right; };
    expect(pointer(3, 4) == 7, "Function stores another callable in integrated tests");
}

}  // namespace demo

int main() {
    demo::run_crtp_benchmark();
    std::cout << "----\n";

    demo::run_policy_examples();
    std::cout << "----\n";

    demo::run_concepts_examples();
    std::cout << "----\n";

    demo::run_constexpr_examples();
    std::cout << "----\n";

    demo::run_month3_integrated_tests();
    return 0;
}
