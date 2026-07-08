# Month 01 — 内存管理实战总结

> **学习周期：** Week 01 ~ Week 04  
> **核心产出：** `FixedAllocator`、`PoolAllocator`、教学版智能指针模型、`scope_guard / DEFER / FileGuard`、ASan 扫描结果、Month 1 benchmark 数据  
> **代码位置：** `CPP-Practice/memory_pool/`、`CPP-Practice/raii/`、`CPP-Practice/month1_benchmark/`

---

## 一、这个月到底学到了什么

Month 1 表面上是在做“内存管理”，但真正串起来以后，会发现它其实在反复回答同一个问题：

> 一块资源应该由谁拥有，什么时候释放，释放逻辑又该放在什么地方。

Week 01 用 `FixedAllocator` 解决的是“小而固定的对象，怎样比 `new/delete` 更快地分配”。Week 02 把问题推进到“任意大小对象怎样做分流”。Week 03 再把视角从“内存块”切到“所有权语义”，把 `unique_ptr / shared_ptr / weak_ptr` 的模型重新拆清楚。Week 04 则把这种思路抽象成 RAII 工具，并用 ASan 和 benchmark 对前面三周做了一次统一体检。

所以这个月真正形成的不是几段零碎代码，而是一条完整路径：

```text
free list 内存复用
  -> size class 分流
  -> 所有权模型
  -> RAII 收尾工具
  -> sanitizer + benchmark 验证
```

---

## 二、free list 内存池为什么能快

`FixedAllocator` 的核心思想非常朴素：只管理固定大小 block，把所有空闲块串成一条 free list。分配时头删一个节点，回收时头插一个节点，理论复杂度就是 O(1)。

最关键的代码其实只有这一类：

```cpp
void* block = free_list_;
free_list_ = *reinterpret_cast<void**>(block);
```

以及回收时：

```cpp
*reinterpret_cast<void**>(ptr) = free_list_;
free_list_ = ptr;
```

它快的原因并不神秘，本质上是因为它主动放弃了通用性：

- 不支持任意大小
- 不负责复杂元数据管理
- 不追求跨类型统一抽象

换来的结果是：分配和回收都退化成几次指针操作。这个月最重要的第一个认知点就是，**性能优化很多时候不是“把代码写得更聪明”，而是“先减少问题的自由度”**。

---

## 三、为什么一定要引入 size class

只有 `FixedAllocator` 还不够，因为真实程序不会只申请一种大小的对象。于是 Week 02 的 `PoolAllocator` 引入了 size class，把“任意大小请求”路由到“有限个固定档位”。

当前教学版用的是这组档位：

```text
8, 16, 24, 32, 48, 64, 80, 96,
128, 192, 256, 384, 512, 768, 1024
```

这背后是在做一个典型折中：

- 档位少，管理简单，但内部碎片会大
- 档位多，碎片更小，但实现复杂度和维护成本更高

这一层设计不是“为了看起来像 jemalloc”，而是因为任何通用 allocator 都绕不过这个问题。你不可能既允许用户申请任意大小，又希望底层永远只处理完全离散、完全无成本的固定块。

更重要的是，这种设计不是空谈，我们已经用整月 benchmark 把收益量化出来了：

| 测试项 | 时间 |
|--------|------|
| FixedAllocator 100 万次 | 18.121 ms |
| PoolAllocator 混合 100 万次 | 18.985 ms |

这组数据说明：即便加上了路由层、header 元数据和多档位管理，当前 `PoolAllocator` 仍然能把成本控制在非常接近 `FixedAllocator` 的范围内。这个结果非常有价值，因为它说明“抽象层增加了，但没有失控”。

---

## 四、shared_ptr 的灵魂为什么是控制块

Month 1 最有意思的部分，其实不是内存池，而是智能指针模型。尤其是 `shared_ptr`，它最容易被误解成“几个指针一起指向同一块内存”。

这其实是错的。`shared_ptr` 真正共享的不是某个裸地址变量，而是**控制块中的生命周期管理权**。

可以把它画成这样：

```text
SharedPtr a ----\
                >---- ControlBlock ---- use_count
SharedPtr b ----/            |
                             +---- weak_count
                             |
                             +---- managed object
```

