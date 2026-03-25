# 每日打卡记录

> 格式：`- [x] YYYY-MM-DD | W{周}D{天} | 完成内容 | 备注`
> 没完成打 `[ ]`，写明卡在哪里

## Month 1 — 内存管理

- [ ] 2026-MM-DD | W1D1 | 建项目骨架，写 FixedAllocator 接口 |
- [ ] 2026-MM-DD | W1D2 | 实现 free list 基础分配 |
- [ ] 2026-MM-DD | W1D3 | 实现 chunk 扩展机制 |
- [ ] 2026-MM-DD | W1D4 | 写单元测试 |
- [ ] 2026-MM-DD | W1D5 | benchmark 对比 new/delete |
- [ ] 2026-MM-DD | W1D6 | 读 nginx 内存池源码 |
- [ ] 2026-MM-DD | W1D7 | 复盘整理 |
- [ ] 2026-MM-DD | W2D1 | 设计 size class 映射 |
- [ ] 2026-MM-DD | W2D2 | 实现 PoolAllocator |
- [ ] 2026-MM-DD | W2D3 | 大对象 fallback |
- [ ] 2026-MM-DD | W2D4 | 替换 std::allocator |
- [ ] 2026-MM-DD | W2D5 | 线程安全版 + 性能测试 |
- [ ] 2026-MM-DD | W2D6 | 读 jemalloc 设计文档 |
- [ ] 2026-MM-DD | W2D7 | 复盘整理 |
- [ ] 2026-MM-DD | W3D1 | 手写 unique_ptr |
- [ ] 2026-MM-DD | W3D2 | 手写 shared_ptr |
- [ ] 2026-MM-DD | W3D3 | 手写 weak_ptr |
- [ ] 2026-MM-DD | W3D4 | 手写 enable_shared_from_this |
- [ ] 2026-MM-DD | W3D5 | 对比 libc++ 实现差异 |
- [ ] 2026-MM-DD | W3D6 | 循环引用测试 + valgrind |
- [ ] 2026-MM-DD | W3D7 | shared_ptr 线程安全性验证 |
- [ ] 2026-MM-DD | W4D1 | 实现 scope_guard |
- [ ] 2026-MM-DD | W4D2 | 实现 defer 宏 |
- [ ] 2026-MM-DD | W4D3 | ASan 扫描前三周代码 |
- [ ] 2026-MM-DD | W4D4 | 修复内存问题 |
- [ ] 2026-MM-DD | W4D5 | 整月 benchmark |
- [ ] 2026-MM-DD | W4D6 | 写总结文章 |
- [ ] 2026-MM-DD | W4D7 | 规划 Month 2 |

## Month 2 — 并发编程

- [ ] 2026-MM-DD | W5D1 | RAII 封装 thread |
- [ ] 2026-MM-DD | W5D2 | 有锁阻塞队列 |
- [ ] 2026-MM-DD | W5D3 | 生产者消费者测试 |
- [ ] 2026-MM-DD | W5D4 | 有界阻塞队列 |
- [ ] 2026-MM-DD | W5D5 | 队列性能测试 |
- [ ] 2026-MM-DD | W5D6 | 读 folly MPMCQueue 注释 |
- [ ] 2026-MM-DD | W5D7 | 复盘整理 |
- [ ] 2026-MM-DD | W6D1 | 基础线程池 |
- [ ] 2026-MM-DD | W6D2 | submit 返回 future |
- [ ] 2026-MM-DD | W6D3 | 优雅关闭 shutdown |
- [ ] 2026-MM-DD | W6D4 | 动态扩缩容 |
- [ ] 2026-MM-DD | W6D5 | 压力测试 |
- [ ] 2026-MM-DD | W6D6 | 对比 folly ThreadPoolExecutor |
- [ ] 2026-MM-DD | W6D7 | 复盘整理 |
- [ ] 2026-MM-DD | W7D1 | memory_order 实验 |
- [ ] 2026-MM-DD | W7D2 | 无锁栈 |
- [ ] 2026-MM-DD | W7D3 | SPSC 无锁队列 |
- [ ] 2026-MM-DD | W7D4 | ABA 问题防御 |
- [ ] 2026-MM-DD | W7D5 | 无锁 vs 有锁性能对比 |
- [ ] 2026-MM-DD | W7D6 | 读《C++ Concurrency》第7章 |
- [ ] 2026-MM-DD | W7D7 | 复盘整理 |
- [ ] 2026-MM-DD | W8D1 | 实现读写锁 |
- [ ] 2026-MM-DD | W8D2 | 实现 CountDownLatch |
- [ ] 2026-MM-DD | W8D3 | 整合到线程池 |
- [ ] 2026-MM-DD | W8D4 | TSan 扫描 |
- [ ] 2026-MM-DD | W8D5 | 整月 benchmark |
- [ ] 2026-MM-DD | W8D6 | 写总结文章 |
- [ ] 2026-MM-DD | W8D7 | 规划 Month 3 |

