# Week 01 — 固定大小内存池（FixedAllocator）复盘总结

> **学习周期：** Day-001 ~ Day-007（约 8.5 小时）  
> **核心产出：** 一个基于 free list + chunk 扩展策略的固定大小内存池  
> **代码位置：** `CPP-Practice/memory_pool/`  
> **完成状态：** 全部 7 天任务已完成

---

## 一、这周到底在干嘛？——七天任务全景

这一周的目标是**从零手写一个固定大小的内存池 `FixedAllocator`**，替代 `new/delete`，最终要在 100 万次分配/释放的 benchmark 中比 `new` 快 3 倍以上。

整体路线是：**接口设计 → 核心实现 → 健壮性增强 → 测试验证 → 性能对比 → 工业代码学习 → 复盘整理**。

### Day 1 — 建骨架，定接口（1h）

**做了什么：**
- 创建 `fixed_allocator.h`，声明 4 个公开接口：

```cpp
class FixedAllocator {
public:
    void init(size_t block_size, size_t block_count);
    void* allocate();
    void  deallocate(void* ptr);
    struct Stats { size_t total, used, chunks; };
    Stats stats() const;
};
```

- 在注释里画出内存布局草图（这一步很关键，先想清楚数据结构再写代码）：

```
chunk: [block0][block1][block2]...[blockN]
free_list: block0 -> block2 -> block5 -> nullptr
每个空闲 block 的前 8 字节存下一个空闲块的指针
```

- 创建 `main.cpp`，`#include "fixed_allocator.h"` 确保编译通过

**为什么这么做：** 先想清楚接口和内存布局，再动手写实现，避免边写边改。

---

### Day 2 — 实现 free list 核心逻辑（1h）

**做了什么：**
- 创建 `fixed_allocator.cpp`，实现三个核心函数：

**`init()`** — 用 `malloc` 申请一整块大内存，把所有 block 串成链表：
```cpp
// 初始化：把每个 block 的前 sizeof(void*) 字节写成指向下一个 block 的指针
for (size_t i = 0; i < block_count - 1; i++) {
    void* current = (char*)chunk + i * block_size;
    void* next    = (char*)chunk + (i + 1) * block_size;
    *reinterpret_cast<void**>(current) = next;
}
// 最后一个 block 指向 nullptr
```

**`allocate()`** — 从 free list 头部 pop 一个 block 返回  
**`deallocate()`** — 把 block 头插回 free list

**测试：** 分配 3 个 block，释放中间那个，再分配 1 个 → 拿到的是刚释放的地址（验证复用）

---

### Day 3 — chunk 扩展 + 边界检查（1h）

**做了什么：**
- 当 `free_list_ == nullptr`（所有 block 都被分配光了）时，自动申请新 chunk
- 翻倍扩容策略：第一次扩容分配 `初始容量 × 2` 个 block，下次再翻倍 → 1, 2, 4, 8, 16...
- 新 chunk 的 free list 尾部接到旧 `free_list_`，完成链表拼接
- 用 `chunks_` 向量管理所有 chunk 指针，析构时逐一 `free`
- `deallocate()` 增加跨 chunk 边界检查：指针必须落在某个 chunk 范围内且对齐

**测试：** `init(32, 2)` 初始 2 个 block → 分配 6 个 → 验证自动扩容后 `chunks == 2`，`total == 6`

---

### Day 4 — 单元测试（1h）

**做了什么：** 写了 4 个测试函数，覆盖不同场景：

| 测试 | 场景 | 验证目标 |
|------|------|----------|
| `test_basic_allocate_deallocate()` | 分配 1000 次 → 释放 1000 次 | `stats.used == 0`，无泄漏 |
| `test_reuse_address()` | 分配 A → 释放 A → 分配 B | `A == B`，地址复用 |
| `test_expand_chunks()` | 初始容量 4 → 分配 100 次 | `chunk 数 > 1`，自动扩容正常 |
| `test_mixed_random()` | 随机分配/释放 10000 次 | `unordered_set` 跟踪无重复分配 |

- 使用自定义 `EXPECT_TRUE/EXPECT_EQ` 宏，失败时抛异常
- `main()` 统一 try/catch，任何断言失败返回非 0 退出码（可接入 CI）

**运行结果：** `Day 004 unit tests passed`

---

### Day 5 — Benchmark 对比（1h）

**做了什么：** 用 `std::chrono::high_resolution_clock` 分别测量三种方式分配/释放 100 万次的耗时：

```cpp
// 方案 A：new/delete
int* p = new int; *p = 42; delete p;

// 方案 B：FixedAllocator
void* p = alloc.allocate(); *(int*)p = 42; alloc.deallocate(p);

// 方案 C：malloc/free
int* p = (int*)malloc(sizeof(int)); *p = 42; free(p);
```

