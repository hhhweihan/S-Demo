# Week 04 — RAII 工具 + 整月复盘总结

> **学习周期：** Day-022 ~ Day-028（约 8 小时）  
> **核心产出：** `ScopeGuard`、`DEFER` 宏、`FileGuard`、Month 1 AddressSanitizer 扫描记录、Month 1 综合 benchmark、Month 2 衔接结论  
> **代码位置：** `CPP-Practice/raii/`、`CPP-Practice/month1_benchmark/`、`CPP-Practice/memory_pool/`  
> **完成状态：** 本周 7 天任务已完成；`git commit / push` 保留未执行

---

## 一、这周到底在干嘛？

如果说 Week01~03 主要在解决“资源如何分配、如何共享、如何释放”，那 Week04 做的事情就是把这些零散结论真正收口。

这一周表面上的任务很多：

- 写 `scope_guard`
- 写 `DEFER` 宏
- 写 `FileGuard`
- 跑 ASan
- 跑整月 benchmark
- 写月度总结
- 准备 Month 2

但它们其实都指向同一件事：

> 把前面三周积累的生命周期管理思路，变成一套可以复用、可以验证、也可以向下个月继续迁移的工作方式。

所以 Week04 不是“补作业周”，而是 Month 1 从概念学习切换到工程收尾的一周。

可以把本周主线压成下面这条：

```text
RAII 基础工具
  -> 作用域退出清理
  -> 真实资源句柄封装
  -> sanitizer 验证
  -> benchmark 量化
  -> 月度认知收束
```

---

## 二、七天任务全景

### Day 22 — 手写 ScopeGuard

这一天下来最关键的收获，是把 RAII 从“堆对象自动 delete”推广到了“任意退出动作自动执行”。

`ScopeGuard` 做的事情很纯粹：

- 构造时接收一个可调用对象
- 析构时执行它
- `dismiss()` 后不再执行

真正重要的不是这几个接口本身，而是背后的抽象：

> 收尾逻辑不应该散落在每个 `return`、每个 `if`、每个异常分支里，而应该绑定到对象生命周期上。

这一点一旦吃透，后面的 `DEFER` 宏、文件句柄封装、锁释放、回滚动作，其实都只是同一个模型的不同写法。

---

### Day 23 — DEFER 宏 + FileGuard

Day 22 搭的是底层模型，Day 23 开始把这个模型变成更顺手的工具。

#### 1. `DEFER` 宏

`DEFER(code)` 的意义不是引入新语义，而是把：

```cpp
auto guard = make_scope_guard([&] { code; });
```

压缩成一行，让“登记退出清理动作”这件事更轻量。

这一层语法糖很小，但工程价值很大，因为它直接降低了使用 RAII 收尾的门槛。

#### 2. `FileGuard`

如果说 `DEFER` 适合一次性的局部清理，`FileGuard` 就是在做更强的资源拥有者封装：

- 构造时 `fopen`
- 析构时 `fclose`
- 禁拷贝，防止 double-close
- 支持 move，明确转移文件句柄所有权
- 构造失败直接抛异常，避免半有效对象

这一天真正落下来的不是 `FILE*` 这个类型，而是一个非常通用的设计模板：

> 资源一旦构造成功，就立刻进入托管状态；后续只允许显式转移，不允许隐式复制。

这条原则在 socket、fd、mutex、transaction guard 上都一样成立。

---

### Day 24 — AddressSanitizer 扫描

Week04 从这一天开始进入“验证阶段”。

本次实际扫描的可执行资产主要有两组：

1. `CPP-Practice/memory_pool/`
2. `CPP-Practice/month1_benchmark/`

也就是说，覆盖到了：

- `FixedAllocator`
- `PoolAllocator`
- `std::allocator` 适配
- 教学版 `UniquePtr / SharedPtr` benchmark 路径