## Month 3 — 模板泛型

- [ ] 2026-MM-DD | W9D1 | 函数模板基础复习 + 特化 |
- [ ] 2026-MM-DD | W9D2 | 类模板 + CTAD |
- [ ] 2026-MM-DD | W9D3 | SFINAE 入门 |
- [ ] 2026-MM-DD | W9D4 | enable_if 条件编译 |
- [ ] 2026-MM-DD | W9D5 | type_traits 手写 |
- [ ] 2026-MM-DD | W9D6 | 读 libc++ type_traits 源码 |
- [ ] 2026-MM-DD | W9D7 | 复盘整理 |
- [ ] 2026-MM-DD | W10D1 | 变参模板基础 |
- [ ] 2026-MM-DD | W10D2 | 折叠表达式 |
- [ ] 2026-MM-DD | W10D3 | 手写 tuple（存储部分）|
- [ ] 2026-MM-DD | W10D4 | 手写 tuple（get/apply）|
- [ ] 2026-MM-DD | W10D5 | 手写 tuple（比较/打印）|
- [ ] 2026-MM-DD | W10D6 | 读 libc++ tuple 源码 |
- [ ] 2026-MM-DD | W10D7 | 复盘整理 |
- [ ] 2026-MM-DD | W11D1 | 编译期递归与 if constexpr |
- [ ] 2026-MM-DD | W11D2 | 手写 variant（类型存储）|
- [ ] 2026-MM-DD | W11D3 | 手写 variant（visit）|
- [ ] 2026-MM-DD | W11D4 | 手写 optional |
- [ ] 2026-MM-DD | W11D5 | 手写 function（类型擦除）|
- [ ] 2026-MM-DD | W11D6 | 读 libc++ any 源码 |
- [ ] 2026-MM-DD | W11D7 | 复盘整理 |
- [ ] 2026-MM-DD | W12D1 | 策略模式模板化 |
- [ ] 2026-MM-DD | W12D2 | CRTP 静态多态 |
- [ ] 2026-MM-DD | W12D3 | 表达式模板入门 |
- [ ] 2026-MM-DD | W12D4 | concepts (C++20) 入门 |
- [ ] 2026-MM-DD | W12D5 | 整月综合项目 |
- [ ] 2026-MM-DD | W12D6 | 写总结文章 |
- [ ] 2026-MM-DD | W12D7 | 规划 Month 4 |

## Month 4 — 自编 STL（序列容器）

