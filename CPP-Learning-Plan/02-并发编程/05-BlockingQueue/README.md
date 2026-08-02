# 线程安全阻塞队列

## 本周目标

实现线程安全的阻塞队列，这是线程池的核心组件。
掌握 `std::mutex` + `std::condition_variable` 的正确用法。

## 本周完成情况

- [x] Day 29：实现/理解 `JoiningThread`，把线程回收纳入 RAII
- [x] Day 30：设计 `BlockingQueue<T>`，明确 `push`、`pop`、`shutdown` 协议
- [x] Day 31：编写生产者消费者测试，验证数量、总和和退出路径
- [x] Day 32：引入 `BoundedQueue<T>`，用容量上限表达背压
- [x] Day 33：设计 blocking/bounded queue benchmark，量化同步成本
- [x] Day 34：阅读 folly MPMCQueue，建立有锁队列和工业无锁队列的对比框架
- [x] Day 35：完成本周复盘，提炼线程池前置规则

## 本周产出

- 周复盘：[Note/C++-Note/阻塞队列-复盘总结.md](../../../Note/C++-Note/阻塞队列-复盘总结.md)
- 后续代码承接：[CPP-Practice/thread_pool](../../../CPP-Practice/thread_pool)

## 阶段小结
Week 05 表面上是在做阻塞队列，实际主线是“线程之间如何安全交接工作，并且能够可靠退出”。这一周把 Month 1 的 RAII 思路带进并发环境：线程对象需要托管，阻塞等待需要谓词，队列关闭需要协议，生产速度大于消费速度时需要背压。

本周最重要的收获有五点：

1. `std::thread` 析构时如果仍然 `joinable()` 会 `std::terminate`，线程本身就是资源。
2. `condition_variable` 等待的是共享状态谓词，不是某次通知本身。
3. 并发组件必须先定义结束协议，否则测试和真实运行都容易卡死。
4. `BoundedQueue` 的容量不是实现细节，而是系统流量控制策略。
5. 队列 benchmark 的意义不是证明“多线程一定快”，而是量化同步、阻塞和背压的成本。

---