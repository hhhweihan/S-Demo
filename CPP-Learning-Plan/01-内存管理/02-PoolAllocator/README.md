# 通用内存池（PoolAllocator）

## 本周目标

支持任意大小对象的分配，内部按 size class 分流到对应的 FixedAllocator。
完成后能作为 `std::vector` 的 allocator 使用。

## 本周完成情况

- 已实现 `size_to_class()` 路由和 15 个小对象档位，覆盖 `8..1024` 字节。
- 已实现 `PoolAllocator`，支持小对象池化和大对象 `malloc/free` fallback。
- 已通过 header 元数据把释放接口收敛为 `deallocate(ptr)`。
- 已提供 `MyAllocator<T>`，并接入 `std::vector<int, MyAllocator<int>>` 验证。
- 已为 `FixedAllocator` 增加互斥锁，支持多线程共享使用。
- 已实现最小 TLS cache 原型，用线程本地 freelist 缓解小对象热路径上的锁竞争。

## 阶段小结
- 这一周的核心成果不是“把一个固定内存池改得更复杂”，而是搭出了一套教学版通用分配器的完整骨架：size class、pool routing、header、allocator 适配、并发保护、TLS cache。
- 从功能角度看，当前版本已经足够支撑小型容器场景和基本并发测试。
- 从性能角度看，最有效的优化是 TLS cache。Release 下 32 字节对象的单线程 benchmark 达到 `5.624 ms`，相对 `new/delete` 的 `27.644 ms` 约快 `4.916x`。
- 从工程角度看，当前实现和 jemalloc 仍有明显差距：没有多 arena、没有更精细的 size class、没有真正的 slab/extent/page 管理，也没有完备的生命周期和统计体系。
- 下一步如果继续演进，优先级最高的方向依然是：`per-thread/per-cpu arena`、更严格的 TLS cache 生命周期管理、以及更完整的统计/调优接口。

---