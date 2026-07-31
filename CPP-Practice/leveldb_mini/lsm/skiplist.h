#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// LSM 的内存侧基础原语：Slice（非拥有视图）、Status（结果码）、Arena（区域分配器）、
// SkipList（有序索引）、BloomFilter（存在性过滤）。原先都挤在 mini_leveldb.h 里；F3 把它们抽到这个
// 独立头，让 memtable.h/table.h 能直接依赖，而 mini_leveldb.h 降为 umbrella 再包含本文件对外暴露。
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

// 布隆过滤器。除内存查询外支持序列化（Encode/Decode），以便作为 SSTable 的持久 meta block 落盘、
// 读路径重建后先做"确定不存在"的快速否定。位数在构造时确定，Encode 把位打包进字节流并前缀记录位数。
class BloomFilter {
 public:
    explicit BloomFilter(std::size_t bits = 2048) : bits_(bits, false) {}
    void Add(const std::string& key) {
        for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u})
            bits_[Hash(key, seed) % bits_.size()] = true;  // 三个独立哈希降低误判率。
    }
    // 只用于快速否定：全命中说明“可能存在”（允许假阳性），任一位缺失即“确定不存在”。
    bool MayContain(const std::string& key) const {
        if (bits_.empty()) return true;  // 空过滤器不否定任何 key（保守但安全）
        for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u}) {
            if (!bits_[Hash(key, seed) % bits_.size()]) return false;
        }
        return true;
    }

    // 序列化：bit_count(4 LE) + 打包位（每字节 8 位，LSB 优先）。
    std::string Encode() const {
        std::string out;
        const std::uint32_t n = static_cast<std::uint32_t>(bits_.size());
        out.push_back(static_cast<char>(n & 0xFFu));
        out.push_back(static_cast<char>((n >> 8) & 0xFFu));
        out.push_back(static_cast<char>((n >> 16) & 0xFFu));
        out.push_back(static_cast<char>((n >> 24) & 0xFFu));
        const std::size_t bytes = (bits_.size() + 7) / 8;
        std::string packed(bytes, '\0');
        for (std::size_t i = 0; i < bits_.size(); ++i) {
            if (bits_[i]) packed[i / 8] = static_cast<char>(packed[i / 8] | (1u << (i % 8)));
        }
        out.append(packed);
        return out;
    }

    static BloomFilter Decode(const std::string& data) {
        BloomFilter f(0);
        if (data.size() < 4) return f;
        const std::uint32_t n =
            static_cast<std::uint32_t>(static_cast<std::uint8_t>(data[0])) |
            (static_cast<std::uint32_t>(static_cast<std::uint8_t>(data[1])) << 8) |
            (static_cast<std::uint32_t>(static_cast<std::uint8_t>(data[2])) << 16) |
            (static_cast<std::uint32_t>(static_cast<std::uint8_t>(data[3])) << 24);
        f.bits_.assign(n, false);
        for (std::size_t i = 0; i < n; ++i) {
            const std::size_t byte_idx = 4 + i / 8;
            if (byte_idx < data.size() &&
                (static_cast<std::uint8_t>(data[byte_idx]) & (1u << (i % 8)))) {
                f.bits_[i] = true;
            }
        }
        return f;
    }

 private:
    static std::uint32_t Hash(const std::string& key, std::uint32_t seed) {
        std::uint32_t hash = seed;
        for (unsigned char ch : key) hash = (hash ^ ch) * 16777619u;  // FNV-1a 风格混合。
        return hash;
    }
    std::vector<bool> bits_;
};

}  // namespace mini_lsm
