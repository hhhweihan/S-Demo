# C++ 内存模型与无锁编程

## 本周目标

真正理解 6 种 memory_order，写一个正确且高效的 SPSC 无锁队列。

## 本周完成情况

- [x] Day 43：理解 relaxed、release/acquire 与 TSan 检查边界
- [x] Day 44：推演无锁栈与 ABA 风险来源
- [x] Day 45：整理 SPSC 队列的最小正确设计
- [x] Day 46：理解 tagged pointer 与 SPSC 天然规避 ABA 的原因
- [x] Day 47：归纳 SPSC 与有锁队列的性能差异来源
- [x] Day 48：阅读《C++ Concurrency in Action》第 7 章并做方案对比
- [x] Day 49：完成本周复盘与打卡
- [x] Day 50：Hazard Pointer 有界安全回收（SMR）+ 修复弱序机 UAF
- [x] Day 51：Vyukov 无锁有界 MPMC 队列

## 本周关键结论

1. 原子变量能避免数据竞争，但不能替代同步语义设计。
2. release/acquire 是无锁结构里最常用、最稳妥的发布-订阅模式。
3. ABA 的核心问题是历史被掩盖，tagged pointer 只解决比较阶段，不自动解决回收阶段。
4. SPSC 之所以适合入门无锁编程，是因为它把共享状态压缩成两个单写者索引。
5. 无锁不等于总是更好；只有热点路径明确受锁开销限制时，才值得承担额外复杂度。
6. "无锁更新"和"安全回收"是两套独立问题：CAS 管前者，hazard pointer / epoch 管后者，把 `retired_` 无界增长压成有界。
7. Hazard pointer 的正确性建立在 seq_cst 全序上：`protect` 发布/校验、摘除 CAS、retire 入链、scan 取链与快照必须同为 seq_cst，且 scan 要**先取链再快照**——任何降级或颠倒都会在 ARM 等弱序机上暴露 use-after-free（x86 掩盖）。
8. 定长环形结构（Vyukov MPMC）用容量上界换掉了整个内存回收问题：元素原地存储、per-cell 序列号一次 CAS 同时判满/空/抢占，无节点分配即无 ABA。

## 本周文件

- [043.md](./043.md)
- [044.md](./044.md)
- [045.md](./045.md)
- [046.md](./046.md)
- [047.md](./047.md)
- [048.md](./048.md)
- [049.md](./049.md)
- [050.md](./050.md) — Hazard Pointer 有界回收 + seq_cst 全序证明
- [051.md](./051.md) — Vyukov 无锁有界 MPMC 队列

---