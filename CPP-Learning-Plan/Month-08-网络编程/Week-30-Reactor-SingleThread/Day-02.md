## Day 2（Tue）— 实现 Poller

**预计时间：1 小时**

**任务：**
- [ ] 创建 `net/poller.h` + `net/epoll_poller.cpp`
  ```cpp
  class Poller {
      int epfd_;
      std::unordered_map<int, Channel*> channels_;
  public:
      Poller();
      ~Poller();
      // 等待事件，填充 active_channels，返回活跃 Channel 列表
      void poll(int timeout_ms, std::vector<Channel*>& active_channels);
      void update_channel(Channel* ch);  // 添加/修改/删除监听
      void remove_channel(Channel* ch);
  };
  ```
- [ ] 实现 `poll()` 内部调用 `epoll_wait`，把活跃 fd 转为 Channel* 输出

**完成标志：** Poller 能正确监听 fd 并返回活跃 Channel

---