- [ ] 2026-MM-DD | W13D1 | 迭代器 5 种类别 + iterator_traits |
- [ ] 2026-MM-DD | W13D2 | 手写各种迭代器适配器 |
- [ ] 2026-MM-DD | W13D3 | tag dispatch 实现 advance |
- [ ] 2026-MM-DD | W13D4 | 手写 distance / copy |
- [ ] 2026-MM-DD | W13D5 | 流迭代器 istream_iterator |
- [ ] 2026-MM-DD | W13D6 | 读 libc++ iterator 源码 |
- [ ] 2026-MM-DD | W13D7 | 复盘整理 |
- [ ] 2026-MM-DD | W14D1 | MyVector 骨架 + 内存管理 |
- [ ] 2026-MM-DD | W14D2 | push_back + 扩容策略 |
- [ ] 2026-MM-DD | W14D3 | insert + erase + 异常安全 |
- [ ] 2026-MM-DD | W14D4 | 随机访问迭代器 + STL 兼容 |
- [ ] 2026-MM-DD | W14D5 | MyDeque 分块存储 |
- [ ] 2026-MM-DD | W14D6 | 读 libc++ vector 源码 |
- [ ] 2026-MM-DD | W14D7 | 复盘整理 |
- [ ] 2026-MM-DD | W15D1 | MyList 节点设计 |
- [ ] 2026-MM-DD | W15D2 | MyList 迭代器 + 常用接口 |
- [ ] 2026-MM-DD | W15D3 | MyList 高级操作（sort/splice）|
- [ ] 2026-MM-DD | W15D4 | MyForwardList |
- [ ] 2026-MM-DD | W15D5 | allocator 接入 |
- [ ] 2026-MM-DD | W15D6 | 读 libc++ list 源码 |
- [ ] 2026-MM-DD | W15D7 | 复盘整理 |
- [ ] 2026-MM-DD | W16D1 | stack + queue 适配器 |
- [ ] 2026-MM-DD | W16D2 | priority_queue（二叉堆）|
- [ ] 2026-MM-DD | W16D3 | 手写堆算法 push_heap/pop_heap |
- [ ] 2026-MM-DD | W16D4 | allocator_traits 基础 |
- [ ] 2026-MM-DD | W16D5 | 整月综合测试 |
- [ ] 2026-MM-DD | W16D6 | 写总结文章 |
- [ ] 2026-MM-DD | W16D7 | 规划 Month 5 |

## Month 5 — 自编 STL（关联容器）

- [ ] 2026-MM-DD | W17D1 | 红黑树性质 + 节点设计 + 旋转 |
- [ ] 2026-MM-DD | W17D2 | insert + insert_fixup |
- [ ] 2026-MM-DD | W17D3 | erase + delete_fixup |
- [ ] 2026-MM-DD | W17D4 | 中序遍历迭代器 |
- [ ] 2026-MM-DD | W17D5 | find + lower_bound + upper_bound |
- [ ] 2026-MM-DD | W17D6 | 验证树高约束 |
- [ ] 2026-MM-DD | W17D7 | 复盘整理 |
- [ ] 2026-MM-DD | W18D1 | MySet 封装 |
- [ ] 2026-MM-DD | W18D2 | MyMap 封装 + operator[] |
- [ ] 2026-MM-DD | W18D3 | MultiSet + MultiMap |
- [ ] 2026-MM-DD | W18D4 | 迭代器 + range-for 支持 |
- [ ] 2026-MM-DD | W18D5 | lower/upper_bound 完整测试 |
- [ ] 2026-MM-DD | W18D6 | 读 libc++ map 源码 |
- [ ] 2026-MM-DD | W18D7 | 复盘整理 |
- [ ] 2026-MM-DD | W19D1 | 哈希表基础结构 |
- [ ] 2026-MM-DD | W19D2 | rehash + 负载因子 |
- [ ] 2026-MM-DD | W19D3 | find / erase / 迭代器 |
- [ ] 2026-MM-DD | W19D4 | MyUnorderedMap + MyUnorderedSet |
- [ ] 2026-MM-DD | W19D5 | 自定义 Hash + 性能测试 |
- [ ] 2026-MM-DD | W19D6 | 读 libc++ unordered_map 源码 |
- [ ] 2026-MM-DD | W19D7 | 复盘整理 |
- [ ] 2026-MM-DD | W20D1 | sort（introsort）|
- [ ] 2026-MM-DD | W20D2 | stable_sort + partial_sort |
- [ ] 2026-MM-DD | W20D3 | binary_search 算法族 |
- [ ] 2026-MM-DD | W20D4 | allocator_traits 完整接入 |
- [ ] 2026-MM-DD | W20D5 | 整月综合 benchmark |
- [ ] 2026-MM-DD | W20D6 | 写总结文章 |
- [ ] 2026-MM-DD | W20D7 | 规划 Month 6 |

## Month 6 — 异步日志系统

