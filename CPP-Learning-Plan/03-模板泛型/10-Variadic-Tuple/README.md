# 变参模板 + 手写 tuple

## 本周目标

掌握变参模板和折叠表达式，从零手写一个完整的 `Tuple`（支持 get / apply / print）。

## 本周完成情况

- [x] Day 64：完成 5 个变参模板函数练习
- [x] Day 65：理解折叠表达式，完成 `max_of` 和标准 `tuple` 打印
- [x] Day 66：实现递归继承版 `Tuple<Ts...>` 存储
- [x] Day 67：实现 `get<N>`，支持引用返回和修改
- [x] Day 68：实现 `apply`、`operator==`、`print(tuple)`
- [x] Day 69：对照 libc++ `tuple`，理解 `__tuple_leaf`、`tuple_element`、`tuple_cat`
- [x] Day 70：整理代码和本周复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：`CPP-Practice/variadic_tuple/`
- 核心头文件：`CPP-Practice/variadic_tuple/templates/my_tuple.h`
- 运行入口：`CPP-Practice/variadic_tuple/main.cpp`
- 知识总结：`Note/C++-Note/Week10-变参模板与Tuple-复盘总结.md`

## 阶段小结
Week 10 的主线是把“参数包”从语法概念变成可用的类型和对象展开工具。前两天通过 `sum`、`print_all`、`count`、`all_integral`、`max_of` 和 `index_sequence` 熟悉变参模板与折叠表达式；后四天把这些能力落到手写 `Tuple` 上，完成了存储、索引访问、展开调用、比较和打印。

本周最重要的收获有四点：

1. 参数包不能直接按运行时循环处理，必须通过递归、折叠表达式或索引序列在编译期展开。
2. `index_sequence` 是把“第几个元素”变成一组编译期整数的桥梁，是 `tuple`、`apply`、`print` 这类工具的核心基础。
3. 教学版 `Tuple` 可以用递归继承理解结构，工业版 `tuple` 会进一步使用 `__tuple_leaf` 做 EBO、allocator-aware 构造和引用生命周期保护。
4. `apply` 的本质是 `get<Is>(tuple)...` 展开，真正难点在于正确生成 `Is...` 并保留引用/转发语义。

## 验证命令

```powershell
cmake -S CPP-Practice/variadic_tuple -B CPP-Practice/variadic_tuple/build
cmake --build CPP-Practice/variadic_tuple/build --config Release
.\CPP-Practice\variadic_tuple\build\Release\variadic_tuple_demo.exe
```

---