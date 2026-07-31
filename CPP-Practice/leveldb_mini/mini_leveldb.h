#pragma once

// mini LevelDB —— umbrella 头。F3 崩溃安全化改造后，各部件下沉到 lsm/ 子目录，本文件只做两件事：
//   1) 汇总包含各部件（Slice/Status/Arena/SkipList/BloomFilter/CRC32C/WAL/Block/SSTable/MemTable/
//      Version），让既有 `#include "mini_leveldb.h"` 与 `mini_lsm::` 命名保持不变；
//   2) 承载崩溃安全的 WAL 封装与 MiniDB（写路径、immutable flush→L0、L0→L1 compaction、Recover）。
//
// 崩溃安全模型（write-ahead + durable SSTable）：
//   - 每次 Put/Delete：先把记录追加进 WAL 并 fsync 落盘，**之后**才改内存 MemTable。返回成功 ⇒ 已
//     durable。进程/机器崩溃后 Recover() 回放 WAL 完整记录，已确认写不丢；崩溃瞬间没写完的尾部
//     record 因 CRC/长度校验失败被干净丢弃（见 lsm/record_log.h）。
//   - Flush()：MemTable → 新 L0 SSTable（写完 fsync），落盘成功后**轮转 WAL**（清空）——此时数据已在
//     SSTable，WAL 使命完成。若在"SSTable 落盘"与"WAL 清空"之间崩溃，恢复会同时载入 SSTable 并重放
//     WAL（内容重复但幂等，Get 以 MemTable 优先，值一致），不丢不错。
//   - SSTable 写入用 O_TRUNC+fsync；半写文件因 footer magic 缺失被 Load 判为 invalid
//   并在恢复时跳过，
//     其数据仍在未轮转的 WAL 中，可被重放恢复。

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "lsm/block.h"
#include "lsm/coding.h"
#include "lsm/crc32c.h"
#include "lsm/memtable.h"
#include "lsm/record_log.h"
#include "lsm/skiplist.h"
#include "lsm/table.h"
#include "lsm/version.h"

namespace mini_lsm {

// WAL 封装：在 lsm/record_log.h 的 CRC record 帧之上，定义 LSM 写记录的 payload 编码并提供
// 打开/追加/回放/轮转。payload = seq(8) + type(1) + keylen(4) + key + vallen(4) + value。
class WAL {
 public:
    struct Record {
        std::uint64_t sequence = 0;
        ValueType type = ValueType::kValue;
        std::string key;
        std::string value;
    };

    explicit WAL(std::filesystem::path path) : path_(std::move(path)) {
        std::filesystem::create_directories(path_.parent_path());
        writer_.Open(path_.string());
    }

    // 追加一条写记录并落盘。返回是否成功（写出 + fsync）。
    bool Append(std::uint64_t sequence, ValueType type, const std::string& key,
                const std::string& value) {
        std::string payload;
        PutFixed64(&payload, sequence);
        payload.push_back(static_cast<char>(type));
        PutFixed32(&payload, static_cast<std::uint32_t>(key.size()));
        payload.append(key);
        PutFixed32(&payload, static_cast<std::uint32_t>(value.size()));
        payload.append(value);
        if (!writer_.AddRecord(payload)) {
            return false;
        }
        return writer_.Sync();  // 返回前落盘 —— 崩溃安全的关键
    }

    // 回放所有完整记录（遇首个损坏/截断 record 即停，见 RecordReader）。解析失败的 payload 被跳过。
    std::vector<Record> Replay() const {
        std::vector<Record> out;
        for (const std::string& payload : RecordReader::Replay(path_.string())) {
            Record r;
            if (DecodePayload(payload, &r)) {
                out.push_back(std::move(r));
            }
        }
        return out;
    }

    // 轮转：flush 后 MemTable 数据已进 SSTable，清空 WAL 从头再记。
    void Reset() {
        writer_.Close();
        // O_TRUNC 清空文件，再以 append 重新打开。
        const int fd = ::open(path_.string().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            ::close(fd);
        }
        writer_.Open(path_.string());
    }

