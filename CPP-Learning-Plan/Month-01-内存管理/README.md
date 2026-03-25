# Month 1 — 内存管理实战

## 月度目标

把笔记里的内存管理理论全部转化为可运行代码，产出一个独立的内存池库。

## 技能树

```
内存池库
├── FixedAllocator     ← Week 1
│   ├── free list
│   └── chunk 扩展
├── PoolAllocator      ← Week 2
│   ├── size class 路由
│   ├── 大对象 fallback
│   └── std::allocator 适配
├── 智能指针           ← Week 3
│   ├── unique_ptr
│   ├── shared_ptr（引用计数控制块）
│   └── weak_ptr
└── RAII 工具          ← Week 4
    ├── scope_guard
    └── defer 宏
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 1 | 固定大小内存池 | [Week-01.md](./Week-01.md) |
| Week 2 | 通用内存池（多 size class）| [Week-02.md](./Week-02.md) |
| Week 3 | 智能指针从零实现 | [Week-03.md](./Week-03.md) |
| Week 4 | RAII 工具 + 整月复盘 | [Week-04.md](./Week-04.md) |

## 验收标准

- [ ] `FixedAllocator<int>` 比 `new int` 快 3x 以上
- [ ] `PoolAllocator` 能作为 `std::vector` 的 allocator 使用
- [ ] 手写 `shared_ptr` 能正确处理循环引用（valgrind 无泄漏）
- [ ] ASan 扫描本月所有代码无报错

## 参考资料

- 你的笔记：`Note/C++-Note/C++笔记3/C++内存管理.md`
- 你的笔记：`Note/C++-Note/C++笔记3/第一回：来写一个简单的内存池.md`
- nginx 内存池：`src/core/ngx_palloc.c`（Week 1 周末阅读）
- jemalloc 论文：搜索 "A Scalable Concurrent malloc(3) Implementation for FreeBSD"
- libc++ shared_ptr：https://github.com/llvm/llvm-project/blob/main/libcxx/include/memory
