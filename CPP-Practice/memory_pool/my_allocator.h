#pragma once

#include "pool_allocator.h"

#include <cstddef>
#include <limits>
#include <type_traits>

// 所有 MyAllocator 实例共享这一个进程级 PoolAllocator，故 is_always_equal 成立。
// 有意 new 且不 delete：避免与容器析构的销毁顺序竞争（Meyers singleton 会在退出时被析构）。
inline PoolAllocator& global_pool_allocator() {
    static PoolAllocator* allocator = new PoolAllocator();
    return *allocator;
}

template <typename T>
class MyAllocator {
 public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;  // 无状态：拿谁的 allocator 释放都一样。

    template <typename U>
    struct rebind {
        using other = MyAllocator<U>;
    };

    MyAllocator() noexcept = default;

    // 允许 rebind 出的其它类型 allocator 隐式转换过来（容器内部会 rebind）。
    template <typename U>
    MyAllocator(const MyAllocator<U>&) noexcept {}

    T* allocate(size_type n) {
        // 底层池按 max_align_t 对齐，无法满足过度对齐类型。
        static_assert(alignof(T) <= alignof(std::max_align_t),
                      "MyAllocator only supports alignments up to std::max_align_t");

        if (n == 0) {
            return nullptr;
        }

        if (n > max_size()) {
            throw std::bad_array_new_length();  // n * sizeof(T) 会溢出。
        }

        void* memory = global_pool_allocator().allocate(n * sizeof(T));
        return static_cast<T*>(memory);
    }

    // 池的 deallocate 从 header 自行取回大小，故忽略 size 参数。
    void deallocate(T* ptr, size_type) noexcept { global_pool_allocator().deallocate(ptr); }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    // 恒等比较：共享同一全局池，任意两个实例都可互换释放。
    template <typename U>
    bool operator==(const MyAllocator<U>&) const noexcept {
        return true;
    }

    template <typename U>
    bool operator!=(const MyAllocator<U>&) const noexcept {
        return false;
    }
};
