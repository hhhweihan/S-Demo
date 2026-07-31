cmake_minimum_required(VERSION 3.16)

project(memory_pool_demo LANGUAGES CXX)

add_executable(memory_pool_demo
    main.cpp
    fixed_allocator.cpp
    fixed_allocator.h
    pool_allocator.h
    my_allocator.h
)

target_compile_features(memory_pool_demo PRIVATE cxx_std_17)
target_include_directories(memory_pool_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(memory_pool_demo PRIVATE /utf-8)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(memory_pool_tests tests/memory_pool_tests.cpp fixed_allocator.cpp)
    target_compile_features(memory_pool_tests PRIVATE cxx_std_17)
    target_include_directories(memory_pool_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(memory_pool_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(memory_pool_tests)
    s_demo_apply_sanitizer(memory_pool_tests)
    add_test(NAME memory_pool COMMAND memory_pool_tests)
endif()