cmake_minimum_required(VERSION 3.16)

project(type_erasure_demo LANGUAGES CXX)

add_executable(type_erasure_demo
    main.cpp
)

target_compile_features(type_erasure_demo PRIVATE cxx_std_17)
target_include_directories(type_erasure_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../variadic_tuple
)

if (MSVC)
    target_compile_options(type_erasure_demo PRIVATE /utf-8)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(type_erasure_tests tests/type_erasure_tests.cpp)
    target_compile_features(type_erasure_tests PRIVATE cxx_std_17)
    target_include_directories(type_erasure_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../variadic_tuple
    )
    target_link_libraries(type_erasure_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(type_erasure_tests)
    s_demo_apply_sanitizer(type_erasure_tests)
    add_test(NAME type_erasure COMMAND type_erasure_tests)
endif()