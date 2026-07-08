#pragma once  // 防止头文件重复包含

#include <algorithm>  // 使用排序和二分查找
#include <array>  // 使用固定大小数组
#include <cstdint>  // 使用固定宽度整数
#include <filesystem>  // 使用文件系统路径
#include <fstream>  // 使用文件读写流
#include <functional>  // 使用比较器
#include <memory>  // 使用智能指针
#include <optional>  // 使用可选返回值
#include <random>  // 使用随机数生成器
#include <sstream>  // 使用字符串流解析
#include <stdexcept>  // 保留异常类型支持
#include <string>  // 使用字符串
#include <string_view>  // 使用字符串视图
#include <utility>  // 使用移动语义和 pair
#include <vector>  // 使用动态数组

namespace mini_lsm {  // 迷你 LSM 数据库命名空间

class Slice {  // 轻量字符串视图封装
 public:  // 对外接口区域
  Slice() = default;  // 使用默认空视图构造
  Slice(std::string_view value) : value_(value) {}  // 保存外部字符串视图
  const char* data() const { return value_.data(); }  // 返回底层字符指针
  std::size_t size() const { return value_.size(); }  // 返回视图长度
  bool empty() const { return value_.empty(); }  // 判断视图是否为空
  std::string ToString() const { return std::string(value_); }  // 拷贝为字符串
 private:  // 内部数据区域
  std::string_view value_;  // 保存非拥有字符串视图
};  // 结束 Slice 定义

class Status {  // 简单状态返回类型
 public:  // 对外接口区域
  static Status OK() { return Status(); }  // 创建成功状态
  static Status NotFound(std::string message) { return Status(false, std::move(message)); }  // 创建未找到状态
  bool ok() const { return ok_; }  // 判断状态是否成功
  std::string ToString() const { return ok_ ? "OK" : message_; }  // 转换为可读文本
 private:  // 内部构造和数据区域
  Status() = default;  // 默认构造成功状态
  Status(bool ok, std::string message) : ok_(ok), message_(std::move(message)) {}  // 构造带消息状态
  bool ok_ = true;  // 保存成功标记
  std::string message_;  // 保存错误消息
};  // 结束 Status 定义

class Arena {  // 简单顺序内存分配器
 public:  // 对外接口区域
  void* Allocate(std::size_t bytes) {  // 分配指定字节数
    blocks_.push_back(std::make_unique<char[]>(bytes));  // 保存新分配块
    memory_usage_ += bytes;  // 累加内存用量
    return blocks_.back().get();  // 返回最新块指针
  }  // 结束 Allocate
  std::size_t memory_usage() const { return memory_usage_; }  // 返回累计内存用量
 private:  // 内部数据区域
  std::vector<std::unique_ptr<char[]>> blocks_;  // 持有所有分配块
  std::size_t memory_usage_ = 0;  // 记录累计分配字节
};  // 结束 Arena 定义

template <typename Key, typename Value, typename Compare = std::less<Key>>  // 定义可比较键值跳表模板
class SkipList {  // 有序跳表容器
  static constexpr int kMaxHeight = 12;  // 设置跳表最大高度
  static constexpr int kBranching = 4;  // 设置随机升层分支因子
  struct Node {  // 跳表节点
    Key key;  // 保存节点键
    Value value;  // 保存节点值
    std::vector<Node*> next;  // 保存各层后继指针
    Node(Key key, Value value, int height) : key(std::move(key)), value(std::move(value)), next(height, nullptr) {}  // 构造指定高度节点
  };  // 结束 Node 定义

 public:  // 对外接口区域
  class iterator {  // 底层链表迭代器
   public:  // 迭代器公开接口
    explicit iterator(Node* node = nullptr) : node_(node) {}  // 保存当前位置节点
    iterator& operator++() { node_ = node_->next[0]; return *this; }  // 前进到下一节点
    bool operator==(const iterator& other) const { return node_ == other.node_; }  // 比较迭代器相等
    bool operator!=(const iterator& other) const { return !(*this == other); }  // 比较迭代器不等
    const Key& key() const { return node_->key; }  // 返回当前键
    const Value& value() const { return node_->value; }  // 返回当前值
   private:  // 迭代器内部数据
    Node* node_ = nullptr;  // 保存当前节点指针
  };  // 结束 iterator 定义

  SkipList() : head_(new Node(Key{}, Value{}, kMaxHeight)), rng_(7) {}  // 构造带哨兵头的跳表

