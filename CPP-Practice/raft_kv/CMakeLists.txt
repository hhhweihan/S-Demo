cmake_minimum_required(VERSION 3.16)

project(raft_kv_demo LANGUAGES CXX)

add_executable(raft_kv_demo main.cpp)
target_compile_features(raft_kv_demo PRIVATE cxx_std_17)
target_include_directories(raft_kv_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(raft_kv_demo PRIVATE /utf-8 /O2)
endif()