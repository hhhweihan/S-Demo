cmake_minimum_required(VERSION 3.16)

project(leveldb_mini_demo LANGUAGES CXX)

# lsm/*.h 为 header-only 部件，列入源码表仅为 IDE 可见（编译单元只有 main.cpp）。
set(LEVELDB_MINI_HEADERS
    mini_leveldb.h
    lsm/coding.h
    lsm/crc32c.h
    lsm/skiplist.h
    lsm/record_log.h
    lsm/block.h
    lsm/table.h
    lsm/memtable.h
    lsm/version.h)

add_executable(leveldb_mini_demo main.cpp ${LEVELDB_MINI_HEADERS})
target_compile_features(leveldb_mini_demo PRIVATE cxx_std_17)
target_include_directories(leveldb_mini_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(leveldb_mini_demo PRIVATE /utf-8 /O2)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(leveldb_mini_tests tests/leveldb_mini_tests.cpp)
    target_compile_features(leveldb_mini_tests PRIVATE cxx_std_17)
    target_include_directories(leveldb_mini_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(leveldb_mini_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(leveldb_mini_tests)
    s_demo_apply_sanitizer(leveldb_mini_tests)
    add_test(NAME leveldb_mini COMMAND leveldb_mini_tests)
endif()