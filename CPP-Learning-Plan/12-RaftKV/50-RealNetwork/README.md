# Raft 真实网络化

## 本阶段目标

把"能跑的确定性仿真"补上一课"真实网络"：每个节点变成独立 OS 进程，用 `127.0.0.1` 上的
真 TCP + 真 `epoll` 事件循环通信，可以真的 `kill -9` 一个节点、观察剩余多数派重新选主、
重启后从磁盘恢复。**不改动 `raft_sim.h` 与现有 12 个 chaos/生产化测试**——新写一份独立的
单节点真实 Raft 服务器，只复用 `raft_storage.h` 里与"仿真 vs 真磁盘"无关的纯数据/持久化层。

## 本阶段完成情况

- [x] 编号 342：为什么要真实网络化 —— 仿真版验证不了什么，四个架构决策（进程模型/事件循环/线格式/复用边界）
- [x] 编号 343：为什么 TCP 需要长度前缀帧 —— 字节流没有消息边界，`net/codec.h` 帧格式，复用 `raft_storage.h` 编解码 helper
- [x] 编号 344：epoll + timerfd 把定时器变成一等公民 —— LT vs ET、`eventfd` 自管道停止、非阻塞 `connect` 两段式状态机
- [x] 编号 345：单节点真实状态机 —— 连接去重（低 id 拨高 id）、真实熵源随机化、ReadIndex 延迟响应模式
- [x] 编号 346：真进程 kill -9 验证了什么 —— SIGKILL 语义、CTest `integration` label 隔离策略

## 本阶段产出

- 代码：`CPP-Practice/raft_kv/net/codec.h`（RPC 帧编解码）、`net/event_loop.h`（epoll+timerfd 封装）、
  `net/raft_server.h`（单节点真实 Raft 服务器）、`raft_net_main.cpp`（`raft_kv_net_demo` 可执行入口）
- 测试：`tests/raft_net_codec_tests.cpp`（编解码 round-trip，跑在默认 CI）、
  `tests/raft_net_integration_test.cpp`（fork+exec+kill -9+重启，打 `integration` label，默认 CI 排除）
- 构建：`CMakeLists.txt` 新增 `raft_kv_net_demo` 可执行目标 + `raft_kv_net_integration_test` 测试目标；
  `.github/workflows/ci.yml` 三个 `ctest` 调用加 `-LE integration`
- 文档：`CPP-Practice/raft_kv/README.md` 新增"真实网络版"一节（三终端手动跑 + 崩溃测试说明）

## 阶段小结

真实网络化补的是仿真版天然补不上的一课：TCP 是字节流没有消息边界，因此需要长度前缀帧
（编号 343）；一个真实节点同时有多个独立到期时间，因此定时器要用 `timerfd` 变成和普通 fd
一样的一等公民而不是塞进 `epoll_wait` 的 timeout 参数（编号 344）；两个节点互相都想拨号
对方会建出冗余连接，因此需要"低 id 拨高 id"的静态去重约定，而每条帧自带 `from` 让拨号方向
之后完全不影响路由（编号 345）；线性一致读在真实异步世界里不能像仿真版那样同步走完整个
"确认领导权"流程，必须变成"记住上下文、由后续事件触发响应"的延迟响应模式（编号 345）；
最强的崩溃验证是真的把进程 `kill -9` 掉——没有析构、没有 `atexit`，唯一幸存的只有真正
`fsync` 过的字节，这是 `Node` 对象重新构造永远验证不到的一层（编号 346）。

关键收获：

1. 仿真版验证协议逻辑，真实网络版验证操作系统语义（TCP 语义、进程语义、非阻塞 IO 状态机）——
   两者互补，不是谁淘汰谁，所以选择新写一份而不是改造 `Cluster`。
2. 长度前缀帧 + 自带 `from` 字段，让"消息边界"和"连接是谁拨的"这两个问题被彻底解耦。
3. `timerfd`/`eventfd` 把"定时器到期"和"外部请求停止"都统一成普通 fd 事件，是生产网络代码
   驱动多个独立截止时间、且能被信号/其他线程安全停止的标准写法。
4. 真实异步世界里"逻辑上同步、物理上要等"的请求（ReadIndex 的领导权确认）必须做成
   延迟响应，同步调用的错觉只有仿真器才能提供。
5. 真实时钟/真实进程调度的测试天然比确定性仿真慢、理论上更容易抖动，用 CTest label 隔离
   出默认 CI 之外是恰当的处理，不是"因为怕抖动就不写"。

## 验证命令

```bash
# 默认路径：12 个仿真用例 + codec 单测，不含 integration，秒级完成
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_tests raft_kv_net_demo -j
ctest --test-dir build -R raft_kv --output-on-failure -LE integration

# 真实网络版手动验收（三终端）+ 真崩溃测试：见 CPP-Practice/raft_kv/README.md「真实网络版」一节
cmake --build build --target raft_kv_net_integration_test -j
ctest --test-dir build -L integration --output-on-failure   # 真进程 kill -9 + 重启恢复，独立跑
```

---