 private:
    static bool DecodePayload(const std::string& payload, Record* r) {
        std::size_t pos = 0;
        if (payload.size() < 8 + 1 + 4) return false;
        r->sequence = DecodeFixed64(payload.data() + pos);
        pos += 8;
        r->type = static_cast<ValueType>(static_cast<std::uint8_t>(payload[pos]));
        pos += 1;
        const std::uint32_t klen = DecodeFixed32(payload.data() + pos);
        pos += 4;
        if (pos + klen + 4 > payload.size()) return false;
        r->key = payload.substr(pos, klen);
        pos += klen;
        const std::uint32_t vlen = DecodeFixed32(payload.data() + pos);
        pos += 4;
        if (pos + vlen > payload.size()) return false;
        r->value = payload.substr(pos, vlen);
        return true;
    }

    std::filesystem::path path_;
    RecordWriter writer_;
};

class MiniDB {
 public:
    // L0 文件数达到此阈值即触发 L0→L1 compaction。
    static constexpr std::size_t kL0CompactionTrigger = 4;

    explicit MiniDB(std::filesystem::path directory)
        : directory_(std::move(directory)), wal_(directory_ / "wal.log") {
        std::filesystem::create_directories(directory_);
    }

    // 重开库：扫描目录载入已有 SSTable（按文件名解析 level/number），再回放 WAL 重建 MemTable。
    void Recover() {
        LoadExistingTables();
        for (const auto& rec : wal_.Replay()) {
            if (rec.type == ValueType::kValue) {
                mem_.Put(rec.sequence, rec.key, rec.value);
            } else {
                mem_.Delete(rec.sequence, rec.key);
            }
            sequence_ = std::max(sequence_, rec.sequence);
        }
    }

    void Put(const std::string& key, const std::string& value) {
        const auto sequence = ++sequence_;
        wal_.Append(sequence, ValueType::kValue, key, value);  // 先 WAL 落盘
        mem_.Put(sequence, key, value);                        // 再改内存
    }

    // 删除：写一条墓碑（tombstone），遮蔽 SSTable 里的旧值，直到 compaction 到最底层回收。
    void Delete(const std::string& key) {
        const auto sequence = ++sequence_;
        wal_.Append(sequence, ValueType::kDeletion, key, std::string());
        mem_.Delete(sequence, key);
    }

    // 跨层查找：MemTable → L0（新→旧）→ L1。任一层命中值即返回；命中墓碑即"不存在"。
    std::optional<std::string> Get(const std::string& key) const {
        std::string value;
        switch (mem_.Lookup(key, &value)) {
            case LookupStatus::kFound:
                return value;
            case LookupStatus::kDeleted:
                return std::nullopt;
            case LookupStatus::kNotFound:
                break;
        }
        // L0：文件可能范围重叠，必须从最新（末尾）向最旧查。
        for (auto it = level0_.rbegin(); it != level0_.rend(); ++it) {
            switch ((*it)->Lookup(key, &value)) {
                case LookupStatus::kFound:
                    return value;
                case LookupStatus::kDeleted:
                    return std::nullopt;
                case LookupStatus::kNotFound:
                    break;
            }
        }
        // L1：compaction 后互不重叠且有序，逐个查（mini 版不做范围二分）。
        for (const auto& table : level1_) {
            switch (table->Lookup(key, &value)) {
                case LookupStatus::kFound:
                    return value;
                case LookupStatus::kDeleted:
                    return std::nullopt;
                case LookupStatus::kNotFound:
                    break;
            }
        }
        return std::nullopt;
    }

    // 把当前 MemTable 落成一个新的 L0 SSTable，然后轮转 WAL；必要时触发 compaction。
    void Flush() {
        if (mem_.empty()) {
            return;
        }
        const std::uint64_t number = ++next_file_number_;
        auto table = std::make_unique<SSTable>();
        table->BuildFromEntries(SSTablePath(directory_, 0, number), mem_.ExportSorted());
        level0_.push_back(std::move(table));
        mem_.Clear();
        wal_.Reset();  // 数据已 durable 进 SSTable，WAL 可清空

        if (level0_.size() >= kL0CompactionTrigger) {
            CompactToBottom();
        }
    }

