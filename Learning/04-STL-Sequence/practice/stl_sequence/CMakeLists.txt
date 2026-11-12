cmake_minimum_required(VERSION 3.16)

project(stl_sequence_demo LANGUAGES CXX)

add_executable(stl_sequence_demo
    main.cpp
)

target_compile_features(stl_sequence_demo PRIVATE cxx_std_17)
target_include_directories(stl_sequence_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(stl_sequence_demo PRIVATE /utf-8)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(stl_sequence_tests tests/stl_sequence_tests.cpp)
    target_compile_features(stl_sequence_tests PRIVATE cxx_std_17)
    target_include_directories(stl_sequence_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(stl_sequence_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(stl_sequence_tests)
    s_demo_apply_sanitizer(stl_sequence_tests)
    add_test(NAME stl_sequence COMMAND stl_sequence_tests)
endif()