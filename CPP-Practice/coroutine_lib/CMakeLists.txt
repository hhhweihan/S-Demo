cmake_minimum_required(VERSION 3.16)

project(coroutine_lib_demo LANGUAGES CXX)

add_executable(coroutine_lib_demo main.cpp)
target_compile_features(coroutine_lib_demo PRIVATE cxx_std_20)
target_include_directories(coroutine_lib_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(coroutine_lib_demo PRIVATE /utf-8 /O2)
endif()