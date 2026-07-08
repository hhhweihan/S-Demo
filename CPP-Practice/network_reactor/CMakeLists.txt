cmake_minimum_required(VERSION 3.16)

project(network_reactor_demo LANGUAGES CXX)

add_executable(network_reactor_demo main.cpp)
target_compile_features(network_reactor_demo PRIVATE cxx_std_17)
target_include_directories(network_reactor_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(network_reactor_demo PRIVATE /utf-8 /O2)
endif()