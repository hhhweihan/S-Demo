cmake_minimum_required(VERSION 3.16)

project(month1_benchmark LANGUAGES CXX)

add_executable(month1_benchmark
    month1_benchmark.cpp
    smart_ptr.h
    ../memory_pool/fixed_allocator.cpp
    ../memory_pool/fixed_allocator.h
    ../memory_pool/pool_allocator.h
)

target_compile_features(month1_benchmark PRIVATE cxx_std_17)
target_include_directories(month1_benchmark PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../memory_pool
)

if (MSVC)
    target_compile_options(month1_benchmark PRIVATE /utf-8)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(month1_benchmark_tests tests/month1_benchmark_tests.cpp)
    target_compile_features(month1_benchmark_tests PRIVATE cxx_std_17)
    target_include_directories(month1_benchmark_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(month1_benchmark_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(month1_benchmark_tests)
    s_demo_apply_sanitizer(month1_benchmark_tests)
    add_test(NAME month1_benchmark COMMAND month1_benchmark_tests)
endif()