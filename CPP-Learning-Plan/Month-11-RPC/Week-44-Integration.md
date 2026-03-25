# Week 44 — 整合 + 压测

## 本周目标

完整端到端测试 RPC 框架，添加拦截器机制，与 gRPC 性能对比，
完成月度验收。

---

## Day 1（Mon）— 拦截器（Interceptor）机制

**预计时间：1 小时**

**任务：**
- [ ] 实现责任链拦截器（AOP 模式）：
  ```cpp
  class Interceptor {
  public:
      virtual ~Interceptor() = default;
      virtual void intercept(RpcContext& ctx, function<void()> next) = 0;
  };

  class LogInterceptor : public Interceptor {
      void intercept(RpcContext& ctx, function<void()> next) override {
          LOG_INFO << "RPC " << ctx.method << " start";
          auto t0 = steady_clock::now();
          next();  // 调用下一个拦截器或实际处理器
          auto ms = duration_cast<milliseconds>(steady_clock::now() - t0).count();
          LOG_INFO << "RPC " << ctx.method << " done " << ms << "ms";
      }
  };
  ```
- [ ] 内置拦截器：日志、鉴权（token 验证）、限流（令牌桶）
- [ ] 测试：注册 3 个拦截器，验证执行顺序

**完成标志：** 拦截器按注册顺序执行，支持短路（鉴权失败直接返回）

---

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

## Day 3（Wed）— 性能压测

**预计时间：1 小时**

**任务：**
- [ ] 压测矩阵：
  ```
  场景：Echo RPC，消息 100B，同机测试
  [并发数 × 调用方式] = [1,4,8,16,32] × [同步,异步,协程]
  ```
- [ ] 记录：QPS、P50/P99 延迟、CPU 使用率
- [ ] 月度目标验证：
  - [ ] 同步延迟 < 200μs
  - [ ] 异步 QPS > 10 万
  - [ ] 服务发现 < 5ms

**完成标志：** 所有月度验收标准通过，数据已记录

---

## Day 4（Thu）— 与 gRPC 对比

**预计时间：1 小时**

**任务：**
- [ ] 安装 gRPC C++ 库，用相同的 proto 实现 Echo 服务
- [ ] 用相同的压测脚本对比：
  ```
  我的 RPC 框架 vs gRPC（同步调用，8并发）
  指标：QPS、P99 延迟
  ```
- [ ] 分析差距（gRPC 有 HTTP/2 多路复用，流控，压缩等额外特性）

**完成标志：** 对比数据已记录，理解差距来源

---

## Day 5（Fri）— 错误处理 + 状态码体系

**预计时间：1 小时**

**任务：**
- [ ] 实现完整状态码体系（仿 gRPC Status）：
  ```cpp
  enum class StatusCode {
      OK = 0, CANCELLED, UNKNOWN, INVALID_ARGUMENT,
      DEADLINE_EXCEEDED, NOT_FOUND, PERMISSION_DENIED,
      RESOURCE_EXHAUSTED, INTERNAL, UNAVAILABLE, UNAUTHENTICATED
  };
  ```
- [ ] 服务端异常 → 设置对应状态码返回
- [ ] 客户端根据状态码决定是否重试
- [ ] 测试：各种错误场景下客户端收到正确状态码

**完成标志：** 错误状态码体系完整，客户端行为符合预期

---

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

## Day 7（Sun）— 复盘 + 规划 Month 12

**预计时间：1 小时**

- [ ] 整理 RPC 框架完整架构图
- [ ] `git commit`，推送

---

## 本周结果（完成后填写）

**Month 11 验收：**
| 验收项 | 结果 | 对比 gRPC |
|--------|------|----------|
| 同步 RPC P99 延迟 | — | — |
| 异步 RPC QPS | — | — |
| 服务发现延迟 | — | — |
