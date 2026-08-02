# 线程池

## 本周目标

实现一个支持 `future` 返回值和动态扩缩容的线程池。

## 本周完成情况

- [x] Day 36：实现固定线程池，打通 submit -> queue -> worker 的执行闭环
- [x] Day 37：支持 `future` 返回值，用 `packaged_task` 连接任务执行和结果回收
- [x] Day 38：完善优雅关闭，明确排空任务、停止接收和 worker 退出顺序
- [x] Day 39：实现动态扩缩容，区分目标线程数、活跃线程数和退出 token
- [x] Day 40：完成压力测试，记录轻任务场景下线程池的真实调度成本
- [x] Day 41：对比 folly executor，理解工业实现的队列、退出和可观测性差异
- [x] Day 42：补齐统计接口和概念复盘
- [x] Day 43：升级为 work-stealing 线程池（Chase-Lev deque + 全局兜底 + 热点计数隔离）

## 本周产出

- 线程池工程：[CPP-Practice/thread_pool](../../../CPP-Practice/thread_pool)
- 周复盘：[Note/C++-Note/线程池-复盘总结.md](../../../Note/C++-Note/线程池-复盘总结.md)

## 阶段小结
Week 06 的核心不是“开几个线程执行任务”，而是把任务提交、结果回收、worker 生命周期、优雅关闭、动态扩缩容和状态观测整理成一个可演进的执行框架。Week 05 的队列只解决了任务交接，本周把交接前后的接口语义和生命周期协议都接上了。

本周最重要的收获有六点：

1. 线程池的本质是任务执行协议，不是线程集合。
2. `packaged_task + future` 是线程池返回值语义最自然的组合，异常也能顺着 `future` 传播。
3. 关闭路径比执行路径更容易出问题，必须先排空、再停止接收、再让 worker 退出。
4. 动态扩缩容是小型状态机问题，不能只用“当前线程数”一个变量描述。
5. `snapshot_stats()` 这类可观测接口不是锦上添花，而是调试动态行为的前提。
6. 对极轻任务，线程池调度成本可能压过并发收益；线程更多不等于吞吐更高。
7. 单全局队列在任务派生子任务时会成为争用与缓存热点；work-stealing 用"每 worker 私有 deque + 全局兜底"把常路争用降到零。
8. Chase-Lev 的 owner(LIFO/bottom) 与 thief(FIFO/top) 各据一端，仅最后一个元素靠 seq_cst 全序裁决归属。
9. 通用 work-stealing deque 的槽位必须原子访问：steal 的投机读与 push 写同址是数据竞争，存 `atomic<T*>` 指针（赢 CAS 后才解引用）是对任意 T 的正确修法。
10. 高频热点计数器要 cache-line 隔离，否则 false sharing 会吃掉并发收益。

## 本周文件

- [036.md](./036.md) ~ [042.md](./042.md)：固定池 → future → 优雅关闭 → 动态扩缩容 → 压测/对比/复盘
- [043.md](./043.md) — Work-Stealing：Chase-Lev deque + 全局兜底 + 热点计数隔离

---