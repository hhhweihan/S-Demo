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