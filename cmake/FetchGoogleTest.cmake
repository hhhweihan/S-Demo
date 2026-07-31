# Provides GTest::gtest_main for the test targets.
#
# Prefer a system / pre-installed GoogleTest (offline, reproducible, fast); only fetch from
# GitHub as a fallback. This is done as an explicit `find_package` first rather than via
# FetchContent's FIND_PACKAGE_ARGS integration, because that integration requires CMake >= 3.24
# while this project's floor is 3.16 (Ubuntu 22.04 ships CMake 3.22) — on 3.22 FIND_PACKAGE_ARGS
# is silently ignored and every configure hard-depends on a network git clone, which breaks
# offline/air-gapped builds and flaky-network CI. `find_package` first works on 3.16+.
#
# Offline: `apt-get install libgtest-dev` (or equivalent) and this uses it with zero network.
# No system GTest: it falls back to a pinned FetchContent clone (needs network once).

find_package(GTest QUIET)

if(GTest_FOUND)
    message(STATUS "GoogleTest: using system package (${GTest_DIR})")
else()
    message(STATUS "GoogleTest: system package not found, fetching pinned v1.15.2 via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE
    )
    # Keep MSVC test builds linking the same CRT as everything else.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()
