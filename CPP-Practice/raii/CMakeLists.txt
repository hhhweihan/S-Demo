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

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(raii_tests tests/raii_tests.cpp)
    target_compile_features(raii_tests PRIVATE cxx_std_17)
    target_include_directories(raii_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(raii_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(raii_tests)
    s_demo_apply_sanitizer(raii_tests)
    add_test(NAME raii COMMAND raii_tests)
endif()