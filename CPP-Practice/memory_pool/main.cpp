#include "fixed_allocator.h"

#include <cassert>
#include <iostream>

int main() {
    FixedAllocator allocator;
    allocator.init(32, 3);

    void* block1 = allocator.allocate();
    void* block2 = allocator.allocate();
    void* block3 = allocator.allocate();

    assert(block1 != nullptr);
    assert(block2 != nullptr);
    assert(block3 != nullptr);
    assert(block1 != block2);
    assert(block2 != block3);

    allocator.deallocate(block2);

    void* block4 = allocator.allocate();
    assert(block4 == block2);

    FixedAllocator::Stats allocator_stats = allocator.stats();
    assert(allocator_stats.total == 3);
    assert(allocator_stats.used == 3);
    assert(allocator_stats.chunks == 1);

    std::cout << "FixedAllocator Day 002 test passed\n";
    return 0;
}