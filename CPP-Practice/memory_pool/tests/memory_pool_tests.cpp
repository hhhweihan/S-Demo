#include "fixed_allocator.h"
#include "pool_allocator.h"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

namespace {

bool is_aligned(void* ptr, std::size_t alignment) {
    return (reinterpret_cast<std::uintptr_t>(ptr) % alignment) == 0;
}

}  // namespace

TEST(PoolAllocator, SizeToClassMapsBoundaries) {
    // Zero is defined to fall into the smallest class, not the large-object path.
    EXPECT_EQ(PoolAllocator::size_to_class(0), 0);
    EXPECT_EQ(PoolAllocator::size_to_class(1), 0);
    EXPECT_EQ(PoolAllocator::size_to_class(8), 0);
    // First byte past a class boundary must roll to the next class.
    EXPECT_EQ(PoolAllocator::size_to_class(9), 1);
    EXPECT_EQ(PoolAllocator::size_to_class(16), 1);
    EXPECT_EQ(PoolAllocator::size_to_class(1024), 14);
    // Anything above the small-object ceiling routes to the large-object path.
    EXPECT_EQ(PoolAllocator::size_to_class(1025), PoolAllocator::kLargeObjectClass);
    EXPECT_EQ(PoolAllocator::size_to_class(8192), PoolAllocator::kLargeObjectClass);
}

TEST(PoolAllocator, ClassToBlockSizeRoundTrips) {
    EXPECT_EQ(PoolAllocator::class_to_block_size(0), 8u);
    EXPECT_EQ(PoolAllocator::class_to_block_size(14), 1024u);
    // Out-of-range indices report zero rather than reading past the table.
    EXPECT_EQ(PoolAllocator::class_to_block_size(-1), 0u);
    EXPECT_EQ(PoolAllocator::class_to_block_size(15), 0u);
}

TEST(PoolAllocator, ReturnsAlignedUsableMemory) {
    PoolAllocator pool;
    // Sample several size classes; each user pointer must honor max_align_t.
    for (std::size_t n :
         {std::size_t{1}, std::size_t{8}, std::size_t{33}, std::size_t{512}, std::size_t{1024}}) {
        void* p = pool.allocate(n);
        ASSERT_NE(p, nullptr);
        EXPECT_TRUE(is_aligned(p, PoolAllocator::kAlignment));
        // The full requested range must be writable without corrupting the header.
        std::memset(p, 0xAB, n);
        pool.deallocate(p);
    }
}

TEST(PoolAllocator, LargeObjectPathAllocatesAndFrees) {
    PoolAllocator pool;
    constexpr std::size_t kLarge = 4096;  // above kMaxSmallObjectSize -> malloc path
    void* p = pool.allocate(kLarge);
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(is_aligned(p, PoolAllocator::kAlignment));
    std::memset(p, 0x7F, kLarge);
    // deallocate must recognize the large header and route to std::free without a class lookup.
    pool.deallocate(p);
}

TEST(PoolAllocator, DistinctLiveAllocationsDoNotOverlap) {
    PoolAllocator pool;
    constexpr int kCount = 256;
    std::vector<void*> ptrs;
    ptrs.reserve(kCount);
    for (int i = 0; i < kCount; ++i) {
        void* p = pool.allocate(32);
        ASSERT_NE(p, nullptr);
        // Tag each block with its index; if any two live blocks aliased, the readback fails.
        *static_cast<int*>(p) = i;
        ptrs.push_back(p);
    }
    for (int i = 0; i < kCount; ++i) {
        EXPECT_EQ(*static_cast<int*>(ptrs[i]), i);
    }
    for (void* p : ptrs) {
        pool.deallocate(p);
    }
}

TEST(PoolAllocator, StatsTrackLiveSmallAllocations) {
    PoolAllocator pool;
    const int cls = PoolAllocator::size_to_class(32);
    void* a = pool.allocate(32);
    void* b = pool.allocate(32);
    EXPECT_EQ(pool.stats_for_class(cls).used, 2u);
    pool.deallocate(a);
    EXPECT_EQ(pool.stats_for_class(cls).used, 1u);
    pool.deallocate(b);
    EXPECT_EQ(pool.stats_for_class(cls).used, 0u);
    // The large-object slot is not a small class and reports empty stats.
    EXPECT_EQ(pool.stats_for_class(PoolAllocator::kLargeObjectClass).used, 0u);
}

