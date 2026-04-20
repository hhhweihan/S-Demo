# Week 3 — 智能指针从零实现

## 本周目标

手写 `unique_ptr`、`shared_ptr`、`weak_ptr`，彻底理解移动语义、引用计数控制块和循环引用。

---

## 本周完成情况

本周已经把智能指针的核心模型从“会自动析构的语法糖”拉回到了它真正的本质：**所有权管理**。

这一周按下面的路线推进：

1. `unique_ptr`：理解独占所有权、move 语义、`release/reset`
2. `shared_ptr`：引入控制块、`use_count`、共享所有权
3. `weak_ptr`：区分“观察者”和“拥有者”，解决循环引用
4. `enable_shared_from_this`：让对象内部安全拿回同一控制块上的 `shared_ptr`
5. 对比 libc++：理解 `__shared_ptr_pointer`、`__shared_ptr_emplace`、`make_shared` 和 deleter
6. 线程安全复盘：搞清楚“控制块线程安全”和“句柄对象线程安全”不是一回事

---

## 本周最重要的三条结论

### 1. 智能指针的核心不是“像指针一样用”

智能指针首先表达的是资源所有权规则：

- `unique_ptr` 表达独占所有权
- `shared_ptr` 表达共享所有权
- `weak_ptr` 表达非拥有型观察关系

### 2. `shared_ptr` 的灵魂是控制块

如果没有控制块，`shared_ptr` 只是一堆指向同一个对象的裸指针变量。只有通过控制块中的 `use_count / weak_count`，生命周期管理才真正成立。

### 3. `weak_ptr` 不是可有可无的辅助类

它负责把“观察”从“拥有”里拆出去，是循环引用问题的标准解法，也是 `enable_shared_from_this` 的基础。

---

## 本周输出

- Day 15：`unique_ptr` 的最小实现与测试思路
- Day 16：`shared_ptr` 的控制块模型与引用计数逻辑
- Day 17：`weak_ptr` 与循环引用打破
- Day 18：`enable_shared_from_this` 的工作原理
- Day 19：libc++ 对比，明确工业实现的扩展点
- Day 20：`shared_ptr` 线程安全边界
- Day 21：周复盘与统一结论收束

---

## 对下周的衔接

Week 4 做 RAII 和 scope guard 时，建议把这周的结论继续抽象一层：

1. RAII 的本质不是“析构函数里写释放代码”
2. 而是“对象生命周期绑定资源生命周期”
3. 智能指针只是 RAII 在堆内存场景中的一个典型实例

如果把这点打通，后面文件句柄、锁、事务回滚和异常安全都会更容易串起来。

---