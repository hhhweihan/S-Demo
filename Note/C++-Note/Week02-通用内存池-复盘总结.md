# Week 02 — 通用内存池（PoolAllocator）复盘总结

> **学习周期：** Day-008 ~ Day-014（约 9.5 小时）  
> **核心产出：** 一个支持多 size class、小对象池化、大对象 fallback、`std::allocator` 适配、线程安全与最小 TLS cache 原型的教学版 `PoolAllocator`  
> **代码位置：** `CPP-Practice/memory_pool/`  
> **完成状态：** 全部 7 天任务已完成

---

## 一、这周到底在干嘛？——七天任务全景

这一周的目标，是在 Week01 的 `FixedAllocator` 基础上，继续往“**通用内存分配器**”方向推进：

- 不是只支持一种固定大小对象
- 而是支持一组 size class
- 小对象走池化
- 大对象走独立 fallback
- 最终还能适配 `std::vector`

整体路线可以概括为：

**size class 设计 → PoolAllocator 路由 → header 元数据 → allocator 适配 → 并发安全 → 读 jemalloc → TLS cache 原型 → 周总结**。

这周真正的核心，不只是“把代码写长”，而是开始接触**分配器设计的体系化问题**：

- 为什么要有 size class
- 为什么释放接口最好只接收 `ptr`
- 为什么多线程下单纯加锁不够
- 为什么 jemalloc 的关键不只是 free list，而是 `arena + size class + tcache`

---

### Day 8 — size class 映射设计（1h）

**做了什么：**

- 创建 `pool_allocator.h`
- 设计 15 个小对象档位：

```cpp
8, 16, 24, 32, 48, 64, 80, 96,
128, 192, 256, 384, 512, 768, 1024
```

- 约定：
  - `1..1024` 走小对象池
  - `>1024` 走大对象 fallback
- 实现：

```cpp
static int size_to_class(size_t n);
static size_t class_to_block_size(int index);
```

- 在 `main.cpp` 中打印 `1..1200` 字节的映射结果，验证每个区间是否落到正确 class

**关键理解：**

size class 是通用 allocator 的第一道分流逻辑。用户申请的是“任意大小”，池内部管理的却必须是“有限个离散档位”。

这本质上是在做一个折中：

- 档位越少，实现越简单，但内部碎片越大
- 档位越多，碎片越小，但实现与管理成本越高

我当前这版显然偏向“教学友好”的粗粒度方案。

---

### Day 9 — 实现 PoolAllocator 核心（1h）

**做了什么：**

- 在 `PoolAllocator` 内部持有多个 `FixedAllocator`
- 构造时按 size class 初始化每个 allocator
- 实现：

```cpp
void* allocate(size_t n);
void  deallocate(void* ptr, size_t n);   // Day10 前的临时接口
```

- 增加按 class 查询统计的接口：

```cpp
FixedAllocator::Stats stats_for_class(int class_index) const;
FixedAllocator::Stats stats_for_size(size_t n) const;
```

- 测试：
  - `20` 字节分配 10 次 → 应命中 `24` 字节 class
  - `200` 字节分配 10 次 → 应命中 `256` 字节 class
  - `used == 10`
  - 回收后 `used == 0`

**关键理解：**

Week01 的 `FixedAllocator` 解决的是“单一 block size 的高速分配”。

Day09 开始，真正引入了“**路由层**”的概念：

> 用户请求 `n` 字节  
> `PoolAllocator` 先判断应该落到哪个 class  
> 再把请求转发到对应的 `FixedAllocator`

这一步之后，整个分配器的结构才从“数据结构”升级成了“系统框架”。

---

### Day 10 — 大对象 fallback + header 标记（1h）

**做了什么：**

- 大对象（`>1024`）改为直接 `malloc/free`
- 在所有返回给用户的指针前增加一个 header，用于记录元信息
- 小对象 header 记录 class index
- 大对象 header 记录 `large` 标记和大小
- 把接口统一成：

```cpp
void* allocate(size_t n);
void  deallocate(void* ptr);
```

也就是说，回收时不再依赖调用方再传一个 `size`

**为什么这一步非常关键：**

如果释放还依赖 `deallocate(ptr, size)`，那 allocator 的调用者就必须保证 size 一定正确。现实里这很脆弱，一旦传错，轻则释放到错误的 free list，重则直接破坏整个 allocator 状态。

加了 header 后，回收逻辑变成：