- [ ] 2026-MM-DD | W21D1 | 日志级别 + 基础结构 |
- [ ] 2026-MM-DD | W21D2 | 格式化（时间戳 + 线程 ID）|
- [ ] 2026-MM-DD | W21D3 | 流式接口（LOG_INFO << msg）|
- [ ] 2026-MM-DD | W21D4 | 文件输出 + 每日滚动 |
- [ ] 2026-MM-DD | W21D5 | 线程安全 + 性能基准 |
- [ ] 2026-MM-DD | W21D6 | 读 muduo 日志实现 |
- [ ] 2026-MM-DD | W21D7 | 复盘整理 |
- [ ] 2026-MM-DD | W22D1 | 双缓冲设计 |
- [ ] 2026-MM-DD | W22D2 | 前台写入逻辑 |
- [ ] 2026-MM-DD | W22D3 | 后台线程写入逻辑 |
- [ ] 2026-MM-DD | W22D4 | 优雅关闭 + 边界处理 |
- [ ] 2026-MM-DD | W22D5 | 性能压测（vs 同步日志）|
- [ ] 2026-MM-DD | W22D6 | 读 muduo AsyncLogging 源码 |
- [ ] 2026-MM-DD | W22D7 | 复盘整理 |
- [ ] 2026-MM-DD | W23D1 | Sink 抽象接口 |
- [ ] 2026-MM-DD | W23D2 | FileSink（滚动文件）|
- [ ] 2026-MM-DD | W23D3 | ConsoleSink（彩色终端）|
- [ ] 2026-MM-DD | W23D4 | 过滤器 + 格式化器 |
- [ ] 2026-MM-DD | W23D5 | 异步 Sink 包装 |
- [ ] 2026-MM-DD | W23D6 | 读 spdlog 源码 |
- [ ] 2026-MM-DD | W23D7 | 复盘整理 |
- [ ] 2026-MM-DD | W24D1 | 全功能集成测试 |
- [ ] 2026-MM-DD | W24D2 | 极限吞吐量压测 |
- [ ] 2026-MM-DD | W24D3 | 与 spdlog 对比 |
- [ ] 2026-MM-DD | W24D4 | 内存使用分析 |
- [ ] 2026-MM-DD | W24D5 | 公开 API 设计 + 文档 |
- [ ] 2026-MM-DD | W24D6 | 月度验收 + 对比 glog |
- [ ] 2026-MM-DD | W24D7 | 规划 Month 7 |

## Month 7 — LevelDB 源码精读

- [ ] 2026-MM-DD | W25D1 | 编译 LevelDB，运行 db_bench |
- [ ] 2026-MM-DD | W25D2 | 读 Arena 内存分配器 |
- [ ] 2026-MM-DD | W25D3 | 仿写 Arena |
- [ ] 2026-MM-DD | W25D4 | 读 Slice + Status |
- [ ] 2026-MM-DD | W25D5 | 读 Comparator + Options |
- [ ] 2026-MM-DD | W25D6 | 画整体架构图 |
- [ ] 2026-MM-DD | W25D7 | 复盘整理 |
- [ ] 2026-MM-DD | W26D1 | 跳表原理推导 |
- [ ] 2026-MM-DD | W26D2 | 读 skiplist.h |
- [ ] 2026-MM-DD | W26D3 | 仿写跳表 |
- [ ] 2026-MM-DD | W26D4 | 对比差异 + 无锁分析 |
- [ ] 2026-MM-DD | W26D5 | 跳表 vs std::map 性能 |
- [ ] 2026-MM-DD | W26D6 | 深挖 memory_order 用法 |
- [ ] 2026-MM-DD | W26D7 | 复盘整理 |
- [ ] 2026-MM-DD | W27D1 | 读 MemTable |
- [ ] 2026-MM-DD | W27D2 | 读 SSTable 格式 |
- [ ] 2026-MM-DD | W27D3 | 读 Block 编码 |
- [ ] 2026-MM-DD | W27D4 | 写 SSTable 解析工具 |
- [ ] 2026-MM-DD | W27D5 | 读 BloomFilter |
- [ ] 2026-MM-DD | W27D6 | 画 Compaction 流程图 |
- [ ] 2026-MM-DD | W27D7 | 复盘整理 |
- [ ] 2026-MM-DD | W28D1 | 读 WAL 格式 |
- [ ] 2026-MM-DD | W28D2 | 读 DB Open 启动流程 |
- [ ] 2026-MM-DD | W28D3 | gdb 跟踪 Put 写入路径 |
- [ ] 2026-MM-DD | W28D4 | gdb 跟踪 Compaction |
- [ ] 2026-MM-DD | W28D5 | 读 Version/VersionSet 头文件 |
- [ ] 2026-MM-DD | W28D6 | 用 LevelDB 写一个小应用 |
- [ ] 2026-MM-DD | W28D7 | 写读书笔记 1000 字 |

