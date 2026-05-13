cmake_minimum_required(VERSION 3.16)

project(lock_free_demo LANGUAGES CXX)

add_executable(lock_free_demo
    main.cpp
    concurrent/aba_demo.h
    concurrent/blocking_queue.h
    concurrent/bounded_queue.h
    concurrent/lock_free_stack.h
    concurrent/memory_order_experiments.h
    concurrent/spsc_queue.h
)

target_compile_features(lock_free_demo PRIVATE cxx_std_17)
target_include_directories(lock_free_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(lock_free_demo PRIVATE /utf-8)
endif()
