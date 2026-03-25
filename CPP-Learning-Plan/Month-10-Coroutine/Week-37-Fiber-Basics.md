# Week 37 — Fiber 基础（ucontext）

## 本周目标

使用 POSIX `ucontext_t` API 实现有栈协程（Fiber），理解上下文切换原理。

---

## Day 1（Mon）— ucontext_t API 熟悉

**预计时间：1 小时**

**任务：**
- [ ] 阅读并理解 4 个 API：
  ```c
  getcontext(ucontext_t *ucp);        // 保存当前上下文
  makecontext(ucp, func, argc, ...);  // 设置上下文入口函数
  swapcontext(oucp, ucp);             // 切换：保存 old，恢复 ucp
  setcontext(ucp);                    // 恢复上下文（不保存当前）
  ```
- [ ] 写第一个协程示例（两个函数交替执行）：
  ```cpp
  ucontext_t main_ctx, coro_ctx;
  char stack[64 * 1024];
  makecontext(&coro_ctx, coro_func, 0);
  // main 和 coro 交替 swapcontext
  ```
- [ ] 测试：验证两个函数交替打印

**完成标志：** 两个 ucontext 能正确切换

---

## Day 2（Tue）— Fiber 类封装

**预计时间：1 小时**

**任务：**
- [ ] 封装 Fiber 类：
  ```cpp
  class Fiber {
  public:
      enum State { READY, RUNNING, SUSPENDED, DONE };
      Fiber(function<void()> fn, size_t stack_size = 128 * 1024);
      ~Fiber();
      void resume();   // 切换到这个 Fiber 执行
      void yield();    // 让出执行权，切回调用者
      State state() const;
  private:
      ucontext_t  ctx_;
      ucontext_t* caller_ctx_;  // 调用者上下文（用于 yield 返回）
      unique_ptr<char[]> stack_;
      function<void()> fn_;
      State state_ = READY;
  };
  ```
- [ ] 处理 Fiber 正常结束（fn_ 返回后状态变为 DONE，切回 caller）
- [ ] 测试：创建 10 个 Fiber，依次 resume

**完成标志：** Fiber 生命周期管理正确（READY→RUNNING→DONE）

---

## Day 3（Wed）— 栈保护 + 内存管理

**预计时间：1 小时**

**任务：**
- [ ] 实现栈溢出检测（使用 mprotect 在栈底设置不可访问页）：
  ```cpp
  // 分配栈并保护底部
  void* stack = mmap(nullptr, stack_size + PAGE_SIZE,
                     PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
  mprotect(stack, PAGE_SIZE, PROT_NONE);  // 保护底部一页
  ```
- [ ] 分析默认栈大小的选择（128KB 是个合理起点）
- [ ] 实现 FiberPool：预分配固定数量 Fiber，避免频繁 mmap

**完成标志：** 栈溢出时触发 SIGSEGV，而不是静默损坏数据

---

## Day 4（Thu）— 性能测试：协程切换开销

**预计时间：1 小时**

**任务：**
- [ ] 测量协程切换开销（vs 线程切换）：
  ```cpp
  // 1 百万次 swapcontext，计时
  for (int i = 0; i < 1000000; i++) {
      swapcontext(&main_ctx, &coro_ctx);
      swapcontext(&coro_ctx, &main_ctx);
  }
  // 对比：同样次数的线程切换（futex + 内核调度）
  ```
- [ ] 期望结果：协程切换 ~10-50ns，线程切换 ~1-10μs

**完成标志：** 协程切换比线程切换快 20 倍以上

---

## Day 5（Fri）— 异常安全 + 非法操作检测

**预计时间：1 小时**

**任务：**
- [ ] 处理 Fiber 内部抛出异常（捕获后设置 state = DONE，在 resume() 重新抛出）
- [ ] 检测非法操作：
  - resume 已 DONE 的 Fiber → 抛 logic_error
  - yield 在非 Fiber 上下文中调用 → 抛 logic_error
- [ ] 测试：各种异常场景的行为正确

**完成标志：** 异常安全，非法操作有明确错误提示

---

## Day 6（Sat）— 阅读 libco 源码（腾讯 C++ 协程库）

**预计时间：2 小时**

**任务：**
- [ ] 阅读 libco `co_routine.cpp`（重点：协程切换的汇编实现）
- [ ] 回答：
  1. libco 为什么用汇编实现 `co_swap` 而不用 `swapcontext`（性能：汇编 ~5ns vs ucontext ~50ns）？
  2. libco 如何用 TLS（线程局部存储）管理每个线程的当前协程？
  3. libco 的 hook 机制（hook read/write 等系统调用）是如何实现的？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理 ucontext vs setjmp/longjmp vs C++20 协程的对比
- [ ] `git commit`，推送
