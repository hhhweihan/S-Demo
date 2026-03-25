## Day 6（Sat）— 阅读 gRPC 核心设计文档

**预计时间：2 小时**

**任务：**
- [ ] 阅读 `grpc.io/docs/what-is-grpc/core-concepts/`
- [ ] 阅读 gRPC 流式 RPC（server streaming / client streaming / bidirectional）
- [ ] 回答：
  1. gRPC 如何实现 bidirectional streaming（HTTP/2 多路复用）？
  2. gRPC 的 deadline propagation（截止时间传播）是如何跨服务边界的？
  3. gRPC-Web 为什么不直接用 gRPC（浏览器不支持 HTTP/2 trailers）？

**完成标志：** 3 个问题有答案

---