这里有一个很值得记住的工程细节：在 Windows + MSVC 下，ASan 并不支持 `detect_leaks=1`。如果把这个选项也塞进 `ASAN_OPTIONS`，程序会因为 **工具平台限制** 直接退出，而不是因为代码本身触发了内存错误。

这一点这周已经踩到过一次，所以最后的正确结论应该是：

- **需要禁用 `detect_leaks` 再跑 Windows ASan**
- 在正确配置下，本轮扫描没有出现 `heap-use-after-free`、`heap-buffer-overflow` 等报错

这一条很重要，因为它说明“退出码非 0”不一定都是代码坏了，也可能是工具能力边界没分清。

---

### Day 25 — 修复内存问题 / 复查结论

因为 Day 24 的有效扫描结果是零报错，所以 Day 25 没有进入“逐条修 bug”的状态，而是转成了一次结构性复查：

- 哪些错误在这个月最容易发生
- 当前代码为什么暂时没被 ASan 抓住
- 现有实现里哪些设计本身就在防这些问题

比如：

- `UniquePtr`、`FileGuard`、`ScopeGuard` 的 move/生命周期约束，在防 double free / double close
- `PoolAllocator` 的 header 和对齐逻辑，在防错 class 释放和越界写
- `FixedAllocator` 的 Debug 范围断言，在防 free list 污染

所以 Day 25 的价值不在“勉强凑出几处修改”，而在于把“当前没报错”对应回了正确的设计原因。

---

### Day 26 — 整月综合 Benchmark

这一天新增了一个独立工程：

- `CPP-Practice/month1_benchmark/`

原因很现实：Week01 和 Week02 已经有可运行代码，但 Week03 主要还是学习计划和设计文档，缺少统一 benchmark 入口。所以需要一个单独程序，把 Month 1 的核心产出放进同一套计时框架：

- `FixedAllocator`
- `PoolAllocator`
- 教学版 `SharedPtr`
- 教学版 `UniquePtr`

本次实测数据如下：

| 测试项 | 时间 |
|--------|------|
| FixedAllocator 100 万次 | 18.121 ms |
| PoolAllocator 混合 100 万次 | 18.985 ms |
| SharedPtr 100 万次 | 47.521 ms |
| UniquePtr 100 万次 | 22.463 ms |

这组数据特别适合拿来帮助理解 Month 1 的抽象代价：

- 固定场景、强约束的方案最快
- 通用 allocator 加了一层抽象，但成本仍然可控
- `SharedPtr` 因为控制块和引用计数明显更重
- `UniquePtr` 更接近“带所有权语义的轻量包装”

这一天的重点不是“谁赢了”，而是终于把 Month 1 的不同设计代价量化出来了。

---

### Day 27 — 月度总结文章

这一天做的不是再写一遍流水账，而是把 Month 1 的关键问题重新组织成一篇可以反复回看的总结文章：

- 为什么 free list 能快
- 为什么通用 allocator 必须引入 size class
- 为什么 `shared_ptr` 的灵魂是控制块
- Month 1 的 1~2 个核心认知点是什么
- 后续可以往哪里继续深入

这一层很重要，因为从 Month 2 开始，内容会切进并发、线程池、future、原子、内存序。如果不先把 Month 1 的生命周期管理和资源收尾模型总结清楚，后面会很容易只剩接口堆砌，没有统一主线。

---

### Day 28 — 规划 Month 2

最后一天不是简单“看一下 README”，而是确认 Month 2 的知识顺序是合理的。

当前计划的衔接路线很清楚：

1. 先把线程生命周期和基本同步写扎实
2. 再做 `condition_variable`、`future/promise`
3. 最后进入 `atomic`、内存模型和无锁结构

这条顺序和当前笔记里的重点部分是对齐的：

- 线程创建、参数传递、所有权转移
- 互斥、锁粒度、死锁规避
- `condition_variable`
- `future / promise / async`
- 原子类型、先行关系、内存序

也就是说，Month 2 不是完全换了一个新话题，而是把 Month 1 的“生命周期和收尾纪律”继续带入并发环境。

