## Day 5（Fri）— 读 Comparator + Options

**预计时间：1 小时**

**任务：**
- [ ] 读 `include/leveldb/comparator.h` + `include/leveldb/options.h`（约 150 行）
- [ ] 理解：LevelDB 的所有 key 比较都通过 `Comparator` 接口，为什么这样设计（解耦、可插拔）
- [ ] 找出 `Options` 里最重要的 5 个配置项，每个用一句话说明作用：
  - `write_buffer_size`
  - `max_open_files`
  - `block_size`
  - `compression`
  - `block_cache`

**完成标志：** 5 个配置项有解释

---