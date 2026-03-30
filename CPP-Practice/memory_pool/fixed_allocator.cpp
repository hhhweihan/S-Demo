#include "fixed_allocator.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <new>

FixedAllocator::~FixedAllocator() {
    std::free(chunk_);
}

void FixedAllocator::init(std::size_t block_size, std::size_t block_count) {
    std::free(chunk_);
    chunk_ = nullptr;
    free_list_ = nullptr;
    used_count_ = 0;

    block_size_ = std::max(block_size, sizeof(void*));
    block_count_ = block_count;

    if (block_count_ == 0) {
        return;
    }

    chunk_ = std::malloc(block_size_ * block_count_);
    if (chunk_ == nullptr) {
        throw std::bad_alloc();
    }

    // 初始化时把每个 block 的前 sizeof(void*) 字节写成指向下一个 block。
    for (std::size_t i = 0; i + 1 < block_count_; ++i) {
        void* current = static_cast<char*>(chunk_) + i * block_size_;
        void* next = static_cast<char*>(chunk_) + (i + 1) * block_size_;
        *reinterpret_cast<void**>(current) = next;
    }

    void* last = static_cast<char*>(chunk_) + (block_count_ - 1) * block_size_;
    *reinterpret_cast<void**>(last) = nullptr;
    free_list_ = chunk_;
}

void* FixedAllocator::allocate() {
    if (free_list_ == nullptr) {
        return nullptr;
    }

    void* block = free_list_;
    free_list_ = *reinterpret_cast<void**>(block);
    ++used_count_;
    return block;
}

void FixedAllocator::deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    char* begin = static_cast<char*>(chunk_);
    char* current = static_cast<char*>(ptr);

    assert(begin != nullptr);
    char* end = begin + block_size_ * block_count_;
    assert(current >= begin && current < end);
    assert((current - begin) % block_size_ == 0);

    *reinterpret_cast<void**>(ptr) = free_list_;
    free_list_ = ptr;

    if (used_count_ > 0) {
        --used_count_;
    }
}

FixedAllocator::Stats FixedAllocator::stats() const {
    return Stats{block_count_, used_count_, chunk_ == nullptr ? 0 : 1};
}