---

## 三、本周最核心的三条收获

### 1. RAII 不是“析构函数里写释放代码”这么简单

Week04 最大的价值，是把 RAII 从“智能指针会自动 delete”这个单一印象里解放出来。

更准确的理解应该是：

> 只要有一段收尾动作必须在离开作用域时执行，就值得考虑把它对象化。

这可以是：

- delete
- fclose
- unlock
- rollback
- 删除临时文件
- 统计埋点收尾

一旦用这种视角看问题，RAII 就不再局限在内存管理，而会变成一种更广义的结构化资源管理方法。

### 2. 工具验证必须理解平台边界

这一周一个非常具体的教训是：Windows + MSVC 的 ASan 不支持 `detect_leaks=1`。如果不先分清楚工具链能力边界，就会把“工具不支持”误判成“代码有 bug”。

所以正确的工作流不只是“跑 sanitizer”，而是：

1. 知道当前平台支持什么
2. 用正确配置运行它
3. 再解释结果

这条经验 Month 2 还会继续复用，尤其到 TSan、并发测试和平台差异时更明显。

### 3. Month 1 到这周才真正闭环

Week01~03 更像是在逐步搭能力：

- 内存池
- 通用 allocator
- 智能指针与控制块

Week04 则把这些能力真正闭环了：

- 有 RAII 工具
- 有可运行样例
- 有 sanitizer 体检
- 有 benchmark 数据
- 有月度总结和下月衔接

没有 Week04，这个月会更像“完成了一堆局部练习”；有了 Week04，它才更像一个完整项目周期。

---

## 四、Week4 的代码与结果归档

### 4.1 本周新增代码

- `CPP-Practice/raii/scope_guard.h`
- `CPP-Practice/raii/file_guard.h`
- `CPP-Practice/raii/main.cpp`
- `CPP-Practice/month1_benchmark/smart_ptr.h`
- `CPP-Practice/month1_benchmark/month1_benchmark.cpp`

### 4.2 本周验证结果

#### RAII 示例程序

- `ScopeGuard` 正常路径通过
- `dismiss()` 路径通过
- 异常路径下清理动作仍会执行
- `DEFER` 宏可正常登记退出清理动作
- `FileGuard` move 后源对象失效，目标对象继续持有句柄
- `FileGuard` 构造失败时会抛异常，不产生半有效对象

#### ASan

- `memory_pool`：在正确 MSVC ASan 配置下无内存错误报告
- `month1_benchmark`：禁用 `detect_leaks` 后运行通过，无内存错误报告

#### Month 1 benchmark

| 测试项 | 时间 |
|--------|------|
| FixedAllocator 100 万次 | 18.121 ms |
| PoolAllocator 混合 100 万次 | 18.985 ms |
| SharedPtr 100 万次 | 47.521 ms |
| UniquePtr 100 万次 | 22.463 ms |

---

## 五、这周对下个月最大的帮助是什么

我觉得不是“又多写了两个类”，而是这周把一种很重要的开发节奏固定下来了：

```text
先建最小模型
  -> 再做具象封装
  -> 再跑工具验证
  -> 再用 benchmark 量化
  -> 最后写复盘收束
```

Month 2 做并发编程时，这个节奏会比 Month 1 更重要，因为并发 bug 通常比内存 bug 更隐蔽、更难靠肉眼发现。

如果还能保持 Week04 这种节奏：

- 小步实现
- 及时验证
- 解释工具输出
- 用总结压缩认知

那后面的线程池、无锁队列、future、内存序就会更容易真正学进去，而不是只会抄接口。

---

## 六、一句话收束 Week4

Week04 真正完成的，不只是 `scope_guard` 和 `FileGuard` 这两个小工具，而是把 Month 1 从“我已经写过这些东西”推进成了“我已经能用工具验证它们、量化它们，并把它们整理成一套完整的工程认知”。