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

