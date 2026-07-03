#include "fixed_allocator.h"  // 引入固定块分配器声明。

#include <algorithm>  // 使用 std::max 计算实际块大小。
#include <cassert>  // 调试模式下使用 assert 校验指针。
#include <cstdlib>  // 使用 std::malloc 和 std::free 管理原始内存。
#include <mutex>  // 使用 std::lock_guard 加锁。
#include <new>  // 使用 std::bad_alloc 报告分配失败。

FixedAllocator::~FixedAllocator() {  // 析构固定块分配器并释放所有 chunk。
    std::lock_guard<std::mutex> lock(mutex_);  // 析构期间保护内部状态。

    // 所有 chunk 都由 allocator 统一持有，析构时整批释放。
    for (const ChunkInfo& chunk : chunks_) {  // 遍历已分配的每个 chunk。
        std::free(chunk.memory);  // 释放 chunk 的原始内存。
    }  // chunk 释放循环结束。
}  // 析构函数结束。

void FixedAllocator::init(std::size_t block_size, std::size_t block_count) {  // 初始化固定块大小和初始块数。
    std::lock_guard<std::mutex> lock(mutex_);  // 初始化过程持锁以保护状态重置。

    for (const ChunkInfo& chunk : chunks_) {  // 遍历旧 chunk。
        std::free(chunk.memory);  // 释放重新初始化前的旧内存。
    }  // 旧 chunk 释放循环结束。
    chunks_.clear();  // 清空 chunk 元信息。

    free_list_ = nullptr;  // 重置空闲链表头。
    used_count_ = 0;  // 重置已使用块计数。
    total_block_count_ = 0;  // 重置总块计数。

    // block 至少要能放下一个 next 指针，才能串成 free list。
    block_size_ = std::max(block_size, sizeof(void*));  // 保证空闲块可存放 next 指针。
    initial_block_count_ = block_count;  // 保存初始块数。
    next_expand_count_ = block_count;  // 下一次扩容先使用初始块数。

    if (initial_block_count_ == 0) {  // 初始块数为零时保持空分配器。
        return;  // 不分配任何 chunk。
    }  // 初始块数检查结束。

    expand();  // 立即创建首个 chunk。
}  // init 结束。

void FixedAllocator::expand() {  // 扩容并补充空闲块。
    if (next_expand_count_ == 0) {  // 没有扩容数量时直接返回。
        return;  // 保持当前状态不变。
    }  // 扩容数量检查结束。

    void* new_chunk = std::malloc(block_size_ * next_expand_count_);  // 申请一整段连续块内存。
    if (new_chunk == nullptr) {  // 检查 malloc 是否失败。
        throw std::bad_alloc();  // 用标准异常报告内存不足。
    }  // malloc 检查结束。

    // 新 chunk 内部先连成一条单独的 free list。
    for (std::size_t i = 0; i + 1 < next_expand_count_; ++i) {  // 遍历除最后一块外的所有新块。
        void* current = static_cast<char*>(new_chunk) + i * block_size_;  // 计算当前块地址。
        void* next = static_cast<char*>(new_chunk) + (i + 1) * block_size_;  // 计算下一块地址。
        *reinterpret_cast<void**>(current) = next;  // 把当前空闲块指向下一块。
    }  // 新 chunk 内部链表构建结束。

    // 再把新 chunk 的尾节点挂到旧 free list 头上，完成拼接。
    void* last = static_cast<char*>(new_chunk) + (next_expand_count_ - 1) * block_size_;  // 计算新 chunk 最后一块地址。
    *reinterpret_cast<void**>(last) = free_list_;  // 让尾块连接旧空闲链表。
    free_list_ = new_chunk;  // 新 chunk 首块成为新的空闲链表头。

    chunks_.push_back(ChunkInfo{new_chunk, next_expand_count_});  // 记录新 chunk 供析构释放。
    total_block_count_ += next_expand_count_;  // 累加总块数量。
    next_expand_count_ *= 2;  // 下次扩容按两倍增长。
}  // expand 结束。

void* FixedAllocator::allocate_locked() {  // 在已持锁状态下分配一个块。
    if (free_list_ == nullptr) {  // 空闲链表为空时需要扩容。
        expand();  // 尝试补充新块。
        if (free_list_ == nullptr) {  // 扩容后仍无可用块。
            return nullptr;  // 返回空指针表示无法分配。
        }  // 扩容结果检查结束。
    }  // 空闲链表检查结束。

    // 头删 free list，O(1) 取出一个可用 block。
    void* block = free_list_;  // 取出当前链表头作为分配结果。
    free_list_ = *reinterpret_cast<void**>(block);  // 链表头后移到下一空闲块。
    ++used_count_;  // 增加已使用块计数。
    return block;  // 返回分配到的块地址。
}  // allocate_locked 结束。

