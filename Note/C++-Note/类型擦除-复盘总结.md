# Week 11 — 类型擦除复盘总结

> 学习周期：Day-071 ~ Day-077  
> 核心产出：`Optional<T>`、`Variant<Ts...>`、`Function<Ret(Args...)>`、命令解析器整合实验  
> 代码位置：`CPP-Practice/type_erasure/`

---

## 一、本周主线

Week 11 的主题是类型擦除，但这一周其实覆盖了三种不同方向：

```text
Optional
  -> 可能没有值
  -> 延迟构造和显式析构

Variant
  -> 固定候选类型集合
  -> index + storage + visit

Function
  -> 统一调用签名
  -> 虚函数类型擦除

Any
  -> 开放类型集合
  -> RTTI / 类型标识 / 小对象优化
```

---

## 二、核心知识点

### 1. Optional 的重点是对象生命周期

`Optional<T>` 不能简单写成：

```cpp
bool has_value_;
T value_;
```

因为这样 `T` 总会被默认构造。正确方向是：

- 先准备对齐存储
- 有值时 placement new 构造
- 清空或析构时显式调用析构函数

这让 `Optional<T>` 可以表达“有空间，但当前没有对象”。

### 2. Variant 的重点是封闭集合内的运行时分发

`Variant<int, std::string>` 的候选类型在编译期已经固定，但当前实际保存哪个类型要运行时才知道。因此需要：

- 一块足够大的对齐存储
- 一个 `index_` 表示当前类型
- 析构、拷贝、移动、访问都按 `index_` 分发

本周实现用函数表完成分发，这是标准库实现里也常见的思路之一。

### 3. visit 把运行时下标接回编译期类型

`visit(visitor, variant)` 做的事情是：

1. 读取 `variant.index()`
2. 选择对应候选类型的访问函数
3. 把原始存储 reinterpret 成真实类型
4. 调用 visitor

它的价值是让调用者不用手写 `switch(index)`，而是把行为集中到一个 visitor 里。

### 4. Function 擦除 callable 类型

`Function<int(int, int)>` 不关心里面是函数指针、lambda 还是带状态 lambda，只关心能否用 `(int, int)` 调用并返回 `int`。

本周使用的结构是：

- `Concept`：虚基类，定义 `invoke` 和 `clone`
- `Model<F>`：模板派生类，保存真实 callable
- `unique_ptr<Concept>`：统一持有 erased object

### 5. any / variant / function 的取舍

- `any`：最灵活，类型集合开放，但取值需要类型检查
- `variant`：类型集合封闭，访问更安全、更高效，但必须提前列出候选类型
- `function`：擦除 callable 的具体类型，只保留调用签名

---

## 三、本周结论

类型擦除的本质，是在某个边界上隐藏具体类型，同时保留一组可操作的公共语义。隐藏得越彻底，运行时检查和间接调用通常越多；候选集合越封闭，编译器能做的检查和优化就越多。
