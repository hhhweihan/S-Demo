#pragma once  // 防止头文件被重复包含。

#include "fixed_allocator.h"  // 使用固定块分配器承载各 size class。

#include <atomic>  // 记录跨线程可见的小对象活跃计数。
#include <array>  // 保存固定数量的分配器和 size class。
#include <cstddef>  // 使用 std::size_t 和 std::max_align_t。
#include <cstdlib>  // 大对象路径使用 std::malloc 和 std::free。
#include <limits>  // 检查大对象分配时的 size_t 溢出。
#include <new>  // 使用 std::bad_alloc 报告分配失败。

class PoolAllocator {  // 多 size class 的小对象池分配器。
public:  // 暴露分配、释放和统计接口。
    static constexpr int kLargeObjectClass = -1;  // 大对象使用特殊 class 标记。
    static constexpr std::size_t kAllocatorCount = 16;  // 小对象 class 加一个大对象预留槽。
    static constexpr std::size_t kSmallObjectClassCount = 15;  // 小对象 size class 数量。
    static constexpr std::size_t kMaxSmallObjectSize = 1024;  // 小对象路径支持的最大字节数。
    static constexpr std::size_t kDefaultBlockCount = 64;  // 每个 class 初始块数量。
    static constexpr std::size_t kLargeObjectSlot = kAllocatorCount - 1;  // 大对象预留槽下标。
    static constexpr std::size_t kHeaderSize = sizeof(std::size_t);  // 每个分配块头部保存一个 size_t。
    static constexpr std::size_t kAlignment = alignof(std::max_align_t);  // 用户指针按最大通用对齐。
    static constexpr std::size_t kThreadCacheRefillCount = 8;  // 线程缓存一次回填的块数。
    static constexpr std::size_t kThreadCacheMaxCount = 32;  // 单个线程缓存桶的最大块数。
    // 用户指针需要满足通用类型对齐，因此 header 之后要补齐到 max_align_t。
    static constexpr std::size_t kUserPointerOffset =  // 用户指针相对 raw 指针的偏移。
        ((kHeaderSize + kAlignment - 1) / kAlignment) * kAlignment;  // 把 header 大小向上取整到对齐边界。
    static constexpr std::array<std::size_t, kSmallObjectClassCount> kSizeClasses = {  // 小对象规格表。
        8, 16, 24, 32, 48, 64, 80, 96, 128, 192, 256, 384, 512, 768, 1024};  // 按升序覆盖 1 到 1024 字节。

    PoolAllocator() {  // 构造并初始化所有小对象固定块分配器。
        for (std::size_t index = 0; index < kSizeClasses.size(); ++index) {  // 遍历每个小对象 class。
            // 每个小对象 class 实际管理的是 "header + 对齐填充 + payload"。
            allocators_[index].init(kUserPointerOffset + kSizeClasses[index], kDefaultBlockCount);  // 初始化对应固定块分配器。
            active_small_allocations_[index].store(0);  // 初始化活跃分配计数。
        }  // 小对象 class 初始化结束。

        // 预留最后一个 slot 给 Day-10 的大对象路径，当前不参与池化。
        allocators_[kLargeObjectSlot].init(0, 0);  // 初始化空的大对象预留槽。
    }  // PoolAllocator 构造结束。

    ~PoolAllocator() {  // 析构前清理当前线程缓存。
        flush_current_thread_cache();  // 把当前线程缓存块归还到底层分配器。
    }  // PoolAllocator 析构结束。

    PoolAllocator(const PoolAllocator&) = delete;  // 禁止拷贝，避免复制内部分配器状态。
    PoolAllocator& operator=(const PoolAllocator&) = delete;  // 禁止拷贝赋值，保持资源所有权唯一。

