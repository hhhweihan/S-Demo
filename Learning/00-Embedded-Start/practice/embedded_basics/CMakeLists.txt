cmake_minimum_required(VERSION 3.16)

project(embedded_basics LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../cmake")
set(S_DEMO_SANITIZER "none" CACHE STRING "Sanitizer: none|address|thread|undefined")

include(StandardWarnings)
include(Sanitizers)
include(CTest)

add_library(embedded_basics STATIC byte_ring.c frame_parser.c)
target_compile_features(embedded_basics PUBLIC c_std_11)
target_include_directories(embedded_basics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
s_demo_apply_warnings(embedded_basics)
s_demo_apply_sanitizer(embedded_basics)

if(BUILD_TESTING)
    include(FetchGoogleTest)
    add_executable(embedded_basics_tests tests/embedded_basics_tests.cpp)
    target_compile_features(embedded_basics_tests PRIVATE cxx_std_17)
    target_link_libraries(embedded_basics_tests PRIVATE embedded_basics GTest::gtest_main)
    s_demo_apply_warnings(embedded_basics_tests)
    s_demo_apply_sanitizer(embedded_basics_tests)

    add_test(NAME embedded_byte_ring COMMAND embedded_basics_tests "--gtest_filter=ByteRing.*")
    add_test(NAME embedded_frame_parser COMMAND embedded_basics_tests "--gtest_filter=FrameParser.*")
    set_tests_properties(embedded_byte_ring embedded_frame_parser PROPERTIES LABELS "exercise")
endif()
