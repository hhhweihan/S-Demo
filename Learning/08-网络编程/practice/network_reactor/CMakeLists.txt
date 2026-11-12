cmake_minimum_required(VERSION 3.16)

project(network_reactor_demo LANGUAGES CXX)

add_executable(network_reactor_demo main.cpp)
target_compile_features(network_reactor_demo PRIVATE cxx_std_17)
target_include_directories(network_reactor_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(network_reactor_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(network_reactor_tests tests/network_reactor_tests.cpp)
    target_compile_features(network_reactor_tests PRIVATE cxx_std_17)
    target_include_directories(network_reactor_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(network_reactor_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(network_reactor_tests)
    s_demo_apply_sanitizer(network_reactor_tests)
    add_test(NAME network_reactor COMMAND network_reactor_tests)
endif()