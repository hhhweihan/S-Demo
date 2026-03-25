## Day 2（Tue）— 完整端到端测试

**预计时间：1 小时**

**任务：**
- [ ] 搭建完整测试环境：
  - 2 个 RpcServer 实例（注册到 etcd）
  - 1 个 RpcClient（从 etcd 发现服务，Round Robin 负载均衡）
  - 测试 Echo 和 Add 两个接口
- [ ] 测试场景：
  - 正常调用 1000 次
  - 关闭一个 Server，验证流量自动切换（< 30s）
  - 超时触发（Server 延迟 2s，Client 1s 超时）
  - 重试后成功

**完成标志：** 所有测试场景通过

---

