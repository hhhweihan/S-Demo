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
  static constexpr int kBranching = 4;
  struct Node {
    Key key;
    Value value;
    std::vector<Node*> next;
    Node(Key key, Value value, int height) : key(std::move(key)), value(std::move(value)), next(height, nullptr) {}
  };

 public:
  class iterator {
   public:
    explicit iterator(Node* node = nullptr) : node_(node) {}
    iterator& operator++() { node_ = node_->next[0]; return *this; }
    bool operator==(const iterator& other) const { return node_ == other.node_; }
    bool operator!=(const iterator& other) const { return !(*this == other); }
    const Key& key() const { return node_->key; }
    const Value& value() const { return node_->value; }
   private:
    Node* node_ = nullptr;
  };

  SkipList() : head_(new Node(Key{}, Value{}, kMaxHeight)), rng_(7) {}

  void Insert(const Key& key, const Value& value) {
    std::array<Node*, kMaxHeight> prev{};
    Node* existing = FindGreaterOrEqual(key, prev.data());
    if (existing && equal(existing->key, key)) {
      existing->value = value;
      return;
    }
    const int height = RandomHeight();
    if (height > max_height_) {
      for (int level = max_height_; level < height; ++level) prev[level] = head_.get();
      max_height_ = height;
    }
    auto node = std::make_unique<Node>(key, value, height);
    Node* raw = node.get();
    for (int level = 0; level < height; ++level) {
      raw->next[level] = prev[level]->next[level];
      prev[level]->next[level] = raw;
    }
    nodes_.push_back(std::move(node));
    ++size_;
  }

  std::optional<Value> Find(const Key& key) const {
    Node* node = FindGreaterOrEqual(key, nullptr);
    if (node && equal(node->key, key)) return node->value;
    return std::nullopt;
  }

  iterator begin() const { return iterator(head_->next[0]); }
  iterator end() const { return iterator(); }
  std::size_t size() const { return size_; }
  int max_height() const { return max_height_; }

 private:
  int RandomHeight() {
    int height = 1;
    while (height < kMaxHeight && (rng_() % kBranching == 0)) ++height;
    return height;
  }

  Node* FindGreaterOrEqual(const Key& key, Node** prev) const {
    Node* current = head_.get();
    int level = max_height_ - 1;
    while (true) {
      Node* next = current->next[level];
      if (next && compare_(next->key, key)) {
        current = next;
      } else {
        if (prev) prev[level] = current;
        if (level == 0) return next;
        --level;
      }
    }
  }

  bool equal(const Key& lhs, const Key& rhs) const { return !compare_(lhs, rhs) && !compare_(rhs, lhs); }

  std::unique_ptr<Node> head_;
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
    for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u}) bits_[Hash(key, seed) % bits_.size()] = true;
  }
  bool MayContain(const std::string& key) const {
    for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u}) {
      if (!bits_[Hash(key, seed) % bits_.size()]) return false;
    }
    return true;
  }
 private:
  static std::uint32_t Hash(const std::string& key, std::uint32_t seed) {
    std::uint32_t hash = seed;
    for (unsigned char ch : key) hash = (hash ^ ch) * 16777619u;
    return hash;
  }
  std::vector<bool> bits_;
};

class MemTable {
 public:
  void Put(std::uint64_t sequence, const std::string& key, const std::string& value) {
    table_.Insert(EncodeInternalKey(sequence, key), value);
  }
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
    entries.erase(std::unique(entries.begin(), entries.end(), [](const auto& lhs, const auto& rhs) { return lhs.first == rhs.first; }), entries.end());
    return entries;
  }
  std::size_t size() const { return table_.size(); }

 private:
  struct DecodedKey { std::uint64_t sequence; std::string user_key; };
  static std::string EncodeInternalKey(std::uint64_t sequence, const std::string& key) {
    return key + "#" + std::to_string(sequence);
  }
  static DecodedKey DecodeInternalKey(const std::string& internal_key) {
    const auto pos = internal_key.rfind('#');
    return {std::stoull(internal_key.substr(pos + 1)), internal_key.substr(0, pos)};
  }
  SkipList<std::string, std::string> table_;
};

class SSTable {
 public:
  void Build(const std::filesystem::path& path, const std::vector<std::pair<std::string, std::string>>& entries) {
    path_ = path;
    std::ofstream out(path_, std::ios::trunc);
    data_ = entries;
    std::sort(data_.begin(), data_.end());
    for (const auto& [key, value] : data_) {
      bloom_.Add(key);
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
      if (pos == std::string::npos) continue;
      data_.push_back({line.substr(0, pos), line.substr(pos + 1)});
      bloom_.Add(data_.back().first);
    }
    std::sort(data_.begin(), data_.end());
  }
  std::optional<std::string> Get(const std::string& key) const {
    if (!bloom_.MayContain(key)) return std::nullopt;
    auto it = std::lower_bound(data_.begin(), data_.end(), std::pair<std::string, std::string>{key, ""});
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
  explicit WAL(std::filesystem::path path) : path_(std::move(path)) { std::filesystem::create_directories(path_.parent_path()); }
  void Append(std::uint64_t sequence, const std::string& key, const std::string& value) {
    std::ofstream out(path_, std::ios::app);
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
      if (std::getline(input, sequence_text, '\t') && std::getline(input, key, '\t') && std::getline(input, value)) {
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
  explicit MiniDB(std::filesystem::path directory) : directory_(std::move(directory)), wal_(directory_ / "wal.log") {
    std::filesystem::create_directories(directory_);
  }
  void Recover() {
    for (const auto& [sequence, key, value] : wal_.Replay()) {
      mem_.Put(sequence, key, value);
      sequence_ = std::max(sequence_, sequence);
    }
  }
  void Put(const std::string& key, const std::string& value) {
    const auto sequence = ++sequence_;
    wal_.Append(sequence, key, value);
    mem_.Put(sequence, key, value);
  }
  std::optional<std::string> Get(const std::string& key) const {
    if (auto value = mem_.Get(key)) return value;
    return sstable_.Get(key);
  }
  void Flush() {
    sstable_.Build(directory_ / "000001.sst", mem_.EntriesByUserKey());
  }
  std::size_t MemSize() const { return mem_.size(); }
  std::size_t SSTableSize() const { return sstable_.size(); }
 private:
  std::filesystem::path directory_;
  WAL wal_;
  MemTable mem_;
  SSTable sstable_;
  std::uint64_t sequence_ = 0;
};

}  // namespace mini_lsm