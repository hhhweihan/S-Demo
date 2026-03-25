## Day 6（Sat）— 读 libc++ unordered_map 源码

**预计时间：2 小时**

**任务：**
- [ ] 阅读 libc++ `include/__hash_table`（重点：`__bucket_list_`、rehash 策略、iterator）
- [ ] 回答：
  1. libc++ 使用 forward_list 而非普通链表存储桶内节点，为什么？
  2. libc++ 如何保证 `end()` 迭代器的合法性（所有桶用一个链表串联？）？
  3. 为什么哈希表的桶数选质数可以减少碰撞？

**完成标志：** 3 个问题有答案

---

