cmake_minimum_required(VERSION 3.16)

project(template_traits_demo LANGUAGES CXX)

add_executable(template_traits_demo
    main.cpp
)

target_compile_features(template_traits_demo PRIVATE cxx_std_17)
target_include_directories(template_traits_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(template_traits_demo PRIVATE /utf-8)
endif()