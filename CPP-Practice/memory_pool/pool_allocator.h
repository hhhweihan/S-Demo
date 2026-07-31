#pragma once

#include "fixed_allocator.h"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <new>

class PoolAllocator {
 public:
    static constexpr int kLargeObjectClass = -1;        // 大对象哨兵，区别于合法的非负 class 下标。
    static constexpr std::size_t kAllocatorCount = 16;  // 15 个小对象 class + 1 个大对象预留槽。
    static constexpr std::size_t kSmallObjectClassCount = 15;
    static constexpr std::size_t kMaxSmallObjectSize = 1024;  // 超过即走 malloc/free 大对象路径。
    static constexpr std::size_t kDefaultBlockCount = 64;
    static constexpr std::size_t kLargeObjectSlot = kAllocatorCount - 1;
    static constexpr std::size_t kHeaderSize =
        sizeof(std::size_t);  // header 记录 class 或大对象标记，供无 size 释放。
    static constexpr std::size_t kAlignment = alignof(std::max_align_t);
    static constexpr std::size_t kThreadCacheRefillCount = 8;
    static constexpr std::size_t kThreadCacheMaxCount =
        32;  // 超过就把热块之外的部分刷回共享池，限制单线程囤积。
    // 用户指针须按 max_align_t 对齐，故把 header 之后的偏移向上取整到对齐边界。
    static constexpr std::size_t kUserPointerOffset =
        ((kHeaderSize + kAlignment - 1) / kAlignment) * kAlignment;
    static constexpr std::array<std::size_t, kSmallObjectClassCount> kSizeClasses = {
        8, 16, 24, 32, 48, 64, 80, 96, 128, 192, 256, 384, 512, 768, 1024};

    PoolAllocator() {
        for (std::size_t index = 0; index < kSizeClasses.size(); ++index) {
            // 底层块要容纳 "header + 对齐填充 + payload"，不只是 payload。
            allocators_[index].init(kUserPointerOffset + kSizeClasses[index], kDefaultBlockCount);
            active_small_allocations_[index].store(0);
        }

        // 大对象槽不池化，init(0,0) 让它保持空，仅占位对齐下标。
        allocators_[kLargeObjectSlot].init(0, 0);
    }

    ~PoolAllocator() {
        flush_current_thread_cache();  // 至少把析构线程自己的缓存归还，减少泄漏。
    }

    // 禁拷贝：内部持有 mutex 和分配器状态，复制无意义且危险。
    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    static int size_to_class(std::size_t n) {
        if (n == 0) {
            return 0;  // 0 字节也给一个最小块，返回合法指针。
        }

        // class 数量少，线性查找足够快且免去查表。
        for (std::size_t index = 0; index < kSizeClasses.size(); ++index) {
            if (n <= kSizeClasses[index]) {
                return static_cast<int>(index);
            }
        }

        return kLargeObjectClass;
    }

    static std::size_t class_to_block_size(int index) {
        if (index < 0 || static_cast<std::size_t>(index) >= kSizeClasses.size()) {
            return 0;  // 非法 class（含大对象哨兵）无对应块大小。
        }
        return kSizeClasses[static_cast<std::size_t>(index)];
    }

    void* allocate(std::size_t n) {
        const int class_index = size_to_class(n);
        if (class_index == kLargeObjectClass) {
            if (n > std::numeric_limits<std::size_t>::max() - kUserPointerOffset) {
                throw std::bad_alloc();  // 加上 header 偏移会溢出。
            }

            void* raw = std::malloc(kUserPointerOffset + n);
            if (raw == nullptr) {
                throw std::bad_alloc();
            }

            // 大对象 header 存 large 标记 + 原始 size，释放时据此直接 free。
            write_header(raw, make_large_header(n));
            return to_user_pointer(raw);
        }

        void* raw = allocate_small_from_thread_cache(static_cast<std::size_t>(class_index));
        if (raw == nullptr) {
            return nullptr;
        }

        active_small_allocations_[static_cast<std::size_t>(class_index)].fetch_add(
            1, std::memory_order_relaxed);
        // header 里存 class index，deallocate 就能不依赖调用方传 size 自行定位。
        write_header(raw, make_pool_header(class_index));
        return to_user_pointer(raw);
    }

