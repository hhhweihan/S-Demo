# C++ 十二个月进阶计划

> **节奏：** 工作日 1 小时 / 周末 2 小时
> **原则：** 每个知识点必须有对应的可运行代码，写 > 读 > 看

## 总体路线

| 月份 | 主题 | 核心产出 |
|------|------|---------|
| Month 01 | 内存管理实战 | 内存池库 + 手写智能指针 |
| Month 02 | 并发编程实战 | 线程池库 + SPSC 无锁队列 |
| Month 03 | 模板与泛型深入 | 手写 tuple / variant / type_list |
| Month 04 | 自编 STL（序列容器）| MyVector / MyDeque / MyList + 迭代器体系 |
| Month 05 | 自编 STL（关联容器）| 红黑树 + set/map + unordered_map + 算法 |
| Month 06 | 异步日志系统 | 双缓冲异步日志 + 多 Sink + PoolAllocator 接入 |
| Month 07 | LevelDB 源码精读 | SkipList + Arena + SSTable + Compaction 仿写 |
| Month 08 | 网络编程 | Reactor 框架 + HTTP Server |
| Month 09 | 高性能网络库 | io_uring + 定时器轮 + 连接池 |
| Month 10 | 协程库 | ucontext Fiber + 调度器 + Channel |
| Month 11 | RPC 框架 | protobuf + 帧协议 + 服务发现 |
| Month 12 | 项目：Mini Raft KV | Leader 选举 + 日志复制 + KV 状态机 |

## 快速导航

- [Month 01 — 内存管理](./Month-01-内存管理/README.md)
- [Month 02 — 并发编程](./Month-02-并发编程/README.md)
- [Month 03 — 模板泛型](./Month-03-模板泛型/README.md)
- [Month 04 — STL 序列容器](./Month-04-STL-Sequence/README.md)
- [Month 05 — STL 关联容器](./Month-05-STL-Associative/README.md)
- [Month 06 — 异步日志](./Month-06-AsyncLogger/README.md)
- [Month 07 — LevelDB](./Month-07-LevelDB/README.md)
- [Month 08 — 网络编程](./Month-08-网络编程/README.md)
- [Month 09 — 高性能网络库](./Month-09-HighPerfNet/README.md)
- [Month 10 — 协程库](./Month-10-Coroutine/README.md)
- [Month 11 — RPC 框架](./Month-11-RPC/README.md)
- [Month 12 — Raft KV](./Month-12-RaftKV/README.md)
- [每日打卡记录](./Progress.md)

## 项目代码存放建议

```
CPP-Learning-Plan/
├── README.md
├── Progress.md
├── Month-01-内存管理/
│   ├── README.md
│   ├── Week-01-固定内存池/
│   │   ├── README.md
│   │   ├── Day-01.md  ← 每日任务说明
│   │   └── Day-07.md
│   └── Week-02 ... Week-04/
├── Month-02-并发编程/ ...
├── ...
└── Month-12-RaftKV/

（代码单独建仓库或放 D:/CodeSpace/CPP-Practice/）
```

## 每日打卡流程

**第一步：找到今天的任务文件**

```
Month-XX-xxx/
└── Week-YY-xxx/
    └── Day-ZZ.md   ← 打开这个
```

**第二步：完成任务，勾选 checkbox**

```markdown
- [x] 实现 FixedAllocator 基础结构   ← 完成后改成 [x]
- [ ] 写单元测试                      ← 未完成保持 [ ]
```

**第三步：git commit 打卡**

```bash
git add .
git commit -m "W1D1: 建项目骨架，实现 FixedAllocator 接口"
git push
```

commit 格式：`W{周数}D{天数}: 完成内容`，例如：
- `W3D2: 手写 shared_ptr 控制块，引用计数测试通过`
- `W7D5: 跳表 vs std::map 性能对比，跳表快 1.8x`

**每周完成后：** 在 [Progress.md](./Progress.md) 对应行填入日期并打 `[x]`

---

## 判断掌握标准

> 不看任何资料，能从零把这个模块完整写出来 → 掌握了
> 写不出来 → 还没掌握，回去再写一遍