一旦把图画清楚，很多以前模糊的地方就会自然清楚：

1. 为什么拷贝 `shared_ptr` 不是复制对象，而是增加 `use_count`
2. 为什么对象销毁和控制块销毁不是同一个时刻
3. 为什么 `weak_ptr` 必须存在，因为“观察”和“拥有”必须拆开

也正因为有了这个模型，后面再去理解 `enable_shared_from_this`、`make_shared`、标准库内部的 `__shared_ptr_pointer` 和 `__shared_ptr_emplace`，就不会觉得它们是在讲另一套陌生系统。

---

## 五、这个月最有收获的两个认知点

### 1. 所有权约束应该尽量写进类型系统

这是从 `unique_ptr` 一路延伸到 `FileGuard`、`ScopeGuard` 的核心经验。能禁拷贝就禁拷贝，能 move 转移就用 move，能把收尾动作绑定到析构里就不要散落在每个 `return` 和 `catch` 里。

这件事的价值不只是“代码更优雅”，而是它会直接减少运行期错误面。比如：

- `UniquePtr` 禁拷贝，避免 double free
- `FileGuard` 禁拷贝，避免 double close
- `ScopeGuard` 生命周期绑定清理动作，避免异常路径漏释放

### 2. 工具验证和设计理解必须同时做

这个月如果只写代码不跑 ASan，很难对“当前实现到底有没有踩内存错误”建立真正信心；但如果只跑工具、不理解 free list、header、控制块和 RAII 的设计本质，修 bug 又会变成碰运气。

Week 4 的 ASan 扫描结果是当前可运行代码无报错，这个结果当然很好，但它真正说明的是：

> 当结构设计、最小测试和工具验证能够互相对上时，代码质量才会真正稳定。

---

## 六、这个月的数据和验证结果

本月最后两项统一验证分别是 sanitizer 和 benchmark：

### 1. AddressSanitizer

当前对 `CPP-Practice/memory_pool/` 和 `CPP-Practice/month1_benchmark/` 做了一轮 MSVC ASan 扫描，结果没有出现：

- `heap-use-after-free`
- `heap-buffer-overflow`
- `memory leak` 提示

这不能证明“实现已经完美”，但至少说明目前被覆盖到的执行路径没有立刻暴露出内存安全错误。

### 2. Month 1 benchmark

| 测试项 | 时间 |
|--------|------|
| FixedAllocator 100 万次 | 18.121 ms |
| PoolAllocator 混合 100 万次 | 18.985 ms |
| SharedPtr 100 万次 | 47.521 ms |
| UniquePtr 100 万次 | 22.463 ms |

从这里能非常直观地看到：

- 固定场景、强约束的方案最轻
- 通用 allocator 为灵活性付出了一些路由成本，但仍然可控
- `SharedPtr` 由于控制块和计数维护，显著重于 `UniquePtr`

这组数据不是为了得出“谁绝对更好”，而是为了把抽象代价变成一件可量化的事情。

---

## 七、后续可以继续深入什么

如果继续沿着 Month 1 往下挖，我觉得最值得深入的方向至少有四个：

1. `PoolAllocator` 继续做 arena、per-thread cache 和跨线程回收策略，把单实例教学版推进到更接近 jemalloc 的模型。
2. 智能指针补上更完整的工程特性，比如 `make_shared`、自定义 deleter、别名构造和 `enable_shared_from_this` 的真正代码实现。
3. 在 RAII 方向继续扩展到锁、事务、socket、fd、临时文件等真实资源。
4. 再往上抽象，可以开始接触 GC、标记清除、标记压缩这些自动内存管理思路，建立“手动管理”和“自动管理”之间的对照视角。

---

## 八、一句话收束 Month 1

Month 1 最大的收获，不是终于会写一个内存池，也不是终于能背出 `shared_ptr` 的接口，而是开始真正理解了：

> 内存管理的核心从来不是 `new` 和 `delete` 这两个语句本身，而是围绕资源生命周期建立一套可验证、可约束、可扩展的结构。

这件事一旦想通，后面的并发、网络、协程、数据库存储，都会更容易接上。