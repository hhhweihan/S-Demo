# 学习进度

> **打卡方式：** 每天在对应 `Day-NNN.md` 里勾选任务 `[x]`（NNN 为全局天数编号，如第二周周一为 Day-008），学完后 `git commit -m "W{周}D{天}: 完成内容"`
> **本文件：** 每周完成后填一行，作为整体进度一览

---

## Month 1 — 内存管理（Week 01–04）

- [x] Week 01 | 04/03~04/09 | 固定内存池 FixedAllocator |
- [x] Week 02 | 04/10~04/16 | PoolAllocator + size class |
- [x] Week 03 | 04/17~04/23 | 手写智能指针 |
- [x] Week 04 | 04/24~04/30 | RAII + scope_guard + 整月验收 |

## Month 2 — 并发编程（Week 05–08）

- [x] Week 05 | 05/01~05/07 | BlockingQueue |
- [x] Week 06 | 05/08~05/14 | ThreadPool + future |
- [ ] Week 07 | ____~____ | 无锁队列 + memory_order |
- [ ] Week 08 | ____~____ | RWMutex + CountDownLatch + 整月验收 |

## Month 3 — 模板泛型（Week 09–12）

- [ ] Week 09 | ____~____ | SFINAE + type_traits |
- [ ] Week 10 | ____~____ | 变参模板 + tuple |
- [ ] Week 11 | ____~____ | variant + optional + function |
- [ ] Week 12 | ____~____ | CRTP + concepts + 整月验收 |

## Month 4 — 自编 STL 序列容器（Week 13–16）

- [ ] Week 13 | ____~____ | 迭代器体系 |
- [ ] Week 14 | ____~____ | MyVector + MyDeque |
- [ ] Week 15 | ____~____ | MyList + MyForwardList |
- [ ] Week 16 | ____~____ | 适配器 + allocator_traits + 整月验收 |

## Month 5 — 自编 STL 关联容器（Week 17–20）

- [ ] Week 17 | ____~____ | 红黑树 |
- [ ] Week 18 | ____~____ | set / map |
- [ ] Week 19 | ____~____ | unordered_map（哈希表）|
- [ ] Week 20 | ____~____ | 排序算法 + allocator 接入 + 整月验收 |

## Month 6 — 异步日志系统（Week 21–24）

- [ ] Week 21 | ____~____ | 同步日志基础 |
- [ ] Week 22 | ____~____ | 双缓冲异步日志 |
- [ ] Week 23 | ____~____ | 多 Sink 支持 |
- [ ] Week 24 | ____~____ | 性能压测 + 整月验收 |

## Month 7 — LevelDB 源码（Week 25–28）

- [ ] Week 25 | ____~____ | Arena + Slice + 整体架构 |
- [ ] Week 26 | ____~____ | SkipList |
- [ ] Week 27 | ____~____ | MemTable + SSTable |
- [ ] Week 28 | ____~____ | WAL + Compaction + 整月验收 |

## Month 8 — 网络编程（Week 29–32）

- [ ] Week 29 | ____~____ | Socket + epoll |
- [ ] Week 30 | ____~____ | 单线程 Reactor |
- [ ] Week 31 | ____~____ | 多线程 Reactor |
- [ ] Week 32 | ____~____ | HTTP Server + 整月验收 |

## Month 9 — 高性能网络库（Week 33–36）

- [ ] Week 33 | ____~____ | 定时器轮 + 心跳检测 |
- [ ] Week 34 | ____~____ | io_uring |
- [ ] Week 35 | ____~____ | 连接池 + 负载均衡 |
- [ ] Week 36 | ____~____ | 零拷贝 + SO_REUSEPORT + 整月验收 |

## Month 10 — 协程库（Week 37–40）

- [ ] Week 37 | ____~____ | ucontext Fiber 基础 |
- [ ] Week 38 | ____~____ | 协程调度器 |
- [ ] Week 39 | ____~____ | Channel 通信 |
- [ ] Week 40 | ____~____ | 协程 + 网络 IO 整合 + 整月验收 |

## Month 11 — RPC 框架（Week 41–44）

- [ ] Week 41 | ____~____ | protobuf + 帧协议 |
- [ ] Week 42 | ____~____ | 同步 + 异步 RPC |
- [ ] Week 43 | ____~____ | 连接池 + 服务发现 |
- [ ] Week 44 | ____~____ | 整合压测 + 整月验收 |

## Month 12 — Mini Raft KV（Week 45–48）

- [ ] Week 45 | ____~____ | Leader 选举 |
- [ ] Week 46 | ____~____ | 日志复制 |
- [ ] Week 47 | ____~____ | KV 状态机 |
- [ ] Week 48 | ____~____ | 快照 + 混沌测试 + 十二个月回顾 |
