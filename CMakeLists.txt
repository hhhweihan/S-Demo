cmake_minimum_required(VERSION 3.16)

# Aggregate build for all CPP-Practice modules. Each module still builds standalone
# (cmake -S CPP-Practice/<mod>); the test/sanitizer wiring is guarded so a standalone
# configure that lacks GTest::gtest_main / s_demo_apply_sanitizer just builds the demo.
project(s_demo LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(S_DEMO_BUILD_TESTS "Build unit tests with GoogleTest" ON)
set(S_DEMO_SANITIZER "none" CACHE STRING "Sanitizer: none|address|thread|undefined")

include(Sanitizers)
include(StandardWarnings)

if(S_DEMO_BUILD_TESTS)
    enable_testing()
    include(FetchGoogleTest) # provides GTest::gtest_main via FetchContent
endif()

set(S_DEMO_MODULES
    memory_pool
    month1_benchmark
    raii
    thread_pool
    lock_free
    template_traits
    variadic_tuple
    type_erasure
    advanced_templates
    stl_sequence
    stl_associative
    async_logger
    leveldb_mini
    network_reactor
    high_perf_net
    coroutine_lib
    rpc_framework
    raft_kv
)

foreach(module_name ${S_DEMO_MODULES})
    add_subdirectory(CPP-Practice/${module_name})
endforeach()
