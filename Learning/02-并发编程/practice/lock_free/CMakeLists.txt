cmake_minimum_required(VERSION 3.16)

project(lock_free_demo LANGUAGES CXX)

add_executable(lock_free_demo
    main.cpp
    concurrent/aba_demo.h
    concurrent/blocking_queue.h
    concurrent/hazard_pointer.h
    concurrent/lock_free_stack.h
    concurrent/memory_order_experiments.h
    concurrent/mpmc_queue.h
    concurrent/spsc_queue.h
)

target_compile_features(lock_free_demo PRIVATE cxx_std_17)
target_include_directories(lock_free_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(lock_free_demo PRIVATE /utf-8)
endif()

# The ABA-safe stack uses std::atomic<Head> where Head = {Node*, size_t} = 16 bytes, i.e. a
# double-width CAS. GCC/Clang lower a 16-byte atomic to __atomic_*_16 library calls unless the
# target advertises the instruction, so this needs handling on two axes:
#   * -mcx16 (x86-64): let the compiler inline CMPXCHG16B, keeping the CAS genuinely lock-free
#     (the whole point of this module) instead of a libatomic lock — see is_lock_free() in tests.
#   * link libatomic: resolves __atomic_*_16 on arches/targets where it can't be inlined, so the
#     build isn't pinned to a specific -march. Harmless (dropped) when the compiler inlines.
# Applied to both the demo and the test target via a helper.
function(lock_free_enable_dwcas target)
    if (NOT MSVC)
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("-mcx16" LOCK_FREE_HAS_MCX16)
        if (LOCK_FREE_HAS_MCX16)
            target_compile_options(${target} PRIVATE -mcx16)
        endif()
        find_library(LOCK_FREE_ATOMIC_LIB NAMES atomic libatomic.so.1)
        if (LOCK_FREE_ATOMIC_LIB)
            target_link_libraries(${target} PRIVATE ${LOCK_FREE_ATOMIC_LIB})
        endif()
    endif()
endfunction()

lock_free_enable_dwcas(lock_free_demo)

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(lock_free_tests tests/lock_free_tests.cpp)
    target_compile_features(lock_free_tests PRIVATE cxx_std_17)
    target_include_directories(lock_free_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(lock_free_tests PRIVATE GTest::gtest_main)
    lock_free_enable_dwcas(lock_free_tests)
    s_demo_apply_warnings(lock_free_tests)
    s_demo_apply_sanitizer(lock_free_tests)
    add_test(NAME lock_free COMMAND lock_free_tests)
endif()
