cmake_minimum_required(VERSION 3.16)

project(stl_associative_demo LANGUAGES CXX)

add_executable(stl_associative_demo main.cpp)
target_compile_features(stl_associative_demo PRIVATE cxx_std_17)
target_include_directories(stl_associative_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(stl_associative_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(stl_associative_tests tests/stl_associative_tests.cpp)
    target_compile_features(stl_associative_tests PRIVATE cxx_std_17)
    target_include_directories(stl_associative_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(stl_associative_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(stl_associative_tests)
    s_demo_apply_sanitizer(stl_associative_tests)
    add_test(NAME stl_associative COMMAND stl_associative_tests)
endif()