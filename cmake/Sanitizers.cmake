# Applies the sanitizer selected via -DS_DEMO_SANITIZER to a target.
# Split out so both the aggregate build and per-module test targets share one policy.

function(s_demo_apply_sanitizer target)
    if(S_DEMO_SANITIZER STREQUAL "none")
        return()
    endif()

    # MSVC only ships /fsanitize=address; TSan/UBSan combos below are clang/gcc-only,
    # so we skip the flags there rather than emit an unsupported-flag error.
    if(MSVC)
        return()
    endif()

    if(S_DEMO_SANITIZER STREQUAL "address")
        set(flags -fsanitize=address,undefined -fno-omit-frame-pointer)
    elseif(S_DEMO_SANITIZER STREQUAL "thread")
        set(flags -fsanitize=thread -fno-omit-frame-pointer)
    elseif(S_DEMO_SANITIZER STREQUAL "undefined")
        set(flags -fsanitize=undefined -fno-omit-frame-pointer)
    else()
        message(FATAL_ERROR "Unknown S_DEMO_SANITIZER='${S_DEMO_SANITIZER}' (use none|address|thread|undefined)")
    endif()

    target_compile_options(${target} PRIVATE ${flags} -g)
    target_link_options(${target} PRIVATE ${flags})
endfunction()
