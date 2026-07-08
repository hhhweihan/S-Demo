# Week13 迭代器体系复盘总结

## 核心目标

Week13 的目标是建立 STL 迭代器协议：算法不直接依赖容器，而是依赖迭代器暴露出的类型信息、移动能力和访问语义。

## 关键知识点

1. 迭代器类别标签

STL 用 tag type 表示迭代器能力层级：

- input iterator：单向读
- output iterator：单向写
- forward iterator：可多次遍历的单向访问
- bidirectional iterator：支持 `--`
- random access iterator：支持 `+n`、`-n`、下标和距离计算

2. `iterator_traits`

`iterator_traits` 是算法和迭代器之间的适配层。类迭代器可以在内部定义 `value_type`、`difference_type`、`iterator_category` 等类型；裸指针没有成员类型，所以需要偏特化。

3. 算法选择由迭代器能力决定

`std::sort` 需要随机访问迭代器，因此可以用于 `MyVector<int>`；`std::reverse` 只需要双向迭代器，因此可以用于 `MyList<int>`。

4. `ReverseIterator`

反向迭代器的 `base()` 指向当前反向位置的后一个正向位置，因此 `operator*` 通常要临时复制并先 `--` 再解引用。

## 本周代码落点

- `CPP-Practice/stl_sequence/mini_stl.h`
- `CPP-Practice/stl_sequence/main.cpp`

## 验证结果

`stl_sequence_demo.exe` 已通过：

- `iterator_traits<int*>` 类型推导
- `ReverseIterator` 解引用语义
- 自定义容器接入标准算法

## 复盘结论

迭代器体系的本质是把“容器怎么存”与“算法怎么走”拆开。只要迭代器协议正确，自定义容器就能复用标准算法。这个设计是 STL 泛型能力的根。 
