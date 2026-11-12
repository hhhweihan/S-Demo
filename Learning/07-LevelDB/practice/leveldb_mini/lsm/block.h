#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "lsm/coding.h"

// SSTable 的数据块（data block）——块式存储的最小单元。一个 SSTable 由若干 data block 顺序拼成，
// 每个块内 key 有序、独立压缩、独立二分。块式的意义：读一个 key 只需定位并解析它所在的那**一个**块
// （几 KB），而非全表；块也是压缩/缓存/校验的天然粒度。
//
// 块内格式（全用 coding.h 的定长小端）：
//   entries 区（逐条）：
//     shared(4)     —— 与**前一条 key** 的公共前缀长度
//     non_shared(4) —— 本 key 去掉公共前缀后剩余字节数
//     value_len(4)  —— value 字节数
//     key_delta     —— 本 key 的后 non_shared 字节
//     value         —— value_len 字节
//   trailer 区：
//     restart[0..R-1](各 4) —— 每个 restart 点的字节偏移（该处 entry 的 shared 必为 0）
//     num_restarts(4)       —— restart 点个数
//
// **前缀压缩 + restart 点**：相邻 key 常有长公共前缀（如
// "user:1001"/"user:1002"），只存增量省空间。 但纯增量无法随机定位，于是每隔 kRestartInterval
// 条强制存一个"整 key"（shared=0）作为 restart 点， 并记录其偏移。查找时先在 restart
// 点上二分（每个都是完整 key，可直接比较），锁定候选区间，再从该 restart
// 点线性解码到目标——把"必须从头解码"降到"从最近的 restart 点解码一小段"。
namespace mini_lsm {

// 块内每隔多少条设一个 restart 点。16 是 LevelDB 默认值：压缩率与查找扫描长度的折中。
inline constexpr std::size_t kBlockRestartInterval = 16;

// 块句柄：一个块在 SSTable 文件中的位置（offset + size）。index block 与 footer 用它指向数据块。
struct BlockHandle {
    std::uint64_t offset = 0;
    std::uint64_t size = 0;

    void EncodeTo(std::string* dst) const {
        PutFixed64(dst, offset);
        PutFixed64(dst, size);
    }
    static BlockHandle Decode(const char* ptr) {
        BlockHandle h;
        h.offset = DecodeFixed64(ptr);
        h.size = DecodeFixed64(ptr + 8);
        return h;
    }
};
inline constexpr std::size_t kBlockHandleSize = 16;  // offset(8) + size(8)

// 块构建器：按 **key 升序** 依次 Add，最后 Finish 得到块字节。调用方保证有序。
class BlockBuilder {
 public:
    void Add(const std::string& key, const std::string& value) {
        std::size_t shared = 0;
        if (counter_ < kBlockRestartInterval) {
            // 与前一 key 求公共前缀长度（受剩余量约束）。
            const std::size_t min_len = std::min(last_key_.size(), key.size());
            while (shared < min_len && last_key_[shared] == key[shared]) {
                ++shared;
            }
        } else {
            // 到达 restart 间隔：本条作为新 restart 点，存整 key（shared=0）。
            restarts_.push_back(static_cast<std::uint32_t>(buffer_.size()));
            counter_ = 0;
        }
        const std::size_t non_shared = key.size() - shared;

        PutFixed32(&buffer_, static_cast<std::uint32_t>(shared));
        PutFixed32(&buffer_, static_cast<std::uint32_t>(non_shared));
        PutFixed32(&buffer_, static_cast<std::uint32_t>(value.size()));
        buffer_.append(key, shared, non_shared);
        buffer_.append(value);

        last_key_ = key;
        ++counter_;
        ++num_entries_;
    }

    // 收尾：追加 restart 数组与计数，返回完整块字节。Finish 后不应再 Add。
    std::string Finish() {
        for (const std::uint32_t r : restarts_) {
            PutFixed32(&buffer_, r);
        }
        PutFixed32(&buffer_, static_cast<std::uint32_t>(restarts_.size()));
        return buffer_;
    }

    bool empty() const { return num_entries_ == 0; }
    std::size_t num_entries() const { return num_entries_; }
    // 估算当前字节数（含尚未写入的 restart trailer），供上层决定何时切块。
    std::size_t CurrentSizeEstimate() const { return buffer_.size() + (restarts_.size() + 1) * 4; }

    void Reset() {
        buffer_.clear();
        restarts_.assign(1, 0u);  // 第一条永远是一个 restart 点（偏移 0）
        last_key_.clear();
        counter_ = 0;
        num_entries_ = 0;
    }

