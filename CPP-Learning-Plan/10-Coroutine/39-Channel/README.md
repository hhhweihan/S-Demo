# Channel 通信

## 本周目标

实现 Go 风格的 Channel（无缓冲 + 有缓冲），以及 select 多路等待，
作为协程间通信的核心原语。

## 本周完成情况

- [x] Day 267：实现 `Channel<T>` 基础结构
- [x] Day 268：实现缓冲 Channel send/recv
- [x] Day 269：实现 `select_recv` 多路等待示例
- [x] Day 270：验证协程生产消息，Channel 接收消息
- [x] Day 271：梳理无缓冲 Channel 同步握手机制
- [x] Day 272：整理 Go channel 与 C++ coroutine 的差异
- [x] Day 273：完成 Week39 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `Channel<T>`
- `select_recv`
- 周复盘：`Note/C++-Note/Week39-Channel通信-复盘总结.md`

## 阶段小结
Week39 完成缓冲 Channel 和 select 风格接收。Channel 是协程之间传递数据和同步的核心抽象；当前版本实现了非阻塞 send/recv 和多个 channel 的轮询接收。真正 Go 风格的无缓冲阻塞握手需要与 Scheduler 深度集成，后续可继续扩展。

## 验证命令

```powershell
cmake --build CPP-Practice/coroutine_lib/build --config Release
.\CPP-Practice\coroutine_lib\build\Release\coroutine_lib_demo.exe
```

---
