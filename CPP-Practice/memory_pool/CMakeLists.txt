cmake_minimum_required(VERSION 3.16)

project(memory_pool_demo LANGUAGES CXX)

add_executable(memory_pool_demo
    main.cpp
    fixed_allocator.h
)

target_compile_features(memory_pool_demo PRIVATE cxx_std_17)
target_include_directories(memory_pool_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})