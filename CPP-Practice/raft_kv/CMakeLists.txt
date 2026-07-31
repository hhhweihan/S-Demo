cmake_minimum_required(VERSION 3.16)

project(raft_kv_demo LANGUAGES CXX)

set(RAFT_KV_HEADERS mini_raft_kv.h raft_sim.h raft_storage.h)
set(RAFT_KV_NET_HEADERS net/codec.h net/event_loop.h net/raft_server.h)
add_executable(raft_kv_demo main.cpp ${RAFT_KV_HEADERS})
target_compile_features(raft_kv_demo PRIVATE cxx_std_17)
target_include_directories(raft_kv_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(raft_kv_demo PRIVATE /utf-8 /O2)
endif()

# Real-network Raft node: one OS process per cluster member, real TCP + epoll (Linux/POSIX only —
# see net/event_loop.h's epoll/timerfd/eventfd usage). Not built under MSVC.
if (NOT MSVC)
    add_executable(raft_kv_net_demo raft_net_main.cpp ${RAFT_KV_NET_HEADERS})
    target_compile_features(raft_kv_net_demo PRIVATE cxx_std_17)
    target_include_directories(raft_kv_net_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    find_package(Threads REQUIRED)
    target_link_libraries(raft_kv_net_demo PRIVATE Threads::Threads)
    s_demo_apply_warnings(raft_kv_net_demo)
endif()

# Test target: only when built from the aggregate root (GTest + helpers available).
if (S_DEMO_BUILD_TESTS AND TARGET GTest::gtest_main)
    add_executable(raft_kv_tests tests/raft_chaos_tests.cpp tests/raft_net_codec_tests.cpp
                                  ${RAFT_KV_NET_HEADERS})
    target_compile_features(raft_kv_tests PRIVATE cxx_std_17)
    target_include_directories(raft_kv_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(raft_kv_tests PRIVATE GTest::gtest_main)
    s_demo_apply_warnings(raft_kv_tests)
    s_demo_apply_sanitizer(raft_kv_tests)
    add_test(NAME raft_kv COMMAND raft_kv_tests)

    # Real fork+exec+kill(-9)+restart crash/recovery test against raft_kv_net_demo. Genuinely slow
    # (real election timeouts, real process scheduling) and mildly nondeterministic vs. everything
    # else here — excluded from the default `ctest`/CI run via LABELS, same treatment leveldb_mini
    # gives its fork/kill-9 durability test. Run explicitly: ctest --test-dir build -L integration.
    if (TARGET raft_kv_net_demo)
        add_executable(raft_kv_net_integration_test tests/raft_net_integration_test.cpp)
        target_compile_features(raft_kv_net_integration_test PRIVATE cxx_std_17)
        target_include_directories(raft_kv_net_integration_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        target_compile_definitions(raft_kv_net_integration_test PRIVATE
            RAFT_KV_NET_DEMO_PATH="$<TARGET_FILE:raft_kv_net_demo>")
        target_link_libraries(raft_kv_net_integration_test PRIVATE GTest::gtest_main)
        s_demo_apply_warnings(raft_kv_net_integration_test)
        add_test(NAME raft_kv_net_integration COMMAND raft_kv_net_integration_test)
        set_tests_properties(raft_kv_net_integration PROPERTIES LABELS integration)
    endif()
endif()