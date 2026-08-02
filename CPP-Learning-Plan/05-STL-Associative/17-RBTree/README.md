# 红黑树 from scratch

## 本周目标

从零实现红黑树，理解 5 条性质如何保证 O(log n) 的高度约束。

## 本周完成情况

- [x] Day 113：完成红黑树性质、节点结构、左右旋实现
- [x] Day 114：完成插入修复中的变色与旋转逻辑
- [x] Day 115：实现标准 `erase(key)`：BST 删除 + transplant + CLRS delete-fixup（不重建树）
- [x] Day 116：实现中序迭代器并验证有序遍历
- [x] Day 117：实现 `find`、`lower_bound`、`upper_bound`、`equal_range`
- [x] Day 118：完成 10 万随机插入高度和黑高约束验证
- [x] Day 119：完成 Week17 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：`CPP-Practice/stl_associative/`
- 核心文件：`CPP-Practice/stl_associative/mini_associative.h`
- 周复盘：`Note/C++-Note/红黑树-复盘总结.md`

## 阶段小结
Week17 的核心是把二叉搜索树的有序性和红黑树的平衡约束结合起来。插入时先按 BST 规则落点，再通过父叔祖关系做变色和旋转；最终根节点强制为黑色。验证不是只看遍历有序，还要检查根黑、红节点无红子、所有路径黑高一致，以及高度不超过约 $2\log_2(n)$。

当前 `erase` 已实现标准 CLRS 删除：先按 BST 规则删除节点，用 transplant 移植子树，再由 delete-fixup 修复双黑不变量（4 种双黑情形及其镜像），全程不重建树，复杂度为对数级。

## 验证命令

```powershell
cmake -S CPP-Practice/stl_associative -B CPP-Practice/stl_associative/build
cmake --build CPP-Practice/stl_associative/build --config Release
.\CPP-Practice\stl_associative\build\Release\stl_associative_demo.exe
```

---
