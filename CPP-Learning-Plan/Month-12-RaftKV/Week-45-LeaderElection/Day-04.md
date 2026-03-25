## Day 4（Thu）— 收集投票 + 成为 Leader

**预计时间：1 小时**

**任务：**
- [ ] Candidate 收到 RequestVoteResponse：
  ```cpp
  void handle_vote_response(RequestVoteResponse resp) {
      if (resp.term > current_term) { become_follower(resp.term); return; }
      if (state_.role != CANDIDATE) return;
      if (resp.vote_granted) {
          votes_received_++;
          if (votes_received_ >= majority()) become_leader();
      }
  }
  ```
- [ ] `become_leader()`：
  - 设置 role = LEADER
  - 立刻向所有节点发心跳（空 AppendEntries）
  - 启动心跳定时器（每 50ms 发一次）

**完成标志：** Candidate 拿到多数票后变为 Leader，开始发心跳

---