    static int size_to_class(std::size_t n) {  // 把请求字节数映射到 size class。
        if (n == 0) {  // 零字节请求映射到最小 class。
            return 0;  // 返回第 0 个 class。
        }  // 零字节检查结束。

        for (std::size_t index = 0; index < kSizeClasses.size(); ++index) {  // 顺序查找第一个能容纳 n 的 class。
            if (n <= kSizeClasses[index]) {  // 当前规格足够容纳请求。
                return static_cast<int>(index);  // 返回当前 class 下标。
            }  // 当前规格检查结束。
        }  // size class 查找结束。

        return kLargeObjectClass;  // 超过小对象上限则走大对象路径。
    }  // size_to_class 结束。

    static std::size_t class_to_block_size(int index) {  // 查询 size class 对应 payload 字节数。
        if (index < 0 || static_cast<std::size_t>(index) >= kSizeClasses.size()) {  // 下标不合法时无对应块大小。
            return 0;  // 返回零表示无效 class。
        }  // class 下标检查结束。
        return kSizeClasses[static_cast<std::size_t>(index)];  // 返回对应规格大小。
    }  // class_to_block_size 结束。

    void* allocate(std::size_t n) {  // 分配 n 字节用户内存。
        const int class_index = size_to_class(n);  // 计算请求所属 size class。
        if (class_index == kLargeObjectClass) {  // 大对象不进入小对象池。
            if (n > std::numeric_limits<std::size_t>::max() - kUserPointerOffset) {  // 检查加 header 后是否溢出。
                throw std::bad_alloc();  // 溢出视为无法分配。
            }  // 大对象尺寸溢出检查结束。

            void* raw = std::malloc(kUserPointerOffset + n);  // 直接向系统申请原始内存。
            if (raw == nullptr) {  // 检查系统分配结果。
                throw std::bad_alloc();  // 分配失败时抛出异常。
            }  // malloc 检查结束。

            // 大对象直接走 malloc/free，header 里只记录 large 标记和原始 size。
            write_header(raw, make_large_header(n));  // 写入大对象 header。
            return to_user_pointer(raw);  // 返回跳过 header 的用户指针。
        }  // 大对象路径结束。

        void* raw = allocate_small_from_thread_cache(static_cast<std::size_t>(class_index));  // 从线程缓存或共享池获取小块。
        if (raw == nullptr) {  // 小对象分配失败时返回空指针。
            return nullptr;  // 保持底层分配失败语义。
        }  // 小对象分配结果检查结束。

        active_small_allocations_[static_cast<std::size_t>(class_index)].fetch_add(1,  // 增加当前 class 的活跃计数。
                                                                                   std::memory_order_relaxed);  // 记录活跃小对象数量。
        // 小对象把 class index 写进 header，回收时就不再依赖调用方传 size。
        write_header(raw, make_pool_header(class_index));  // 写入小对象 class header。
        return to_user_pointer(raw);  // 返回用户可用地址。
    }  // allocate 结束。

    void deallocate(void* ptr) {  // 释放先前分配的用户指针。
        if (ptr == nullptr) {  // 空指针释放无需处理。
            return;  // 直接返回。
        }  // 空指针检查结束。

        void* raw = from_user_pointer(ptr);  // 找回带 header 的原始块地址。
        const std::size_t header = read_header(raw);  // 读取分配时写入的 header。

        if (is_large_header(header)) {  // 大对象按系统分配路径释放。
            std::free(raw);  // 释放原始 malloc 指针。
            return;  // 大对象释放完成。
        }  // 大对象释放路径结束。

        const std::size_t class_index = pool_class_from_header(header);  // 从 header 取回小对象 class。
        active_small_allocations_[class_index].fetch_sub(1, std::memory_order_relaxed);  // 减少活跃分配计数。
        deallocate_small_to_thread_cache(class_index, raw);  // 先归还到当前线程缓存。
    }  // deallocate 结束。

