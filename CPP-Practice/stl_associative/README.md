# stl_associative

Month 05（Week 17-20）对应的自编 STL 关联容器、哈希表、算法和 allocator 练习代码放在这个目录。

## 覆盖内容

- `RBTree<Key, Value>`：红黑树插入、旋转、变色、中序迭代、`lower_bound` / `upper_bound`
- `MySet<T>` / `MyMap<K, V>`：基于 RBTree 的有序关联容器封装，含教学版 `erase`
- `MyMultiSet<T>` / `MyMultiMap<K, V>`：允许重复 key 的接口差异示例
- `MyUnorderedMap<K, V>`：开链哈希表、负载因子、自动 rehash、`erase`
- `heap_sort` / `stable_sort` / `partial_sort` / `binary_search`：算法族最小闭环
- `PoolAllocator<T>`：接入标准 `allocator_traits` 的 allocator 示例

## 构建

```powershell
cmake -S CPP-Practice/stl_associative -B CPP-Practice/stl_associative/build
cmake --build CPP-Practice/stl_associative/build --config Release
.\CPP-Practice\stl_associative\build\Release\stl_associative_demo.exe
```