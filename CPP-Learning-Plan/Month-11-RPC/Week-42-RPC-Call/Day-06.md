## Day 6（Sat）— 阅读 muduo RPC 示例

**预计时间：2 小时**

**任务：**
- [ ] 阅读 muduo `examples/protobuf/rpc/`（重点：RpcChannel、RpcServer）
- [ ] 回答：
  1. muduo 的 `RpcChannel::CallMethod` 如何将 protobuf Service 调用转为 TCP 发送？
  2. muduo 如何用 `google::protobuf::RpcController` 传递错误信息？
  3. 多路复用 vs 连接池：什么时候用多路复用更好（低延迟 vs 高隔离性）？

**完成标志：** 3 个问题有答案

---

