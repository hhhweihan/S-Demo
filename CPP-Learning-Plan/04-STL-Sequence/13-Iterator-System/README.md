# 迭代器体系（iterator_traits + 5 种迭代器）

## 本周目标

建立 STL 迭代器的完整体系，理解迭代器类别对算法效率的影响。

## 本周完成情况

- [x] Day 85：实现 5 种迭代器类别标签和 `iterator_traits` 裸指针特化
- [x] Day 86：理解并验证 `iterator_traits` 对算法分派的意义
- [x] Day 87：实现可配合双向迭代器工作的 `ReverseIterator`
- [x] Day 88：梳理输入/输出/前向/双向/随机访问迭代器能力边界
- [x] Day 89：用范围 for 验证自定义容器 `begin/end`
- [x] Day 90：把迭代器接口接入后续序列容器工程
- [x] Day 91：完成 Week13 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：`CPP-Practice/stl_sequence/`
- 核心文件：`CPP-Practice/stl_sequence/mini_stl.h`
- 验证入口：`CPP-Practice/stl_sequence/main.cpp`
- 周复盘：`Note/C++-Note/Week13-迭代器体系-复盘总结.md`

## 阶段小结
Week 13 的核心不是“写几个空 tag”，而是建立 STL 算法识别容器能力的协议。`iterator_traits` 让算法不用关心迭代器是类还是裸指针；迭代器类别则决定算法能否随机跳转、双向移动或只能单向扫描。这个协议后续直接影响 `MyVector` 能否进入 `std::sort`，以及 `MyList` 能否进入 `std::reverse`。

本周实现了自定义迭代器标签、裸指针 traits、`ReverseIterator`，并通过 `MyVector` / `MyList` 的后续测试验证协议可用。

## 验证命令

```powershell
cmake -S CPP-Practice/stl_sequence -B CPP-Practice/stl_sequence/build
cmake --build CPP-Practice/stl_sequence/build --config Release
.\CPP-Practice\stl_sequence\build\Release\stl_sequence_demo.exe
```

---