```cpp
raw = ptr - header_offset
read header
if large -> free(raw)
else      -> 回到对应 class 的 pool
```

这一步让接口从“教学演示版”更接近真实 allocator 的使用方式。

---

### Day 11 — 适配 `std::allocator` 接口（1h）

**做了什么：**

- 新增 `my_allocator.h`
- 实现：

```cpp
template <typename T>
class MyAllocator {
public:
    using value_type = T;
    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);
};
```

- 内部通过单例 `PoolAllocator` 复用统一底层分配器
- 增加 `rebind`、`max_size`、跨类型构造等必要语义
- 用如下代码验证：

```cpp
std::vector<int, MyAllocator<int>> v;
for (int i = 0; i < 10000; ++i) v.push_back(i);
assert(v[9999] == 9999);
```

还额外验证了 copy 和 move 情况

**关键理解：**

这一天不是在“套模板”，而是在让分配器真正进入容器生态。

当 `std::vector` 扩容时，它一次申请的是 `n * sizeof(T)` 字节的连续空间，这就要求底层 `PoolAllocator` 至少要保证：

- 返回的用户指针对齐正确
- 大块连续分配能 fallback 到 `malloc`
- 释放时只靠 `ptr` 也能回收

所以 Day11 顺带修了一个很关键的问题：**header 之后的用户指针需要补齐到 `std::max_align_t` 对齐**，否则 allocator 在某些类型上是不安全的。

---

### Day 12 — 线程安全 + 性能测试（1h）

**做了什么：**

- 给 `FixedAllocator` 增加 `std::mutex`
- 对 `init / allocate / deallocate / stats` 加锁
- 增加 4 线程并发测试：
  - 共享一个 `PoolAllocator`
  - 每线程分配/释放 10 万次
  - 验证不崩溃，统计回到 0
- 增加 benchmark：

```text
new/delete
PoolAllocator（单线程）
PoolAllocator（4 线程竞争）
```

**阶段性结论：**

单纯加锁，功能是对的，但性能提升并不理想。

这是这周非常重要的认知转折点：

> 多线程 allocator 的问题，往往不是“锁写对了没有”  
> 而是“热路径能不能根本不碰锁”

也正是因为这一天的 benchmark 不够漂亮，才逼着我继续往 jemalloc 的思路去看。

---

### Day 13 — 读 jemalloc 设计文档（2h）

**读了什么：**

- 论文：
  - *A Scalable Concurrent malloc(3) Implementation for FreeBSD*（Jason Evans, 2006）
- 辅助材料：
  - jemalloc 官方手册中关于 `tcache` 的说明

**回答了 4 个核心问题：**

1. **arena 解决了什么问题？**  
   把多线程对同一套 allocator 元数据的竞争拆散，减少锁冲突和缓存行来回迁移。

2. **size class 比我的更精细还是更粗糙？**  
   明显更精细。我的是 15 档静态划分，jemalloc 是分层分级设计，并且后续版本在每次容量翻倍之间放多个档位。

3. **tcache 的作用是什么？**  
   把大量小对象分配/释放留在线程本地完成，绕开共享锁；代价是额外缓存和内存占用。

4. **和我的实现差距在哪里？**  
   最大差距不是“代码量”，而是体系结构：`arena + tcache + 更细的 size class + slab/extent/page 管理`。

**这一天最重要的收获：**

我开始真正理解：

> allocator 的上限，不是由 free list 本身决定的  
> 而是由分层结构、局部性、并发路径和元数据管理共同决定的

---

### Day 14 — TLS cache 原型 + 周总结（1.5h）

**做了什么：**

- 给 `FixedAllocator` 增加批量接口：

```cpp
std::size_t allocate_batch(void** blocks, std::size_t count);
void deallocate_batch(void** blocks, std::size_t count);
```

- 在 `PoolAllocator` 上增加最小 TLS cache 原型：
  - 每线程、每 size class 一个本地 freelist
  - 本地 cache miss 时批量 refill
  - 本地 cache 超限时批量 flush

- 增加测试：
  - 同线程释放后再次申请同一 class，应优先复用 TLS cache 中的地址

- 重新跑 Release benchmark

**最终结果：**

| 方案 | 耗时 |
|------|------|
| new/delete | 27.644 ms |
| PoolAllocator 单线程 | 5.624 ms |
| PoolAllocator 4 线程竞争 | 24.514 ms |