 private:
    std::string buffer_;
    std::vector<std::uint32_t> restarts_{0u};  // 首个 restart 点在偏移 0
    std::string last_key_;
    std::size_t counter_ = 0;      // 距上个 restart 点已写条数
    std::size_t num_entries_ = 0;  // 总条数
};

// 块读取器：持有块字节的拷贝，支持按 key 精确查找（restart 二分 + 段内线性解码）。
// 所有偏移解析都做边界检查——块可能来自损坏/截断文件，越界读是 ASan 会抓的真 UB。
class Block {
 public:
    explicit Block(std::string contents) : data_(std::move(contents)) {
        if (data_.size() < 4) {
            num_restarts_ = 0;
            restart_offset_ = 0;
            return;
        }
        num_restarts_ = DecodeFixed32(data_.data() + data_.size() - 4);
        // restart 数组区大小 = num_restarts*4 + 4(计数本身)。其起点即 entries 区的终点。
        const std::size_t restart_bytes = num_restarts_ * 4 + 4;
        restart_offset_ = restart_bytes <= data_.size() ? data_.size() - restart_bytes : 0;
    }

    // 精确查找 target。命中返回 value，否则 nullopt。
    std::optional<std::string> Get(const std::string& target) const {
        if (num_restarts_ == 0) {
            return std::nullopt;
        }
        // ① 在 restart 点上二分：找最后一个"restart key <= target"的 restart 点作为扫描起点。
        std::size_t left = 0;
        std::size_t right = num_restarts_ - 1;
        while (left < right) {
            const std::size_t mid = (left + right + 1) / 2;
            std::string key;
            if (!KeyAtRestart(mid, &key)) {
                return std::nullopt;  // 结构损坏
            }
            if (key <= target) {
                left = mid;
            } else {
                right = mid - 1;
            }
        }
        // ② 从该 restart 点线性解码，维护前缀累积出完整 key，直到 key >= target。
        std::size_t offset = RestartPointOffset(left);
        std::string key;
        while (offset < restart_offset_) {
            std::string value;
            std::size_t next = 0;
            if (!DecodeEntry(offset, &key, &value, &next)) {
                return std::nullopt;
            }
            if (key == target) {
                return value;
            }
            if (key > target) {
                return std::nullopt;  // 已越过，块内有序，后面不可能再有
            }
            offset = next;
        }
        return std::nullopt;
    }

 private:
    std::size_t RestartPointOffset(std::size_t index) const {
        return DecodeFixed32(data_.data() + restart_offset_ + index * 4);
    }

    // 取第 index 个 restart 点处的整 key（该处 shared 必为 0）。
    bool KeyAtRestart(std::size_t index, std::string* key) const {
        const std::size_t offset = RestartPointOffset(index);
        std::string value;
        std::size_t next = 0;
        std::string acc;  // restart 点 shared=0，前缀累积从空开始
        return DecodeEntryFrom(offset, acc, key, &value, &next);
    }

    // 解码 offset 处一条 entry。key 传入时应为"前一条 key"（用于拼接 shared 前缀），返回完整 key。
    bool DecodeEntry(std::size_t offset, std::string* key, std::string* value,
                     std::size_t* next) const {
        return DecodeEntryFrom(offset, *key, key, value, next);
    }

    // 核心解码：prev 为前一条完整 key，据 shared 拼出本条 key。全程边界检查。
    bool DecodeEntryFrom(std::size_t offset, const std::string& prev, std::string* key,
                         std::string* value, std::size_t* next) const {
        if (offset + 12 > restart_offset_) {
            return false;  // 连三个定长头都放不下
        }
        const std::uint32_t shared = DecodeFixed32(data_.data() + offset);
        const std::uint32_t non_shared = DecodeFixed32(data_.data() + offset + 4);
        const std::uint32_t value_len = DecodeFixed32(data_.data() + offset + 8);
        const std::size_t key_pos = offset + 12;
        const std::size_t value_pos = key_pos + non_shared;
        if (value_pos + value_len > restart_offset_ || shared > prev.size()) {
            return false;  // 越界或 shared 不合法
        }
        key->assign(prev, 0, shared);
        key->append(data_, key_pos, non_shared);
        value->assign(data_, value_pos, value_len);
        *next = value_pos + value_len;
        return true;
    }

    std::string data_;
    std::uint32_t num_restarts_ = 0;
    std::size_t restart_offset_ = 0;  // entries 区终点 = restart 数组区起点
};

}  // namespace mini_lsm
