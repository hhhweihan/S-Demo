#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "lsm/block.h"
#include "lsm/coding.h"
#include "lsm/memtable.h"
#include "lsm/skiplist.h"

// SSTable（Sorted String Table）：MemTable 落盘后的不可变有序文件。块式布局：
//
//   [data block 0]           —— 前缀压缩 + restart 二分（见 block.h）
//   [data block 1]
//   ...
//   [bloom block]            —— 持久化布隆过滤器（BloomFilter::Encode）
//   [index block]            —— 每个 data block 的"最后一个 key → BlockHandle"，供二分定位块
//   [footer]                 —— 定长 48 字节：bloom handle + index handle + 条目数 + magic
//
// 读一个 key：footer → 载入 bloom（快速否定）→ 二分 index 定位候选块 → 只解析那一个块。
// 每个块内 value 编码为 [type:1][value]，把 kValue/kDeletion 墓碑一并持久化——删除信息要落盘才能在
// 更高层遮蔽底层旧值，直到 compaction 到最底层回收。
namespace mini_lsm {

// 单个 data block 的目标大小。超过即切新块。4KB 对齐典型页/块设备粒度。
inline constexpr std::size_t kBlockSize = 4096;
// footer 魔数，标记文件尾、校验是否为合法 SSTable（截断/损坏文件读不到正确 magic）。
inline constexpr std::uint64_t kTableMagic = 0x6c736d7462ull;  // "lsmtb"
inline constexpr std::size_t kFooterSize = 48;  // bloom(16)+index(16)+num_entries(8)+magic(8)

// LookupStatus 定义在 memtable.h（MemTable 与 SSTable 共用），此处直接使用。

class SSTable {
 public:
    // 兼容旧接口：把 (key,value) 对全部当作 kValue 写入。
    void Build(const std::filesystem::path& path,
               const std::vector<std::pair<std::string, std::string>>& entries) {
        std::vector<MemEntry> mem;
        mem.reserve(entries.size());
        for (const auto& [k, v] : entries) {
            mem.push_back(MemEntry{k, ValueType::kValue, v});
        }
        BuildFromEntries(path, mem);
    }

    // 从 MemEntry 列表构建（含墓碑）。entries 会按 user_key 排序后写出。
    void BuildFromEntries(const std::filesystem::path& path, std::vector<MemEntry> entries) {
        path_ = path;
        std::sort(entries.begin(), entries.end(),
                  [](const MemEntry& a, const MemEntry& b) { return a.user_key < b.user_key; });

        std::string file;  // 累积整个文件字节，最后一次性落盘
        BloomFilter bloom(BloomBits(entries.size()));
        std::string index;  // 索引块：num_blocks(4) + 每块 [len-prefixed last_key][handle]
        std::uint32_t num_blocks = 0;
        BlockBuilder builder;
        builder.Reset();
        std::string block_last_key;

        auto flush_block = [&]() {
            if (builder.empty()) return;
            const std::string contents = builder.Finish();
            BlockHandle handle{static_cast<std::uint64_t>(file.size()), contents.size()};
            file.append(contents);
            PutLengthPrefixed(&index, block_last_key);
            handle.EncodeTo(&index);
            ++num_blocks;
            builder.Reset();
        };

        num_entries_ = 0;
        for (const auto& e : entries) {
            bloom.Add(e.user_key);
            std::string encoded;
            encoded.push_back(static_cast<char>(e.type));  // [type:1]
            encoded.append(e.value);                       // [value]
            builder.Add(e.user_key, encoded);
            block_last_key = e.user_key;
            ++num_entries_;
            if (builder.CurrentSizeEstimate() >= kBlockSize) {
                flush_block();
            }
        }
        flush_block();

        // bloom block
        const std::string bloom_bytes = bloom.Encode();
        const BlockHandle bloom_handle{static_cast<std::uint64_t>(file.size()), bloom_bytes.size()};
        file.append(bloom_bytes);

        // index block（前缀 num_blocks）
        std::string index_full;
        PutFixed32(&index_full, num_blocks);
        index_full.append(index);
        const BlockHandle index_handle{static_cast<std::uint64_t>(file.size()), index_full.size()};
        file.append(index_full);

        // footer（定长）：bloom handle + index handle + num_entries + magic
        bloom_handle.EncodeTo(&file);
        index_handle.EncodeTo(&file);
        PutFixed64(&file, num_entries_);
        PutFixed64(&file, kTableMagic);

        WriteFileSync(path_, file);

        // 构建后即处于"已载入"状态，可直接 Get。
        file_ = std::move(file);
        LoadFromMemory();
    }