    void deallocate(void* ptr) {
        if (ptr == nullptr) {
            return;
        }

        void* raw = from_user_pointer(ptr);
        const std::size_t header = read_header(raw);

        if (is_large_header(header)) {
            std::free(raw);  // 大对象当初就是 malloc 来的。
            return;
        }

        const std::size_t class_index = pool_class_from_header(header);
        active_small_allocations_[class_index].fetch_sub(1, std::memory_order_relaxed);
        deallocate_small_to_thread_cache(class_index, raw);
    }

    FixedAllocator::Stats stats_for_class(int class_index) const {
        if (class_index < 0 || static_cast<std::size_t>(class_index) >= kSmallObjectClassCount) {
            return FixedAllocator::Stats{0, 0, 0};
        }

        const std::size_t index = static_cast<std::size_t>(class_index);
        const FixedAllocator::Stats shared_stats = allocators_[index].stats();
        // used 用独立原子计数替换底层值：底层 used 含 TLS cache 里的块，不反映真实活跃数。
        return FixedAllocator::Stats{
            shared_stats.total, active_small_allocations_[index].load(std::memory_order_relaxed),
            shared_stats.chunks};
    }

    FixedAllocator::Stats stats_for_size(std::size_t n) const {
        return stats_for_class(size_to_class(n));
    }

    void flush_current_thread_cache() { tls_registry().release(this); }

 private:
    struct ThreadCacheBin {
        void* head = nullptr;
        std::size_t count = 0;
    };

    struct ThreadCacheEntry {
        PoolAllocator* owner = nullptr;  // 一个线程可同时缓存多个 PoolAllocator，用 owner 区分。
        std::array<ThreadCacheBin, kSmallObjectClassCount> bins{};
        ThreadCacheEntry* next = nullptr;
    };

    // 每线程一张注册表，串起该线程为各 PoolAllocator 持有的缓存条目。
    struct ThreadCacheRegistry {
        ThreadCacheEntry* head = nullptr;

        ThreadCacheEntry& get(PoolAllocator* owner) {
            for (ThreadCacheEntry* current = head; current != nullptr; current = current->next) {
                if (current->owner == owner) {
                    return *current;
                }
            }

            ThreadCacheEntry* entry = new ThreadCacheEntry();
            entry->owner = owner;
            entry->next = head;
            head = entry;
            return *entry;
        }

        void release(PoolAllocator* owner) {
            ThreadCacheEntry* previous = nullptr;
            ThreadCacheEntry* current = head;

            while (current != nullptr) {
                if (current->owner == owner) {
                    if (previous == nullptr) {
                        head = current->next;
                    } else {
                        previous->next = current->next;
                    }

                    owner->flush_thread_cache_entry(
                        *current);  // 摘链前把缓存块还给共享池，防泄漏。
                    delete current;
                    return;
                }

                previous = current;
                current = current->next;
            }
        }

        // 线程退出时触发：把本线程所有缓存条目的块刷回各自 owner。
        ~ThreadCacheRegistry() {
            while (head != nullptr) {
                ThreadCacheEntry* current = head;
                head = current->next;
                if (current->owner != nullptr) {
                    current->owner->flush_thread_cache_entry(*current);
                }
                delete current;
            }
        }
    };

    static constexpr std::size_t kLargeFlagMask =
        std::size_t{1} << (sizeof(std::size_t) * 8 - 1);  // 用 header 最高位标记大对象。

    static ThreadCacheRegistry& tls_registry() {
        static thread_local ThreadCacheRegistry registry;
        return registry;
    }

    static std::size_t make_pool_header(int class_index) {
        return static_cast<std::size_t>(class_index);
    }

