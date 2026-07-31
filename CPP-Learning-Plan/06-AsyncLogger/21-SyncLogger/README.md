# 同步日志基础

## 本周目标

构建同步日志系统骨架：日志级别、格式化、流式接口、线程安全写入。

## 本周完成情况

- [x] Day 141：定义 DEBUG/INFO/WARN/ERROR/FATAL 日志级别
- [x] Day 142：实现时间戳、线程 ID、文件行号格式化
- [x] Day 143：实现 `LOG_INFO << value` 风格流式接口
- [x] Day 144：实现线程安全同步写入
- [x] Day 145：接入内存 sink 做可断言测试
- [x] Day 146：整理同步 logger 接口和边界
- [x] Day 147：完成 Week21 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：`CPP-Practice/async_logger/`
- 核心文件：`CPP-Practice/async_logger/mini_logger.h`
- 周复盘：`Note/C++-Note/Week21-同步日志基础-复盘总结.md`

## 阶段小结
Week21 完成同步日志骨架。同步 logger 的优点是语义简单：调用线程完成格式化并直接写入 sink；缺点是锁竞争和 IO 会进入业务路径。本周重点是把日志级别、时间戳、线程 ID、文件行号和流式接口先做成稳定抽象，为后续异步化提供一致前端。

## 验证命令

```powershell
cmake -S CPP-Practice/async_logger -B CPP-Practice/async_logger/build
cmake --build CPP-Practice/async_logger/build --config Release
.\CPP-Practice\async_logger\build\Release\async_logger_demo.exe
```

---
