# 05-Windows PE/COFF

## 这一章的核心问题

前面主要讲 ELF，这是 Linux/Unix 世界常见的目标文件和可执行文件格式。到了 Windows，主角换成 PE/COFF。

大白话说：ELF 和 PE 都是二进制文件的组织格式。它们都要描述代码在哪、数据在哪、入口点在哪、依赖哪些库、怎么重定位。只是两套系统的具体格式不同。

## COFF 和 PE 是什么关系

COFF 是一种目标文件格式。Windows 的 `.obj` 文件和 PE 文件都和 COFF 有关系。

PE 是 Portable Executable，Windows 的可执行文件格式。常见的 `.exe`、`.dll` 基本都是 PE 格式。

可以粗略理解为：PE 是在 COFF 基础上扩展出来的 Windows 可执行文件格式。

## PE 文件大概长什么样

一个 PE 文件大致包含：

- DOS 头。
- PE 签名。
- COFF 文件头。
- Optional Header。
- 节表。
- 各个节的内容。

这里最容易误解的是 Optional Header。名字叫 optional，但对可执行文件来说非常重要，里面有入口点、镜像基址、数据目录等装载必需信息。

## 为什么 PE 开头有 DOS 头

这是历史兼容。PE 文件开头保留一个 DOS 头，如果在 DOS 环境下运行，会显示类似“这个程序不能在 DOS 模式运行”的提示。

真正的 PE 头位置由 DOS 头里的字段指出。

## 节表是什么

PE 里的节表和 ELF 的 section 表有点像。常见节包括：

- `.text`：代码。
- `.data`：可读写数据。
- `.rdata`：只读数据。
- `.bss`：未初始化数据。
- `.idata`：导入表相关内容。
- `.edata`：导出表相关内容。
- `.rsrc`：资源，比如图标、版本信息、字符串资源。
- `.reloc`：基址重定位信息。

每个节表项会描述节名、虚拟大小、虚拟地址、文件偏移、原始数据大小、属性等。

## RVA 是什么

RVA 是 Relative Virtual Address，相对虚拟地址。

Windows 模块有一个装载基址，比如 `0x400000`。如果某个数据的 RVA 是 `0x1000`，那它实际虚拟地址大致是：

```text
模块基址 + RVA = 0x400000 + 0x1000
```

注意：RVA 不是文件偏移。要从 RVA 找到文件里的位置，需要查节表。

## 数据目录是什么

PE Optional Header 里有一个 Data Directory，里面指向很多重要结构：

- 导入表。
- 导出表。
- 资源表。
- 异常表。
- 重定位表。
- TLS 表。
- 调试信息。

你可以把数据目录理解为 PE 文件内部重要功能区的导航表。

## 导入表和导出表

导入表记录：这个程序需要哪些 DLL，分别需要 DLL 里的哪些函数。

比如一个程序用了 `printf`、`CreateFileW`，最终可能依赖某些运行库 DLL 和 `kernel32.dll` 等。

导出表记录：一个 DLL 对外提供哪些函数或变量。

这两个表是 Windows 动态链接的基础。

## 调试信息放在哪里

Windows/MSVC 体系里，调试信息很多时候不直接塞进 PE 文件，而是放到 PDB 文件里。

所以你经常会看到：

- `app.exe`：程序本体。
- `app.pdb`：调试符号。

发布时可能不带 PDB，但调试崩溃、看调用栈、定位源码时 PDB 很重要。

## 链接指示信息是什么

MSVC 编译器可以在 `.obj` 文件里放一些链接指示，比如默认需要链接哪个库。

这就是为什么有时你只 include 某个头文件，编译器/链接器就自动帮你链接某些默认库。当然这也可能带来隐式依赖，让问题不那么直观。

## 和 ELF 对比怎么记

可以这样对比：

- ELF 有 ELF Header，PE 有 DOS Header + PE Header。
- ELF 有 section header，PE 有 section table。
- ELF 有 program header 用于装载，PE 用 Optional Header 和节表等信息装载。
- ELF 动态链接看 `.dynamic`、`.dynsym` 等，PE 动态链接看导入表、导出表、IAT。
- ELF 常用 `readelf/objdump/nm`，PE 常用 `dumpbin`、Dependencies、PE-bear 等工具。

## 本章最重要的 takeaway

PE/COFF 和 ELF 解决的是同类问题：如何把代码、数据、符号、重定位、调试、导入导出这些信息组织成操作系统和链接器都能读懂的二进制文件。

不要死背字段，先理解它们存在的目的：让链接器能拼，让装载器能装，让调试器能查。

## 建议动手实验

在 Windows 开发者命令行里试：

```bat
dumpbin /headers app.exe
dumpbin /imports app.exe
dumpbin /exports some.dll
```

重点观察：

- 入口点地址。
- ImageBase。
- Section table。
- Import table。
- Data directory。
