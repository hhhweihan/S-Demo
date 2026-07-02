# variadic_tuple

Week 10（Day 064-070）对应的变参模板和手写 `Tuple` 练习代码放在这个目录。

## 覆盖内容

- 变参模板函数：`sum`、`print_all`、`count`、`all_integral`、`nth_type`
- 折叠表达式与 `std::index_sequence`
- 手写递归继承版 `Tuple<Ts...>`
- 手写 `get<N>`、`apply`、`operator==`、`print(tuple)`

## 目录

- `main.cpp`：运行示例与 `static_assert` 验证
- `templates/my_tuple.h`：手写 `Tuple` 实现

## 构建

```powershell
cmake -S CPP-Practice/variadic_tuple -B CPP-Practice/variadic_tuple/build
cmake --build CPP-Practice/variadic_tuple/build --config Release
.\CPP-Practice\variadic_tuple\build\Release\variadic_tuple_demo.exe
```