## Month 8 — 网络编程

- [ ] 2026-MM-DD | W29D1 | Socket 基础 + 回显服务器 |
- [ ] 2026-MM-DD | W29D2 | TCP 粘包处理 |
- [ ] 2026-MM-DD | W29D3 | 非阻塞 Socket + fcntl |
- [ ] 2026-MM-DD | W29D4 | select 多路复用 |
- [ ] 2026-MM-DD | W29D5 | epoll 基础（LT 模式）|
- [ ] 2026-MM-DD | W29D6 | epoll ET 模式 |
- [ ] 2026-MM-DD | W29D7 | 复盘整理 |
- [ ] 2026-MM-DD | W30D1 | Reactor 模式设计 |
- [ ] 2026-MM-DD | W30D2 | 实现 EventLoop |
- [ ] 2026-MM-DD | W30D3 | 实现 Channel + Poller |
- [ ] 2026-MM-DD | W30D4 | 实现 Acceptor |
- [ ] 2026-MM-DD | W30D5 | 整合：单线程 Reactor |
- [ ] 2026-MM-DD | W30D6 | 读 muduo 对应代码 |
- [ ] 2026-MM-DD | W30D7 | 复盘整理 |
- [ ] 2026-MM-DD | W31D1 | 多线程 Reactor（one loop per thread）|
- [ ] 2026-MM-DD | W31D2 | 实现 TcpConnection |
- [ ] 2026-MM-DD | W31D3 | 实现 Buffer（环形缓冲）|
- [ ] 2026-MM-DD | W31D4 | 跨线程调用（runInLoop）|
- [ ] 2026-MM-DD | W31D5 | 多线程 Reactor 压测 |
- [ ] 2026-MM-DD | W31D6 | 读 muduo TcpServer 源码 |
- [ ] 2026-MM-DD | W31D7 | 复盘整理 |
- [ ] 2026-MM-DD | W32D1 | HTTP 协议解析 |
- [ ] 2026-MM-DD | W32D2 | HTTP 响应构造 + 连接管理 |
- [ ] 2026-MM-DD | W32D3 | 路由 + 静态文件服务 |
- [ ] 2026-MM-DD | W32D4 | 连接池压测（wrk）|
- [ ] 2026-MM-DD | W32D5 | 整月综合 benchmark |
- [ ] 2026-MM-DD | W32D6 | 读 muduo 网络库源码 |
- [ ] 2026-MM-DD | W32D7 | 规划 Month 9 |

## Month 9 — 高性能网络库

- [ ] 2026-MM-DD | W33D1 | 定时器最小堆方案 |
- [ ] 2026-MM-DD | W33D2 | 时间轮设计 |
- [ ] 2026-MM-DD | W33D3 | 时间轮实现 + 取消 |
- [ ] 2026-MM-DD | W33D4 | 心跳检测集成 |
- [ ] 2026-MM-DD | W33D5 | 层级时间轮（可选）|
- [ ] 2026-MM-DD | W33D6 | 读 Netty HashedWheelTimer 源码 |
- [ ] 2026-MM-DD | W33D7 | 复盘整理 |
- [ ] 2026-MM-DD | W34D1 | io_uring 基础原理 |
- [ ] 2026-MM-DD | W34D2 | io_uring 网络 IO |
- [ ] 2026-MM-DD | W34D3 | 注册固定缓冲区 |
- [ ] 2026-MM-DD | W34D4 | SQPOLL 内核轮询模式 |
- [ ] 2026-MM-DD | W34D5 | io_uring vs epoll 对比 |
- [ ] 2026-MM-DD | W34D6 | 读 liburing 源码 |
- [ ] 2026-MM-DD | W34D7 | 复盘整理 |
- [ ] 2026-MM-DD | W35D1 | 连接池基础设计 |
- [ ] 2026-MM-DD | W35D2 | 连接健康检查 |
- [ ] 2026-MM-DD | W35D3 | 多后端负载均衡 |
- [ ] 2026-MM-DD | W35D4 | 后端故障摘除（熔断）|
- [ ] 2026-MM-DD | W35D5 | 连接池压测 |
- [ ] 2026-MM-DD | W35D6 | 读 gRPC 连接池设计 |
- [ ] 2026-MM-DD | W35D7 | 复盘整理 |
- [ ] 2026-MM-DD | W36D1 | 零拷贝 sendfile |
- [ ] 2026-MM-DD | W36D2 | splice + 管道零拷贝 |
- [ ] 2026-MM-DD | W36D3 | SO_REUSEPORT 多线程 accept |
- [ ] 2026-MM-DD | W36D4 | writev 散布写 |
- [ ] 2026-MM-DD | W36D5 | 月度综合压测 |
- [ ] 2026-MM-DD | W36D6 | 读 nginx 事件模型 |
- [ ] 2026-MM-DD | W36D7 | 规划 Month 10 |

