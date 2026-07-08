cmake_minimum_required(VERSION 3.16)

project(leveldb_mini_demo LANGUAGES CXX)

add_executable(leveldb_mini_demo main.cpp)
target_compile_features(leveldb_mini_demo PRIVATE cxx_std_17)
target_include_directories(leveldb_mini_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(leveldb_mini_demo PRIVATE /utf-8 /O2)
endif()