    // 从磁盘载入。文件损坏/截断（magic 不符或太短）时置为空表。
    void Load(const std::filesystem::path& path) {
        path_ = path;
        file_ = ReadWholeFile(path);
        LoadFromMemory();
    }

    // 三态查找。bloom 否定 → kNotFound；命中块内解析 type。
    LookupStatus Lookup(const std::string& user_key, std::string* value) const {
        if (!valid_ || !bloom_.MayContain(user_key)) {
            return LookupStatus::kNotFound;
        }
        // 二分 index：第一个 last_key >= user_key 的块可能含目标。
        auto it = std::lower_bound(
            index_.begin(), index_.end(), user_key,
            [](const IndexEntry& e, const std::string& k) { return e.last_key < k; });
        if (it == index_.end()) {
            return LookupStatus::kNotFound;
        }
        if (it->handle.offset + it->handle.size > file_.size()) {
            return LookupStatus::kNotFound;  // 句柄越界（损坏）
        }
        Block block(file_.substr(it->handle.offset, it->handle.size));
        auto encoded = block.Get(user_key);
        if (!encoded.has_value() || encoded->empty()) {
            return LookupStatus::kNotFound;
        }
        const auto type = static_cast<ValueType>(static_cast<std::uint8_t>((*encoded)[0]));
        if (type == ValueType::kDeletion) {
            return LookupStatus::kDeleted;
        }
        if (value != nullptr) {
            value->assign(*encoded, 1, encoded->size() - 1);
        }
        return LookupStatus::kFound;
    }

    // 兼容旧接口：命中值返回之，墓碑/缺失均返回 nullopt。
    std::optional<std::string> Get(const std::string& key) const {
        std::string value;
        if (Lookup(key, &value) == LookupStatus::kFound) {
            return value;
        }
        return std::nullopt;
    }

    // 顺序解出全部条目（含墓碑），供 compaction 归并。按 user_key 升序。
    // Block 只提供精确 Get，无遍历接口；用载入时缓存的有序 all_keys_ 逐个 Lookup 还原 type/value。
    std::vector<MemEntry> Entries() const {
        std::vector<MemEntry> out;
        if (!valid_) return out;
        for (const auto& k : all_keys_) {
            std::string value;
            const LookupStatus st = Lookup(k, &value);
            if (st == LookupStatus::kFound) {
                out.push_back(MemEntry{k, ValueType::kValue, value});
            } else if (st == LookupStatus::kDeleted) {
                out.push_back(MemEntry{k, ValueType::kDeletion, std::string()});
            }
        }
        return out;
    }

    std::size_t size() const { return num_entries_; }
    bool valid() const { return valid_; }
    const std::filesystem::path& path() const { return path_; }
    // 供 version/compaction 判定 key 范围重叠。
    const std::string& smallest_key() const { return smallest_key_; }
    const std::string& largest_key() const { return largest_key_; }

 private:
    struct IndexEntry {
        std::string last_key;
        BlockHandle handle;
    };

    // 解析内存中的 file_：读 footer → bloom → index → 缓存有序 key 列表。
    void LoadFromMemory() {
        valid_ = false;
        index_.clear();
        all_keys_.clear();
        smallest_key_.clear();
        largest_key_.clear();
        if (file_.size() < kFooterSize) {
            num_entries_ = 0;
            bloom_ = BloomFilter(0);
            return;
        }
        const char* footer = file_.data() + file_.size() - kFooterSize;
        const BlockHandle bloom_handle = BlockHandle::Decode(footer);
        const BlockHandle index_handle = BlockHandle::Decode(footer + kBlockHandleSize);
        num_entries_ = DecodeFixed64(footer + 2 * kBlockHandleSize);
        const std::uint64_t magic = DecodeFixed64(footer + 2 * kBlockHandleSize + 8);
        if (magic != kTableMagic) {
            num_entries_ = 0;
            bloom_ = BloomFilter(0);
            return;  // 非法/损坏文件
        }
        if (bloom_handle.offset + bloom_handle.size > file_.size() ||
            index_handle.offset + index_handle.size > file_.size()) {
            num_entries_ = 0;
            bloom_ = BloomFilter(0);
            return;
        }
        bloom_ = BloomFilter::Decode(file_.substr(bloom_handle.offset, bloom_handle.size));

        // 解析 index block。
        const std::string index = file_.substr(index_handle.offset, index_handle.size);
        if (index.size() < 4) {
            bloom_ = BloomFilter(0);
            return;
        }
        std::size_t pos = 0;
        const std::uint32_t num_blocks = DecodeFixed32(index.data());
        pos += 4;
        for (std::uint32_t i = 0; i < num_blocks; ++i) {
            if (pos + 4 > index.size()) return;
            const std::uint32_t klen = DecodeFixed32(index.data() + pos);
            pos += 4;
            if (pos + klen + kBlockHandleSize > index.size()) return;
            IndexEntry e;
            e.last_key = index.substr(pos, klen);
            pos += klen;
            e.handle = BlockHandle::Decode(index.data() + pos);
            pos += kBlockHandleSize;
            index_.push_back(std::move(e));
        }
        valid_ = true;

        // 缓存有序 user_key 列表（供 Entries 归并与范围判定）：逐块线性解码每条 key。
        for (const auto& idx : index_) {
            if (idx.handle.offset + idx.handle.size > file_.size()) {
                valid_ = false;
                return;
            }
            DecodeBlockKeys(file_.substr(idx.handle.offset, idx.handle.size));
        }
        if (!all_keys_.empty()) {
            smallest_key_ = all_keys_.front();
            largest_key_ = all_keys_.back();
        }
    }