单线程相对 `new/delete` 约快 **4.916x**。

这说明 TLS cache 确实击中了前一天发现的主要瓶颈：**热路径锁竞争**。

---

## 二、核心知识点：为什么要有 size class？

### 2.1 通用分配器面临的问题

如果用户可以申请任意大小，那么 allocator 内部如果也按“任意大小”管理，就必须频繁做：

- 查找最合适的空闲块
- 拆分大块
- 合并碎片
- 维护复杂元数据

这会让热路径很长。

### 2.2 size class 的本质

size class 的核心思想是：

> 把无限多种用户请求大小，映射为有限个内部管理档位

例如：

- 用户申请 `20` 字节 → 映射到 `24` 字节 class
- 用户申请 `200` 字节 → 映射到 `256` 字节 class

这样 allocator 就不需要为每一种 size 单独维护结构，而只要维护有限个池。

### 2.3 size class 的代价

代价是**内部碎片**。

例如用户要 `129` 字节，你给的是 `192` 字节档位，中间有 `63` 字节浪费。

所以 size class 设计永远是在平衡：

- 档位少 → 实现简单，但碎片大
- 档位多 → 碎片小，但管理复杂

我当前这版 `PoolAllocator` 的设计明显偏“简单可解释”。这对学习阶段是合理的，但离工业 allocator 还有距离。

---

## 三、核心知识点：为什么 header 元数据这么重要？

### 3.1 没有 header 时的问题

如果接口长这样：

```cpp
void deallocate(void* ptr, std::size_t n);
```

那 allocator 必须相信调用方传入的 `n` 是真的。

但一旦传错，就会出现：

- 释放到错误的 size class
- 把某个块塞进不属于它的 free list
- 后续 allocate 返回错误尺寸的内存

最终导致数据损坏。

### 3.2 header 的作用

所以更稳的做法是，在用户指针前面藏一点 metadata：

- 小对象：记录 class index
- 大对象：记录 `large` 标记和 size

这样回收逻辑只需要：

```cpp
raw = ptr - offset;
read header;
if large -> free(raw)
else      -> 回到对应 class
```

这一步实际上是让 allocator 的状态管理**从调用方转回 allocator 自己手里**。

这也是通用 allocator 和简单对象池之间非常重要的一步分水岭。

---

## 四、核心知识点：为什么单纯加锁不够？

### 4.1 Day12 的问题

当我把 `FixedAllocator` 全部加上互斥锁之后，功能确实正确了：

- 多线程不崩溃
- 统计回到 0
- 逻辑可用

但性能没有想象中好。

### 4.2 根因

因为如果每次小对象分配/释放都要：

- 找到对应 class
- 进入共享 `FixedAllocator`
- 拿锁
- pop/push free list

那热路径还是会被锁开销主导。

多线程 allocator 真正应该追求的是：

> 大部分常见路径不需要同步

这正是 jemalloc 中 tcache 的思想。

---

## 五、核心知识点：TLS cache 原型到底解决了什么？

### 5.1 设计思路

在 `PoolAllocator` 上面再加一层：

```cpp
thread_local ThreadCache
```

里面对每个 size class 维护一个本地 freelist。

分配时：

- 先看本地 cache 有没有
- 有 → 直接返回，不加锁
- 没有 → 从共享 `FixedAllocator` 批量拉一小批块

回收时：

- 先回本地 cache
- 本地太多了 → 批量刷回共享池

### 5.2 为什么它有效

因为把原本“每次都要碰共享锁”的频率，变成了：

- **本地命中时：0 次加锁**
- **cache miss 时：一次批量 refill 的加锁**
- **cache flush 时：一次批量归还的加锁**

也就是说，原来是**每个对象一次锁**，现在变成**一批对象一次锁**。

### 5.3 这版原型还不完整的地方

这次实现的是教学版最小原型，还没彻底处理：

- allocator 生命周期和线程局部缓存之间的协调
- 多 allocator 共存时更严格的 thread-local 管理
- 更详细的 shared/tls 统计拆分
- 真正的 per-thread/per-cpu arena

但作为学习路径，它已经足够把性能瓶颈和设计方向讲清楚。

---

## 六、性能结果：这周 benchmark 到底说明了什么？

### 6.1 最终结果（Release）

| 方案 | 耗时 |
|------|------|
| new/delete | 27.644 ms |
| PoolAllocator 单线程 | 5.624 ms |
| PoolAllocator 4 线程竞争 | 24.514 ms |

