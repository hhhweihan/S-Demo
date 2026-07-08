cmake_minimum_required(VERSION 3.16)

project(variadic_tuple_demo LANGUAGES CXX)

add_executable(variadic_tuple_demo
    main.cpp
)

target_compile_features(variadic_tuple_demo PRIVATE cxx_std_17)
target_include_directories(variadic_tuple_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(variadic_tuple_demo PRIVATE /utf-8)
endif()