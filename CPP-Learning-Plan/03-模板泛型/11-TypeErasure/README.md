# 类型擦除（variant / optional / function）

## 本周目标

理解并手写三种类型擦除技术，它们是现代 C++ 库的核心基础设施。

## 本周完成情况

- [x] Day 71：实现 `Optional<T>`，支持延迟构造、析构、拷贝/移动、`value` 和 `value_or`
- [x] Day 72：实现 `Variant<Ts...>` 存储和类型到下标映射
- [x] Day 73：实现 `visit(visitor, variant)` 运行时分发
- [x] Day 74：实现 `Function<Ret(Args...)>` 虚函数类型擦除
- [x] Day 75：对照 libc++ `any`，理解 any / variant / function 的适用场景和 SBO/SOO
- [x] Day 76：完成命令解析器整合实验
- [x] Day 77：整理代码和本周复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：`CPP-Practice/type_erasure/`
- 核心头文件：`my_optional.h`、`my_variant.h`、`my_function.h`
- 整合入口：`CPP-Practice/type_erasure/main.cpp`
- 知识总结：`Note/C++-Note/类型擦除-复盘总结.md`

## 阶段小结
Week 11 的重点是理解“类型擦除”不是一种固定写法，而是一组在开放性、性能、类型安全之间做取舍的设计。`Optional<T>` 解决“可能没有值”的延迟构造问题；`Variant<Ts...>` 解决“固定候选类型集合里当前是哪一个”的运行时分发问题；`Function<Ret(Args...)>` 解决“可调用对象具体类型未知，但调用签名统一”的问题；`any` 则代表更开放的运行时类型容器。

本周最重要的收获有五点：

1. `Optional` 的核心是未构造存储和显式生命周期管理，不是简单包一层 `T`。
2. `Variant` 的核心状态是 `index_`，核心动作是按下标分发析构、拷贝、移动和访问。
3. `visit` 把运行时下标接回编译期类型，通常通过函数表或递归分发实现。
4. `Function` 用虚基类和模板派生类擦除 callable 类型，`clone` 决定它能否安全拷贝。
5. `any` 比 `variant` 更灵活，但需要 RTTI/类型标识和更强运行时检查；小对象优化能显著减少堆分配。

## 验证命令

```powershell
cmake -S CPP-Practice/type_erasure -B CPP-Practice/type_erasure/build
cmake --build CPP-Practice/type_erasure/build --config Release
.\CPP-Practice\type_erasure\build\Release\type_erasure_demo.exe
```

---