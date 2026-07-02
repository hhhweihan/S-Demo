# Week38 协程调度器复盘总结

## 核心目标

Week38 实现协作式调度器：ready queue、yield 和 resume。

## 调度模型

`Scheduler` 保存可运行协程句柄。`yield` awaiter 在挂起时把当前协程重新放回 ready queue，调度器循环取出句柄并 resume。

## 关键语义

- 协程不会被抢占，只在 `co_await` 点主动让出。
- ready queue 决定恢复顺序。
- 调度器必须保证不 resume 已完成协程。

## 复盘结论

协程调度器的核心不是线程，而是状态机恢复顺序。先实现单线程协作式调度，再扩展多线程工作窃取或 IO 唤醒会更稳。