## Month 10 — 协程库（ucontext Fiber）

- [ ] 2026-MM-DD | W37D1 | ucontext_t API 熟悉 |
- [ ] 2026-MM-DD | W37D2 | Fiber 类封装 |
- [ ] 2026-MM-DD | W37D3 | 栈保护 + 内存管理 |
- [ ] 2026-MM-DD | W37D4 | 协程切换性能测试 |
- [ ] 2026-MM-DD | W37D5 | 异常安全 + 非法操作检测 |
- [ ] 2026-MM-DD | W37D6 | 读 libco 源码 |
- [ ] 2026-MM-DD | W37D7 | 复盘整理 |
- [ ] 2026-MM-DD | W38D1 | 单线程调度器 |
- [ ] 2026-MM-DD | W38D2 | 协程间通信（回调 + 唤醒）|
- [ ] 2026-MM-DD | W38D3 | 工作窃取调度器（多线程）|
- [ ] 2026-MM-DD | W38D4 | 协程优先级 |
- [ ] 2026-MM-DD | W38D5 | 调度器性能测试 |
- [ ] 2026-MM-DD | W38D6 | 读 goroutine 调度器设计 |
- [ ] 2026-MM-DD | W38D7 | 复盘整理 |
- [ ] 2026-MM-DD | W39D1 | 无缓冲 Channel |
- [ ] 2026-MM-DD | W39D2 | 有缓冲 Channel |
- [ ] 2026-MM-DD | W39D3 | select 多路等待 |
- [ ] 2026-MM-DD | W39D4 | 扇入 + 扇出模式 |
- [ ] 2026-MM-DD | W39D5 | Channel 性能测试 |
- [ ] 2026-MM-DD | W39D6 | 读 Go Channel 实现 |
- [ ] 2026-MM-DD | W39D7 | 复盘整理 |
- [ ] 2026-MM-DD | W40D1 | Hook 系统调用原理 |
- [ ] 2026-MM-DD | W40D2 | 协程式 epoll 等待 |
- [ ] 2026-MM-DD | W40D3 | 协程式 sleep |
- [ ] 2026-MM-DD | W40D4 | 协程式 HTTP 客户端 |
- [ ] 2026-MM-DD | W40D5 | 月度验收 + 性能测试 |
- [ ] 2026-MM-DD | W40D6 | 读 libco / sylar 源码 |
- [ ] 2026-MM-DD | W40D7 | 规划 Month 11 |

## Month 11 — RPC 框架

