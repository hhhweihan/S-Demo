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

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(thread_pool_tests tests/thread_pool_tests.cpp)
    target_compile_features(thread_pool_tests PRIVATE cxx_std_20)
    target_include_directories(thread_pool_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(thread_pool_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(thread_pool_tests)
    s_demo_apply_sanitizer(thread_pool_tests)
    add_test(NAME thread_pool COMMAND thread_pool_tests)
endif()