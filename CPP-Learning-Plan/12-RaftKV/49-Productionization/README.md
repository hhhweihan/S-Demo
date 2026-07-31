# Raft 生产化（旗舰改造）

## 本阶段目标

把「能跑的离散事件 Raft 仿真」升级为**贴近生产**的实现：以 `raft_sim.h`（真 per-node RPC handler +
虚拟时间网络 + 每步安全不变量校验）为唯一核心，补齐四块硬骨头——真持久化、快照/日志压缩、
线性一致读、防扰动预投票。全部在本机 macOS(arm64)/libc++ 上以 POSIX 文件 IO 可完整验证。
成员变更（joint consensus）明确推迟到后续独立旗舰。

## 本阶段完成情况

- [x] 编号 337：Storage 抽象与持久化边界 —— crash 丢全部易失态，restart 从磁盘真重载（Mem/File 双实现 + 原子写 + CRC）
- [x] 编号 338：快照 + 日志压缩 + InstallSnapshot —— snapshot-aware 索引偏移、落后 follower 追赶
- [x] 编号 339：ReadIndex 线性一致读 —— 记 readIndex → 不同节点多数派确认 → 等 apply 追上
- [x] 编号 340：Pre-Vote —— 分区节点重连不抬 term、不扰动在任 Leader，且不牺牲崩溃后活性
- [x] 编号 341：生产化测试范式 —— 确定性仿真 + 每步不变量，6 个 RaftProd 用例，成员变更推迟说明

## 本阶段产出

- 代码：`CPP-Practice/raft_kv/raft_storage.h`（Storage 抽象 + MemStorage/FileStorage + CRC 序列化）
  + 改造的 `raft_sim.h`（真持久化、snapshot/InstallSnapshot、ReadIndex、Pre-Vote、snapshot-aware 索引）
- 测试：`CPP-Practice/raft_kv/tests/raft_chaos_tests.cpp`（5 个既有 chaos 保绿 + 6 个 RaftProd 新增）
- `mini_raft_kv.h` 降级为文档标注的上帝视角教学基线；测试 target 只依赖 `raft_sim.h`

## 阶段小结

生产化的地基是**把「哪些状态必须活过掉电」变成代码强制的边界**：`currentTerm/votedFor/log/snapshot`
经 `Storage` 落盘，`crash()` 丢弃全部易失态，`restart()` 从 `Storage::load()` 真磁盘重建——忘了持久化
的字段会真的丢、被测试当场抓住，而不是靠活对象「魔法保留」蒙混过关。在此之上：快照把无界日志压缩成
`(lastIncludedIndex, lastIncludedTerm)` + 状态机 blob，所有索引换算变成 snapshot-aware，Leader 在
`nextIndex[peer] <= lastIncludedIndex` 时改发 InstallSnapshot 让落后 follower 追平；ReadIndex 用一轮
「不同节点多数派」的心跳确认领导权、不写日志就服务线性一致读，被分区的旧 Leader 因凑不齐确认而读不可用；
Pre-Vote 用一轮不递增 term 的预投票拦住分区回归节点，避免它虚高的 term 扰动健康集群。

关键收获：

1. 持久 vs 易失的红线是崩溃恢复正确性的根——`commitIndex/lastApplied` 故意不持久，重启重放重建。
2. snapshot-aware 索引的全部难点是一次偏移换算 `entryAt(i)=log[i-firstIndex()]`，`termAt(lastIncludedIndex)` 要回退到元信息。
3. ReadIndex 的多数派确认必须按**不同节点**去重，否则少数派 Leader 靠重复回复凑数，破坏线性一致。
4. Pre-Vote 不碰持久状态，只在预投票过半时才升 term——既防扰动又不牺牲崩溃后选举活性。
5. 确定性离散事件 + 种子化 RNG + 每步不变量校验，是让「时序 bug 可复现、可最小化」的测试范式。

## 验证命令

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target raft_kv_tests -j
ctest --test-dir build -R raft_kv --output-on-failure    # 11 个用例（5 chaos + 6 prod）

# ASan/UBSan（序列化/反序列化的缓冲算术 + snapshot 索引换算是风险点）
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DS_DEMO_SANITIZER=address
cmake --build build-asan --target raft_kv_tests -j
./build-asan/CPP-Practice/raft_kv/raft_kv_tests
```

---
