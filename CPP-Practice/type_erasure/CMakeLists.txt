cmake_minimum_required(VERSION 3.16)

project(type_erasure_demo LANGUAGES CXX)

add_executable(type_erasure_demo
    main.cpp
)

target_compile_features(type_erasure_demo PRIVATE cxx_std_17)
target_include_directories(type_erasure_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../variadic_tuple
)

if (MSVC)
    target_compile_options(type_erasure_demo PRIVATE /utf-8)
endif()