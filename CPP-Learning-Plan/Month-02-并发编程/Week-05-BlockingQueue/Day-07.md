## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理 condition_variable 的正确用法（while 判断 vs if，notify_one vs notify_all）
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**性能测试（100 万个 int 交换）：**

| 方案 | 1P1C 耗时 | 4P4C 耗时 |
|------|----------|----------|
| std::queue（单线程基准）| — ms | — |
| BlockingQueue | — ms | — ms |
| BoundedQueue (cap=1000) | — ms | — ms |

**folly MPMCQueue 笔记：**

1. MPMC 与 BlockingQueue 的本质区别：
2. folly 替代 mutex 的机制：
3. MPMC 为何比 SPSC 复杂：