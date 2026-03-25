## Day 2（Tue）— 读 SSTable 文件格式

**预计时间：1 小时**

**任务：**
- [ ] 读 `table/format.h`（约 100 行）
- [ ] 画出一个 SSTable 文件的 byte layout：
  ```
  [Data Block 0]
  [Data Block 1]
  ...
  [Data Block N]
  [Meta Block: BloomFilter]
  [Meta Index Block]
  [Index Block]        ← 每个 Data Block 的 key 范围 + offset
  [Footer]             ← 固定 48 字节，包含 Index Block 的 offset/size
  ```
- [ ] 回答：读取一个 key 时，如何通过 Footer → Index Block → Data Block 定位？

**完成标志：** 能画出完整的 SSTable 文件布局图

---