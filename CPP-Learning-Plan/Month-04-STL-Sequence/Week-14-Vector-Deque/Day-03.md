## Day 3（Wed）— insert + erase + 异常安全

**预计时间：1 小时**

**任务：**
- [ ] 实现 `insert(pos, val)`：pos 之后的元素右移，可能触发扩容
- [ ] 实现 `erase(pos)` 和 `erase(first, last)`：左移填补
- [ ] 异常安全保证：`push_back` 若 T 的构造函数抛异常，vector 的状态不变（strong guarantee）
  - 技巧：先在新空间里构造，成功后再更新指针
- [ ] 实现 `emplace_back(args...)` 和 `emplace(pos, args...)`（原地构造）

**完成标志：** 随机插入/删除结果正确，ASan 无报错

---