- [ ] 2026-MM-DD | W41D1 | protobuf 基础 |
- [ ] 2026-MM-DD | W41D2 | RPC 帧格式设计 |
- [ ] 2026-MM-DD | W41D3 | RPC 请求/响应消息 |
- [ ] 2026-MM-DD | W41D4 | 基础 RPC Server |
- [ ] 2026-MM-DD | W41D5 | 基础 RPC Client（同步版）|
- [ ] 2026-MM-DD | W41D6 | 读 brpc 帧协议设计 |
- [ ] 2026-MM-DD | W41D7 | 复盘整理 |
- [ ] 2026-MM-DD | W42D1 | 多路复用 + 异步响应匹配 |
- [ ] 2026-MM-DD | W42D2 | 实现 Promise/Future |
- [ ] 2026-MM-DD | W42D3 | 超时与取消 |
- [ ] 2026-MM-DD | W42D4 | 重试机制 |
- [ ] 2026-MM-DD | W42D5 | 同步 vs 异步 RPC 压测 |
- [ ] 2026-MM-DD | W42D6 | 读 muduo RPC 示例 |
- [ ] 2026-MM-DD | W42D7 | 复盘整理 |
- [ ] 2026-MM-DD | W43D1 | RPC 连接池集成 |
- [ ] 2026-MM-DD | W43D2 | etcd 客户端基础 |
- [ ] 2026-MM-DD | W43D3 | 服务注册 |
- [ ] 2026-MM-DD | W43D4 | 服务发现 + 动态更新 |
- [ ] 2026-MM-DD | W43D5 | 一致性哈希负载均衡 |
- [ ] 2026-MM-DD | W43D6 | 读 etcd 服务发现最佳实践 |
- [ ] 2026-MM-DD | W43D7 | 复盘整理 |
- [ ] 2026-MM-DD | W44D1 | 拦截器（Interceptor）机制 |
- [ ] 2026-MM-DD | W44D2 | 完整端到端测试 |
- [ ] 2026-MM-DD | W44D3 | 性能压测 |
- [ ] 2026-MM-DD | W44D4 | 与 gRPC 对比 |
- [ ] 2026-MM-DD | W44D5 | 错误处理 + 状态码体系 |
- [ ] 2026-MM-DD | W44D6 | 读 gRPC 核心设计文档 |
- [ ] 2026-MM-DD | W44D7 | 规划 Month 12 |

## Month 12 — Mini Raft KV

- [ ] 2026-MM-DD | W45D1 | 搭项目骨架 + 定义消息格式 |
- [ ] 2026-MM-DD | W45D2 | 实现 RaftNode 状态机 |
- [ ] 2026-MM-DD | W45D3 | 实现选举超时计时器 |
- [ ] 2026-MM-DD | W45D4 | 实现 RequestVote RPC |
- [ ] 2026-MM-DD | W45D5 | 实现 Leader 心跳 |
- [ ] 2026-MM-DD | W45D6 | 3 节点选举联调 |
- [ ] 2026-MM-DD | W45D7 | 复盘整理 |
- [ ] 2026-MM-DD | W46D1 | 设计日志结构 |
- [ ] 2026-MM-DD | W46D2 | 实现 AppendEntries RPC |
- [ ] 2026-MM-DD | W46D3 | 实现一致性检查 |
- [ ] 2026-MM-DD | W46D4 | 实现 commit index 推进 |
- [ ] 2026-MM-DD | W46D5 | 日志复制联调 |
- [ ] 2026-MM-DD | W46D6 | 分区容错测试 |
- [ ] 2026-MM-DD | W46D7 | 复盘整理 |
- [ ] 2026-MM-DD | W47D1 | KV 状态机接口 |
- [ ] 2026-MM-DD | W47D2 | 实现 Put/Get/Delete |
- [ ] 2026-MM-DD | W47D3 | 线性一致性读（ReadIndex）|
- [ ] 2026-MM-DD | W47D4 | 持久化（WAL + LevelDB）|
- [ ] 2026-MM-DD | W47D5 | 端到端 KV 测试 |
- [ ] 2026-MM-DD | W47D6 | 性能测试 |
- [ ] 2026-MM-DD | W47D7 | 复盘整理 |
- [ ] 2026-MM-DD | W48D1 | 实现快照 Snapshot |
- [ ] 2026-MM-DD | W48D2 | 实现日志压缩 |
- [ ] 2026-MM-DD | W48D3 | 集群成员变更 |
- [ ] 2026-MM-DD | W48D4 | 混沌测试（随机 kill 节点）|
- [ ] 2026-MM-DD | W48D5 | 整体性能优化 |
- [ ] 2026-MM-DD | W48D6 | 写项目文档 |
- [ ] 2026-MM-DD | W48D7 | 十二个月回顾 |
