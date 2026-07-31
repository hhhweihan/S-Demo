#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "lsm/coding.h"
#include "lsm/skiplist.h"

// MemTable：写入首先落地的内存有序表，用 SkipList 做 O(log n) 索引。相比原教学版把版本信息塞进
// 文本 key（"key#seq"）、Get 靠全表扫描，这里改用 LevelDB 的 **internal key** 二进制格式并支持
// **tombstone（删除墓碑）**，让删除、多版本、跨层遮蔽都能正确表达。
//
// internal key = user_key + tag(8 字节小端)，tag = (sequence << 8) | value_type。
// 排序（InternalKeyComparator）：user_key 升序；同 user_key 时按 **sequence 降序**（tag
// 越大越靠前）， 于是同一 user_key 的最新版本排在最前——Get
// 顺序扫到的第一条即最新，无需比较所有版本。
namespace mini_lsm {

// 值类型。删除写入一条 kDeletion 墓碑而非真正抹掉旧值：墓碑在读时遮蔽更旧的值，在 compaction 到最底
// 层时才被真正回收。0/1 的取值让 tag 里 type 位不影响"seq 降序"的主序。
enum class ValueType : std::uint8_t {
    kDeletion = 0,
    kValue = 1,
};

inline constexpr std::uint64_t kMaxSequence = (1ull << 56) - 1;

// 跨层查找的三态结果：命中值 / 命中墓碑（已删除）/ 本层无此 key。三态是必需的——墓碑必须能中止向
// 更旧层继续查找，否则删除会被底层旧值"复活"。MemTable 与 SSTable 共用此枚举。
enum class LookupStatus {
    kNotFound,
    kFound,
    kDeleted,
};

inline std::uint64_t PackTag(std::uint64_t sequence, ValueType type) {
    return (sequence << 8) | static_cast<std::uint64_t>(type);
}

inline std::string MakeInternalKey(std::uint64_t sequence, ValueType type,
                                   const std::string& user_key) {
    std::string out = user_key;
    PutFixed64(&out, PackTag(sequence, type));
    return out;
}

inline std::string ExtractUserKey(const std::string& internal_key) {
    // 末 8 字节是 tag，其余为 user_key。
    return internal_key.substr(0, internal_key.size() - 8);
}

inline std::uint64_t ExtractTag(const std::string& internal_key) {
    return DecodeFixed64(internal_key.data() + internal_key.size() - 8);
}

// internal key 比较器：user_key 升序，tag 降序（→ sequence 降序，最新版本在前）。
struct InternalKeyComparator {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
        const std::string ua = ExtractUserKey(lhs);
        const std::string ub = ExtractUserKey(rhs);
        if (ua != ub) {
            return ua < ub;
        }
        // 同 user_key：tag 大的（新）排前 → lhs < rhs 当 lhs.tag > rhs.tag。
        return ExtractTag(lhs) > ExtractTag(rhs);
    }
};

// 落盘/合并用的一条记录：user_key + 类型 + 值（墓碑时 value 为空）。
struct MemEntry {
    std::string user_key;
    ValueType type = ValueType::kValue;
    std::string value;
};

class MemTable {
 public:
    // 写入一个值版本。sequence 单调递增，决定新旧。
    void Put(std::uint64_t sequence, const std::string& key, const std::string& value) {
        table_.Insert(MakeInternalKey(sequence, ValueType::kValue, key), value);
    }

    // 写入一条删除墓碑。读到它即认为该 key 已删除（遮蔽更旧的值）。
    void Delete(std::uint64_t sequence, const std::string& key) {
        table_.Insert(MakeInternalKey(sequence, ValueType::kDeletion, key), std::string());
    }

    // 三态查询最新版本。同 user_key 按 seq 降序排列，扫描中遇到的第一条匹配即最新版本。
    // 供 MiniDB 跨层查找：kDeleted 必须中止向 SSTable 继续查，否则删除会被旧值复活。
    LookupStatus Lookup(const std::string& key, std::string* value) const {
        for (auto it = table_.begin(); it != table_.end(); ++it) {
            const std::string user_key = ExtractUserKey(it.key());
            if (user_key < key) continue;
            if (user_key > key) break;  // 已越过目标 user_key（升序），不存在
            const auto type = static_cast<ValueType>(ExtractTag(it.key()) & 0xFFu);
            if (type == ValueType::kDeletion) return LookupStatus::kDeleted;
            if (value != nullptr) *value = it.value();
            return LookupStatus::kFound;
        }
        return LookupStatus::kNotFound;
    }

    // 兼容旧接口：命中值返回之，墓碑/缺失均返回 nullopt。
    std::optional<std::string> Get(const std::string& key) const {
        std::string value;
        if (Lookup(key, &value) == LookupStatus::kFound) {
            return value;
        }
        return std::nullopt;
    }

    // 兼容旧接口：仅返回未删除的 (user_key, value)，按 user_key 有序去重（每 key 取最新）。
    std::vector<std::pair<std::string, std::string>> EntriesByUserKey() const {
        std::vector<std::pair<std::string, std::string>> entries;
        for (const auto& e : ExportSorted()) {
            if (e.type == ValueType::kValue) {
                entries.emplace_back(e.user_key, e.value);
            }
        }
        return entries;
    }

    // 落盘用的完整快照：按 user_key 升序，每个 user_key 只保留最新版本（含墓碑）。
    // flush 会把墓碑一并写进 SSTable，使删除能在更高层遮蔽旧值，直到 compaction 到最底层回收。
    std::vector<MemEntry> ExportSorted() const {
        std::vector<MemEntry> out;
        std::string last_user_key;
        bool has_last = false;
        for (auto it = table_.begin(); it != table_.end(); ++it) {
            const std::string user_key = ExtractUserKey(it.key());
            if (has_last && user_key == last_user_key) {
                continue;  // 同 user_key 的更旧版本，跳过（第一条已是最新）
            }
            has_last = true;
            last_user_key = user_key;
            const auto type = static_cast<ValueType>(ExtractTag(it.key()) & 0xFFu);
            out.push_back(MemEntry{user_key, type, it.value()});
        }
        return out;
    }

    std::size_t size() const { return table_.size(); }
    bool empty() const { return table_.size() == 0; }

    // flush 后清空：SkipList 无 erase，直接重建一个空表（旧节点随对象析构释放）。
    void Clear() { table_ = SkipList<std::string, std::string, InternalKeyComparator>{}; }

 private:
    SkipList<std::string, std::string, InternalKeyComparator> table_;
};

}  // namespace mini_lsm
