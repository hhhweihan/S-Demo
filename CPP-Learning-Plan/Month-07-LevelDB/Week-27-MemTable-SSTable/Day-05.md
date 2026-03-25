## Day 5（Fri）— 读 BloomFilter

**预计时间：1 小时**

**任务：**
- [ ] 读 `util/bloom.cc`（约 80 行）
- [ ] 理解 Bloom Filter 的两个操作：
  - `CreateFilter(keys)`：对每个 key 用 k 个哈希函数，各置位
  - `KeyMayMatch(key, filter)`：检查 k 个位是否全为 1
- [ ] 回答：
  1. LevelDB 用了几个哈希函数（hint：`bits_per_key * ln2`）？
  2. 假设 10 bits/key，误判率大约是多少（约 1%）？
  3. Bloom Filter 只能有假阳性（false positive），不能有假阴性，为什么？

**完成标志：** 3 个问题有答案

---