cmake_minimum_required(VERSION 3.16)

project(stl_sequence_demo LANGUAGES CXX)

add_executable(stl_sequence_demo
    main.cpp
)

target_compile_features(stl_sequence_demo PRIVATE cxx_std_17)
target_include_directories(stl_sequence_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(stl_sequence_demo PRIVATE /utf-8)
endif()