**测试结果（MSVC Release, Ninja, 1M 次迭代）：**

| 方案 | 耗时 | vs FixedAllocator |
|------|------|-------------------|
| new/delete | 22.077 ms | 10.03x 慢 |
| malloc/free | 23.373 ms | 10.62x 慢 |
| **FixedAllocator** | **2.200 ms** | **基准** |

远超"至少 3x"的目标，达到了 **10 倍以上**的性能提升。

---

### Day 6 — 读 nginx 内存池源码（2h）

**做了什么：** 精读 nginx 源码中的 `src/core/ngx_palloc.h` 和 `ngx_palloc.c`（共约 250 行），带着 4 个问题边读边记。

详见下方"五、nginx 内存池源码解读"。

---

### Day 7 — 复盘整理（1.5h）

**做了什么：**
- 给所有代码补充注释（重要逻辑必须有注释）
- 在 Day-005 的文件里填写 benchmark 数据
- 记录 2 个本周最难的问题（见下方"六、难点与踩坑"）
- git commit 推送

---

## 二、核心知识点：Free List 到底是什么？

### 2.1 问题背景

`new/delete` 和 `malloc/free` 是**通用分配器**——它能分配任意大小的内存。但"通用"意味着每次分配都要：
- 搜索合适大小的空闲块（链表/树遍历）
- 处理内存碎片（分裂、合并）
- 并发加锁（多线程安全）
- 维护元数据（cookie、header）

如果我们知道"每次都要分配相同大小的对象"，就可以跳过上面所有步骤。

### 2.2 Free List 的核心思想

**Free list（空闲链表）** 是一种极其简洁的数据结构：

```
内存池初始状态（假设 block_size = 32, block_count = 4）:

chunk 起始地址: 0x1000
┌──────────┬──────────┬──────────┬──────────┐
│ block 0  │ block 1  │ block 2  │ block 3  │
│ 32 bytes │ 32 bytes │ 32 bytes │ 32 bytes │
└──────────┴──────────┴──────────┴──────────┘

free_list → block0 → block1 → block2 → block3 → nullptr
```

**关键技巧：每个空闲 block 的前 `sizeof(void*)` 字节（64 位系统上是 8 字节）存放指向下一个空闲 block 的指针。**

这意味着：
- **不需要额外的数据结构**来维护空闲链表
- 空闲 block 自身就是链表节点
- 前提条件：`block_size >= sizeof(void*)`（至少能放下一个指针）

### 2.3 分配与释放——O(1) 操作

**分配（allocate）：**
```
操作前:  free_list → [block0] → [block1] → [block2] → nullptr
操作:    result = free_list; free_list = free_list->next;
操作后:  free_list → [block1] → [block2] → nullptr
返回:    block0 的地址
```

就是链表头部的 pop 操作，**一次指针赋值**，O(1)。

**释放（deallocate）：**
```
操作前:  free_list → [block1] → [block2] → nullptr
操作:    ptr->next = free_list; free_list = ptr;
操作后:  free_list → [block0] → [block1] → [block2] → nullptr
```

就是链表头部的 push 操作，**一次指针赋值**，O(1)。

对比通用分配器需要搜索、合并、加锁等操作，free list 的路径极短。

### 2.4 初始化——串链表

`init()` 阶段把一整块连续内存切成等大的 block，然后把所有 block 串成链表：

```cpp
void init(size_t block_size, size_t block_count) {
    void* chunk = malloc(block_size * block_count);
    // 把每个 block 前 8 字节写成指向下一个 block 的指针
    for (size_t i = 0; i < block_count - 1; i++) {
        void* current = (char*)chunk + i * block_size;
        void* next    = (char*)chunk + (i + 1) * block_size;
        *reinterpret_cast<void**>(current) = next;
    }
    // 最后一个 block 指向 nullptr
    *reinterpret_cast<void**>((char*)chunk + (block_count - 1) * block_size) = nullptr;
    free_list_ = chunk;
}
```

核心是 `*reinterpret_cast<void**>(current) = next;` 这行——把当前 block 的头部**强制解释为指针**，写入下一个 block 的地址。用户拿到这个 block 后，会用自己的数据覆盖这个指针，所以不影响使用。

---

## 三、核心知识点：Chunk 扩展策略

### 3.1 为什么需要扩展？

初始化时分配的 block 有限。当所有 block 都被用完（`free_list_ == nullptr`），需要申请新的内存。

### 3.2 翻倍扩容

