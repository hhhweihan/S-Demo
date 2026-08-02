# Week 12 — 高级模板与整月复盘总结

> 学习周期：Day-078 ~ Day-084  
> 核心产出：CRTP benchmark、Policy-Based Allocator、concepts、constexpr 算法、Month 3 综合测试、LevelDB 预研  
> 代码位置：`CPP-Practice/advanced_templates/`

---

## 一、本周主线

Week 12 是 Month 3 的收束周。它不再只做单个组件，而是把模板能力放进更接近工程设计的问题里：

```text
静态多态
  -> CRTP
  -> virtual 对比

策略组合
  -> Policy-Based Design
  -> ThreadingPolicy / GrowthPolicy

接口约束
  -> concepts
  -> std::invocable submit

编译期计算
  -> constexpr algorithm
  -> static_assert

月度整合
  -> type_traits / Tuple / Optional / Variant / Function
```

---

## 二、关键结论

### 1. CRTP vs 虚函数

CRTP 的调用路径是静态绑定，编译器更容易内联；虚函数是运行时分发，适合需要基类指针统一管理未知派生类的场景。

本次 1000 万次面积调用实测：

- CRTP：约 `6.517 ms`
- virtual：约 `12.798 ms`

这组数据说明当前场景下 CRTP 调用成本更低，但选择 CRTP 也意味着类型集合和接口关系更偏编译期，扩展方式不如虚函数自然。

### 2. Policy-Based Design

策略设计把变化点拆成模板参数。例如固定块分配器可以拆成：

- `ThreadingPolicy`：是否加锁
- `GrowthPolicy`：如何扩容

这样 `PolicyFixedAllocator<SingleThreaded, DoubleGrowth>` 和 `PolicyFixedAllocator<MultiThreaded, LinearGrowth>` 是不同类型，各自没有运行时策略分发成本。

### 3. concepts 是更清楚的模板接口

SFINAE 的问题是“失败是隐式发生的”，调用者经常只看到很长的替换错误。concepts 把约束放到模板声明上：

```cpp
template <typename F, typename... Args>
  requires std::invocable<F, Args...>
auto submit(F&& f, Args&&... args);
```

这个写法更像接口契约，也更容易在错误信息里定位问题。

### 4. constexpr 已经能写小型算法

本周用 `constexpr` 完成了：

- 斐波那契
- 数组排序
- 质数筛
- 字符串哈希

这些结果都用 `static_assert` 验证，说明计算发生在编译期。

### 5. LevelDB 预研结果

按 Day84 要求，已完成：

- 克隆 `TestProject/leveldb`
- 初始化 google benchmark / googletest 子模块
- 构建 `db_bench.exe`
- 小规模运行 `fillseq/readrandom`

结果：

- `fillseq`: `11.921 micros/op`, `9.3 MB/s`
- `readrandom`: `0.808 micros/op`, `10000 of 10000 found`

需要注意：当前仓库实际 Month04 是 STL 序列容器，LevelDB 正式计划在 Month07。

---

## 三、本周结论

Week 12 的价值，是把模板从“类型技巧”推进到“接口设计和工程取舍”。CRTP、policy、concepts、constexpr 都不是孤立语法点，它们分别回答了静态分发、策略组合、接口约束、编译期计算这四类工程问题。
