# lock_free

Week 07（Day 043 - Day 049）对应的练习代码统一放在这个目录。

## 覆盖内容

- Day 043: memory_order 发布/观察实验
- Day 044: Treiber 风格无锁栈骨架
- Day 045: SPSC 无锁环形队列
- Day 046: ABA 与 tagged pointer 对比实验
- Day 047: SPSCQueue / BlockingQueue / BoundedQueue 1P1C benchmark
- Day 048: 贴近教材的 shared_ptr + 延迟回收无锁栈
- Day 049: 统一测试入口与结果汇总输出

## 三版无锁栈

### LockFreeStack

- 目标：保留 Day 44 的 Treiber 栈骨架，突出 CAS 更新 head 的主线
- 回收策略：`pop()` 成功后先放进 retired 列表，析构时统一释放
- 适合场景：理解 ABA 和“结构更新不等于安全回收”这件事
- 当前验证：单线程 push/pop，外加并发 push 后单线程 pop 校验数量与唯一性

### ReclaimingLockFreeStack

- 目标：贴近《C++ Concurrency in Action》里“更新协议”和“回收协议”分开设计的写法
- 回收策略：`threads_in_pop_ + to_be_deleted_` 延迟回收；只有确认没有其他线程仍处在 `pop()` 临界路径时才删除旧节点
- 返回值：`pop()` 返回 `std::shared_ptr<T>`，让读到的数据对象继续独立存活
- 当前验证：单线程 push/pop，外加多生产者多消费者并发 push/pop 校验无重复、无丢失

### TaggedPointerStack

- 目标：把 Day 46 的 tagged pointer 思路落成一个可运行栈原型，让 `head` 的 CAS 从“只比较指针”升级成“比较指针 + tag”
- 回收策略：仍然沿用 retired 列表，析构时统一释放；它解决的是 ABA 比较阶段，不单独解决并发回收
- 适合场景：对比 tagged pointer 和教材版延迟回收方案分别在解决哪一层问题
- 当前验证：单线程 push/pop，外加 4P1C push-drain benchmark

## 测试范围

- `main.cpp` 会先跑 Day 43/44/45/46/48 的功能测试
- 无锁栈额外包含两组 stress test：
  - `LockFreeStack`：4 个生产者并发 push，随后单线程完整 pop
  - `TaggedPointerStack`：单线程 push/pop，聚焦 ABA 防御原型本身
  - `ReclaimingLockFreeStack`：4 个生产者并发 push，4 个消费者并发 pop
- benchmark 维持 Day 47 / Day 49 的统一口径：100 万次 `int` 交接、1P1C

## benchmark

- 队列 benchmark：`std::queue`、`BlockingQueue`、`BoundedQueue`、`SPSCQueue`，口径都是 100 万次 `int` 交接、1P1C
- 栈 benchmark：
  - `LockFreeStack_4P1C_push_drain`：4 个生产者并发 push，随后单线程 drain
  - `TaggedStack_4P1C_push_drain`：同样是 4P1C 口径，用来观察 tagged CAS 的额外成本
  - `ReclaimStack_4P1C_push_drain`：和上面同口径，方便直接比较回收协议成本
  - `ReclaimStack_4P4C_push_pop`：4 生产者 + 4 消费者并发 push/pop，体现教材版栈在完整并发 pop 场景下的可用性
- 这里的栈 benchmark 重点是比较“骨架实现”“tagged pointer 防 ABA”“带回收协议实现”三种思路分别增加了什么成本，不把它误写成泛化的 MPMC 容器结论

## 构建

```powershell
cmake -S CPP-Practice/lock_free -B CPP-Practice/lock_free/build
cmake --build CPP-Practice/lock_free/build --config Release
./CPP-Practice/lock_free/build/Release/lock_free_demo.exe
```
