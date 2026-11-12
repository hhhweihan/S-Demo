cmake_minimum_required(VERSION 3.16)

project(template_traits_demo LANGUAGES CXX)

add_executable(template_traits_demo
    main.cpp
)

target_compile_features(template_traits_demo PRIVATE cxx_std_17)
target_include_directories(template_traits_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(template_traits_demo PRIVATE /utf-8)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(template_traits_tests tests/template_traits_tests.cpp)
    target_compile_features(template_traits_tests PRIVATE cxx_std_17)
    target_include_directories(template_traits_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(template_traits_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(template_traits_tests)
    s_demo_apply_sanitizer(template_traits_tests)
    add_test(NAME template_traits COMMAND template_traits_tests)
endif()