    FixedAllocator::Stats stats_for_class(int class_index) const {  // 查询指定小对象 class 的统计。
        if (class_index < 0 || static_cast<std::size_t>(class_index) >= kSmallObjectClassCount) {  // 非小对象 class 无统计。
            return FixedAllocator::Stats{0, 0, 0};  // 返回空统计。
        }  // class 合法性检查结束。

        const std::size_t index = static_cast<std::size_t>(class_index);  // 转换为数组下标。
        const FixedAllocator::Stats shared_stats = allocators_[index].stats();  // 读取底层固定块统计。
        return FixedAllocator::Stats{shared_stats.total,  // 沿用底层固定块总数。
                         active_small_allocations_[index].load(std::memory_order_relaxed),  // 使用独立活跃计数。
                                     shared_stats.chunks};  // 用活跃计数替换底层 used 计数。
    }  // stats_for_class 结束。

    FixedAllocator::Stats stats_for_size(std::size_t n) const {  // 按请求大小查询对应 class 统计。
        return stats_for_class(size_to_class(n));  // 先映射 class 再返回统计。
    }  // stats_for_size 结束。

    void flush_current_thread_cache() {  // 清空当前线程中属于本分配器的缓存。
        tls_registry().release(this);  // 从 TLS 注册表释放当前 owner 条目。
    }  // flush_current_thread_cache 结束。

private:  // 以下成员服务于内部实现。
    struct ThreadCacheBin {  // 单个 size class 的线程本地缓存桶。
        void* head = nullptr;  // 缓存块链表头。
        std::size_t count = 0;  // 缓存块数量。
    };  // ThreadCacheBin 定义结束。

    struct ThreadCacheEntry {  // 某个 PoolAllocator 在当前线程的缓存条目。
        PoolAllocator* owner = nullptr;  // 缓存所属的 PoolAllocator 实例。
        std::array<ThreadCacheBin, kSmallObjectClassCount> bins{};  // 每个小对象 class 一个缓存桶。
        ThreadCacheEntry* next = nullptr;  // 同线程内下一个分配器缓存条目。
    };  // ThreadCacheEntry 定义结束。

    struct ThreadCacheRegistry {  // 当前线程持有的 PoolAllocator 缓存注册表。
        ThreadCacheEntry* head = nullptr;  // 缓存条目链表头。

        ThreadCacheEntry& get(PoolAllocator* owner) {  // 获取指定 owner 的线程缓存条目。
            for (ThreadCacheEntry* current = head; current != nullptr; current = current->next) {  // 遍历当前线程已有条目。
                if (current->owner == owner) {  // 找到匹配的分配器实例。
                    return *current;  // 返回已有条目。
                }  // owner 匹配检查结束。
            }  // 条目查找循环结束。

            ThreadCacheEntry* entry = new ThreadCacheEntry();  // 为该 owner 创建新缓存条目。
            entry->owner = owner;  // 记录所属分配器。
            entry->next = head;  // 新条目插入链表头。
            head = entry;  // 更新注册表链表头。
            return *entry;  // 返回新建条目。
        }  // get 结束。

        void release(PoolAllocator* owner) {  // 释放指定 owner 的线程缓存条目。
            ThreadCacheEntry* previous = nullptr;  // 保存遍历时的前驱节点。
            ThreadCacheEntry* current = head;  // 从链表头开始查找。

            while (current != nullptr) {  // 遍历缓存条目链表。
                if (current->owner == owner) {  // 找到目标 owner。
                    if (previous == nullptr) {  // 目标位于链表头。
                        head = current->next;  // 移动链表头。
                    } else {  // 目标位于链表中间或末尾。
                        previous->next = current->next;  // 前驱跳过当前节点。
                    }  // 链表摘除结束。

                    owner->flush_thread_cache_entry(*current);  // 把缓存块归还共享池。
                    delete current;  // 删除线程缓存条目。
                    return;  // 释放完成后返回。
                }  // owner 匹配分支结束。

                previous = current;  // 前驱后移。
                current = current->next;  // 当前节点后移。
            }  // 缓存条目遍历结束。
        }  // release 结束。

