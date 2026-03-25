## Day 3（Wed）— AddressSanitizer 扫描

**预计时间：1 小时**

**任务：**
- [ ] 用以下编译选项重新编译前三周所有代码：
  ```bash
  g++ -fsanitize=address -fno-omit-frame-pointer -g -O1 your_test.cpp
  ```
  （Windows 上如果用 MSVC 则用 `/fsanitize=address`）
- [ ] 运行所有测试，记录 ASan 报出的每一个问题：
  - heap-use-after-free
  - heap-buffer-overflow
  - memory leak
- [ ] 把每个问题的报告截图或复制到本文件底部

**完成标志：** 跑完所有测试，记录了所有报错

---