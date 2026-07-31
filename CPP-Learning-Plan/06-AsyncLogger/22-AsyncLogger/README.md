# 双缓冲异步日志

## 本周目标

实现双缓冲（Double Buffering）异步日志，业务线程写前台缓冲，后台线程将满缓冲
刷到磁盘，消除同步日志的锁竞争和 IO 阻塞。

## 本周完成情况

- [x] Day 148：实现前台缓冲区收集日志行
- [x] Day 149：实现后台线程和条件变量唤醒
- [x] Day 150：实现批量交换与 flush
- [x] Day 151：实现析构 stop，确保后台线程干净退出
- [x] Day 152：完成多线程正确性和 enqueue latency smoke test；5 倍吞吐留待专项压测
- [x] Day 153：整理异步 logger 数据流和关闭流程
- [x] Day 154：完成 Week22 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `AsyncLogger`：前台缓冲、后台线程、条件变量、批量写 sink
- 多线程 4x2000 条日志无丢失验证
- 周复盘：`Note/C++-Note/Week22-双缓冲异步日志-复盘总结.md`

## 阶段小结
Week22 把日志写入从业务线程拆到后台线程。业务线程只负责格式化和追加到前台缓冲，后台线程被唤醒后交换批量数据并写入 sink（条件变量无超时等待，事件驱动，仅由新日志或停止唤醒）。这个结构降低了业务线程直接等待 IO 的概率，但性能结论必须依赖稳定 benchmark；当前完成的是正确性和 smoke test，不把它包装成百万级吞吐结论。

## 验证命令

```powershell
cmake --build CPP-Practice/async_logger/build --config Release
.\CPP-Practice\async_logger\build\Release\async_logger_demo.exe
```

---
