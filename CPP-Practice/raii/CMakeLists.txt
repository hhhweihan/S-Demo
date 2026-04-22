cmake_minimum_required(VERSION 3.16)

project(raii_scope_guard_demo LANGUAGES CXX)

add_executable(raii_scope_guard_demo
    main.cpp
)

target_compile_features(raii_scope_guard_demo PRIVATE cxx_std_17)
target_include_directories(raii_scope_guard_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(raii_scope_guard_demo PRIVATE /utf-8)
endif()