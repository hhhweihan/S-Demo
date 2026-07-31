#include "smart_ptr.h"

#include "../memory_pool/fixed_allocator.h"
#include "../memory_pool/pool_allocator.h"

#include <array>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr int kIterations = 1'000'000;
constexpr std::array<std::size_t, 8> kMixedSizes = {8, 16, 24, 32, 64, 128, 256, 512};

struct BenchResult {
    std::string name;
    double elapsed_ms;
};

struct Payload32 {
    std::array<int, 8> data{};
};

template <typename Fn>
BenchResult run_benchmark(const std::string& name, Fn&& fn) {
    const auto start = std::chrono::high_resolution_clock::now();
    fn();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    return BenchResult{name, elapsed};
}

double run_fixed_allocator_benchmark() {
    volatile int sink = 0;  // volatile 制造副作用，防止整个分配循环被优化掉。
    FixedAllocator allocator;
    allocator.init(32,
                   static_cast<std::size_t>(kIterations));  // 预留满容量，把扩容成本挡在计时外。

    const BenchResult result = run_benchmark("FixedAllocator 32B", [&]() {
        for (int index = 0; index < kIterations; ++index) {
            void* raw = allocator.allocate();
            auto* value = static_cast<int*>(raw);
            *value = index;
            sink += *value;
            allocator.deallocate(raw);
        }
    });

    if (sink == 0) {
        std::cout << "sink=" << sink << "\n";
    }

    return result.elapsed_ms;
}

double run_pool_allocator_benchmark() {
    volatile std::size_t sink = 0;
    PoolAllocator allocator;
    std::mt19937 rng(20260422);  // 固定种子，尺寸序列可复现。
    std::uniform_int_distribution<int> dist(0, static_cast<int>(kMixedSizes.size() - 1));

    const BenchResult result = run_benchmark("PoolAllocator mixed", [&]() {
        for (int index = 0; index < kIterations; ++index) {
            // 随机尺寸模拟真实混合负载，覆盖多个 size class。
            const std::size_t size = kMixedSizes[static_cast<std::size_t>(dist(rng))];
            void* raw = allocator.allocate(size);
            auto* bytes = static_cast<unsigned char*>(raw);
            bytes[0] = static_cast<unsigned char>(size);
            sink += bytes[0];
            allocator.deallocate(raw);
        }
    });

    allocator.flush_current_thread_cache();  // 收尾清 TLS cache，避免析构顺序影响后续测量。

    if (sink == 0) {
        std::cout << "sink=" << sink << "\n";
    }

    return result.elapsed_ms;
}

double run_shared_ptr_benchmark() {
    volatile int sink = 0;

    const BenchResult result = run_benchmark("SharedPtr create/destroy", [&]() {
        for (int index = 0; index < kIterations; ++index) {
            SharedPtr<Payload32> ptr(new Payload32{});  // 每轮构造+析构，测原子引用计数开销。
            ptr->data[0] = index;
            sink += ptr->data[0];
        }
    });

    if (sink == 0) {
        std::cout << "sink=" << sink << "\n";
    }

    return result.elapsed_ms;
}

double run_unique_ptr_benchmark() {
    volatile int sink = 0;

    const BenchResult result = run_benchmark("UniquePtr create/destroy", [&]() {
        for (int index = 0; index < kIterations; ++index) {
            UniquePtr<Payload32> ptr(new Payload32{});  // 与 SharedPtr 对照：无引用计数的基线。
            ptr->data[0] = index;
            sink += ptr->data[0];
        }
    });

    if (sink == 0) {
        std::cout << "sink=" << sink << "\n";
    }

    return result.elapsed_ms;
}

}  // namespace

int main() {
    const double fixed_allocator_ms = run_fixed_allocator_benchmark();
    const double pool_allocator_ms = run_pool_allocator_benchmark();
    const double shared_ptr_ms = run_shared_ptr_benchmark();
    const double unique_ptr_ms = run_unique_ptr_benchmark();

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Month 1 benchmark summary\n";
    std::cout << "FixedAllocator 1000000 ops: " << fixed_allocator_ms << " ms\n";
    std::cout << "PoolAllocator mixed 1000000 ops: " << pool_allocator_ms << " ms\n";
    std::cout << "SharedPtr 1000000 ops: " << shared_ptr_ms << " ms\n";
    std::cout << "UniquePtr 1000000 ops: " << unique_ptr_ms << " ms\n";
    return 0;
}
