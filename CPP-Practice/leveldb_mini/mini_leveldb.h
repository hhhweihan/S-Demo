#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <optional>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace mini_lsm {

class Slice {
 public:
    Slice() = default;
    Slice(std::string_view value) : value_(value) {}
    const char* data() const { return value_.data(); }
    std::size_t size() const { return value_.size(); }
    bool empty() const { return value_.empty(); }
    std::string ToString() const { return std::string(value_); }

 private:
    // 非拥有视图：调用方须保证被引用的字节在 Slice 存活期间不失效。
    std::string_view value_;
};

class Status {
 public:
    static Status OK() { return Status(); }
    static Status NotFound(std::string message) { return Status(false, std::move(message)); }
    bool ok() const { return ok_; }
    std::string ToString() const { return ok_ ? "OK" : message_; }

 private:
    Status() = default;
    Status(bool ok, std::string message) : ok_(ok), message_(std::move(message)) {}
    bool ok_ = true;
    std::string message_;
};

class Arena {
 public:
    // 只分配不单独释放：所有块随 Arena 一起销毁，换取分配路径无碎片、无 per-node free。
    void* Allocate(std::size_t bytes) {
        blocks_.push_back(std::make_unique<char[]>(bytes));
        memory_usage_ += bytes;
        return blocks_.back().get();
    }
    std::size_t memory_usage() const { return memory_usage_; }

 private:
    std::vector<std::unique_ptr<char[]>> blocks_;
    std::size_t memory_usage_ = 0;
};

template <typename Key, typename Value, typename Compare = std::less<Key>>
class SkipList {
    static constexpr int kMaxHeight = 12;
    static constexpr int kBranching = 4;  // 每升一层概率 1/4，期望节点高度 O(1)，查找 O(log n)。
    struct Node {
        Key key;
        Value value;
        std::vector<Node*> next;  // next[i] 为第 i 层后继；层数越高跨度越大，实现对数级跳跃。
        Node(Key key, Value value, int height)
            : key(std::move(key)), value(std::move(value)), next(height, nullptr) {}
    };

 public:
    class iterator {
     public:
        explicit iterator(Node* node = nullptr) : node_(node) {}
        iterator& operator++() {
            node_ = node_->next[0];
            return *this;
        }  // 第 0 层串起全部节点，故遍历即有序序列。
        bool operator==(const iterator& other) const { return node_ == other.node_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        const Key& key() const { return node_->key; }
        const Value& value() const { return node_->value; }

     private:
        Node* node_ = nullptr;
    };

    SkipList()
        : head_(new Node(Key{}, Value{}, kMaxHeight)),
          rng_(7) {}  // rng 固定种子 → 结构可复现，便于测试。

    void Insert(const Key& key, const Value& value) {
        std::array<Node*, kMaxHeight> prev{};
        Node* existing =
            FindGreaterOrEqual(key, prev.data());  // 顺带记录各层前驱，供后续接线使用。
        if (existing && equal(existing->key, key)) {
            existing->value = value;  // upsert：同 key 覆盖旧值，不新增节点。
            return;
        }
        const int height = RandomHeight();
        if (height > max_height_) {
            // 新节点比现有塔更高，超出部分的前驱只能是 head。
            for (int level = max_height_; level < height; ++level) prev[level] = head_.get();
            max_height_ = height;
        }
        auto node = std::make_unique<Node>(key, value, height);
        Node* raw = node.get();
        for (int level = 0; level < height; ++level) {
            raw->next[level] = prev[level]->next[level];
            prev[level]->next[level] = raw;
        }
        nodes_.push_back(
            std::move(node));  // 所有权集中在 nodes_，next 指针仅作导航，避免递归析构。
        ++size_;
    }

    std::optional<Value> Find(const Key& key) const {
        Node* node = FindGreaterOrEqual(key, nullptr);
        if (node && equal(node->key, key)) return node->value;
        return std::nullopt;
    }

    iterator begin() const {
        return iterator(head_->next[0]);
    }  // 跳过哨兵 head，从首个真实节点开始。
    iterator end() const { return iterator(); }
    std::size_t size() const { return size_; }
    int max_height() const { return max_height_; }

 private:
    int RandomHeight() {
        int height = 1;
        while (height < kMaxHeight && (rng_() % kBranching == 0))
            ++height;  // 每层 1/kBranching 概率继续升高。
        return height;
    }

