# stl_sequence

Month 04（Week 13-16）对应的自编 STL 序列容器和迭代器体系练习代码放在这个目录。

## 覆盖内容

- `iterator_traits`、五类迭代器标签、`ReverseIterator`
- `MyVector<T>`：动态数组、扩容、随机访问迭代器，支持 `std::sort`
- `MyDeque<T>`：环形缓冲双端队列，支持头尾插入/删除和下标访问
- `MyList<T>`：双向链表、双向迭代器，支持 `std::reverse`
- `MyForwardList<T>`：单向链表、前向迭代器
- `MyStack`、`MyQueue`、`MyPriorityQueue` 容器适配器

## 构建

```powershell
cmake -S CPP-Practice/stl_sequence -B CPP-Practice/stl_sequence/build
cmake --build CPP-Practice/stl_sequence/build --config Release
.\CPP-Practice\stl_sequence\build\Release\stl_sequence_demo.exe
```