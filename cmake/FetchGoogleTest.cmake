# Pulls GoogleTest via FetchContent. Pinned to a release tag for reproducible CI.
# OPT_IN lets a system-installed GTest satisfy the dependency offline (find_package)
# without forcing every build to hit the network.

include(FetchContent)

set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE OPT_IN)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    GIT_SHALLOW TRUE
    FIND_PACKAGE_ARGS NAMES GTest
)

# Keep MSVC test builds linking the same CRT as everything else.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
