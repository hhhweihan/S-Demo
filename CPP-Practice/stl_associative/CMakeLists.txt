cmake_minimum_required(VERSION 3.16)

project(stl_associative_demo LANGUAGES CXX)

add_executable(stl_associative_demo main.cpp)
target_compile_features(stl_associative_demo PRIVATE cxx_std_17)
target_include_directories(stl_associative_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(stl_associative_demo PRIVATE /utf-8 /O2)
endif()