```
初始:   chunk_0 有 N 个 block
第1次:  chunk_1 有 2N 个 block
第2次:  chunk_2 有 4N 个 block
第3次:  chunk_3 有 8N 个 block
...
```

这和 `std::vector` 的扩容策略一样——**摊还分析**下，每次分配的平均成本仍是 O(1)。

### 3.3 多 chunk 管理

```cpp
std::vector<void*> chunks_;  // 记录所有 chunk 指针
size_t next_expand_count_;   // 下次扩容的 block 数

void expand() {
    void* new_chunk = malloc(block_size_ * next_expand_count_);
    // 初始化新 chunk 内部的 free list
    // 新 chunk 最后一个 block 的 next 指向旧 free_list_（链表拼接）
    chunks_.push_back(new_chunk);
    next_expand_count_ *= 2;
}
```

析构时遍历 `chunks_` 逐一 `free`，保证无内存泄漏。

---

## 四、核心知识点：边界安全检查

### 4.1 问题：deallocate 传入非法指针怎么办？

如果用户传入一个不属于内存池管理的指针，或者传入一个虽然在 chunk 范围内但没有按 `block_size_` 对齐的指针，free list 就会被污染——后续 allocate 会返回非法地址，导致崩溃或数据损坏。

### 4.2 检查逻辑

```cpp
void deallocate(void* ptr) {
    // 遍历所有 chunk，检查 ptr 是否在某个 chunk 范围内
    for (auto* chunk : chunks_) {
        char* start = (char*)chunk;
        char* end   = start + chunk 对应的 block 数 * block_size_;
        if ((char*)ptr >= start && (char*)ptr < end) {
            // 检查对齐：(ptr - start) 必须是 block_size_ 的整数倍
            if (((char*)ptr - start) % block_size_ == 0) {
                // 合法！头插回 free list
                *reinterpret_cast<void**>(ptr) = free_list_;
                free_list_ = ptr;
                return;
            }
        }
    }
    // 所有 chunk 都不匹配 → 非法指针，拒绝操作
}
```

虽然遍历 chunks 看起来是 O(chunk_count)，但翻倍扩容策略下 chunk 数量增长极慢（100 万个 block 也只需要约 20 个 chunk），实际开销可忽略。

---

## 五、nginx 内存池源码解读

### 5.1 nginx 内存池的整体设计

nginx 的内存池是**请求级别**的——每个 HTTP 请求创建一个 pool，请求结束时整池销毁。这和我们的 FixedAllocator（对象级别的分配/释放）是完全不同的使用场景。

### 5.2 四个核心问题

**Q1：nginx 如何区分「小块」和「大块」分配？阈值是多少？**

入口函数 `ngx_palloc(pool, size)` 的判断逻辑：
- `size <= pool->max` → 走小块分配（`ngx_palloc_small`）
- `size > pool->max` → 走大块分配（`ngx_palloc_large`）

`pool->max` 在 `ngx_create_pool()` 里设为：
```c
pool->max = min(size - sizeof(ngx_pool_t), NGX_MAX_ALLOC_FROM_POOL);
// NGX_MAX_ALLOC_FROM_POOL = ngx_pagesize - 1，典型 x86 上是 4095
```

所以阈值不是写死的常量，而是取决于池大小和页大小。

**Q2：`ngx_pool_t` 结构体里 `d.last`、`d.end`、`d.next` 分别是什么？**

```
┌─────────────────────────────────────┐
│             pool block              │
│  ┌──────────────────────────────┐   │
│  │ d.last  → 下一次分配的起点   │   │
│  │ d.end   → 这个 block 的末尾  │   │
│  │ d.next  → 下一个 pool block  │   │
│  └──────────────────────────────┘   │
│  [已分配区域] [可用空间........]    │
│  ^            ^                ^    │
│  header       d.last           d.end│
└─────────────────────────────────────┘
```

- `d.last`：已分配区域的尾部，也是下次小块分配的起点（bump pointer）
- `d.end`：当前 pool block 的末尾边界
- `d.next`：链表指针，串联多个 pool block

**Q3：nginx 为什么不支持释放单个小块？**

nginx 小块分配是 **bump pointer**（线性推进 `d.last`），不维护每个小对象的独立元数据。

```
分配前: [...已分配...][可用空间............]
                       ^ d.last
分配后: [...已分配...][新对象][可用空间.....]
                              ^ d.last（往前推了）
```

这种设计让分配路径极短（一次指针加法 + 比较），但代价是无法回收单个小块——因为根本不知道哪些块是空闲的。

释放策略：
- `ngx_reset_pool()` → 重置所有 pool block 的 `d.last`，等于全部回收
- `ngx_destroy_pool()` → 整池销毁
- 只有**大块**（`pool->large` 链）支持 `ngx_pfree()` 按块释放