        ~ThreadCacheRegistry() {  // 线程退出时释放所有缓存条目。
            while (head != nullptr) {  // 逐个弹出链表节点。
                ThreadCacheEntry* current = head;  // 保存当前头节点。
                head = current->next;  // 链表头后移。
                if (current->owner != nullptr) {  // owner 仍有效时可归还缓存块。
                    current->owner->flush_thread_cache_entry(*current);  // 刷回该 owner 的所有缓存桶。
                }  // owner 检查结束。
                delete current;  // 删除当前缓存条目。
            }  // 注册表清理循环结束。
        }  // ThreadCacheRegistry 析构结束。
    };  // ThreadCacheRegistry 定义结束。

    static constexpr std::size_t kLargeFlagMask = std::size_t{1} << (sizeof(std::size_t) * 8 - 1);  // header 最高位标记大对象。

    static ThreadCacheRegistry& tls_registry() {  // 获取当前线程的缓存注册表。
        static thread_local ThreadCacheRegistry registry;  // 每个线程独立维护注册表。
        return registry;  // 返回当前线程注册表。
    }  // tls_registry 结束。

    static std::size_t make_pool_header(int class_index) {  // 生成小对象 header。
        return static_cast<std::size_t>(class_index);  // header 直接保存 class 下标。
    }  // make_pool_header 结束。

    static std::size_t make_large_header(std::size_t size) {  // 生成大对象 header。
        return kLargeFlagMask | size;  // 最高位标记大对象，其余位保存大小。
    }  // make_large_header 结束。

    static bool is_large_header(std::size_t header) {  // 判断 header 是否表示大对象。
        return (header & kLargeFlagMask) != 0;  // 检查最高位标记。
    }  // is_large_header 结束。

    static std::size_t pool_class_from_header(std::size_t header) {  // 从小对象 header 取 class 下标。
        return header & ~kLargeFlagMask;  // 清掉大对象标记位后返回。
    }  // pool_class_from_header 结束。

    static void write_header(void* raw, std::size_t header) {  // 写入块头部元信息。
        *static_cast<std::size_t*>(raw) = header;  // 把 header 存到原始块开头。
    }  // write_header 结束。

    static std::size_t read_header(void* raw) {  // 读取块头部元信息。
        return *static_cast<std::size_t*>(raw);  // 从原始块开头取出 header。
    }  // read_header 结束。

    static void* to_user_pointer(void* raw) {  // 从原始块地址转换到用户地址。
        return static_cast<char*>(raw) + kUserPointerOffset;  // 跳过 header 和对齐填充。
    }  // to_user_pointer 结束。

    static void* from_user_pointer(void* ptr) {  // 从用户地址找回原始块地址。
        return static_cast<char*>(ptr) - kUserPointerOffset;  // 回退 header 和对齐填充大小。
    }  // from_user_pointer 结束。

    static void push_raw_block(ThreadCacheBin& bin, void* raw) {  // 把原始块压入线程缓存桶。
        *reinterpret_cast<void**>(raw) = bin.head;  // 复用空闲块开头保存 next 指针。
        bin.head = raw;  // 更新缓存桶链表头。
        ++bin.count;  // 增加缓存块数量。
    }  // push_raw_block 结束。

    static void* pop_raw_block(ThreadCacheBin& bin) {  // 从线程缓存桶弹出一个原始块。
        if (bin.head == nullptr) {  // 缓存桶为空时无法弹出。
            return nullptr;  // 返回空指针表示未命中。
        }  // 空桶检查结束。

        void* raw = bin.head;  // 保存当前头块。
        bin.head = *reinterpret_cast<void**>(raw);  // 链表头后移。
        --bin.count;  // 减少缓存块数量。
        return raw;  // 返回弹出的原始块。
    }  // pop_raw_block 结束。

