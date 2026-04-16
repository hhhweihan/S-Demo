#pragma once

#include <cstddef>
#include <mutex>
#include <vector>

class FixedAllocator {
public:
    FixedAllocator() = default;
    ~FixedAllocator();

    FixedAllocator(const FixedAllocator&) = delete;
    FixedAllocator& operator=(const FixedAllocator&) = delete;

    // block_size: 每块字节数，block_count: 初始块数
    void init(std::size_t block_size, std::size_t block_count);
    void* allocate();
    void deallocate(void* ptr);
    std::size_t allocate_batch(void** blocks, std::size_t count);
    void deallocate_batch(void** blocks, std::size_t count);

    struct Stats {
        std::size_t total;
        std::size_t used;
        std::size_t chunks;
    };

    Stats stats() const;

private:
    struct ChunkInfo {
        void* memory;
        std::size_t block_count;
    };

    void expand();
    void* allocate_locked();
    void deallocate_locked(void* ptr);

#ifndef NDEBUG
    void assert_pointer_in_range_locked(void* ptr) const;
#endif

    // 内存布局草图：
    //   chunk:
    //   [ block0 ][ block1 ][ block2 ] ... [ blockN ]
    //
    //   free_list:
    //   block0 -> block2 -> block5 -> nullptr
    //
    //   每个空闲 block 的前 8 字节存放下一个空闲块的指针。
    std::size_t block_size_ = 0;
    std::size_t initial_block_count_ = 0;
    std::size_t next_expand_count_ = 0;
    std::size_t total_block_count_ = 0;
    std::size_t used_count_ = 0;
    void* free_list_ = nullptr;
    std::vector<ChunkInfo> chunks_;
    mutable std::mutex mutex_;
};