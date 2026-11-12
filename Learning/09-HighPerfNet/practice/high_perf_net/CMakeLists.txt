cmake_minimum_required(VERSION 3.16)

project(high_perf_net_demo LANGUAGES CXX)

add_executable(high_perf_net_demo main.cpp)
target_compile_features(high_perf_net_demo PRIVATE cxx_std_17)
target_include_directories(high_perf_net_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(high_perf_net_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(high_perf_net_tests tests/high_perf_net_tests.cpp)
    target_compile_features(high_perf_net_tests PRIVATE cxx_std_17)
    target_include_directories(high_perf_net_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(high_perf_net_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(high_perf_net_tests)
    s_demo_apply_sanitizer(high_perf_net_tests)
    add_test(NAME high_perf_net COMMAND high_perf_net_tests)
endif()