  void Insert(const Key& key, const Value& value) {  // 插入或更新键值
    std::array<Node*, kMaxHeight> prev{};  // 保存每层前驱节点
    Node* existing = FindGreaterOrEqual(key, prev.data());  // 查找插入位置或已有节点
    if (existing && equal(existing->key, key)) {  // 发现同键节点时更新
      existing->value = value;  // 覆盖旧值
      return;  // 更新完成后返回
    }  // 结束已有键分支
    const int height = RandomHeight();  // 随机生成新节点高度
    if (height > max_height_) {  // 新高度超过当前最大高度
      for (int level = max_height_; level < height; ++level) prev[level] = head_.get();  // 高层前驱设为头节点
      max_height_ = height;  // 更新跳表最大高度
    }  // 结束高度扩展分支
    auto node = std::make_unique<Node>(key, value, height);  // 创建新节点
    Node* raw = node.get();  // 保存裸指针用于链接
    for (int level = 0; level < height; ++level) {  // 逐层接入链表
      raw->next[level] = prev[level]->next[level];  // 新节点指向原后继
      prev[level]->next[level] = raw;  // 前驱改指向新节点
    }  // 结束逐层接入
    nodes_.push_back(std::move(node));  // 保存节点所有权
    ++size_;  // 更新节点数量
  }  // 结束 Insert

  std::optional<Value> Find(const Key& key) const {  // 查找指定键
    Node* node = FindGreaterOrEqual(key, nullptr);  // 定位第一个不小于目标键的节点
    if (node && equal(node->key, key)) return node->value;  // 命中时返回值
    return std::nullopt;  // 未命中时返回空
  }  // 结束 Find

  iterator begin() const { return iterator(head_->next[0]); }  // 返回首个真实节点迭代器
  iterator end() const { return iterator(); }  // 返回尾后迭代器
  std::size_t size() const { return size_; }  // 返回节点数量
  int max_height() const { return max_height_; }  // 返回当前最大高度

 private:  // 内部辅助和数据区域
  int RandomHeight() {  // 随机生成节点高度
    int height = 1;  // 最小高度为一层
    while (height < kMaxHeight && (rng_() % kBranching == 0)) ++height;  // 按概率提升高度
    return height;  // 返回最终高度
  }  // 结束 RandomHeight

  Node* FindGreaterOrEqual(const Key& key, Node** prev) const {  // 查找首个不小于目标键的节点
    Node* current = head_.get();  // 从头节点开始
    int level = max_height_ - 1;  // 从最高有效层开始
    while (true) {  // 持续向右或向下搜索
      Node* next = current->next[level];  // 读取当前层后继
      if (next && compare_(next->key, key)) {  // 后继键仍小于目标键
        current = next;  // 向右移动
      } else {  // 否则准备下降或返回
        if (prev) prev[level] = current;  // 记录当前层前驱
        if (level == 0) return next;  // 底层返回候选节点
        --level;  // 向下一层搜索
      }  // 结束下降分支
    }  // 结束搜索循环
  }  // 结束 FindGreaterOrEqual

  bool equal(const Key& lhs, const Key& rhs) const { return !compare_(lhs, rhs) && !compare_(rhs, lhs); }  // 用比较器判断等价

  std::unique_ptr<Node> head_;  // 持有跳表头节点
  std::vector<std::unique_ptr<Node>> nodes_;  // 持有所有真实节点
  std::size_t size_ = 0;  // 记录真实节点数
  int max_height_ = 1;  // 记录当前最大高度
  Compare compare_{};  // 保存键比较器
  std::mt19937 rng_;  // 保存随机数生成器
};  // 结束 SkipList 定义

class BloomFilter {  // 简单布隆过滤器
 public:  // 对外接口区域
  explicit BloomFilter(std::size_t bits = 2048) : bits_(bits, false) {}  // 初始化位图大小
  void Add(const std::string& key) {  // 添加键到过滤器
    for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u}) bits_[Hash(key, seed) % bits_.size()] = true;  // 设置多个哈希位
  }  // 结束 Add
  bool MayContain(const std::string& key) const {  // 判断键是否可能存在
    for (std::uint32_t seed : {0x9e3779b9u, 0x85ebca6bu, 0xc2b2ae35u}) {  // 检查多个哈希种子
      if (!bits_[Hash(key, seed) % bits_.size()]) return false;  // 任一位未设置则必不存在
    }  // 结束哈希位检查
    return true;  // 所有位命中则可能存在
  }  // 结束 MayContain
 private:  // 内部辅助和数据区域
  static std::uint32_t Hash(const std::string& key, std::uint32_t seed) {  // 计算带种子的哈希值
    std::uint32_t hash = seed;  // 使用种子初始化哈希
    for (unsigned char ch : key) hash = (hash ^ ch) * 16777619u;  // 混合每个字符
    return hash;  // 返回哈希结果
  }  // 结束 Hash
  std::vector<bool> bits_;  // 保存布隆过滤位图
};  // 结束 BloomFilter 定义

