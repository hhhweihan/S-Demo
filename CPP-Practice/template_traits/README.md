# template_traits

Week 09（Day 057）对应的模板基础练习代码放在这个目录。

## 覆盖内容

- 函数模板、显式特化、普通函数重载的优先级差异
- 用 3 个最小例子区分“模板特化”和“函数重载”分别在什么时候生效
- ADL（Argument-Dependent Lookup）配合 `using std::swap; swap(a, b);` 的常见写法
- 最简版 `is_same<T, U>` 实现与静态断言验证

## 构建

```powershell
cmake -S CPP-Practice/template_traits -B CPP-Practice/template_traits/build
cmake --build CPP-Practice/template_traits/build --config Release
.\CPP-Practice\template_traits\build\template_traits_demo.exe
```
