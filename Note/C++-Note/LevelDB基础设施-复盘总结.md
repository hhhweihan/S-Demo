# LevelDB 基础设施复盘总结

## 核心目标

Week25 聚焦 LevelDB 的基础设施：`Slice`、`Status`、`Arena`，并建立写入路径的整体认知。

## Slice

`Slice` 是非 owning 字节视图，避免频繁拷贝字符串。它的价值在于把“查看一段数据”和“拥有一段数据”分开，让接口传参更轻量。

## Status

`Status` 用返回值表达成功/失败，而不是依赖异常。LevelDB 这类底层存储库更倾向显式错误处理，因为调用者通常需要根据错误类型决定重试、降级或终止。

## Arena

`Arena` 通过批量分配和整体释放降低小对象分配成本。SkipList 节点、MemTable 内部结构这类生命周期一致的对象很适合使用 Arena。

## 写入路径

LevelDB 的写入大致是：

```text
Put -> WAL -> MemTable -> Immutable MemTable -> SSTable -> Compaction
```

WAL 保证崩溃恢复，MemTable 提供内存有序写入，SSTable 负责磁盘有序存储，Compaction 负责回收和整理层级。

## 复盘结论

LevelDB 的基础设施代码短，但边界非常清楚。理解这些小组件，有助于后续阅读 SkipList、MemTable 和 TableBuilder 时少被细节淹没。
