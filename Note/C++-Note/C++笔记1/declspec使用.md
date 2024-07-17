## C++中部分关键字的作用
### __declspec(dllexport)和__declspec(dllimport)
这个关键字主要用于Windows平台上的DLL编程。当你在一个DLL中定义一个函数或变量，并希望它能够被其他应用程序导入和使用时，你可以使用__declspec(dllexport)来标记这个函数或变量，这样编译器就会将其包含在DLL导出表来。__declspec(dllimport)跟__declspec(dllexport)所用的条件差不多，使用__declspec(dllimport)来声明这些函数或变量，这样编辑器才知道它们是需要从DLL中导入的。
```C++
__declspec(dllexport) void myFunction()
{
    // 函数实现
}
__declspec(dllimport) void myFunction()
{
    // 函数实现
}
```
### __declspec(dllimport)


### __attribute__和__cdecl
__attribute__这是GCC和Clang编译器提供的一种属性声明机制，用于给函数、变量或类型添加额外的元数据或行为。__attribute__可以用来指定函数的调用约定、内存对齐方式、返回类型等。在Microsoft的编译器中，类似的功能通常通过__declspec关键字实现。
例如，在GCC中，你可以使用__attribute__((cdecl))来指定一个函数使用__cdecl调用约定：
```C++
void myFunction(int arg) __attribute__((cdecl));
```
你还可以使用__attribute__来指定函数的返回值不被修改（pure）、函数不抛出异常（nothrow）、函数不会返回（noreturn）等特性。总结来说，__declspec(dllexport)用于DLL导出，__cdecl用于指定函数调用约定，而__attribute__则提供了GCC和Clang编译器的扩展功能，可以用于多种不同的用途。在跨平台编程时，需要注意这些特性在不同编译器之间的差异。例如，在非Windows平台上，__declspec关键字通常是不可用的，而在GCC/Clang中，__attribute__则提供了更多功能。