    // 线性解码一个 data block 的所有 user_key，追加到 all_keys_。镜像 block.h 的 entry 编码。
    void DecodeBlockKeys(const std::string& block) {
        if (block.size() < 4) return;
        const std::uint32_t num_restarts = DecodeFixed32(block.data() + block.size() - 4);
        const std::size_t restart_bytes = num_restarts * 4 + 4;
        if (restart_bytes > block.size()) return;
        const std::size_t entries_end = block.size() - restart_bytes;
        std::size_t offset = 0;
        std::string prev;
        while (offset + 12 <= entries_end) {
            const std::uint32_t shared = DecodeFixed32(block.data() + offset);
            const std::uint32_t non_shared = DecodeFixed32(block.data() + offset + 4);
            const std::uint32_t value_len = DecodeFixed32(block.data() + offset + 8);
            const std::size_t key_pos = offset + 12;
            const std::size_t value_pos = key_pos + non_shared;
            if (value_pos + value_len > entries_end || shared > prev.size()) return;
            std::string key = prev.substr(0, shared);
            key.append(block, key_pos, non_shared);
            all_keys_.push_back(key);
            prev = std::move(key);
            offset = value_pos + value_len;
        }
    }

    static std::size_t BloomBits(std::size_t num_keys) {
        // ~10 bits/key（LevelDB 默认），下限 64 位避免空表退化。
        const std::size_t bits = num_keys * 10;
        return bits < 64 ? 64 : bits;
    }

    static std::string ReadWholeFile(const std::filesystem::path& path) {
        std::string out;
        const int fd = ::open(path.c_str(), O_RDONLY);
        if (fd < 0) return out;
        char buf[4096];
        while (true) {
            const ssize_t r = ::read(fd, buf, sizeof(buf));
            if (r < 0) {
                if (errno == EINTR) continue;
                break;
            }
            if (r == 0) break;
            out.append(buf, static_cast<std::size_t>(r));
        }
        ::close(fd);
        return out;
    }

    // 写整个文件并 fsync——SSTable 落盘必须 durable，崩溃恢复才能信任已存在的 .sst。
    static void WriteFileSync(const std::filesystem::path& path, const std::string& contents) {
        const int fd = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) return;
        std::size_t written = 0;
        while (written < contents.size()) {
            const ssize_t r = ::write(fd, contents.data() + written, contents.size() - written);
            if (r < 0) {
                if (errno == EINTR) continue;
                break;
            }
            written += static_cast<std::size_t>(r);
        }
#if defined(__APPLE__)
        if (::fcntl(fd, F_FULLFSYNC) != 0) {
            ::fsync(fd);
        }
#elif defined(__linux__)
        ::fdatasync(fd);
#else
        ::fsync(fd);
#endif
        ::close(fd);
    }

    std::filesystem::path path_;
    std::string file_;  // 整个 SSTable 字节常驻内存（mini 版简化；真库按需读块）
    std::vector<IndexEntry> index_;
    std::vector<std::string> all_keys_;  // 有序 user_key 缓存（Entries/范围判定用）
    BloomFilter bloom_{0};
    std::string smallest_key_;
    std::string largest_key_;
    std::uint64_t num_entries_ = 0;
    bool valid_ = false;
};

}  // namespace mini_lsm
