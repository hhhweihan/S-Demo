#pragma once  // 防止头文件被重复包含。

#include "pool_allocator.h"  // 复用底层 PoolAllocator。

#include <cstddef>  // 使用 std::size_t 和 std::ptrdiff_t。
#include <limits>  // 使用 std::numeric_limits 计算最大元素数。
#include <type_traits>  // 使用 allocator traits 相关类型标记。

// STL allocator 适配层统一复用同一个底层 PoolAllocator。
inline PoolAllocator& global_pool_allocator() {  // 返回进程内共享的池分配器。
    static PoolAllocator* allocator = new PoolAllocator();  // 延迟创建并有意保持到进程结束。
    return *allocator;  // 返回共享分配器引用。
}  // global_pool_allocator 结束。

template <typename T>  // 为任意元素类型 T 提供 STL allocator 适配。
class MyAllocator {  // 标准容器可使用的池化分配器。
public:  // 暴露 allocator 必需类型和接口。
    using value_type = T;  // 声明容器元素类型。
    using size_type = std::size_t;  // 声明元素数量类型。
    using difference_type = std::ptrdiff_t;  // 声明指针差值类型。
    using propagate_on_container_move_assignment = std::true_type;  // 允许移动赋值传播 allocator。
    using is_always_equal = std::true_type;  // 所有实例共享同一底层池，视为等价。

    template <typename U>  // 为其他元素类型生成同类 allocator。
    struct rebind {  // 兼容旧式 allocator rebind 机制。
        using other = MyAllocator<U>;  // 指向 U 类型对应的 MyAllocator。
    };  // rebind 定义结束。

    MyAllocator() noexcept = default;  // 默认构造无状态 allocator。

    template <typename U>  // 允许从其他元素类型的 allocator 转换构造。
    MyAllocator(const MyAllocator<U>&) noexcept {  // 转换构造保持无状态语义。
    }  // 转换构造结束。

    T* allocate(size_type n) {  // 为 n 个 T 分配连续存储。
        static_assert(alignof(T) <= alignof(std::max_align_t),  // 检查 T 的对齐不超过通用最大对齐。
                      "MyAllocator only supports alignments up to std::max_align_t");  // 限制为池分配器支持的通用对齐。

        if (n == 0) {  // 零元素分配不需要内存。
            return nullptr;  // 返回空指针符合本实现约定。
        }  // 零元素检查结束。

        if (n > max_size()) {  // 检查元素数量是否会造成字节数溢出。
            throw std::bad_array_new_length();  // 抛出数组分配长度异常。
        }  // 最大数量检查结束。

        // vector 这类容器会一次申请 n 个连续 T，因此这里按字节总量转发。
        void* memory = global_pool_allocator().allocate(n * sizeof(T));  // 从共享池申请总字节数。
        return static_cast<T*>(memory);  // 转回 T 指针给容器使用。
    }  // allocate 结束。

    void deallocate(T* ptr, size_type) noexcept {  // 释放先前分配的连续存储。
        global_pool_allocator().deallocate(ptr);  // 归还给共享池分配器。
    }  // deallocate 结束。

    size_type max_size() const noexcept {  // 返回理论上最多可分配的元素个数。
        return std::numeric_limits<size_type>::max() / sizeof(T);  // 用字节上限除以单元素大小。
    }  // max_size 结束。

    template <typename U>  // 比较不同元素类型的 allocator。
    bool operator==(const MyAllocator<U>&) const noexcept {  // 所有实例都等价。
        return true;  // 共享同一全局池，恒等。
    }  // operator== 结束。

    template <typename U>  // 比较不同元素类型的 allocator。
    bool operator!=(const MyAllocator<U>&) const noexcept {  // 所有实例都不应视为不等。
        return false;  // 与恒等语义对应。
    }  // operator!= 结束。
};  // MyAllocator 定义结束。