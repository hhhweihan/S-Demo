# protobuf + 帧协议

## 本周目标

掌握 protobuf 序列化，设计 RPC 帧格式，实现基础的请求/响应编解码层。

## 本周完成情况

- [x] Day 281：设计 `RpcRequest` / `RpcResponse` 消息结构
- [x] Day 282：实现 protobuf-like key-value 编解码和转义
- [x] Day 283：实现长度前缀 `FrameCodec`
- [x] Day 284：验证半包等待和完整帧解析
- [x] Day 285：整理帧协议字段和错误边界
- [x] Day 286：验证 request/response 编解码
- [x] Day 287：完成 Week41 复盘；`git commit / push` 等待明确确认后再执行

## 本周产出

- `RpcRequest` / `RpcResponse`
- `FrameCodec`
- 周复盘：`Note/C++-Note/RPC帧协议-复盘总结.md`

## 阶段小结
Week41 完成 RPC 编解码层。真实 protobuf 依赖外部代码生成器，本工程采用简化 key-value 编码来突出 RPC 消息结构和帧协议。长度前缀帧解决半包问题，请求/响应结构提供调用链元数据。

## 验证命令

```powershell
cmake -S CPP-Practice/rpc_framework -B CPP-Practice/rpc_framework/build
cmake --build CPP-Practice/rpc_framework/build --config Release
.\CPP-Practice\rpc_framework\build\Release\rpc_framework_demo.exe
```

---
