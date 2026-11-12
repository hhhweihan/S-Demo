cmake_minimum_required(VERSION 3.16)

project(rpc_framework_demo LANGUAGES CXX)

add_executable(rpc_framework_demo main.cpp)
target_compile_features(rpc_framework_demo PRIVATE cxx_std_17)
target_include_directories(rpc_framework_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(rpc_framework_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(rpc_framework_tests tests/rpc_framework_tests.cpp)
    target_compile_features(rpc_framework_tests PRIVATE cxx_std_17)
    target_include_directories(rpc_framework_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(rpc_framework_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(rpc_framework_tests)
    s_demo_apply_sanitizer(rpc_framework_tests)
    add_test(NAME rpc_framework COMMAND rpc_framework_tests)
endif()