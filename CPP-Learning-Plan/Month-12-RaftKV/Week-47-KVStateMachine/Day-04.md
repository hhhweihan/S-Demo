## Day 4（Thu）— LevelDB 持久化

**预计时间：1 小时**

**任务：**
- [ ] 把 KVStateMachine 改为 LevelDB 后端：
  ```cpp
  class KVStateMachine : public StateMachine {
      leveldb::DB* db_;
  public:
      void apply(const std::string& cmd) override {
          // 解析 cmd，调用 db_->Put 或 db_->Delete
      }
      std::string get(const std::string& key) override {
          std::string val;
          db_->Get(leveldb::ReadOptions(), key, &val);
          return val;
      }
  };
  ```
- [ ] Raft 日志本身也要持久化（WAL）：重启后从 WAL 恢复，重新 apply 所有已 commit 日志

**完成标志：** 重启后 KV 数据不丢失

---