class MemTable {  // 内存表封装
 public:  // 对外接口区域
  void Put(std::uint64_t sequence, const std::string& key, const std::string& value) {  // 写入带序列号的键值
    table_.Insert(EncodeInternalKey(sequence, key), value);  // 使用内部键插入跳表
  }  // 结束 Put
  std::optional<std::string> Get(const std::string& key) const {  // 查询用户键最新值
    std::optional<std::string> latest;  // 保存当前最新值
    std::uint64_t latest_sequence = 0;  // 保存当前最新序列号
    for (auto it = table_.begin(); it != table_.end(); ++it) {  // 遍历内部键
      auto decoded = DecodeInternalKey(it.key());  // 解码用户键和序列号
      if (decoded.user_key == key && decoded.sequence >= latest_sequence) {  // 匹配用户键且更新
        latest_sequence = decoded.sequence;  // 更新最新序列号
        latest = it.value();  // 更新最新值
      }  // 结束匹配更新分支
    }  // 结束内存表遍历
    return latest;  // 返回查找结果
  }  // 结束 Get
  std::vector<std::pair<std::string, std::string>> EntriesByUserKey() const {  // 导出按用户键排序的条目
    std::vector<std::pair<std::string, std::string>> entries;  // 保存导出条目
    for (auto it = table_.begin(); it != table_.end(); ++it) {  // 遍历跳表条目
      auto decoded = DecodeInternalKey(it.key());  // 解码内部键
      entries.push_back({decoded.user_key, it.value()});  // 添加用户键和值
    }  // 结束条目收集
    std::sort(entries.begin(), entries.end());  // 按用户键和值排序
    entries.erase(std::unique(entries.begin(), entries.end(), [](const auto& lhs, const auto& rhs) { return lhs.first == rhs.first; }), entries.end());  // 移除重复用户键
    return entries;  // 返回去重条目
  }  // 结束 EntriesByUserKey
  std::size_t size() const { return table_.size(); }  // 返回内存表条目数

 private:  // 内部辅助和数据区域
  struct DecodedKey { std::uint64_t sequence; std::string user_key; };  // 保存解码后的内部键
  static std::string EncodeInternalKey(std::uint64_t sequence, const std::string& key) {  // 编码内部键
    return key + "#" + std::to_string(sequence);  // 拼接用户键和序列号
  }  // 结束 EncodeInternalKey
  static DecodedKey DecodeInternalKey(const std::string& internal_key) {  // 解码内部键
    const auto pos = internal_key.rfind('#');  // 定位序列号分隔符
    return {std::stoull(internal_key.substr(pos + 1)), internal_key.substr(0, pos)};  // 返回序列号和用户键
  }  // 结束 DecodeInternalKey
  SkipList<std::string, std::string> table_;  // 保存内部键到值的跳表
};  // 结束 MemTable 定义

class SSTable {  // 简化的磁盘有序表
 public:  // 对外接口区域
  void Build(const std::filesystem::path& path, const std::vector<std::pair<std::string, std::string>>& entries) {  // 构建 SSTable 文件
    path_ = path;  // 保存文件路径
    std::ofstream out(path_, std::ios::trunc);  // 以覆盖方式打开输出文件
    data_ = entries;  // 缓存输入条目
    std::sort(data_.begin(), data_.end());  // 保证条目有序
    for (const auto& [key, value] : data_) {  // 写出每个键值
      bloom_.Add(key);  // 将键加入布隆过滤器
      out << key << '\t' << value << '\n';  // 写入制表符分隔记录
    }  // 结束文件写出循环
  }  // 结束 Build
  void Load(const std::filesystem::path& path) {  // 从文件加载 SSTable
    path_ = path;  // 保存文件路径
    data_.clear();  // 清空旧数据
    std::ifstream in(path_);  // 打开输入文件
    std::string line;  // 保存当前读取行
    while (std::getline(in, line)) {  // 逐行读取记录
      const auto pos = line.find('\t');  // 查找键值分隔符
      if (pos == std::string::npos) continue;  // 跳过格式不完整的行
      data_.push_back({line.substr(0, pos), line.substr(pos + 1)});  // 解析并保存键值
      bloom_.Add(data_.back().first);  // 将键加入布隆过滤器
    }  // 结束文件读取循环
    std::sort(data_.begin(), data_.end());  // 保证内存数据有序
  }  // 结束 Load
  std::optional<std::string> Get(const std::string& key) const {  // 查询指定键
    if (!bloom_.MayContain(key)) return std::nullopt;  // 布隆过滤器排除缺失键
    auto it = std::lower_bound(data_.begin(), data_.end(), std::pair<std::string, std::string>{key, ""});  // 二分定位键
    if (it != data_.end() && it->first == key) return it->second;  // 命中时返回值
    return std::nullopt;  // 未命中时返回空
  }  // 结束 Get
  std::size_t size() const { return data_.size(); }  // 返回条目数量
 private:  // 内部数据区域
  std::filesystem::path path_;  // 保存 SSTable 路径
  std::vector<std::pair<std::string, std::string>> data_;  // 保存有序键值数据
  BloomFilter bloom_;  // 保存布隆过滤器
};  // 结束 SSTable 定义

