#include "fixed_allocator.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <new>

FixedAllocator::~FixedAllocator() {
    for (const ChunkInfo& chunk : chunks_) {
        std::free(chunk.memory);
    }
}

void FixedAllocator::init(std::size_t block_size, std::size_t block_count) {
    for (const ChunkInfo& chunk : chunks_) {
        std::free(chunk.memory);
    }
    chunks_.clear();

    free_list_ = nullptr;
    used_count_ = 0;
    total_block_count_ = 0;

    block_size_ = std::max(block_size, sizeof(void*));
    initial_block_count_ = block_count;
    next_expand_count_ = block_count;

    if (initial_block_count_ == 0) {
        return;
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

    // 初始化时把每个 block 的前 sizeof(void*) 字节写成指向下一个 block。
    for (std::size_t i = 0; i + 1 < next_expand_count_; ++i) {
        void* current = static_cast<char*>(new_chunk) + i * block_size_;
        void* next = static_cast<char*>(new_chunk) + (i + 1) * block_size_;
        *reinterpret_cast<void**>(current) = next;
    }

    void* last = static_cast<char*>(new_chunk) + (next_expand_count_ - 1) * block_size_;
    *reinterpret_cast<void**>(last) = free_list_;
    free_list_ = new_chunk;

    chunks_.push_back(ChunkInfo{new_chunk, next_expand_count_});
    total_block_count_ += next_expand_count_;
    next_expand_count_ *= 2;
}

void* FixedAllocator::allocate() {
    if (free_list_ == nullptr) {
        expand();
        if (free_list_ == nullptr) {
            return nullptr;
        }
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

    char* current = static_cast<char*>(ptr);
    bool in_range = false;

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

    *reinterpret_cast<void**>(ptr) = free_list_;
    free_list_ = ptr;

    if (used_count_ > 0) {
        --used_count_;
    }
}

FixedAllocator::Stats FixedAllocator::stats() const {
    return Stats{total_block_count_, used_count_, chunks_.size()};
}