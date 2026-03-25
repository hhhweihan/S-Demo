## Day 7（Sun）— 复盘整理

**预计时间：1.5 小时**

**任务：**
- [ ] 给本周代码补充注释
- [ ] 思考：如果要实现 TLS（线程局部）cache，需要改哪里？（写思路，不用实现）
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**Benchmark（32 字节对象，分配/释放 100 万次）：**
| 方案 | 耗时 |
|------|------|
| new/delete | — ms |
| PoolAllocator 单线程 | — ms |
| PoolAllocator 4 线程竞争 | — ms |

**读文档笔记：**

1. arena 解决的问题：
2. size class 划分：
3. tcache 作用：
4. 与我的实现的差距：