    void* allocate_small_from_thread_cache(std::size_t class_index) {  // 从线程缓存分配小对象原始块。
        ThreadCacheEntry& entry = tls_registry().get(this);  // 获取当前线程对应本分配器的缓存条目。
        ThreadCacheBin& bin = entry.bins[class_index];  // 定位对应 size class 的缓存桶。

        if (bin.head == nullptr) {  // 缓存桶为空时需要回填。
            refill_thread_cache_bin(class_index, bin);  // 从共享固定块分配器批量取块。
        }  // 缓存回填检查结束。

        return pop_raw_block(bin);  // 从缓存桶弹出一个块。
    }  // allocate_small_from_thread_cache 结束。

    void deallocate_small_to_thread_cache(std::size_t class_index, void* raw) {  // 把小对象原始块归还当前线程缓存。
        ThreadCacheEntry& entry = tls_registry().get(this);  // 获取当前线程对应本分配器的缓存条目。
        ThreadCacheBin& bin = entry.bins[class_index];  // 定位对应 size class 的缓存桶。
        push_raw_block(bin, raw);  // 先压入当前线程缓存。

        if (bin.count >= kThreadCacheMaxCount) {  // 缓存桶过大时刷回一部分。
            flush_thread_cache_bin(class_index, bin, kThreadCacheRefillCount);  // 保留少量热块，其余归还共享池。
        }  // 缓存上限检查结束。
    }  // deallocate_small_to_thread_cache 结束。

    void refill_thread_cache_bin(std::size_t class_index, ThreadCacheBin& bin) {  // 为线程缓存桶批量补充块。
        void* blocks[kThreadCacheRefillCount] = {};  // 暂存从共享池取出的块。
        const std::size_t pulled =  // 记录本次成功拉取的块数。
            allocators_[class_index].allocate_batch(blocks, kThreadCacheRefillCount);  // 从底层分配器批量拉取块。
        for (std::size_t index = 0; index < pulled; ++index) {  // 遍历成功拉取的块。
            push_raw_block(bin, blocks[index]);  // 放入线程缓存桶。
        }  // 回填循环结束。
    }  // refill_thread_cache_bin 结束。

    void flush_thread_cache_bin(std::size_t class_index, ThreadCacheBin& bin, std::size_t limit) {  // 把缓存桶多余块刷回共享池。
        void* blocks[kThreadCacheMaxCount] = {};  // 暂存待归还的块。
        std::size_t flushed = 0;  // 记录已取出的块数。

        while (bin.count > limit && flushed < kThreadCacheMaxCount) {  // 保留 limit 个块，其余尽量取出。
            blocks[flushed++] = pop_raw_block(bin);  // 弹出一个块并计数。
        }  // 缓存桶取块循环结束。

        allocators_[class_index].deallocate_batch(blocks, flushed);  // 批量归还到底层固定块分配器。
    }  // flush_thread_cache_bin 结束。

    void flush_thread_cache_entry(ThreadCacheEntry& entry) {  // 刷回一个线程缓存条目的所有桶。
        for (std::size_t class_index = 0; class_index < kSmallObjectClassCount; ++class_index) {  // 遍历所有小对象 class。
            flush_thread_cache_bin(class_index, entry.bins[class_index], 0);  // 当前 class 全量刷回共享池。
        }  // 所有缓存桶刷回结束。
    }  // flush_thread_cache_entry 结束。

    // 当前实现是“单实例 + 多 size class + 最小 TLS cache”模型，还没有引入 per-arena 亲和。
    std::array<FixedAllocator, kAllocatorCount> allocators_;  // 每个 size class 一个底层固定块分配器。
    std::array<std::atomic<std::size_t>, kSmallObjectClassCount> active_small_allocations_;  // 记录每个小对象 class 的活跃用户分配数。
};  // PoolAllocator 定义结束。