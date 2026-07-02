# advanced_templates

Week 12（Day 078-084）对应的高级模板技巧和 Month 3 综合验证代码放在这个目录。

## 覆盖内容

- CRTP 静态多态与虚函数动态多态 benchmark
- Policy-Based Design：线程策略和扩容策略组合版固定块分配器
- C++20 concepts：`Printable`、`Container`、`CallableTask`、受约束的 `submit`
- `constexpr` 编译期计算：斐波那契、排序、质数筛、字符串哈希
- Month 3 综合测试：`my_type_traits.h`、`my_tuple.h`、`my_optional.h`、`my_variant.h`、`my_function.h`

## 构建

```powershell
cmake -S CPP-Practice/advanced_templates -B CPP-Practice/advanced_templates/build
cmake --build CPP-Practice/advanced_templates/build --config Release
.\CPP-Practice\advanced_templates\build\Release\advanced_templates_demo.exe
```