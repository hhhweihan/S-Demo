#pragma once  // 防止头文件被重复包含

#include <algorithm>  // 提供通用算法
#include <filesystem>  // 提供路径与目录操作
#include <fstream>  // 提供文件输入输出
#include <map>  // 提供有序映射容器
#include <optional>  // 提供可选返回值类型
#include <sstream>  // 提供字符串流解析
#include <stdexcept>  // 提供标准异常类型
#include <string>  // 提供字符串类型
#include <utility>  // 提供移动语义工具
#include <vector>  // 提供动态数组容器

namespace mini_raft {  // 定义迷你 RaftKV 命名空间

enum class Role { Follower, Candidate, Leader };  // 表示 Raft 节点角色

struct LogEntry {  // 表示一条 Raft 日志项
  int term = 0;  // 记录日志所属任期
  std::string op;  // 记录状态机操作类型
  std::string key;  // 记录操作键
  std::string value;  // 记录操作值
};  // 结束日志项定义

class KVStore {  // 定义内存 KV 状态机
 public:  // 公开状态机操作接口
  void apply(const LogEntry& entry) {  // 将日志项应用到 KV 状态机
    if (entry.op == "put") data_[entry.key] = entry.value;  // 处理写入操作
    if (entry.op == "delete") data_.erase(entry.key);  // 处理删除操作
  }  // 结束日志应用函数
  std::optional<std::string> get(const std::string& key) const {  // 查询指定键的值
    auto it = data_.find(key);  // 在映射中查找键
    if (it == data_.end()) return std::nullopt;  // 未找到时返回空值
    return it->second;  // 找到时返回对应值
  }  // 结束查询函数
  std::size_t size() const { return data_.size(); }  // 返回当前键值数量
  const std::map<std::string, std::string>& data() const { return data_; }  // 返回只读底层数据
  void restore(std::map<std::string, std::string> data) { data_ = std::move(data); }  // 用快照数据恢复状态机
 private:  // 私有状态数据
  std::map<std::string, std::string> data_;  // 保存有序键值数据
};  // 结束 KV 状态机定义

class RaftNode {  // 定义单个 Raft 节点
 public:  // 公开节点操作接口
  explicit RaftNode(int id) : id_(id) {}  // 使用节点编号构造节点
  int id() const { return id_; }  // 返回节点编号
  Role role() const { return role_; }  // 返回当前角色
  int term() const { return current_term_; }  // 返回当前任期
  bool alive() const { return alive_; }  // 返回节点存活状态
  int commit_index() const { return commit_index_; }  // 返回已提交日志下标
  const std::vector<LogEntry>& log() const { return log_; }  // 返回只读日志列表
  const KVStore& store() const { return store_; }  // 返回只读状态机

  void crash() { alive_ = false; role_ = Role::Follower; }  // 模拟节点崩溃并降为 Follower
  void restart() { alive_ = true; role_ = Role::Follower; voted_for_ = -1; }  // 重启节点并清空投票记录
  void become_follower(int term) { current_term_ = std::max(current_term_, term); role_ = Role::Follower; voted_for_ = -1; }  // 切换为指定任期的 Follower
  void become_leader() { role_ = Role::Leader; }  // 将节点切换为 Leader

  bool request_vote(int term, int candidate_id, int candidate_last_log_index) {  // 处理候选者投票请求
    if (!alive_) return false;  // 节点宕机时拒绝投票
    if (term < current_term_) return false;  // 旧任期请求直接拒绝
    if (term > current_term_) become_follower(term);  // 新任期请求使节点跟随新任期
    const bool log_ok = candidate_last_log_index >= static_cast<int>(log_.size()) - 1;  // 检查候选者日志是否足够新
    if ((voted_for_ == -1 || voted_for_ == candidate_id) && log_ok) {  // 未投票或已投给该候选者且日志合格时同意
      voted_for_ = candidate_id;  // 记录本任期投票对象
      return true;  // 返回同意投票
    }  // 结束同意投票分支
    return false;  // 其他情况拒绝投票
  }  // 结束投票请求处理

