#pragma once

#include "pool_allocator.h"

#include <cstddef>
#include <limits>
#include <type_traits>

// STL allocator 适配层统一复用同一个底层 PoolAllocator。
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
    using is_always_equal = std::true_type;

    template <typename U>
    struct rebind {
        using other = MyAllocator<U>;
    };

    MyAllocator() noexcept = default;

    template <typename U>
    MyAllocator(const MyAllocator<U>&) noexcept {
    }

    T* allocate(size_type n) {
        static_assert(alignof(T) <= alignof(std::max_align_t),
                      "MyAllocator only supports alignments up to std::max_align_t");

        if (n == 0) {
            return nullptr;
        }

        if (n > max_size()) {
            throw std::bad_array_new_length();
        }

        // vector 这类容器会一次申请 n 个连续 T，因此这里按字节总量转发。
        void* memory = global_pool_allocator().allocate(n * sizeof(T));
        return static_cast<T*>(memory);
    }

    void deallocate(T* ptr, size_type) noexcept {
        global_pool_allocator().deallocate(ptr);
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    template <typename U>
    bool operator==(const MyAllocator<U>&) const noexcept {
        return true;
    }

    template <typename U>
    bool operator!=(const MyAllocator<U>&) const noexcept {
        return false;
    }
};