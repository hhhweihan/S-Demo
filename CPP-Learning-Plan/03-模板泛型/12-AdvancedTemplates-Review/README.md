# 高级技巧 + 整月复盘

## 本周目标

CRTP 静态多态、policy-based design、C++20 concepts 入门，跑整月综合项目。

## 本周完成情况

- [x] Day 78：实现 CRTP 与虚函数版本面积计算，并完成 1000 万次调用 benchmark
- [x] Day 79：实现 Policy-Based Design 版本固定块分配器
- [x] Day 80：用 concepts 替代 SFINAE，并给线程池 `submit` 增加 C++20 可调用约束
- [x] Day 81：实现 constexpr 斐波那契、排序、质数筛和字符串哈希
- [x] Day 82：整合 Month 3 小型工具库并跑综合测试
- [x] Day 83：完成 C++ 模板泛型实战总结文章
- [x] Day 84：完成 LevelDB clone、子模块初始化、`db_bench` 构建和小规模运行；`git commit / push` 等待明确确认后再执行

## 本周产出

- 练习工程：`CPP-Practice/advanced_templates/`
- 线程池约束改动：`CPP-Practice/thread_pool/concurrent/thread_pool.h`
- LevelDB 预研目录：`TestProject/leveldb/`
- 周复盘：`Note/C++-Note/Week12-高级模板与整月复盘总结.md`
- 月总结：`Note/C++-Note/Month03-模板泛型实战总结.md`

## 阶段小结
Week 12 是 Month 3 的高级技巧和整月收束周。CRTP 和虚函数对比解决“静态多态还是动态多态”的选择问题；Policy-Based Design 把策略变化从运行时分支提升为模板参数；concepts 把 SFINAE 的隐式失败变成更清晰的接口约束；constexpr 把一部分计算提前到编译期；综合项目则把 Week09-11 的 `type_traits`、`Tuple`、`Optional`、`Variant`、`Function` 放进同一工程验证。

本周最重要的收获有五点：

1. CRTP 适合类型集合已知、追求内联和零虚调用成本的场景；虚函数适合运行时扩展和 ABI 边界。
2. Policy-Based Design 的价值是把变化点参数化，让不同策略组合在编译期生成不同类型。
3. concepts 让模板约束成为接口的一部分，错误信息和可读性通常优于 `enable_if`。
4. constexpr 不只是常量函数，C++20 下已经能承担数组排序、筛法和字符串哈希这类小型编译期算法。
5. Month 3 的组件能一起编译运行，说明模板工具、变参展开和类型擦除之间已经形成最小闭环。

## 验证命令

```powershell
cmake -S CPP-Practice/advanced_templates -B CPP-Practice/advanced_templates/build
cmake --build CPP-Practice/advanced_templates/build --config Release
.\CPP-Practice\advanced_templates\build\Release\advanced_templates_demo.exe
```

## LevelDB 预研命令

```powershell
git clone --depth 1 https://github.com/google/leveldb TestProject/leveldb
cd TestProject/leveldb
git submodule update --init --recursive
cd ..\..
cmake -S TestProject/leveldb -B TestProject/leveldb/build -DLEVELDB_BUILD_TESTS=ON -DLEVELDB_BUILD_BENCHMARKS=ON
cmake --build TestProject/leveldb/build --config Release --target db_bench
.\TestProject\leveldb\build\Release\db_bench.exe --benchmarks=fillseq,readrandom --num=10000 --reads=10000
```

---