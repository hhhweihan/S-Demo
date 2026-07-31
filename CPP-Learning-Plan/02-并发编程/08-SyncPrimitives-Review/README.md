# 高级同步原语 + 整月复盘

## 本周目标

实现读写锁、CountDownLatch，整合进线程池，补齐暂停/恢复控制，完成整月 benchmark 与复盘。

## 本周产出

- 代码实现落在 [CPP-Practice/thread_pool/concurrent/rw_mutex.h](../../../CPP-Practice/thread_pool/concurrent/rw_mutex.h)、[CPP-Practice/thread_pool/concurrent/countdown_latch.h](../../../CPP-Practice/thread_pool/concurrent/countdown_latch.h)、[CPP-Practice/thread_pool/concurrent/thread_pool.h](../../../CPP-Practice/thread_pool/concurrent/thread_pool.h)
- Week 08 相关测试与 benchmark 入口统一放在 [CPP-Practice/thread_pool/main.cpp](../../../CPP-Practice/thread_pool/main.cpp)
- 并发整月总结文章见 [Note/C++-Note/Month02-并发编程实战总结.md](../../../Note/C++-Note/Month02-并发编程实战总结.md)

## 验证结论

- Day 50: 4 读 1 写压力测试通过，读写互斥逻辑正确
- Day 51: CountDownLatch 支持 wait、wait_for、count_up/count_down，5 任务汇合测试通过
- Day 52: ThreadPool 新增 pause/resume，暂停期间新提交任务会阻塞，resume 后恢复提交并最终完成
- Day 53: 当前 Windows + MSVC 环境缺少 clang/TSan，已完成可执行压力验证并记录后续 Linux/WSL 验证命令
- Day 54: 已补齐 BlockingQueue、SPSCQueue、ThreadPool QPS 和提交延迟数据

## Day 54 数据

- BlockingQueue 1P1C 100 万次: 214.873 ms, 4.654 Mops/s
- SPSCQueue 1P1C 100 万次: 43.592 ms, 22.940 Mops/s
- ThreadPool 4 线程 100 万轻量任务: 363.874 ms, 2.748 Mops/s
- ThreadPool 单任务提交延迟: p50 1.200 us, p99 45.200 us

---