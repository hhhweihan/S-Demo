#pragma once

#include <cstddef>

class FixedAllocator {
public:
    // block_size: 每块字节数，block_count: 初始块数
    void init(std::size_t block_size, std::size_t block_count);
    void* allocate();
    void deallocate(void* ptr);

    struct Stats {
        std::size_t total;
        std::size_t used;
        std::size_t chunks;
    };

    Stats stats() const;

private:
    // 内存布局草图：
    //   chunk:
    //   [ block0 ][ block1 ][ block2 ] ... [ blockN ]
    //
    //   free_list:
    //   block0 -> block2 -> block5 -> nullptr
    //
    //   每个空闲 block 的前 8 字节存放下一个空闲块的指针。
    std::size_t block_size_;
    std::size_t block_count_;
};