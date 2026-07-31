# Reactor 框架（单线程）

## 本周目标

实现单线程 Reactor 框架（EventLoop + Channel + Poller + Acceptor），这是 muduo 的核心骨架。

## 本周完成情况

- [x] Day 204：定义 Channel，封装 fd 与回调
- [x] Day 205：定义 Poller 抽象并用 FakePoller 做可测试实现
- [x] Day 206：实现 EventLoop 单次事件分发
- [x] Day 207：验证 Channel read callback 被正确触发
- [x] Day 208：整理 Reactor 与 Proactor 区别
- [x] Day 209：梳理 muduo EventLoop/Channel/Poller 分层
- [x] Day 210：完成 Week30 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `Channel` / `FakePoller` / `EventLoop`
- 周复盘：`Note/C++-Note/Week30-单线程Reactor-复盘总结.md`

## 阶段小结
Week30 完成单线程 Reactor 的核心抽象。Reactor 的职责不是处理业务，而是等待事件、分发事件、调用注册回调。`Channel` 代表一个 fd 上关心的事件，`Poller` 负责返回活跃 Channel，`EventLoop` 负责调度。FakePoller 让这套结构能在 Windows 下被单元测试验证。

## 验证命令

```powershell
cmake --build CPP-Practice/network_reactor/build --config Release
.\CPP-Practice\network_reactor\build\Release\network_reactor_demo.exe
```

---