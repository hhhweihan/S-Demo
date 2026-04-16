#pragma once

#include "fixed_allocator.h"

#include <atomic>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <new>

class PoolAllocator {
public:
    static constexpr int kLargeObjectClass = -1;
    static constexpr std::size_t kAllocatorCount = 16;
    static constexpr std::size_t kSmallObjectClassCount = 15;
    static constexpr std::size_t kMaxSmallObjectSize = 1024;
    static constexpr std::size_t kDefaultBlockCount = 64;
    static constexpr std::size_t kLargeObjectSlot = kAllocatorCount - 1;
    static constexpr std::size_t kHeaderSize = sizeof(std::size_t);
    static constexpr std::size_t kAlignment = alignof(std::max_align_t);
    static constexpr std::size_t kThreadCacheRefillCount = 8;
    static constexpr std::size_t kThreadCacheMaxCount = 32;
    // 用户指针需要满足通用类型对齐，因此 header 之后要补齐到 max_align_t。
    static constexpr std::size_t kUserPointerOffset =
        ((kHeaderSize + kAlignment - 1) / kAlignment) * kAlignment;
    static constexpr std::array<std::size_t, kSmallObjectClassCount> kSizeClasses = {
        8, 16, 24, 32, 48, 64, 80, 96, 128, 192, 256, 384, 512, 768, 1024};

    PoolAllocator() {
        for (std::size_t index = 0; index < kSizeClasses.size(); ++index) {
            // 每个小对象 class 实际管理的是 "header + 对齐填充 + payload"。
            allocators_[index].init(kUserPointerOffset + kSizeClasses[index], kDefaultBlockCount);
            active_small_allocations_[index].store(0);
        }

        // 预留最后一个 slot 给 Day-10 的大对象路径，当前不参与池化。
        allocators_[kLargeObjectSlot].init(0, 0);
    }

    ~PoolAllocator() {
        flush_current_thread_cache();
    }

    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    static int size_to_class(std::size_t n) {
        if (n == 0) {
            return 0;
        }

        for (std::size_t index = 0; index < kSizeClasses.size(); ++index) {
            if (n <= kSizeClasses[index]) {
                return static_cast<int>(index);
            }
        }

        return kLargeObjectClass;
    }

    static std::size_t class_to_block_size(int index) {
        if (index < 0 || static_cast<std::size_t>(index) >= kSizeClasses.size()) {
            return 0;
        }
        return kSizeClasses[static_cast<std::size_t>(index)];
    }

    void* allocate(std::size_t n) {
        const int class_index = size_to_class(n);
        if (class_index == kLargeObjectClass) {
            if (n > std::numeric_limits<std::size_t>::max() - kUserPointerOffset) {
                throw std::bad_alloc();
            }

            void* raw = std::malloc(kUserPointerOffset + n);
            if (raw == nullptr) {
                throw std::bad_alloc();
            }

            // 大对象直接走 malloc/free，header 里只记录 large 标记和原始 size。
            write_header(raw, make_large_header(n));
            return to_user_pointer(raw);
        }

        void* raw = allocate_small_from_thread_cache(static_cast<std::size_t>(class_index));
        if (raw == nullptr) {
            return nullptr;
        }

        active_small_allocations_[static_cast<std::size_t>(class_index)].fetch_add(1,
                                                                                   std::memory_order_relaxed);
        // 小对象把 class index 写进 header，回收时就不再依赖调用方传 size。
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
            std::free(raw);
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
        return FixedAllocator::Stats{shared_stats.total,
                                     active_small_allocations_[index].load(std::memory_order_relaxed),
                                     shared_stats.chunks};
    }

    FixedAllocator::Stats stats_for_size(std::size_t n) const {
        return stats_for_class(size_to_class(n));
    }

    void flush_current_thread_cache() {
        tls_registry().release(this);
    }

private:
    struct ThreadCacheBin {
        void* head = nullptr;
        std::size_t count = 0;
    };

    struct ThreadCacheEntry {
        PoolAllocator* owner = nullptr;
        std::array<ThreadCacheBin, kSmallObjectClassCount> bins{};
        ThreadCacheEntry* next = nullptr;
    };

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

                    owner->flush_thread_cache_entry(*current);
                    delete current;
                    return;
                }

                previous = current;
                current = current->next;
            }
        }

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

    static constexpr std::size_t kLargeFlagMask = std::size_t{1} << (sizeof(std::size_t) * 8 - 1);

    static ThreadCacheRegistry& tls_registry() {
        static thread_local ThreadCacheRegistry registry;
        return registry;
    }

    static std::size_t make_pool_header(int class_index) {
        return static_cast<std::size_t>(class_index);
    }

    static std::size_t make_large_header(std::size_t size) {
        return kLargeFlagMask | size;
    }

    static bool is_large_header(std::size_t header) {
        return (header & kLargeFlagMask) != 0;
    }

    static std::size_t pool_class_from_header(std::size_t header) {
        return header & ~kLargeFlagMask;
    }

    static void write_header(void* raw, std::size_t header) {
        *static_cast<std::size_t*>(raw) = header;
    }

    static std::size_t read_header(void* raw) {
        return *static_cast<std::size_t*>(raw);
    }

    static void* to_user_pointer(void* raw) {
        return static_cast<char*>(raw) + kUserPointerOffset;
    }

    static void* from_user_pointer(void* ptr) {
        return static_cast<char*>(ptr) - kUserPointerOffset;
    }

    static void push_raw_block(ThreadCacheBin& bin, void* raw) {
        *reinterpret_cast<void**>(raw) = bin.head;
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
            refill_thread_cache_bin(class_index, bin);
        }

        return pop_raw_block(bin);
    }

    void deallocate_small_to_thread_cache(std::size_t class_index, void* raw) {
        ThreadCacheEntry& entry = tls_registry().get(this);
        ThreadCacheBin& bin = entry.bins[class_index];
        push_raw_block(bin, raw);

        if (bin.count >= kThreadCacheMaxCount) {
            flush_thread_cache_bin(class_index, bin, kThreadCacheRefillCount);
        }
    }

    void refill_thread_cache_bin(std::size_t class_index, ThreadCacheBin& bin) {
        void* blocks[kThreadCacheRefillCount] = {};
        const std::size_t pulled =
            allocators_[class_index].allocate_batch(blocks, kThreadCacheRefillCount);
        for (std::size_t index = 0; index < pulled; ++index) {
            push_raw_block(bin, blocks[index]);
        }
    }

    void flush_thread_cache_bin(std::size_t class_index, ThreadCacheBin& bin, std::size_t limit) {
        void* blocks[kThreadCacheMaxCount] = {};
        std::size_t flushed = 0;

        while (bin.count > limit && flushed < kThreadCacheMaxCount) {
            blocks[flushed++] = pop_raw_block(bin);
        }

        allocators_[class_index].deallocate_batch(blocks, flushed);
    }

    void flush_thread_cache_entry(ThreadCacheEntry& entry) {
        for (std::size_t class_index = 0; class_index < kSmallObjectClassCount; ++class_index) {
            flush_thread_cache_bin(class_index, entry.bins[class_index], 0);
        }
    }

    // 当前实现是“单实例 + 多 size class + 最小 TLS cache”模型，还没有引入 per-arena 亲和。
    std::array<FixedAllocator, kAllocatorCount> allocators_;
    std::array<std::atomic<std::size_t>, kSmallObjectClassCount> active_small_allocations_;
};