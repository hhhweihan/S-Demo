## shared_ptr 是线程安全的吗

回答不能只说“是”或者“不是”，必须拆成三个层次：

1. **控制块引用计数的增减是线程安全的**
2. **不同 `shared_ptr` 句柄共享同一控制块时，并发拷贝/析构通常是安全的**
3. **并发修改同一个 `shared_ptr` 变量对象不是线程安全的**

再补一句最容易被忽略的话：

> `shared_ptr` 不会让被管理对象自动线程安全，它只管理对象生命周期。

---

## 一、为什么这个问题容易答错

因为“`shared_ptr` 线程安全”这句话经常把三件事混在一起：

- 控制块计数是不是原子的
- 智能指针变量对象本身能不能被多个线程同时改
- 被管理对象内部状态能不能被多个线程同时访问

这三件事不是一个问题。

---

## 二、先区分两个概念

### 1. 同一个 `shared_ptr` 对象

例如：

```cpp
std::shared_ptr<int> sp;
```

如果多个线程同时对这个 `sp` 变量做赋值、`reset()`、`swap()` 等操作，就是在并发修改同一个智能指针对象本身。

### 2. 不同 `shared_ptr` 对象，但共享同一控制块

例如：

```cpp
auto sp1 = std::make_shared<int>(42);
auto sp2 = sp1;
```

`sp1` 和 `sp2` 是两个不同句柄对象，但它们连到同一个控制块。

标准库里“shared_ptr 的引用计数是线程安全的”主要针对的是这种场景。

---

## 三、准确结论

### 结论 1：多线程并发拷贝或析构不同 `shared_ptr` 句柄，安全

前提是这些句柄共享同一个控制块，但线程操作的是不同变量对象。

原因：控制块内部的共享计数增减使用原子操作，多个线程同时增加或减少引用计数不会把计数写坏。

### 结论 2：多线程并发修改同一个 `shared_ptr` 变量，不安全

例如：

```cpp
std::shared_ptr<int> sp = std::make_shared<int>(1);

std::thread t1([&] { sp = std::make_shared<int>(2); });
std::thread t2([&] { sp.reset(); });
```

这里竞争的不是控制块，而是 `sp` 这个对象本身。赋值和 `reset()` 不是单一原子整体，因此需要外部同步。

### 结论 3：`shared_ptr` 不保证对象本身线程安全

例如：

```cpp
auto sp = std::make_shared<std::vector<int>>();
```

就算 `shared_ptr` 生命周期管理没问题，也不意味着多个线程可以无锁同时对 `*sp` 做 `push_back()`。

对象内部状态是否线程安全，要看对象自身和调用方同步策略。

---

## 四、shared_ptr 的控制块里有什么

一个典型的 `shared_ptr` 控制块大致包含：

- `use_count`：有多少 `shared_ptr` 在拥有对象
- `weak_count`：有多少 `weak_ptr` 在观察控制块
- 销毁对象的逻辑
- 控制块自身的释放逻辑

所以它的线程安全，主要体现在：

> 多线程对 `use_count` / `weak_count` 的增减不会互相踩坏。

这不等于整个智能指针对象的所有操作都自动线程安全。

---

## 五、三个典型场景

| 场景 | 是否安全 | 原因 |
|------|----------|------|
| 多线程只读一个稳定的 `shared_ptr` 值 | 一般安全 | 前提是没有线程并发修改同一个句柄对象 |
| 多线程拷贝/析构不同 `shared_ptr` 句柄，但共享同一对象 | 安全 | 控制块引用计数增减是原子的 |
| 多线程并发赋值或 `reset()` 同一个 `shared_ptr` 变量 | 不安全 | 竞争的是同一个智能指针对象本身，需要外部加锁 |

---

## 六、验证样例

### 场景 1：不同句柄共享同一控制块，并发拷贝

```cpp
#include <memory>
#include <thread>
#include <vector>

struct BigObj {
    int data[256]{};
};

int main() {
    auto base = std::make_shared<BigObj>();
    std::vector<std::thread> threads;

    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([base] {
            for (int j = 0; j < 100000; ++j) {
                std::shared_ptr<BigObj> local = base;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}
```

这类代码依赖的是控制块计数线程安全。

### 场景 2：同一个 `shared_ptr` 变量并发修改

```cpp
#include <memory>
#include <thread>

int main() {
    std::shared_ptr<int> sp = std::make_shared<int>(1);

    std::thread t1([&] {
        for (int i = 0; i < 100000; ++i) {
            sp = std::make_shared<int>(i);
        }
    });

    std::thread t2([&] {
        for (int i = 0; i < 100000; ++i) {
            sp.reset();
        }
    });

    t1.join();
    t2.join();
}
```

这类代码理论上应被 ThreadSanitizer 视为数据竞争。

---

## 七、一个常见误区

很多人会说：

> 多线程同时拷贝同一个 shared_ptr 对象，不会有问题。

这句话容易误导。更严谨的说法是：

> 多线程各自操作不同 `shared_ptr` 句柄，但这些句柄共享同一控制块时，控制块引用计数增减是安全的。

如果真的所有线程同时读写的是**同一个变量对象**，那就已经是另一个问题了。

---

## 八、最后的记忆版总结

可以直接记成四句话：

1. `shared_ptr` 的控制块计数线程安全。
2. 不同 `shared_ptr` 句柄共享同一控制块时，并发拷贝/析构通常安全。
3. 同一个 `shared_ptr` 变量对象并发写，不安全。
4. `shared_ptr` 不保证被管理对象本身线程安全。