class WAL {  // 预写日志封装
 public:  // 对外接口区域
  explicit WAL(std::filesystem::path path) : path_(std::move(path)) { std::filesystem::create_directories(path_.parent_path()); }  // 保存日志路径并确保目录存在
  void Append(std::uint64_t sequence, const std::string& key, const std::string& value) {  // 追加一条日志记录
    std::ofstream out(path_, std::ios::app);  // 以追加方式打开日志文件
    out << sequence << '\t' << key << '\t' << value << '\n';  // 写入制表符分隔记录
  }  // 结束 Append
  std::vector<std::tuple<std::uint64_t, std::string, std::string>> Replay() const {  // 回放日志文件
    std::vector<std::tuple<std::uint64_t, std::string, std::string>> records;  // 保存解析出的记录
    std::ifstream in(path_);  // 打开日志输入文件
    std::string line;  // 保存当前读取行
    while (std::getline(in, line)) {  // 逐行读取日志
      std::istringstream input(line);  // 为当前行创建解析流
      std::string sequence_text;  // 保存序列号文本
      std::string key;  // 保存解析出的键
      std::string value;  // 保存解析出的值
      if (std::getline(input, sequence_text, '\t') && std::getline(input, key, '\t') && std::getline(input, value)) {  // 解析完整三段记录
        records.push_back({std::stoull(sequence_text), key, value});  // 添加回放记录
      }  // 结束解析成功分支
    }  // 结束日志读取循环
    return records;  // 返回全部日志记录
  }  // 结束 Replay
 private:  // 内部数据区域
  std::filesystem::path path_;  // 保存日志文件路径
};  // 结束 WAL 定义

class MiniDB {  // 迷你 LevelDB 门面类
 public:  // 对外接口区域
  explicit MiniDB(std::filesystem::path directory) : directory_(std::move(directory)), wal_(directory_ / "wal.log") {  // 初始化数据库目录和 WAL
    std::filesystem::create_directories(directory_);  // 确保数据库目录存在
  }  // 结束 MiniDB 构造
  void Recover() {  // 从 WAL 恢复内存表
    for (const auto& [sequence, key, value] : wal_.Replay()) {  // 遍历日志记录
      mem_.Put(sequence, key, value);  // 恢复记录到内存表
      sequence_ = std::max(sequence_, sequence);  // 更新最大序列号
    }  // 结束日志记录遍历
  }  // 结束 Recover
  void Put(const std::string& key, const std::string& value) {  // 写入键值对
    const auto sequence = ++sequence_;  // 生成新的递增序列号
    wal_.Append(sequence, key, value);  // 先写入 WAL
    mem_.Put(sequence, key, value);  // 再写入内存表
  }  // 结束 Put
  std::optional<std::string> Get(const std::string& key) const {  // 读取键对应的值
    if (auto value = mem_.Get(key)) return value;  // 优先查询内存表
    return sstable_.Get(key);  // 未命中时查询 SSTable
  }  // 结束 Get
  void Flush() {  // 刷写内存表到 SSTable
    sstable_.Build(directory_ / "000001.sst", mem_.EntriesByUserKey());  // 构建固定文件名的 SSTable
  }  // 结束 Flush
  std::size_t MemSize() const { return mem_.size(); }  // 返回内存表大小
  std::size_t SSTableSize() const { return sstable_.size(); }  // 返回 SSTable 大小
 private:  // 内部数据区域
  std::filesystem::path directory_;  // 保存数据库目录
  WAL wal_;  // 保存预写日志对象
  MemTable mem_;  // 保存内存表对象
  SSTable sstable_;  // 保存单个 SSTable 对象
  std::uint64_t sequence_ = 0;  // 保存最新序列号
};  // 结束 MiniDB 定义

}  // namespace mini_lsm