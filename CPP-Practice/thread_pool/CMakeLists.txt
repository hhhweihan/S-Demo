cmake_minimum_required(VERSION 3.16)

project(thread_pool_demo LANGUAGES CXX)

add_executable(thread_pool_demo
    main.cpp
    concurrent/joining_thread.h
    concurrent/blocking_queue.h
    concurrent/countdown_latch.h
    concurrent/rw_mutex.h
    concurrent/thread_pool.h
)

target_compile_features(thread_pool_demo PRIVATE cxx_std_17)
target_include_directories(thread_pool_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(thread_pool_demo PRIVATE /utf-8)
endif()