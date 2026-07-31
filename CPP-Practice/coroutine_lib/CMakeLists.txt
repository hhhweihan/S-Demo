cmake_minimum_required(VERSION 3.16)

project(coroutine_lib_demo LANGUAGES CXX)

add_executable(coroutine_lib_demo main.cpp)
target_compile_features(coroutine_lib_demo PRIVATE cxx_std_20)
target_include_directories(coroutine_lib_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(coroutine_lib_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(coroutine_lib_tests tests/coroutine_lib_tests.cpp)
    target_compile_features(coroutine_lib_tests PRIVATE cxx_std_20)
    target_include_directories(coroutine_lib_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(coroutine_lib_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(coroutine_lib_tests)
    s_demo_apply_sanitizer(coroutine_lib_tests)
    add_test(NAME coroutine_lib COMMAND coroutine_lib_tests)
endif()