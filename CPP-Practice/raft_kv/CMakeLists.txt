cmake_minimum_required(VERSION 3.16)

project(raft_kv_demo LANGUAGES CXX)

add_executable(raft_kv_demo main.cpp)
target_compile_features(raft_kv_demo PRIVATE cxx_std_17)
target_include_directories(raft_kv_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(raft_kv_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(raft_kv_tests tests/raft_chaos_tests.cpp)
    target_compile_features(raft_kv_tests PRIVATE cxx_std_17)
    target_include_directories(raft_kv_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(raft_kv_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(raft_kv_tests)
    s_demo_apply_sanitizer(raft_kv_tests)
    add_test(NAME raft_kv COMMAND raft_kv_tests)
endif()