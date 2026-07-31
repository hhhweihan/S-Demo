# Baseline warning set for first-party targets. Not -Werror on purpose: the existing
# demos predate this and we don't want the aggregate build to fail on legacy warnings.
# CI gates style via clang-format instead.

function(s_demo_apply_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()