    // 从最高层向右、够不着再下降的经典跳表搜索：每层跨过一段小于 key 的节点，整体 O(log n)。
    Node* FindGreaterOrEqual(const Key& key, Node** prev) const {
        Node* current = head_.get();
        int level = max_height_ - 1;
        while (true) {
            Node* next = current->next[level];
            if (next && compare_(next->key, key)) {
                current = next;  // 后继仍小于 key，同层继续右移。
            } else {
                if (prev) prev[level] = current;  // 记录本层前驱，插入时据此接线。
                if (level == 0) return next;      // 落到底层，next 即第一个 >= key 的节点。
                --level;
            }
        }
    }

    bool equal(const Key& lhs, const Key& rhs) const {
        return !compare_(lhs, rhs) && !compare_(rhs, lhs);
    }  // 仅凭 Compare 判等，不额外要求 operator==。

    std::unique_ptr<Node> head_;  // 哨兵头节点，高度恒为 kMaxHeight，简化插入边界。
    std::vector<std::unique_ptr<Node>> nodes_;
    std::size_t size_ = 0;
    int max_height_ = 1;
    Compare compare_{};
    std::mt19937 rng_;
};

class BloomFilter {
 public:
    explicit BloomFilter(std::size_t bits = 2048) : bits_(bits, false) {}
    void Add(const std::string& key) {
        for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u})
            bits_[Hash(key, seed) % bits_.size()] = true;  // 三个独立哈希降低误判率。
    }
    // 只用于快速否定：全命中说明“可能存在”（允许假阳性），任一位缺失即“确定不存在”。
    bool MayContain(const std::string& key) const {
        for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u}) {
            if (!bits_[Hash(key, seed) % bits_.size()]) return false;
        }
        return true;
    }

 private:
    static std::uint32_t Hash(const std::string& key, std::uint32_t seed) {
        std::uint32_t hash = seed;
        for (unsigned char ch : key) hash = (hash ^ ch) * 16777619u;  // FNV-1a 风格混合。
        return hash;
    }
    std::vector<bool> bits_;
};

class MemTable {
 public:
    void Put(std::uint64_t sequence, const std::string& key, const std::string& value) {
        table_.Insert(EncodeInternalKey(sequence, key),
                      value);  // 以 internal key 存储，让同一 user key 的多版本共存。
    }
    // 同一 user key 可能有多条不同序列号的记录，取序列号最大者即最新写入。
    std::optional<std::string> Get(const std::string& key) const {
        std::optional<std::string> latest;
        std::uint64_t latest_sequence = 0;
        for (auto it = table_.begin(); it != table_.end(); ++it) {
            auto decoded = DecodeInternalKey(it.key());
            if (decoded.user_key == key && decoded.sequence >= latest_sequence) {
                latest_sequence = decoded.sequence;
                latest = it.value();
            }
        }
        return latest;
    }
    std::vector<std::pair<std::string, std::string>> EntriesByUserKey() const {
        std::vector<std::pair<std::string, std::string>> entries;
        for (auto it = table_.begin(); it != table_.end(); ++it) {
            auto decoded = DecodeInternalKey(it.key());
            entries.push_back({decoded.user_key, it.value()});
        }
        std::sort(entries.begin(), entries.end());
        // 落盘前按 user key 去重：SSTable 每个 key 只保留一条，供二分查找。
        entries.erase(
            std::unique(entries.begin(), entries.end(),
                        [](const auto& lhs, const auto& rhs) { return lhs.first == rhs.first; }),
            entries.end());
        return entries;
    }
    std::size_t size() const { return table_.size(); }

 private:
    struct DecodedKey {
        std::uint64_t sequence;
        std::string user_key;
    };
    // internal key = user_key#sequence；教学用文本编码，真 LevelDB 用定长小端整数。
    static std::string EncodeInternalKey(std::uint64_t sequence, const std::string& key) {
        return key + "#" + std::to_string(sequence);
    }
    static DecodedKey DecodeInternalKey(const std::string& internal_key) {
        const auto pos = internal_key.rfind('#');  // 用最后一个 '#' 分隔，容忍 user key 内含 '#'。
        return {std::stoull(internal_key.substr(pos + 1)), internal_key.substr(0, pos)};
    }
    SkipList<std::string, std::string> table_;
};

