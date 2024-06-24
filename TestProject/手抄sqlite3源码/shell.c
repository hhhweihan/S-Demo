#if (defined(_WIN32) || defined(WIN32)) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
typedef unsigned int u32;
typedef unsigned short int u16;

# define SHELL_STRINGIFY_(f) #f
# define SHELL_STRINGIFY(f) SHELL_STRINGIFY_(f)
#ifdef SQLITE_CUSTOM_INCLUDE
# include SHELL_STRINGIFY(SQLITE_CUSTOM_INCLUDE)
#endif

#if !defined(SQLITE_OS_WINRT)
# define SQLITE_OS_WINRT 0
#endif

