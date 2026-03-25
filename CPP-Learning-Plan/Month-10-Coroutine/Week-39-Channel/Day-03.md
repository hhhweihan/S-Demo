## Day 3（Wed）— select 多路等待

**预计时间：1 小时**

**任务：**
- [ ] 实现 `select` 语句（等待多个 Channel 中第一个就绪的）：
  ```cpp
  // 目标用法：
  select(
      on_recv(ch1, [](int v){ ... }),
      on_recv(ch2, [](string s){ ... }),
      on_send(ch3, 42, []{ ... }),
      default_([]{ ... })  // 可选 default（非阻塞）
  );
  ```
- [ ] 实现原理：将当前 Fiber 注册到所有备选 Channel 的等待列表，
        任意一个就绪时唤醒（并从其他 Channel 取消注册）
- [ ] 测试：select 从两个 Channel 中接收，先就绪的优先

**完成标志：** select 语义正确，无死锁

---