#ifndef NDEBUG  // 调试构建启用归还指针校验。
void FixedAllocator::assert_pointer_in_range_locked(void* ptr) const {  // 校验指针来自当前分配器。
    char* current = static_cast<char*>(ptr);  // 把待校验指针转为字节地址。
    bool in_range = false;  // 标记指针是否落在任一 chunk 内。

    // 只接受来自当前 allocator 管理区间内、且按 block_size 对齐的指针。
    for (const ChunkInfo& chunk : chunks_) {  // 遍历所有 chunk 范围。
        char* begin = static_cast<char*>(chunk.memory);  // 计算 chunk 起始地址。
        char* end = begin + block_size_ * chunk.block_count;  // 计算 chunk 结束地址。
        if (current >= begin && current < end) {  // 判断指针是否落在当前 chunk 内。
            assert((current - begin) % block_size_ == 0);  // 校验指针正好对齐到块边界。
            in_range = true;  // 记录校验通过。
            break;  // 找到所属 chunk 后停止遍历。
        }  // 当前 chunk 命中分支结束。
    }  // chunk 范围遍历结束。
    assert(in_range);  // 确认指针属于本分配器。
}  // assert_pointer_in_range_locked 结束。
#endif  // 调试指针校验实现结束。

void FixedAllocator::deallocate_locked(void* ptr) {  // 在已持锁状态下归还一个块。
#ifndef NDEBUG  // 调试构建启用归还指针校验。
    assert_pointer_in_range_locked(ptr);  // 检查 ptr 是否来自本分配器。
#endif  // 调试指针校验结束。

    // 头插回 free list，地址可立即被后续分配复用。
    *reinterpret_cast<void**>(ptr) = free_list_;  // 让归还块指向旧链表头。
    free_list_ = ptr;  // 归还块成为新的空闲链表头。

    if (used_count_ > 0) {  // 防御性检查避免计数下溢。
        --used_count_;  // 减少已使用块计数。
    }  // 计数更新结束。
}  // deallocate_locked 结束。

void* FixedAllocator::allocate() {  // 对外分配单个块。
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护空闲链表。
    return allocate_locked();  // 复用已持锁分配逻辑。
}  // allocate 结束。

std::size_t FixedAllocator::allocate_batch(void** blocks, std::size_t count) {  // 批量分配多个块。
    if (blocks == nullptr || count == 0) {  // 空输出数组或零数量无需处理。
        return 0;  // 返回实际分配数量为零。
    }  // 参数检查结束。

    std::lock_guard<std::mutex> lock(mutex_);  // 一次持锁完成批量分配。
    std::size_t allocated = 0;  // 记录实际分配到的块数。
    for (; allocated < count; ++allocated) {  // 逐个取块直到达到请求数量。
        void* block = allocate_locked();  // 从内部空闲链表取一个块。
        if (block == nullptr) {  // 分配失败时停止批量操作。
            break;  // 保留已成功分配的前缀结果。
        }  // 分配失败检查结束。
        blocks[allocated] = block;  // 写入输出数组。
    }  // 批量分配循环结束。
    return allocated;  // 返回实际分配数量。
}  // allocate_batch 结束。

void FixedAllocator::deallocate(void* ptr) {  // 对外归还单个块。
    if (ptr == nullptr) {  // 空指针无需释放。
        return;  // 直接返回保持释放语义宽容。
    }  // 空指针检查结束。

    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护空闲链表。
    deallocate_locked(ptr);  // 复用已持锁归还逻辑。
}  // deallocate 结束。

void FixedAllocator::deallocate_batch(void** blocks, std::size_t count) {  // 批量归还多个块。
    if (blocks == nullptr || count == 0) {  // 空数组或零数量无需处理。
        return;  // 直接返回。
    }  // 参数检查结束。

    std::lock_guard<std::mutex> lock(mutex_);  // 一次持锁完成批量归还。
    for (std::size_t index = 0; index < count; ++index) {  // 遍历输入块数组。
        if (blocks[index] != nullptr) {  // 跳过空指针项。
            deallocate_locked(blocks[index]);  // 归还当前非空块。
        }  // 非空块处理结束。
    }  // 批量归还循环结束。
}  // deallocate_batch 结束。

FixedAllocator::Stats FixedAllocator::stats() const {  // 读取当前统计信息。
    std::lock_guard<std::mutex> lock(mutex_);  // 加锁保证统计快照一致。
    return Stats{total_block_count_, used_count_, chunks_.size()};  // 返回总块、已用块和 chunk 数。
}  // stats 结束。