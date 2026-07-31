# 定时器轮 + 心跳检测

## 本周目标

实现时间轮（Timing Wheel）定时器，集成到 Reactor 事件循环中，
用于连接心跳检测和超时清理。

## 本周完成情况

- [x] Day 225：理解时间轮 slot 与 tick 推进模型
- [x] Day 226：实现 `TimerWheel::add_or_refresh`
- [x] Day 227：实现连接空闲超时回调
- [x] Day 228：实现精确定时器最小堆
- [x] Day 229：验证刷新连接后按最新过期时间清理
- [x] Day 230：整理心跳检测和 Reactor 集成方式
- [x] Day 231：完成 Week33 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `TimerWheel`
- `PrecisionTimerHeap`
- 周复盘：`Note/C++-Note/Week33-TimerWheel-复盘总结.md`

## 阶段小结
Week33 完成定时器轮和精确定时器。时间轮适合大量连接的粗粒度超时管理，刷新连接只需更新过期 tick；最小堆适合数量较少但要求更精确的定时任务。当前 demo 验证了刷新、过期回调和按 deadline 弹出。

## 验证命令

```powershell
cmake -S CPP-Practice/high_perf_net -B CPP-Practice/high_perf_net/build
cmake --build CPP-Practice/high_perf_net/build --config Release
.\CPP-Practice\high_perf_net\build\Release\high_perf_net_demo.exe
```

---