class SSTable {
 public:
    void Build(const std::filesystem::path& path,
               const std::vector<std::pair<std::string, std::string>>& entries) {
        path_ = path;
        std::ofstream out(path_, std::ios::trunc);
        data_ = entries;
        std::sort(data_.begin(),
                  data_.end());  // SSTable 的核心契约：条目按 key 有序，才能二分查找。
        for (const auto& [key, value] : data_) {
            bloom_.Add(key);  // 同步构建 bloom，读路径可先否定缺失 key。
            out << key << '\t' << value << '\n';
        }
    }
    void Load(const std::filesystem::path& path) {
        path_ = path;
        data_.clear();
        std::ifstream in(path_);
        std::string line;
        while (std::getline(in, line)) {
            const auto pos = line.find('\t');
            if (pos == std::string::npos) continue;  // 跳过残缺行，容忍写入中途崩溃产生的截断。
            data_.push_back({line.substr(0, pos), line.substr(pos + 1)});
            bloom_.Add(data_.back().first);
        }
        std::sort(data_.begin(), data_.end());  // 文件本已有序，重排是防御性保险。
    }
    std::optional<std::string> Get(const std::string& key) const {
        if (!bloom_.MayContain(key))
            return std::nullopt;  // bloom 先挡掉绝大多数缺失 key，省去二分。
        auto it = std::lower_bound(data_.begin(), data_.end(),
                                   std::pair<std::string, std::string>{key, ""});
        if (it != data_.end() && it->first == key) return it->second;
        return std::nullopt;
    }
    std::size_t size() const { return data_.size(); }

 private:
    std::filesystem::path path_;
    std::vector<std::pair<std::string, std::string>> data_;
    BloomFilter bloom_;
};

class WAL {
 public:
    explicit WAL(std::filesystem::path path) : path_(std::move(path)) {
        std::filesystem::create_directories(path_.parent_path());
    }
    void Append(std::uint64_t sequence, const std::string& key, const std::string& value) {
        std::ofstream out(path_, std::ios::app);  // 追加写：崩溃前已落盘的记录不会被覆盖。
        out << sequence << '\t' << key << '\t' << value << '\n';
    }
    std::vector<std::tuple<std::uint64_t, std::string, std::string>> Replay() const {
        std::vector<std::tuple<std::uint64_t, std::string, std::string>> records;
        std::ifstream in(path_);
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream input(line);
            std::string sequence_text;
            std::string key;
            std::string value;
            // 只有三段齐全才算完整记录；末尾半行（崩溃残留）被自然丢弃。
            if (std::getline(input, sequence_text, '\t') && std::getline(input, key, '\t') &&
                std::getline(input, value)) {
                records.push_back({std::stoull(sequence_text), key, value});
            }
        }
        return records;
    }

 private:
    std::filesystem::path path_;
};

class MiniDB {
 public:
    explicit MiniDB(std::filesystem::path directory)
        : directory_(std::move(directory)), wal_(directory_ / "wal.log") {
        std::filesystem::create_directories(directory_);
    }
    void Recover() {
        for (const auto& [sequence, key, value] : wal_.Replay()) {
            mem_.Put(sequence, key, value);
            sequence_ =
                std::max(sequence_, sequence);  // 续用历史最大序列号，避免恢复后新写入序列号回退。
        }
    }
    void Put(const std::string& key, const std::string& value) {
        const auto sequence = ++sequence_;
        // 先写 WAL 再改内存表：一旦 memtable 未落盘就崩溃，仍可由 WAL 回放恢复（write-ahead）。
        wal_.Append(sequence, key, value);
        mem_.Put(sequence, key, value);
    }
    std::optional<std::string> Get(const std::string& key) const {
        if (auto value = mem_.Get(key))
            return value;  // memtable 保存最新写入，优先于已落盘的旧版本。
        return sstable_.Get(key);
    }
    void Flush() {
        sstable_.Build(
            directory_ / "000001.sst",
            mem_.EntriesByUserKey());  // 教学版固定单文件名；真 LSM 会生成递增编号并分层。
    }
    std::size_t MemSize() const { return mem_.size(); }
    std::size_t SSTableSize() const { return sstable_.size(); }

 private:
    std::filesystem::path directory_;
    WAL wal_;
    MemTable mem_;
    SSTable sstable_;
    std::uint64_t sequence_ = 0;  // 单调递增，为每次写入定序，决定同 key 的新旧。
};

}  // namespace mini_lsm