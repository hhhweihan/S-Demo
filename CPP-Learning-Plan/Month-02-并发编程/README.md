# Month 2 — 并发编程实战

## 月度目标

实现一个生产可用的线程池（支持 future 返回值、动态扩缩容、优雅关闭），
深入理解 C++ 内存模型，写一个正确的 SPSC 无锁队列。

## 技能树

```
并发编程库
├── 线程安全队列        ← Week 5
│   ├── BlockingQueue（有锁）
│   └── BoundedQueue（有界阻塞）
├── 线程池             ← Week 6
│   ├── 基础版（固定线程数）
│   ├── 支持 future 返回
│   └── 动态扩缩容
├── 无锁数据结构        ← Week 7
│   ├── LockFreeStack（单线程）
│   └── SPSCQueue（无锁，单生产者单消费者）
└── 高级同步原语        ← Week 8
    ├── RWMutex（读写锁）
    └── CountDownLatch
```

## 周计划

| 周 | 主题 | 文件 |
|----|------|------|
| Week 5 | 线程安全阻塞队列 | [Week-05.md](./Week-05.md) |
| Week 6 | 线程池 | [Week-06.md](./Week-06.md) |
| Week 7 | 无锁数据结构 | [Week-07.md](./Week-07.md) |
| Week 8 | 高级同步原语 + 整月复盘 | [Week-08.md](./Week-08.md) |

## 验收标准

- [ ] 线程池：4 线程，100 万轻量任务，无任务丢失
- [ ] `submit()` 返回 `std::future`，能获取任务结果
- [ ] SPSC 无锁队列比有锁队列快 5x 以上（单生产者单消费者场景）
- [ ] TSan 扫描本月所有代码无数据竞争

## 参考资料

- 你的笔记：`Note/C++-Note/C++笔记3/C++并发编程.md`
- 书：《C++ Concurrency in Action》第 2 版（重点：第 3、4、7 章）
- folly：`https://github.com/facebook/folly`（MPMCQueue、ThreadPoolExecutor）
- muduo 网络库源码（Week 8 后可选阅读）
