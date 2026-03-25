## Day 2（Tue）— submit 返回 future

**预计时间：1 小时**

**任务：**
- [ ] 修改 `submit`，支持任意可调用对象，返回 `std::future`：
  ```cpp
  template<typename F, typename... Args>
  auto submit(F&& f, Args&&... args)
      -> std::future<std::invoke_result_t<F, Args...>>
  {
      using RetType = std::invoke_result_t<F, Args...>;
      auto task = std::make_shared<std::packaged_task<RetType()>>(
          std::bind(std::forward<F>(f), std::forward<Args>(args)...)
      );
      std::future<RetType> fut = task->get_future();
      queue_.push([task]{ (*task)(); });
      return fut;
  }
  ```
- [ ] 测试：提交 10 个 lambda（各自返回不同数字），用 future.get() 收集结果并验证

**完成标志：** future.get() 能拿到正确的返回值

---