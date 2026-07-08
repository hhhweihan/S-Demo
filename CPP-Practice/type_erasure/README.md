# type_erasure

Week 11（Day 071-077）对应的类型擦除和现代 C++ 基础设施练习代码放在这个目录。

## 覆盖内容

- 手写 `Optional<T>`：延迟构造、析构、拷贝/移动、`value()`、`value_or()`
- 手写 `Variant<Ts...>`：最大对齐存储、运行时下标、析构/拷贝/移动跳表、`visit()`
- 手写 `Function<Ret(Args...)>`：虚函数类型擦除、lambda / 函数指针 / 带状态 lambda 存储
- 整合实验：用 `Variant` 表示命令参数，用 `Function` 表示命令处理器，用 Week10 `Tuple` 存储命令条目

## 目录

- `main.cpp`：运行示例与整合命令解析器
- `templates/my_optional.h`：手写 `Optional`
- `templates/my_variant.h`：手写 `Variant`
- `templates/my_function.h`：手写 `Function`

## 构建

```powershell
cmake -S CPP-Practice/type_erasure -B CPP-Practice/type_erasure/build
cmake --build CPP-Practice/type_erasure/build --config Release
.\CPP-Practice\type_erasure\build\Release\type_erasure_demo.exe
```