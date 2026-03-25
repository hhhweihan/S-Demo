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

