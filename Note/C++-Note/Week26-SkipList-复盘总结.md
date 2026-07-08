# Week26 SkipList 复盘总结

## 核心目标

Week26 实现教学版 SkipList，理解随机高度、多层索引和 LevelDB 原版的并发发布思路。

## 跳表思想

有序链表查找是 $O(n)$。跳表在链表上方增加多层索引，每一层以一定概率晋升节点，使查询路径从高层快速跳跃，再逐层下降，期望复杂度接近 $O(\log n)$。

## 随机高度

LevelDB 使用 `kBranching = 4`，即每层约 1/4 概率晋升：

```cpp
while (height < kMaxHeight && rnd.OneIn(4)) ++height;
```

这让高层节点数量快速减少，空间开销可控。

## 插入流程

1. 从最高层开始查找插入位置。
2. 记录每层前驱 `prev[level]`。
3. 生成随机高度。
4. 更新每一层 forward 指针。

## LevelDB 原版并发点

LevelDB SkipList 依赖单写多读模型。写线程发布新节点时需要保证节点内容先构造完成，再让读线程通过指针看见；这就是 acquire/release memory order 的价值。

## 复盘结论

SkipList 比红黑树更容易实现有序插入，也更适合追加写多、并发读多的 MemTable 场景。它牺牲确定性高度，换来实现简单和良好的工程可维护性。