    static std::size_t make_large_header(std::size_t size) {
        return kLargeFlagMask | size;  // 最高位=大对象标记，低位=原始 size。
    }

    static bool is_large_header(std::size_t header) { return (header & kLargeFlagMask) != 0; }

    static std::size_t pool_class_from_header(std::size_t header) {
        return header & ~kLargeFlagMask;
    }

    static void write_header(void* raw, std::size_t header) {
        *static_cast<std::size_t*>(raw) = header;
    }

    static std::size_t read_header(void* raw) { return *static_cast<std::size_t*>(raw); }

    static void* to_user_pointer(void* raw) { return static_cast<char*>(raw) + kUserPointerOffset; }

    static void* from_user_pointer(void* ptr) {
        return static_cast<char*>(ptr) - kUserPointerOffset;
    }

    static void push_raw_block(ThreadCacheBin& bin, void* raw) {
        *reinterpret_cast<void**>(raw) = bin.head;  // 复用空闲块首字节当 next 指针，零额外内存。
        bin.head = raw;
        ++bin.count;
    }

    static void* pop_raw_block(ThreadCacheBin& bin) {
        if (bin.head == nullptr) {
            return nullptr;
        }

        void* raw = bin.head;
        bin.head = *reinterpret_cast<void**>(raw);
        --bin.count;
        return raw;
    }

    void* allocate_small_from_thread_cache(std::size_t class_index) {
        ThreadCacheEntry& entry = tls_registry().get(this);
        ThreadCacheBin& bin = entry.bins[class_index];

        if (bin.head == nullptr) {
            refill_thread_cache_bin(class_index, bin);  // 缓存空了才碰共享池，尽量少加锁。
        }

        return pop_raw_block(bin);
    }

    void deallocate_small_to_thread_cache(std::size_t class_index, void* raw) {
        ThreadCacheEntry& entry = tls_registry().get(this);
        ThreadCacheBin& bin = entry.bins[class_index];
        push_raw_block(bin, raw);

        if (bin.count >= kThreadCacheMaxCount) {
            // 超上限就刷回，只留 kThreadCacheRefillCount 个热块继续本地复用。
            flush_thread_cache_bin(class_index, bin, kThreadCacheRefillCount);
        }
    }

    void refill_thread_cache_bin(std::size_t class_index, ThreadCacheBin& bin) {
        void* blocks[kThreadCacheRefillCount] = {};
        // 一次批量取多块摊薄加锁成本，即使池不够也接受少于请求数。
        const std::size_t pulled =
            allocators_[class_index].allocate_batch(blocks, kThreadCacheRefillCount);
        for (std::size_t index = 0; index < pulled; ++index) {
            push_raw_block(bin, blocks[index]);
        }
    }

    void flush_thread_cache_bin(std::size_t class_index, ThreadCacheBin& bin, std::size_t limit) {
        void* blocks[kThreadCacheMaxCount] = {};
        std::size_t flushed = 0;

        while (bin.count > limit &&
               flushed < kThreadCacheMaxCount) {  // 保留 limit 个，其余批量还回。
            blocks[flushed++] = pop_raw_block(bin);
        }

        allocators_[class_index].deallocate_batch(blocks, flushed);
    }

    void flush_thread_cache_entry(ThreadCacheEntry& entry) {
        for (std::size_t class_index = 0; class_index < kSmallObjectClassCount; ++class_index) {
            flush_thread_cache_bin(class_index, entry.bins[class_index], 0);  // limit=0：整桶清空。
        }
    }

    // 当前是“单实例 + 多 size class + 最小 TLS cache”模型，尚未引入 per-arena 亲和。
    std::array<FixedAllocator, kAllocatorCount> allocators_;
    // 独立于底层 used：底层块可能滞留在 TLS cache，这里只记真正交到用户手里的数量。
    std::array<std::atomic<std::size_t>, kSmallObjectClassCount> active_small_allocations_;
};
