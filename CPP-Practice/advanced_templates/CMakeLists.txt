cmake_minimum_required(VERSION 3.16)

project(advanced_templates_demo LANGUAGES CXX)

add_executable(advanced_templates_demo
    main.cpp
)

target_compile_features(advanced_templates_demo PRIVATE cxx_std_20)
target_include_directories(advanced_templates_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../template_traits
    ${CMAKE_CURRENT_SOURCE_DIR}/../variadic_tuple
    ${CMAKE_CURRENT_SOURCE_DIR}/../type_erasure
)

if (MSVC)
    target_compile_options(advanced_templates_demo PRIVATE /utf-8)
endif()