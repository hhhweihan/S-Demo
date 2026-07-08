# 08-Linux 共享库的组织

## 这一章的核心问题

第 7 章讲动态链接机制，这一章更偏工程管理：Linux 系统里那么多 `.so` 文件，怎么命名？怎么找？怎么升级？怎么保证老程序还能跑？

也就是说，第 7 章讲“共享库怎么被链接”，第 8 章讲“共享库怎么被管理”。

## 共享库版本为什么麻烦

假设你的程序依赖 `libfoo.so`。

今天 `libfoo.so` 有个函数：

```c
int add(int, int);
```

明天库作者把它改成：

```c
long add(long, long);
```

源码重新编译也许能改，但已经发布出去的二进制程序怎么办？它还按旧 ABI 调用，可能直接崩。

所以共享库升级最怕破坏 ABI。

## API 和 ABI 的区别

API 是源码层面的接口。比如函数声明、头文件、类方法。

ABI 是二进制层面的接口。包括：

- 函数符号名。
- 参数怎么传。
- 返回值怎么传。
- 结构体内存布局。
- 对齐规则。
- 调用约定。
- C++ 名字修饰。
- 异常和 RTTI 规则。

源码兼容不代表 ABI 兼容。你改了结构体字段顺序，源码可能还能编译，但旧二进制就可能坏。

## 共享库的三个名字

Linux 共享库常有三个名字：

### 1. Real Name

真实文件名，带完整版本号：

```text
libfoo.so.1.2.3
```

### 2. SO-NAME

二进制兼容层面的名字：

```text
libfoo.so.1
```

程序运行时通常依赖这个名字。只要 ABI 兼容，小版本升级可以继续用同一个 SO-NAME。

### 3. Link Name

链接时用的名字：

```text
libfoo.so
```

编译链接时写 `-lfoo`，链接器找的通常就是这个。

## 这些名字怎么配合

常见符号链接结构：

```text
libfoo.so      -> libfoo.so.1
libfoo.so.1    -> libfoo.so.1.2.3
libfoo.so.1.2.3
```

开发时链接器用 `libfoo.so`。

运行时动态链接器根据可执行文件里的 `NEEDED` 找 `libfoo.so.1`。

真实加载的文件可能是 `libfoo.so.1.2.3`。

## SO-NAME 的意义

SO-NAME 告诉系统：这个库的 ABI 兼容版本是什么。

如果只是修 bug 或新增不破坏 ABI 的函数，可以保留 `libfoo.so.1`。

如果删函数、改函数签名、改结构体布局，破坏 ABI，就应该升级到 `libfoo.so.2`。

## 符号版本是什么

符号版本更细。它允许同一个共享库里保留某个函数的多个版本。

例如老程序继续绑定到 `foo@LIB_1.0`，新程序绑定到 `foo@@LIB_2.0`。

这样库可以升级实现，又尽量不破坏老程序。

这套机制对 glibc 这类基础库非常重要。

## 共享库搜索路径

程序运行时，动态链接器要找到依赖库。常见搜索来源包括：

- 可执行文件里的 RPATH/RUNPATH。
- 环境变量 `LD_LIBRARY_PATH`。
- `/etc/ld.so.cache` 缓存。
- 默认系统目录，比如 `/lib`、`/usr/lib`。

如果库找不到，你就会看到类似：

```text
error while loading shared libraries: libfoo.so.1: cannot open shared object file
```

## LD_LIBRARY_PATH 为什么要慎用

`LD_LIBRARY_PATH` 很方便，但也危险。

它能让程序优先加载你指定目录里的库。开发调试很有用，但生产环境乱设可能导致程序加载到错误版本的库。

这种问题排查起来很烦，因为程序本身没变，只是环境变了。

## LD_PRELOAD 是什么

`LD_PRELOAD` 可以让某个库在其他库之前加载。

用途包括：

- hook 某些函数。
- 临时替换库函数行为。
- 调试内存分配、文件访问等。

比如你可以写一个库重定义 `malloc`，然后用 `LD_PRELOAD` 让程序优先用你的 `malloc`。

这很强大，也很容易制造难查的问题。

## 创建共享库要注意什么

基本命令：

```bash
gcc -fPIC -shared foo.c -o libfoo.so
```

通常还要注意：

- 用 `-fPIC` 生成地址无关代码。
- 设置合适的 SO-NAME。
- 控制导出符号，别把内部函数都暴露出去。
- 安装后更新动态链接器缓存。
- 保持 ABI 兼容。

## 构造函数和析构函数

共享库可以定义加载时执行的函数和卸载时执行的函数。

这适合做少量初始化和清理。但不要在这里做太复杂的事情，因为加载顺序、锁、依赖库状态都可能带来问题。

## 本章最重要的 takeaway

共享库不是只会生成 `.so` 就完事了。真正工程化使用共享库，需要管理 ABI、SO-NAME、搜索路径、符号可见性、安装和升级策略。

## 建议动手实验

```bash
gcc -fPIC -shared foo.c -Wl,-soname,libfoo.so.1 -o libfoo.so.1.0.0
ln -s libfoo.so.1.0.0 libfoo.so.1
ln -s libfoo.so.1 libfoo.so
gcc main.c -L. -lfoo -o main
readelf -d main
ldd main
```

观察 `NEEDED` 里记录的是哪个名字。
