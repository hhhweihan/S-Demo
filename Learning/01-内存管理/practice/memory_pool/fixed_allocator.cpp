#include "fixed_allocator.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <mutex>
#include <new>

FixedAllocator::~FixedAllocator() {
    std::lock_guard<std::mutex> lock(mutex_);

    // chunk 由 allocator 独占持有，析构时整批 free；单个 block 不单独归还给系统。
    for (const ChunkInfo& chunk : chunks_) {
        std::free(chunk.memory);
    }
}

void FixedAllocator::init(std::size_t block_size, std::size_t block_count) {
    std::lock_guard<std::mutex> lock(mutex_);

    // init 可重复调用，先释放旧 chunk 再重置全部状态。
    for (const ChunkInfo& chunk : chunks_) {
        std::free(chunk.memory);
    }
    chunks_.clear();

    free_list_ = nullptr;
    used_count_ = 0;
    total_block_count_ = 0;

    // 空闲 block 复用自身首 8 字节串成 free list，故块至少要能放下一个指针。
    block_size_ = std::max(block_size, sizeof(void*));
    initial_block_count_ = block_count;
    next_expand_count_ = block_count;

    if (initial_block_count_ == 0) {
        return;  // block_count 为 0 时保持空分配器，交由首次 allocate 触发扩容。
    }

    expand();
}

void FixedAllocator::expand() {
    if (next_expand_count_ == 0) {
        return;
    }

    void* new_chunk = std::malloc(block_size_ * next_expand_count_);
    if (new_chunk == nullptr) {
        throw std::bad_alloc();
    }

    // 先把新 chunk 内部串成一条 free list。
    for (std::size_t i = 0; i + 1 < next_expand_count_; ++i) {
        void* current = static_cast<char*>(new_chunk) + i * block_size_;
        void* next = static_cast<char*>(new_chunk) + (i + 1) * block_size_;
        *reinterpret_cast<void**>(current) = next;
    }

    // 尾块接上旧 free list，新 chunk 首块成为新表头，完成拼接。
    void* last = static_cast<char*>(new_chunk) + (next_expand_count_ - 1) * block_size_;
    *reinterpret_cast<void**>(last) = free_list_;
    free_list_ = new_chunk;

    chunks_.push_back(ChunkInfo{new_chunk, next_expand_count_});
    total_block_count_ += next_expand_count_;
    next_expand_count_ *= 2;  // 几何增长，摊薄扩容次数。
}

void* FixedAllocator::allocate_locked() {
    if (free_list_ == nullptr) {
        expand();
        if (free_list_ == nullptr) {
            return nullptr;  // 扩容仍拿不到块（如 next_expand_count_ 归零）。
        }
    }

    // 头删 free list，O(1) 取出一个块。
    void* block = free_list_;
    free_list_ = *reinterpret_cast<void**>(block);
    ++used_count_;
    return block;
}

#ifndef NDEBUG
void FixedAllocator::assert_pointer_in_range_locked(void* ptr) const {
    char* current = static_cast<char*>(ptr);
    bool in_range = false;

    // 只接受落在本 allocator 某个 chunk 内、且按 block_size 对齐的指针，拦截误归还。
    for (const ChunkInfo& chunk : chunks_) {
        char* begin = static_cast<char*>(chunk.memory);
        char* end = begin + block_size_ * chunk.block_count;
        if (current >= begin && current < end) {
            assert((current - begin) % block_size_ == 0);
            in_range = true;
            break;
        }
    }
    assert(in_range);
}
#endif

void FixedAllocator::deallocate_locked(void* ptr) {
#ifndef NDEBUG
    assert_pointer_in_range_locked(ptr);
#endif

    // 头插回 free list，地址可立即被后续 allocate 复用。
    *reinterpret_cast<void**>(ptr) = free_list_;
    free_list_ = ptr;

    if (used_count_ > 0) {  // 防御下溢：重复归还不至于把计数绕回极大值。
        --used_count_;
    }
}

void* FixedAllocator::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);
    return allocate_locked();
}

std::size_t FixedAllocator::allocate_batch(void** blocks, std::size_t count) {
    if (blocks == nullptr || count == 0) {
        return 0;
    }

    // 一次持锁完成整批，摊薄锁开销。
    std::lock_guard<std::mutex> lock(mutex_);
    std::size_t allocated = 0;
    for (; allocated < count; ++allocated) {
        void* block = allocate_locked();
        if (block == nullptr) {
            break;  // 池耗尽时返回已成功分配的前缀，而非全部失败。
        }
        blocks[allocated] = block;
    }
    return allocated;
}

void FixedAllocator::deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;  // 对 nullptr 宽容，与 free() 语义一致。
    }

    std::lock_guard<std::mutex> lock(mutex_);
    deallocate_locked(ptr);
}

void FixedAllocator::deallocate_batch(void** blocks, std::size_t count) {
    if (blocks == nullptr || count == 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    for (std::size_t index = 0; index < count; ++index) {
        if (blocks[index] != nullptr) {
            deallocate_locked(blocks[index]);
        }
    }
}

FixedAllocator::Stats FixedAllocator::stats() const {
    std::lock_guard<std::mutex> lock(mutex_);  // 持锁读，保证快照三个字段互相一致。
    return Stats{total_block_count_, used_count_, chunks_.size()};
}
