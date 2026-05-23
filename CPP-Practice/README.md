# CPP-Practice

## 这个目录是干什么的

这里存放和主线学习计划配套的练习代码，重点是把 `CPP-Learning-Plan` 中的阶段目标落实为可编译、可测试、可复盘的小项目。

现在主要有这几块：

- [memory_pool](./memory_pool)：内存池相关实现
- [month1_benchmark](./month1_benchmark)：Month 01 对应的 benchmark
- [raii](./raii)：RAII 相关练习
- [thread_pool](./thread_pool)：线程池实现
- [lock_free/README.md](./lock_free/README.md)：无锁结构练习
- [template_traits](./template_traits)：Month 03 / Week 09 模板基础练习

这个目录还会继续补，基本和主线学习计划同步往前推。

## 从哪里开始看

- 先看上层计划说明：[../CPP-Learning-Plan/README.md](../CPP-Learning-Plan/README.md)
- 如果想看 Month 01，对应入口通常是 [memory_pool](./memory_pool) 和 [month1_benchmark](./month1_benchmark)
- 如果想直接看并发方向，可以从 [thread_pool](./thread_pool) 或 [lock_free/README.md](./lock_free/README.md) 开始
- 如果想看模板泛型基础，可以从 [template_traits](./template_traits) 开始

不同子目录基本都是独立构建的，通常需要进到对应目录里单独生成 `build` 再跑自己的构建命令。
