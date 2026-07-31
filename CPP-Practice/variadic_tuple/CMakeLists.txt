cmake_minimum_required(VERSION 3.16)

project(variadic_tuple_demo LANGUAGES CXX)

add_executable(variadic_tuple_demo
    main.cpp
)

target_compile_features(variadic_tuple_demo PRIVATE cxx_std_17)
target_include_directories(variadic_tuple_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(variadic_tuple_demo PRIVATE /utf-8)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(variadic_tuple_tests tests/variadic_tuple_tests.cpp)
    target_compile_features(variadic_tuple_tests PRIVATE cxx_std_17)
    target_include_directories(variadic_tuple_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(variadic_tuple_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(variadic_tuple_tests)
    s_demo_apply_sanitizer(variadic_tuple_tests)
    add_test(NAME variadic_tuple COMMAND variadic_tuple_tests)
endif()