    // L0（+现有 L1）→ 新 L1 的 leveled
    // compaction。最新写覆盖旧写；到达最底层，墓碑被真正回收（丢弃）。
    void CompactToBottom() {
        if (level0_.empty()) {
            return;
        }
        // 从旧到新归并：先 L1（最旧），再 L0 front→back（旧→新）。后写覆盖先写。
        std::map<std::string, MemEntry> merged;
        for (const auto& table : level1_) {
            for (auto& e : table->Entries()) {
                merged[e.user_key] = e;
            }
        }
        for (const auto& table : level0_) {
            for (auto& e : table->Entries()) {
                merged[e.user_key] = e;
            }
        }

        // 最底层输出：丢弃墓碑（kDeletion），只保留活跃值。
        std::vector<MemEntry> output;
        output.reserve(merged.size());
        for (auto& [user_key, entry] : merged) {
            if (entry.type == ValueType::kValue) {
                output.push_back(std::move(entry));
            }
        }

        // 收集待删旧文件路径，写出新 L1 文件后统一删盘。
        std::vector<std::filesystem::path> obsolete;
        for (const auto& table : level0_) obsolete.push_back(table->path());
        for (const auto& table : level1_) obsolete.push_back(table->path());

        std::vector<std::unique_ptr<SSTable>> new_level1;
        if (!output.empty()) {
            const std::uint64_t number = ++next_file_number_;
            auto table = std::make_unique<SSTable>();
            table->BuildFromEntries(SSTablePath(directory_, 1, number), std::move(output));
            new_level1.push_back(std::move(table));
        }

        level0_.clear();
        level1_ = std::move(new_level1);
        for (const auto& path : obsolete) {
            std::error_code ec;
            std::filesystem::remove(path, ec);
        }
    }

    std::size_t MemSize() const { return mem_.size(); }
    // 已落盘条目总数（所有 SSTable 之和）。
    std::size_t SSTableSize() const {
        std::size_t total = 0;
        for (const auto& t : level0_) total += t->size();
        for (const auto& t : level1_) total += t->size();
        return total;
    }
    std::size_t Level0FileCount() const { return level0_.size(); }
    std::size_t Level1FileCount() const { return level1_.size(); }

 private:
    // 扫描目录，按文件名解析 level/number，载入合法 SSTable，重建分层文件集与文件号计数。
    void LoadExistingTables() {
        struct Loaded {
            std::uint64_t number;
            int level;
            std::unique_ptr<SSTable> table;
        };
        std::vector<Loaded> loaded;
        std::error_code ec;
        for (const auto& entry : std::filesystem::directory_iterator(directory_, ec)) {
            if (ec) break;
            if (!entry.is_regular_file()) continue;
            const std::string name = entry.path().filename().string();
            int level = 0;
            std::uint64_t number = 0;
            if (!ParseSSTableFileName(name, &level, &number)) continue;
            auto table = std::make_unique<SSTable>();
            table->Load(entry.path());
            if (!table->valid()) continue;  // 半写/损坏文件跳过（数据仍在 WAL）
            next_file_number_ = std::max(next_file_number_, number);
            loaded.push_back(Loaded{number, level, std::move(table)});
        }
        // L0 按 number 升序（旧→新，末尾最新）；L1 按 number 升序。
        std::sort(loaded.begin(), loaded.end(),
                  [](const Loaded& a, const Loaded& b) { return a.number < b.number; });
        for (auto& l : loaded) {
            if (l.level == 0) {
                level0_.push_back(std::move(l.table));
            } else {
                level1_.push_back(std::move(l.table));
            }
        }
    }

    std::filesystem::path directory_;
    WAL wal_;
    MemTable mem_;
    std::vector<std::unique_ptr<SSTable>> level0_;  // 末尾最新
    std::vector<std::unique_ptr<SSTable>> level1_;  // compaction 后互不重叠
    std::uint64_t sequence_ = 0;
    std::uint64_t next_file_number_ = 0;
};

}  // namespace mini_lsm
