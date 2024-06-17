### Freestanding环境下也可以使用的帝国一
- stddef.h - size_t
- stdint.h - int32_t, uint64_t
- stdbool.h - bool, true. false
- float.h
- limits.h
- stdarg.h
    - syscall就用到了(但syscall0, syscall1, ...更高效)
- inttypes.h