  bool append_entries(int term, const std::vector<LogEntry>& leader_log, int leader_commit) {  // 处理 Leader 日志追加请求
    if (!alive_) return false;  // 节点宕机时拒绝追加
    if (term < current_term_) return false;  // 旧任期 Leader 请求直接拒绝
    become_follower(term);  // 接受当前 Leader 任期并保持 Follower
    log_ = leader_log;  // 用 Leader 日志覆盖本地日志
    commit_to(leader_commit);  // 提交到 Leader 指定位置
    return true;  // 返回追加成功
  }  // 结束日志追加处理

  LogEntry append_local(std::string op, std::string key, std::string value) {  // 在 Leader 本地追加一条日志
    LogEntry entry{current_term_, std::move(op), std::move(key), std::move(value)};  // 构造当前任期日志项
    log_.push_back(entry);  // 将日志项追加到本地日志末尾
    return entry;  // 返回追加的日志项副本
  }  // 结束本地追加函数

  void commit_to(int index) {  // 将日志提交推进到指定下标
    while (commit_index_ < index && commit_index_ + 1 < static_cast<int>(log_.size())) {  // 逐条推进尚未提交的日志
      ++commit_index_;  // 移动到下一条提交日志
      store_.apply(log_[commit_index_]);  // 将提交日志应用到状态机
    }  // 结束提交推进循环
  }  // 结束提交函数

  void persist(const std::filesystem::path& path) const {  // 将节点任期、提交位置和日志持久化
    std::filesystem::create_directories(path.parent_path());  // 确保持久化目录存在
    std::ofstream out(path, std::ios::trunc);  // 以截断方式打开输出文件
    out << current_term_ << ' ' << commit_index_ << '\n';  // 写入当前任期和提交下标
    for (const auto& entry : log_) out << entry.term << '\t' << entry.op << '\t' << entry.key << '\t' << entry.value << '\n';  // 逐条写入日志项
  }  // 结束持久化函数

  void recover(const std::filesystem::path& path) {  // 从持久化文件恢复节点状态
    std::ifstream in(path);  // 打开输入文件流
    if (!in) return;  // 文件不存在或打开失败时直接返回
    log_.clear();  // 清空当前内存日志
    store_.restore({});  // 清空当前状态机数据
    in >> current_term_ >> commit_index_;  // 读取任期和提交下标
    std::string rest;  // 保存首行剩余内容
    std::getline(in, rest);  // 吃掉首行换行前剩余字符
    std::string line;  // 保存逐行读取的日志文本
    while (std::getline(in, line)) {  // 逐行读取持久化日志
      std::istringstream input(line);  // 构造当前日志行解析流
      LogEntry entry;  // 准备恢复一条日志项
      std::string term;  // 临时保存文本形式任期
      std::getline(input, term, '\t');  // 解析日志任期字段
      std::getline(input, entry.op, '\t');  // 解析操作字段
      std::getline(input, entry.key, '\t');  // 解析键字段
      std::getline(input, entry.value);  // 解析值字段
      entry.term = std::stoi(term);  // 将任期文本转换为整数
      log_.push_back(entry);  // 追加恢复出的日志项
    }  // 结束日志读取循环
    int saved_commit = commit_index_;  // 暂存文件中的提交下标
    commit_index_ = -1;  // 重置提交下标以便重放状态机
    commit_to(saved_commit);  // 重放已提交日志恢复状态机
  }  // 结束恢复函数

  std::map<std::string, std::string> snapshot() const { return store_.data(); }  // 生成当前状态机快照
  void install_snapshot(std::map<std::string, std::string> data, int last_included_index) {  // 安装外部传入的状态机快照
    store_.restore(std::move(data));  // 用快照数据恢复状态机
    log_.clear();  // 清空已被快照覆盖的日志
    commit_index_ = last_included_index;  // 更新快照包含的最后日志下标
  }  // 结束快照安装函数

 private:  // 私有节点状态
  int id_;  // 节点编号
  Role role_ = Role::Follower;  // 当前节点角色
  int current_term_ = 0;  // 当前任期号
  int voted_for_ = -1;  // 当前任期已投票候选者
  bool alive_ = true;  // 节点是否存活
  std::vector<LogEntry> log_;  // 本地日志序列
  int commit_index_ = -1;  // 已提交日志下标
  KVStore store_;  // 节点本地 KV 状态机
};  // 结束 Raft 节点定义

class RaftCluster {  // 定义用于测试的 Raft 集群
 public:  // 公开集群操作接口
  explicit RaftCluster(int count) {  // 按节点数量构造集群
    for (int id = 0; id < count; ++id) nodes_.push_back(RaftNode(id));  // 逐个创建节点
  }  // 结束集群构造函数

