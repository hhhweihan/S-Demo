cmake_minimum_required(VERSION 3.16)

project(advanced_templates_demo LANGUAGES CXX)

add_executable(advanced_templates_demo
    main.cpp
)

target_compile_features(advanced_templates_demo PRIVATE cxx_std_20)
target_include_directories(advanced_templates_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../template_traits
    ${CMAKE_CURRENT_SOURCE_DIR}/../variadic_tuple
    ${CMAKE_CURRENT_SOURCE_DIR}/../type_erasure
)

if (MSVC)
    target_compile_options(advanced_templates_demo PRIVATE /utf-8)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(advanced_templates_tests tests/advanced_templates_tests.cpp)
    target_compile_features(advanced_templates_tests PRIVATE cxx_std_20)
    target_include_directories(advanced_templates_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../template_traits
        ${CMAKE_CURRENT_SOURCE_DIR}/../variadic_tuple
        ${CMAKE_CURRENT_SOURCE_DIR}/../type_erasure
    )
    target_link_libraries(advanced_templates_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(advanced_templates_tests)
    s_demo_apply_sanitizer(advanced_templates_tests)
    add_test(NAME advanced_templates COMMAND advanced_templates_tests)
endif()