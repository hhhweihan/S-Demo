cmake_minimum_required(VERSION 3.16)

project(high_perf_net_demo LANGUAGES CXX)

add_executable(high_perf_net_demo main.cpp)
target_compile_features(high_perf_net_demo PRIVATE cxx_std_17)
target_include_directories(high_perf_net_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(high_perf_net_demo PRIVATE /utf-8 /O2)
endif()