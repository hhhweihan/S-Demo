## Day 3（Wed）— 实现 RequestVote RPC

**预计时间：1 小时**

**任务：**
- [ ] Candidate 发 RequestVote：
  ```cpp
  void RaftNode::start_election() {
      state_.role = CANDIDATE;
      state_.current_term++;
      state_.voted_for = state_.node_id;  // 给自己投票
      votes_received_ = 1;
      reset_election_timer();
      for (auto& peer : peers_) {
          peer.send_request_vote(state_.current_term, state_.node_id,
                                 last_log_index(), last_log_term());
      }
  }
  ```
- [ ] Follower 处理 RequestVote：
  ```cpp
  // 投票条件：
  // 1. req.term >= current_term
  // 2. 本 term 还没投票（或已投给该候选人）
  // 3. 候选人的日志不比自己旧（暂时先跳过，Week 22 再加）
  ```

**完成标志：** Candidate 能发出 RequestVote，Follower 能正确投票

---