# Week 43 — 连接池 + 服务发现

## 本周目标

将 Month 9 连接池接入 RPC 框架，实现基于 etcd 的服务注册与发现，
完成客户端负载均衡。

---

## Day 1（Mon）— RPC 连接池集成

**预计时间：1 小时**

**任务：**
- [ ] 将 Month 9 的 `TcpConnPool` 接入 `AsyncRpcClient`：
  ```cpp
  class PooledRpcClient {
      LoadBalancer balancer_;  // 持有多后端连接池
  public:
      Future<RpcResponse> call(const string& svc, const string& method,
                                const Message& req) {
          auto conn = balancer_.acquire();        // 从池中取连接
          return async_call(conn, svc, method, req)
              .then([conn, &balancer_](auto resp){
                  balancer_.release(conn);        // 用完归还
                  return resp;
              });
      }
  };
  ```
- [ ] 测试：8 线程并发 RPC，连接池大小 16，验证连接复用

**完成标志：** 连接池与 RPC 框架集成，并发调用无报错

---

## Day 2（Tue）— etcd 客户端基础

**预计时间：1 小时**

**任务：**
- [ ] 了解 etcd v3 gRPC API（使用 HTTP/2 + JSON 或 protobuf）
- [ ] 实现简单 etcd 客户端（HTTP GET/PUT/DELETE，使用 libcurl）：
  ```cpp
  class EtcdClient {
  public:
      void put(const string& key, const string& value, int ttl_sec = 0);
      string get(const string& key);
      vector<pair<string,string>> get_prefix(const string& prefix);
      void watch(const string& prefix, function<void(WatchEvent)> cb);
  };
  ```
- [ ] 测试：手动 put/get 键值

**完成标志：** 能通过代码向 etcd 写入和读取数据

---

## Day 3（Wed）— 服务注册

**预计时间：1 小时**

**任务：**
- [ ] 服务启动时向 etcd 注册：
  ```cpp
  // 注册路径：/services/{service_name}/{host}:{port}
  // 值：{"host":"127.0.0.1","port":8080,"weight":1}
  void RpcServer::register_to_etcd(EtcdClient& etcd, const string& svc_name) {
      string key = "/services/" + svc_name + "/" + host_ + ":" + to_string(port_);
      etcd.put(key, metadata_json, /*ttl=*/30);
      // 启动租约续期线程（每 10 秒续期一次）
      start_lease_renew_thread();
  }
  ```
- [ ] 测试：启动 2 个相同服务实例，etcd 中看到 2 条记录

**完成标志：** etcd 中有正确的服务注册信息，进程退出后 TTL 过期自动清除

---

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

## Day 5（Fri）— 一致性哈希负载均衡

**预计时间：1 小时**

**任务：**
- [ ] 实现一致性哈希（有状态服务场景，相同 key 路由到相同实例）：
  ```cpp
  class ConsistentHash {
      map<uint32_t, Endpoint> ring_;  // 虚拟节点环
      int virtual_nodes_ = 150;       // 每个实例 150 个虚拟节点
  public:
      void add(const Endpoint& ep);
      void remove(const Endpoint& ep);
      Endpoint get(const string& key);  // 顺时针找最近节点
  };
  ```
- [ ] 测试：增删节点后，只有 key/(总节点数) 比例的 key 需要重新路由

**完成标志：** 一致性哈希在节点变更时迁移量接近理论最优

---

## Day 6（Sat）— 阅读 etcd 服务发现最佳实践

**预计时间：2 小时**

**任务：**
- [ ] 阅读 etcd 官方文档 `etcd.io/docs/v3.5/dev-guide/`
- [ ] 回答：
  1. etcd 租约（lease）机制如何实现 TTL 到期自动删除（而不是轮询检查）？
  2. etcd Watch 是如何实现实时通知的（gRPC streaming）？
  3. 服务发现中为什么要用 etcd 而不是 ZooKeeper（性能、一致性模型区别）？

**完成标志：** 3 个问题有答案

---

## Day 7（Sun）— 复盘整理

**预计时间：1 小时**

- [ ] 整理服务发现的完整流程图（注册 → 发现 → Watch 更新 → 负载均衡）
- [ ] `git commit`，推送