### 6.2 结论

- **PoolAllocator 单线程** 相对 `new/delete` 提升约 **4.916x**
- **4 线程竞争** 下耗时回到 `24.514 ms`，虽然比单线程慢很多，但仍然比 `new/delete` 更有竞争力

### 6.3 这一组数据最重要的意义

不是“我已经做出了 jemalloc”，而是：

> 我验证了一个非常关键的工程判断：  
> **性能真正改善，不是来自继续微调锁，而是来自把锁从热路径拿掉。**

Day12 只加锁时，结果一般；Day14 加 TLS cache 后，性能才出现明显跃迁。

这比“多快了几毫秒”更有价值，因为它说明设计方向是对的。

---

## 七、与 jemalloc 对照：我现在处在什么位置？

### 7.1 我已经做到了什么

和 Week01 相比，这周已经从“单一对象池”进化成了一个简化版通用分配器框架：

- size class 路由
- 多个 `FixedAllocator` 组合
- header 元数据
- 大对象 fallback
- `std::allocator` 适配
- 线程安全
- TLS cache 原型

这已经不是单一数据结构练习，而是 allocator 设计入门。

### 7.2 和 jemalloc 的差距

我和 jemalloc 的差距，不在于“有没有 free list”，而在于是否具备完整体系：

| 维度 | 当前实现 | jemalloc |
|------|----------|----------|
| size class | 静态 15 档 | 更细粒度、系统化分层 |
| 并发结构 | 单实例 + 多 class 锁 | 多 arena |
| 本地缓存 | 最小 TLS 原型 | 完整 tcache |
| 内存管理单元 | block + chunk | slab / page / extent / arena |
| 生命周期管理 | 教学版 | 工业级 |
| 调优与统计 | 很少 | mallctl + 丰富 stats |

也就是说，我这周做的更像是：

> **从 FixedAllocator 迈向 jemalloc 思路的第一步**

而不是“已经接近 jemalloc 本身”。

---

## 八、本周遇到的最难问题

### 8.1 对齐问题

加 header 看起来简单：

```cpp
return raw + sizeof(size_t);
```

但这在通用 allocator 里并不安全，因为返回给用户的指针必须满足足够严格的对齐要求。最终需要把 header 后的偏移补齐到 `std::max_align_t`。

这个问题非常典型：

- 在 demo 里很容易忽略
- 在真实容器和真实类型上却是硬性要求

### 8.2 生命周期问题

TLS cache 一旦引入，就不再只是“数据结构”问题，而会变成“对象生命周期和线程生命周期协作”问题。

比如：

- allocator 析构了，但某个线程还缓存着它的块怎么办？
- 线程退出时，如何把缓存安全刷回？

这类问题在教学版里可以规避，在工程版里却必须正面解决。

---

## 九、这周最大的收获是什么？

这一周最大的收获，不是多写了几个接口，而是对 allocator 的理解开始从“代码实现”转向“设计结构”。

如果说 Week01 的关键词是：

> **free list 是什么**

那 Week02 的关键词就是：

> **allocator 不是单个 free list，而是一整套分层结构。**

它包括：

- 请求大小如何离散化
- 元数据放在哪里
- 小对象和大对象如何分流
- 多线程如何绕开共享锁
- 统计和生命周期如何管理

也正因为这样，我现在再回头看 jemalloc 的 arena、size class、tcache，会发现它不是“技巧集合”，而是一套自洽的系统设计。

---

## 十、如果继续优化，下一步该做什么？

按优先级看，最值得继续做的是：

1. **把 TLS cache 做完整**
   - 更严格的线程退出 flush
   - allocator 生命周期管理
   - shared/tls 分层统计

2. **引入 per-thread / per-cpu arena**
   - 让线程从结构上减少共享竞争
   - 而不是只靠一层 thread-local freelist 缓冲

3. **细化 size class**
   - 降低内部碎片
   - 让分配粒度更接近 jemalloc 风格

4. **引入更系统的页/extent 视角**
   - 从“block/chunk”进化到更通用的内存管理单元

---

## 十一、一句话总结 Week02

> **Week02 的本质，是把 Week01 的固定大小对象池，升级成了一个能路由、能回收、能适配容器、能在多线程下工作的教学版通用分配器，并第一次用 TLS cache 触碰到了现代 allocator 的真正核心。**

---