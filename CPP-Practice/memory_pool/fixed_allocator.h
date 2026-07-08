#pragma once  // 防止头文件被重复包含。

#include <cstddef>  // 使用 std::size_t 表示块大小和数量。
#include <mutex>  // 使用 std::mutex 保护分配器内部状态。
#include <vector>  // 使用 std::vector 保存所有 chunk 元信息。

class FixedAllocator {  // 固定大小块内存分配器。
public:  // 暴露初始化、分配、释放和统计接口。
    FixedAllocator() = default;  // 默认构造，实际内存由 init 延迟初始化。
    ~FixedAllocator();  // 析构时释放所有 chunk 内存。

    FixedAllocator(const FixedAllocator&) = delete;  // 禁止拷贝，避免多个对象释放同一批 chunk。
    FixedAllocator& operator=(const FixedAllocator&) = delete;  // 禁止拷贝赋值，保持资源所有权唯一。

    // block_size: 每块字节数，block_count: 初始块数
    void init(std::size_t block_size, std::size_t block_count);  // 初始化块大小和初始块数量。
    void* allocate();  // 分配一个固定大小的块。
    void deallocate(void* ptr);  // 归还一个先前分配的块。
    std::size_t allocate_batch(void** blocks, std::size_t count);  // 批量分配最多 count 个块。
    void deallocate_batch(void** blocks, std::size_t count);  // 批量归还 count 个块。

    struct Stats {  // 保存分配器当前统计信息。
        std::size_t total;  // 当前总块数。
        std::size_t used;  // 当前已借出的块数。
        std::size_t chunks;  // 当前持有的 chunk 数量。
    };  // Stats 定义结束。

    Stats stats() const;  // 读取当前统计快照。

private:  // 以下成员只服务于分配器内部实现。
    struct ChunkInfo {  // 记录一次扩容得到的连续内存。
        void* memory;  // chunk 起始地址。
        std::size_t block_count;  // chunk 内包含的块数量。
    };  // ChunkInfo 定义结束。

    void expand();  // 扩容并把新块挂入空闲链表。
    void* allocate_locked();  // 在已持锁状态下分配一个块。
    void deallocate_locked(void* ptr);  // 在已持锁状态下归还一个块。

#ifndef NDEBUG  // 非调试构建会跳过指针范围校验声明。
    void assert_pointer_in_range_locked(void* ptr) const;  // 调试模式下校验归还指针来自本分配器。
#endif  // NDEBUG 条件声明结束。

    // 内存布局草图：
    //   chunk:
    //   [ block0 ][ block1 ][ block2 ] ... [ blockN ]
    //
    //   free_list:
    //   block0 -> block2 -> block5 -> nullptr
    //
    //   每个空闲 block 的前 8 字节存放下一个空闲块的指针。
    std::size_t block_size_ = 0;  // 每个固定块的字节大小。
    std::size_t initial_block_count_ = 0;  // 首次初始化或扩容参考的块数量。
    std::size_t next_expand_count_ = 0;  // 下一次扩容准备新增的块数量。
    std::size_t total_block_count_ = 0;  // 当前总块数量。
    std::size_t used_count_ = 0;  // 当前已分配给调用方的块数量。
    void* free_list_ = nullptr;  // 空闲块链表头指针。
    std::vector<ChunkInfo> chunks_;  // 保存所有 chunk，负责统一释放。
    mutable std::mutex mutex_;  // 保护空闲链表、统计计数和 chunk 列表。
};  // FixedAllocator 定义结束。