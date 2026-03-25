## Day 4（Thu）— 服务发现 + 动态更新

**预计时间：1 小时**

**任务：**
- [ ] 实现 `ServiceRegistry`（客户端侧）：
  - 启动时从 etcd 拉取所有实例
  - 通过 Watch 监听变化，实时更新实例列表
  ```cpp
  class ServiceRegistry {
      map<string, vector<Endpoint>> services_;  // service → endpoints
  public:
      void init(EtcdClient& etcd, const string& prefix);
      vector<Endpoint> resolve(const string& svc_name);
  };
  ```
- [ ] 测试：运行时关闭一个服务实例，客户端自动感知并切走流量

**完成标志：** 服务实例下线后 30 秒内客户端不再路由到该实例

---

