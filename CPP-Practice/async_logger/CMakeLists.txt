cmake_minimum_required(VERSION 3.16)

project(async_logger_demo LANGUAGES CXX)

add_executable(async_logger_demo main.cpp)
target_compile_features(async_logger_demo PRIVATE cxx_std_17)
target_include_directories(async_logger_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(async_logger_demo PRIVATE /utf-8 /O2)
endif()