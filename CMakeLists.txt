cmake_minimum_required(VERSION 3.16)

# Aggregate build for the implemented projects under Learning/*/practice.
# Each project still builds standalone; the test/sanitizer wiring is guarded so a standalone
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
    Learning/01-内存管理/practice/memory_pool
    Learning/01-内存管理/practice/month1_benchmark
    Learning/01-内存管理/practice/raii
    Learning/02-并发编程/practice/thread_pool
    Learning/02-并发编程/practice/lock_free
    Learning/03-模板泛型/practice/template_traits
    Learning/03-模板泛型/practice/variadic_tuple
    Learning/03-模板泛型/practice/type_erasure
    Learning/03-模板泛型/practice/advanced_templates
    Learning/04-STL-Sequence/practice/stl_sequence
    Learning/05-STL-Associative/practice/stl_associative
    Learning/06-AsyncLogger/practice/async_logger
    Learning/07-LevelDB/practice/leveldb_mini
    Learning/08-网络编程/practice/network_reactor
    Learning/09-HighPerfNet/practice/high_perf_net
    Learning/10-Coroutine/practice/coroutine_lib
    Learning/11-RPC/practice/rpc_framework
    Learning/12-RaftKV/practice/raft_kv
)

foreach(module_name ${S_DEMO_MODULES})
    add_subdirectory(${module_name})
endforeach()
