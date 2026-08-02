# Note

这里存放学习笔记、阶段复盘和主题总结。当前与 C++ 后端主线最相关的是 [C++-Note](./C++-Note)，其中模块总结和阶段复盘对应 [CPP-Learning-Plan](../CPP-Learning-Plan/README.md) 和 [CPP-Practice](../CPP-Practice/README.md)。

## 目录说明

| 目录 | 内容 | 说明 |
| --- | --- | --- |
| [C++-Note](./C++-Note) | C++ 主线学习笔记、模块总结、阶段复盘、早期 C++ 笔记 | 当前主线复盘入口 |
| [分布式一致性学习](./分布式一致性学习) | Raft/一致性相关图和材料 | 可与 RaftKV 模块对照阅读 |
| [操作系统](./操作系统) | 操作系统、Linux 内核、算法等资料 | 辅助背景材料 |

## C++ 主线复盘

[C++-Note](./C++-Note) 下的文件分三类：

- `内存管理实战总结.md`、`RaftKV实战总结.md` 这类文件：按模块总结目标、实现内容、验证方式和后续衔接。
- `固定内存池-复盘总结.md`、`LevelDB崩溃安全化-复盘总结.md` 这类文件：按阶段记录关键知识点与复盘结论；文件名采用主题命名，不再使用时间编号前缀。
- `C++笔记1/2/3`：早期 C++ 基础、并发、内存管理、Linux 多进程等资料沉淀。

## 维护原则

- Note 是复盘，不替代代码 README；实现细节和验证命令以 [CPP-Practice](../CPP-Practice/README.md) 为准。
- 当 [CPP-Learning-Plan](../CPP-Learning-Plan/README.md) 或 [CPP-Practice](../CPP-Practice/README.md) 重构时，模块总结和阶段复盘需要同步更新，避免旧结论误导后续学习。
- 对真实验证、教学模拟和 proxy benchmark 要写清楚边界，不把学习验证包装成生产指标。
