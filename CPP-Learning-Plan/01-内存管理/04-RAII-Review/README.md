# RAII 工具 + 整月复盘

## 本周目标

用 RAII 思想封装常见资源，用 ASan 彻底清查前三周代码，跑整月 benchmark。

## 本周完成情况

- [x] Day 22：手写 `ScopeGuard`，把任意退出动作绑定到对象生命周期
- [x] Day 23：实现 `DEFER` 宏和 `FileGuard`，练习作用域清理与真实资源句柄封装
- [x] Day 24：对 Month 1 代码做 AddressSanitizer 扫描，记录 Windows + MSVC 工具边界
- [x] Day 25：复查内存问题，确认当前有效 ASan 配置下未出现核心内存错误
- [x] Day 26：补充 Month 1 综合 benchmark，量化内存池和智能指针的抽象成本
- [x] Day 27：整理 Month 1 总结文章
- [x] Day 28：规划 Month 2，并把 RAII 生命周期纪律接到并发学习

## 本周产出

- RAII 练习：[CPP-Practice/raii](../../../CPP-Practice/raii)
- 月度 benchmark：[CPP-Practice/month1_benchmark](../../../CPP-Practice/month1_benchmark)
- 月度总结：[Note/C++-Note/内存管理实战总结.md](../../../Note/C++-Note/内存管理实战总结.md)
- 周复盘：[Note/C++-Note/RAII工具-整月复盘总结.md](../../../Note/C++-Note/RAII工具-整月复盘总结.md)

## 阶段小结
Week 04 的主线不是单独写几个 RAII 小工具，而是把 Month 1 的资源管理能力真正收口。前三周分别处理固定内存池、通用 allocator 和智能指针，本周则把“资源生命周期必须被对象托管”这条原则推广到作用域退出清理、文件句柄、sanitizer 验证和整月 benchmark。

本周最重要的收获有三点：

1. RAII 不等于“析构函数里 delete”，它适用于任何必须在离开作用域时执行的收尾动作。
2. 工具验证要理解平台边界；Windows + MSVC 下 ASan 不支持 `detect_leaks=1`，不能把工具限制误判成代码问题。
3. Month 1 的闭环来自“实现 + 测试 + sanitizer + benchmark + 总结”，而不只是完成几个类。

---