cmake_minimum_required(VERSION 3.16)

project(async_logger_demo LANGUAGES CXX)

add_executable(async_logger_demo main.cpp)
target_compile_features(async_logger_demo PRIVATE cxx_std_17)
target_include_directories(async_logger_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(async_logger_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(async_logger_tests tests/async_logger_tests.cpp)
    target_compile_features(async_logger_tests PRIVATE cxx_std_17)
    target_include_directories(async_logger_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(async_logger_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(async_logger_tests)
    s_demo_apply_sanitizer(async_logger_tests)
    add_test(NAME async_logger COMMAND async_logger_tests)
endif()