cmake_minimum_required(VERSION 3.16)

project(rpc_framework_demo LANGUAGES CXX)

add_executable(rpc_framework_demo main.cpp)
target_compile_features(rpc_framework_demo PRIVATE cxx_std_17)
target_include_directories(rpc_framework_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(rpc_framework_demo PRIVATE /utf-8 /O2)
endif()