TEST(PoolAllocator, DeallocateNullIsNoOp) {
    PoolAllocator pool;
    pool.deallocate(nullptr);  // must not crash
    SUCCEED();
}

TEST(PoolAllocator, ConcurrentAllocFreeStress) {
    // The interesting case under TSan/ASan: per-thread caches and a shared FixedAllocator
    // exercised by many threads. Each thread churns its own allocations, so a data race
    // or double-free in the cache-refill/flush paths would surface here.
    PoolAllocator pool;
    constexpr int kThreads = 8;
    constexpr int kIterations = 5000;
    std::atomic<int> failures{0};

    std::vector<std::thread> workers;
    workers.reserve(kThreads);
    for (int t = 0; t < kThreads; ++t) {
        workers.emplace_back([&pool, &failures, t] {
            std::vector<void*> live;
            live.reserve(64);
            for (int i = 0; i < kIterations; ++i) {
                const std::size_t n = 8 + ((i * 13 + t) % 300);  // spread across size classes
                void* p = pool.allocate(n);
                if (p == nullptr || !is_aligned(p, PoolAllocator::kAlignment)) {
                    failures.fetch_add(1, std::memory_order_relaxed);
                    continue;
                }
                std::memset(p, t & 0xFF, n);
                live.push_back(p);
                if (live.size() > 32) {
                    pool.deallocate(live.front());
                    live.erase(live.begin());
                }
            }
            for (void* p : live) {
                pool.deallocate(p);
            }
        });
    }
    for (auto& w : workers) {
        w.join();
    }
    EXPECT_EQ(failures.load(), 0);
}

TEST(FixedAllocator, AllocateReturnsDistinctBlocks) {
    FixedAllocator alloc;
    alloc.init(64, 4);
    void* a = alloc.allocate();
    void* b = alloc.allocate();
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    EXPECT_NE(a, b);
    EXPECT_EQ(alloc.stats().used, 2u);
    alloc.deallocate(a);
    alloc.deallocate(b);
    EXPECT_EQ(alloc.stats().used, 0u);
}

TEST(FixedAllocator, ReusesFreedBlock) {
    FixedAllocator alloc;
    alloc.init(32, 2);
    void* a = alloc.allocate();
    alloc.deallocate(a);
    // Free list is LIFO, so the just-returned block is handed back immediately.
    void* b = alloc.allocate();
    EXPECT_EQ(a, b);
    alloc.deallocate(b);
}

TEST(FixedAllocator, ExpandsBeyondInitialCapacity) {
    FixedAllocator alloc;
    alloc.init(16, 2);  // only two blocks up front
    std::vector<void*> blocks;
    for (int i = 0; i < 10; ++i) {
        void* p = alloc.allocate();
        ASSERT_NE(p, nullptr);  // exhausting the initial chunk must trigger expand()
        blocks.push_back(p);
    }
    EXPECT_EQ(alloc.stats().used, 10u);
    EXPECT_GE(alloc.stats().total, 10u);
    EXPECT_GT(alloc.stats().chunks, 1u);
    for (void* p : blocks) {
        alloc.deallocate(p);
    }
}

TEST(FixedAllocator, BatchAllocateAndDeallocate) {
    FixedAllocator alloc;
    alloc.init(48, 8);
    void* blocks[8] = {};
    const std::size_t got = alloc.allocate_batch(blocks, 8);
    EXPECT_EQ(got, 8u);
    for (std::size_t i = 0; i < got; ++i) {
        EXPECT_NE(blocks[i], nullptr);
    }
    alloc.deallocate_batch(blocks, got);
    EXPECT_EQ(alloc.stats().used, 0u);
}

TEST(FixedAllocator, BlockSizeAtLeastPointerForFreeList) {
    // A tiny block_size still must hold a next pointer; init() clamps it up silently.
    FixedAllocator alloc;
    alloc.init(1, 4);
    void* a = alloc.allocate();
    void* b = alloc.allocate();
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    EXPECT_NE(a, b);
    alloc.deallocate(a);
    alloc.deallocate(b);
}
