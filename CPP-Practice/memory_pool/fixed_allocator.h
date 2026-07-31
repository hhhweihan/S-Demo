#pragma once

#include <cstddef>
#include <mutex>
#include <vector>

class FixedAllocator {
 public:
    FixedAllocator() = default;
    ~FixedAllocator();

    // 禁拷贝：多个副本会重复 free 同一批 chunk。
    FixedAllocator(const FixedAllocator&) = delete;
    FixedAllocator& operator=(const FixedAllocator&) = delete;

    // block_size 会被抬升到至少 sizeof(void*)（free list 需要）；block_count 为 0 时延迟到首次
    // allocate 才扩容。
    void init(std::size_t block_size, std::size_t block_count);
    void* allocate();
    void deallocate(void* ptr);
    // 批量接口一次持锁完成，返回实际分配数（可能小于 count）。
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
    // *_locked 变体假定调用方已持锁，供批量接口复用而不重复加锁。
    void* allocate_locked();
    void deallocate_locked(void* ptr);

#ifndef NDEBUG
    // 仅 debug 构建校验归还指针合法，release 下零开销。
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
    std::size_t next_expand_count_ = 0;  // 每次扩容翻倍，见 expand()。
    std::size_t total_block_count_ = 0;
    std::size_t used_count_ = 0;
    void* free_list_ = nullptr;
    std::vector<ChunkInfo> chunks_;
    mutable std::mutex mutex_;  // mutable：const 的 stats() 也要加锁。
};
