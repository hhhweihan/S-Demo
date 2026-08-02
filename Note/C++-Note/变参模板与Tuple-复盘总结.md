# Week 10 — 变参模板与 Tuple 复盘总结

> 学习周期：Day-064 ~ Day-070  
> 核心产出：变参模板练习、折叠表达式、手写 `Tuple<Ts...>`、`get<N>`、`apply`、比较和打印  
> 代码位置：`CPP-Practice/variadic_tuple/`

---

## 一、本周主线

Week 10 的核心，是把参数包从“语法上能写 `typename... Args`”推进到“能真正组织类型和对象”。这一周的路线非常清楚：

```text
参数包基础
  -> sizeof...
  -> 折叠表达式
  -> tuple_element

索引展开
  -> index_sequence
  -> std::get<Is>(tuple)...

手写 Tuple
  -> 递归继承存储
  -> get<N>
  -> apply / == / print

源码对照
  -> libc++ __tuple_leaf
  -> EBO
  -> tuple_cat
```

---

## 二、最关键的知识点

### 1. 参数包不是运行时数组

参数包存在于编译期，不能像数组一样直接 for 循环。要处理参数包，常见方式有三种：

- 递归模板：每次拆出 `Head` 和 `Tail...`
- 折叠表达式：对所有参数应用同一个操作
- 索引序列：生成 `0, 1, 2...` 后配合 `get<Is>` 展开

### 2. 折叠表达式适合“同构动作”

例如求和：

```cpp
template <typename... Args>
auto sum(Args... args) {
  return (args + ...);
}
```

又如逐个打印：

```cpp
((os << args), ...);
```

但折叠表达式不适合直接写比较链求最大值，`(first > ... > rest)` 不是正确的 `max` 语义。`max_of` 更适合用递归或逐个更新当前最大值。

### 3. `index_sequence` 是 tuple 工具的核心桥梁

`apply(f, tuple)` 的本质是：

```cpp
f(get<0>(tuple), get<1>(tuple), get<2>(tuple)...)
```

但参数个数不固定，所以要先生成：

```cpp
std::make_index_sequence<sizeof...(Ts)>{}
```

然后在实现函数里展开 `Is...`。

### 4. 教学版 Tuple 的存储模型

本周实现采用递归继承：

```cpp
Tuple<int, double, string>
  -> value: int
  -> base Tuple<double, string>
       -> value: double
       -> base Tuple<string>
            -> value: string
```

`get<N>` 的关键是找到第 N 层节点类型，再 `static_cast` 到那一层取 `value`。

### 5. libc++ tuple 更复杂的原因

libc++ 不只是把递归继承写完整，而是使用 `__tuple_leaf<Index, T>`：

- 用 Index 区分重复类型
- 对空类型做 EBO，减少对象大小
- 支持 allocator-aware 构造
- 处理引用元素、临时对象生命周期、tuple-like、三路比较等标准细节

---

## 三、本周结论

变参模板最实用的价值，是把“不定数量的类型和对象”变成可组合的编译期结构。只要掌握参数包展开、折叠表达式和 `index_sequence`，`Tuple`、`apply`、序列化、批量比较、构造转发这类工具就都有了共同语言。
