## Day 4（Thu）— commit index 推进

**预计时间：1 小时**

**任务：**
- [ ] Leader 收到 AppendEntriesResponse：
  ```cpp
  if (resp.success) {
      match_index_[peer] = prev_index + entries.size();
      next_index_[peer]  = match_index_[peer] + 1;
      // 尝试推进 commit_index
      try_advance_commit_index();
  } else {
      // 一致性检查失败，回退 next_index
      next_index_[peer]--;
      replicate_log(peer);  // 重试
  }
  ```
- [ ] `try_advance_commit_index()`：
  ```cpp
  // 找最大的 N，满足：
  // 1. N > commit_index_
  // 2. 多数节点的 match_index >= N
  // 3. log[N].term == current_term（只 commit 当前 term 的日志）
  ```
- [ ] commit 后，把 log[commit_index_+1..new_commit] 应用到状态机（Week 23 实现状态机）

**完成标志：** 多数确认后 commit_index 正确推进

---