  bool elect(int candidate_id) {  // 发起一次简化 Leader 选举
    RaftNode& candidate = nodes_.at(candidate_id);  // 获取候选节点引用
    if (!candidate.alive()) return false;  // 候选节点宕机时选举失败
    int next_term = max_term() + 1;  // 计算下一任期号
    candidate.become_follower(next_term);  // 将候选者切入新任期
    int votes = 1;  // 候选者默认投自己一票
    for (auto& node : nodes_) {  // 遍历集群中所有节点请求投票
      if (node.id() == candidate_id) continue;  // 跳过候选者自身
      if (node.request_vote(next_term, candidate_id, static_cast<int>(candidate.log().size()) - 1)) ++votes;  // 统计同意票数
    }  // 结束投票请求循环
    if (votes >= majority()) {  // 票数达到多数时当选
      for (auto& node : nodes_) if (node.id() != candidate_id && node.alive()) node.become_follower(next_term);  // 使其他存活节点跟随新任期
      candidate.become_leader();  // 将候选者切换为 Leader
      leader_id_ = candidate_id;  // 记录当前 Leader 编号
      return true;  // 返回选举成功
    }  // 结束当选分支
    return false;  // 未达多数时选举失败
  }  // 结束选举函数

  bool put(const std::string& key, const std::string& value) { return replicate("put", key, value); }  // 通过 Raft 复制写入操作
  bool erase(const std::string& key) { return replicate("delete", key, ""); }  // 通过 Raft 复制删除操作
  std::optional<std::string> get(const std::string& key) const {  // 从当前 Leader 状态机读取键值
    if (leader_id_ < 0) return std::nullopt;  // 无 Leader 时返回空值
    return nodes_[leader_id_].store().get(key);  // 返回 Leader 上的键值查询结果
  }  // 结束读取函数
  void crash_leader() { if (leader_id_ >= 0) { nodes_[leader_id_].crash(); leader_id_ = -1; } }  // 模拟当前 Leader 崩溃
  RaftNode& node(int id) { return nodes_.at(id); }  // 返回指定节点可变引用
  const RaftNode& node(int id) const { return nodes_.at(id); }  // 返回指定节点只读引用
  int leader_id() const { return leader_id_; }  // 返回当前 Leader 编号

 private:  // 私有集群辅助逻辑
  bool replicate(const std::string& op, const std::string& key, const std::string& value) {  // 由 Leader 复制一条状态机操作
    if (leader_id_ < 0) return false;  // 无 Leader 时复制失败
    RaftNode& leader = nodes_[leader_id_];  // 获取当前 Leader 引用
    if (!leader.alive() || leader.role() != Role::Leader) return false;  // Leader 不可用时复制失败
    leader.append_local(op, key, value);  // 先在 Leader 本地追加日志
    int ack = 1;  // Leader 自身确认一票
    for (auto& node : nodes_) {  // 遍历所有节点进行日志同步
      if (node.id() == leader_id_) continue;  // 跳过 Leader 自身
      if (node.append_entries(leader.term(), leader.log(), static_cast<int>(leader.log().size()) - 1)) ++ack;  // 统计成功复制确认数
    }  // 结束复制循环
    if (ack >= majority()) {  // 达到多数确认后提交日志
      leader.commit_to(static_cast<int>(leader.log().size()) - 1);  // Leader 提交最新日志
      for (auto& node : nodes_) if (node.id() != leader_id_ && node.alive()) node.append_entries(leader.term(), leader.log(), leader.commit_index());  // 将提交位置同步给 Follower
      return true;  // 返回复制成功
    }  // 结束多数确认分支
    return false;  // 未达多数时复制失败
  }  // 结束复制函数
  int majority() const { return static_cast<int>(nodes_.size() / 2 + 1); }  // 计算多数派数量
  int max_term() const { int result = 0; for (const auto& node : nodes_) result = std::max(result, node.term()); return result; }  // 计算集群当前最大任期

  std::vector<RaftNode> nodes_;  // 保存集群内所有节点
  int leader_id_ = -1;  // 记录当前 Leader 编号
};  // 结束 Raft 集群定义

}  // namespace mini_raft