**Q4：FixedAllocator 和 nginx pool 的核心设计差异**

| 维度 | FixedAllocator | nginx pool |
|------|----------------|------------|
| **分配模型** | 固定大小 + free list | 变长 bump pointer + 大块链表 |
| **单块释放** | 支持，O(1) | 小块不支持，大块支持 |
| **内存布局** | 等大 block 链表 | 线性空间顺序切分 |
| **适用场景** | 大量同尺寸对象，频繁分配/释放 | 请求级内存，生命周期集中结束 |
| **碎片特点** | 无外部碎片，可能有内部碎片 | 几乎无管理开销，不追求精细回收 |
| **设计哲学** | 用**尺寸约束**换性能 | 用**生命周期约束**换性能 |

**共同点：** 两者都是"放弃通用分配器的灵活性，在特定约束下获得极致性能"。

---

## 六、难点与踩坑

### 踩坑 1：`deallocate()` 不加边界检查 → 链表污染

**现象：** 如果传入一个不属于内存池的指针，`deallocate` 会把它插入 free list。后续 `allocate` 返回这个非法地址，写入时直接段错误。

**根因：** 最简单的 free list 实现只做头插，不做任何验证。

**解决：** 逐 chunk 检查指针范围 + 对齐检查。虽然把 O(1) 变成了 O(chunk_count)，但 chunk 数极少，实测无可感知的性能损失。

**教训：** 防御性编程不是可选项。内部数据结构的完整性必须自己保护。

### 踩坑 2：Benchmark 混入扩容成本 → 结果不可信

**现象：** 第一版 benchmark 里 `init(sizeof(int), 100)` 只预分配 100 个 block，但测试分配 100 万次。结果 FixedAllocator 只比 new 快 2 倍左右。

**根因：** 大量的 `expand()` 调用（malloc 新 chunk + 串链表）被算进了热路径。

**解决：** 改为 `init(sizeof(int), 1000000)` 一次性预分配全部 block，测试只测量稳态下的 allocate/deallocate。这样结果才反映 free list 方案的**真实性能优势**。

**教训：** Benchmark 要明确"测的是什么"。如果目标是衡量稳态分配速度，就必须排除初始化/扩容开销。

---

## 七、为什么 FixedAllocator 比 new/delete 快 10 倍？——原理拆解

这是本周最核心的"so what"，拆开看每个层面的差异：

| 维度 | new/delete (malloc/free) | FixedAllocator |
|------|--------------------------|----------------|
| **分配算法** | 搜索空闲块（首次适配/最佳适配），可能需要分裂大块 | 链表头 pop，1 次指针操作 |
| **释放算法** | 归还 + 尝试与相邻空闲块合并 | 链表头 push，1 次指针操作 |
| **元数据** | 每块前后有 cookie/header（至少 16 字节） | 零额外开销（空闲块自身存指针） |
| **碎片处理** | 动态维护空闲链/红黑树，处理外部碎片 | 固定大小，不存在外部碎片 |
| **线程安全** | 内部 mutex 或 per-thread cache | 无（单线程版本，开销为零） |
| **缓存局部性** | 分散分配，cache miss 率高 | 同一 chunk 内连续排列，cache friendly |
| **系统调用** | 可能触发 brk/mmap | 只在 expand 时触发，热路径全用户态 |

**一句话总结：** 通用分配器为了应对"任意大小、任意顺序、多线程"付出了大量开销。固定大小内存池通过放弃灵活性（只分配固定大小），把分配/释放简化到极致——**一次指针操作**。

---

## 八、关键收获

1. **"约束即性能"** —— 放弃通用性（固定大小 / 固定生命周期），可以大幅简化分配器路径。FixedAllocator 和 nginx pool 方向不同，但思路一致。

2. **Free list 是内存池的基石数据结构** —— 空闲块自身的前几个字节存链表指针，零额外空间开销，分配和释放都是 O(1)。

3. **翻倍扩容是通用策略** —— 无论是 `std::vector` 还是内存池的 chunk 扩展，摊还 O(1) 的核心都是倍增。

4. **Benchmark 要控制变量** —— 预分配 vs 带扩容的测试结果差异巨大。先想清楚"测的是什么"，再设计测试。

5. **读工业代码的价值** —— nginx pool 让我看到了另一种设计哲学（bump pointer + 批量释放），拓宽了对内存管理的认知，不只是"free list 一条路"。

6. **防御性编程保护数据结构** —— deallocate 的边界检查看似多余，但非法指针会**无声地**破坏整条空闲链表，排查成本远高于预防成本。
