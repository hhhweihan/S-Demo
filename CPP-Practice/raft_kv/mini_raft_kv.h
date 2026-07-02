#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace mini_raft {

enum class Role { Follower, Candidate, Leader };

struct LogEntry {
  int term = 0;
  std::string op;
  std::string key;
  std::string value;
};

class KVStore {
 public:
  void apply(const LogEntry& entry) {
    if (entry.op == "put") data_[entry.key] = entry.value;
    if (entry.op == "delete") data_.erase(entry.key);
  }
  std::optional<std::string> get(const std::string& key) const {
    auto it = data_.find(key);
    if (it == data_.end()) return std::nullopt;
    return it->second;
  }
  std::size_t size() const { return data_.size(); }
  const std::map<std::string, std::string>& data() const { return data_; }
  void restore(std::map<std::string, std::string> data) { data_ = std::move(data); }
 private:
  std::map<std::string, std::string> data_;
};

class RaftNode {
 public:
  explicit RaftNode(int id) : id_(id) {}
  int id() const { return id_; }
  Role role() const { return role_; }
  int term() const { return current_term_; }
  bool alive() const { return alive_; }
  int commit_index() const { return commit_index_; }
  const std::vector<LogEntry>& log() const { return log_; }
  const KVStore& store() const { return store_; }

  void crash() { alive_ = false; role_ = Role::Follower; }
  void restart() { alive_ = true; role_ = Role::Follower; voted_for_ = -1; }
  void become_follower(int term) { current_term_ = std::max(current_term_, term); role_ = Role::Follower; voted_for_ = -1; }
  void become_leader() { role_ = Role::Leader; }

  bool request_vote(int term, int candidate_id, int candidate_last_log_index) {
    if (!alive_) return false;
    if (term < current_term_) return false;
    if (term > current_term_) become_follower(term);
    const bool log_ok = candidate_last_log_index >= static_cast<int>(log_.size()) - 1;
    if ((voted_for_ == -1 || voted_for_ == candidate_id) && log_ok) {
      voted_for_ = candidate_id;
      return true;
    }
    return false;
  }

  bool append_entries(int term, const std::vector<LogEntry>& leader_log, int leader_commit) {
    if (!alive_) return false;
    if (term < current_term_) return false;
    become_follower(term);
    log_ = leader_log;
    commit_to(leader_commit);
    return true;
  }

  LogEntry append_local(std::string op, std::string key, std::string value) {
    LogEntry entry{current_term_, std::move(op), std::move(key), std::move(value)};
    log_.push_back(entry);
    return entry;
  }

  void commit_to(int index) {
    while (commit_index_ < index && commit_index_ + 1 < static_cast<int>(log_.size())) {
      ++commit_index_;
      store_.apply(log_[commit_index_]);
    }
  }

  void persist(const std::filesystem::path& path) const {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::trunc);
    out << current_term_ << ' ' << commit_index_ << '\n';
    for (const auto& entry : log_) out << entry.term << '\t' << entry.op << '\t' << entry.key << '\t' << entry.value << '\n';
  }

  void recover(const std::filesystem::path& path) {
    std::ifstream in(path);
    if (!in) return;
    log_.clear();
    store_.restore({});
    in >> current_term_ >> commit_index_;
    std::string rest;
    std::getline(in, rest);
    std::string line;
    while (std::getline(in, line)) {
      std::istringstream input(line);
      LogEntry entry;
      std::string term;
      std::getline(input, term, '\t');
      std::getline(input, entry.op, '\t');
      std::getline(input, entry.key, '\t');
      std::getline(input, entry.value);
      entry.term = std::stoi(term);
      log_.push_back(entry);
    }
    int saved_commit = commit_index_;
    commit_index_ = -1;
    commit_to(saved_commit);
  }

  std::map<std::string, std::string> snapshot() const { return store_.data(); }
  void install_snapshot(std::map<std::string, std::string> data, int last_included_index) {
    store_.restore(std::move(data));
    log_.clear();
    commit_index_ = last_included_index;
  }

 private:
  int id_;
  Role role_ = Role::Follower;
  int current_term_ = 0;
  int voted_for_ = -1;
  bool alive_ = true;
  std::vector<LogEntry> log_;
  int commit_index_ = -1;
  KVStore store_;
};

class RaftCluster {
 public:
  explicit RaftCluster(int count) {
    for (int id = 0; id < count; ++id) nodes_.push_back(RaftNode(id));
  }

  bool elect(int candidate_id) {
    RaftNode& candidate = nodes_.at(candidate_id);
    if (!candidate.alive()) return false;
    int next_term = max_term() + 1;
    candidate.become_follower(next_term);
    int votes = 1;
    for (auto& node : nodes_) {
      if (node.id() == candidate_id) continue;
      if (node.request_vote(next_term, candidate_id, static_cast<int>(candidate.log().size()) - 1)) ++votes;
    }
    if (votes >= majority()) {
      for (auto& node : nodes_) if (node.id() != candidate_id && node.alive()) node.become_follower(next_term);
      candidate.become_leader();
      leader_id_ = candidate_id;
      return true;
    }
    return false;
  }

  bool put(const std::string& key, const std::string& value) { return replicate("put", key, value); }
  bool erase(const std::string& key) { return replicate("delete", key, ""); }
  std::optional<std::string> get(const std::string& key) const {
    if (leader_id_ < 0) return std::nullopt;
    return nodes_[leader_id_].store().get(key);
  }
  void crash_leader() { if (leader_id_ >= 0) { nodes_[leader_id_].crash(); leader_id_ = -1; } }
  RaftNode& node(int id) { return nodes_.at(id); }
  const RaftNode& node(int id) const { return nodes_.at(id); }
  int leader_id() const { return leader_id_; }

 private:
  bool replicate(const std::string& op, const std::string& key, const std::string& value) {
    if (leader_id_ < 0) return false;
    RaftNode& leader = nodes_[leader_id_];
    if (!leader.alive() || leader.role() != Role::Leader) return false;
    leader.append_local(op, key, value);
    int ack = 1;
    for (auto& node : nodes_) {
      if (node.id() == leader_id_) continue;
      if (node.append_entries(leader.term(), leader.log(), static_cast<int>(leader.log().size()) - 1)) ++ack;
    }
    if (ack >= majority()) {
      leader.commit_to(static_cast<int>(leader.log().size()) - 1);
      for (auto& node : nodes_) if (node.id() != leader_id_ && node.alive()) node.append_entries(leader.term(), leader.log(), leader.commit_index());
      return true;
    }
    return false;
  }
  int majority() const { return static_cast<int>(nodes_.size() / 2 + 1); }
  int max_term() const { int result = 0; for (const auto& node : nodes_) result = std::max(result, node.term()); return result; }

  std::vector<RaftNode> nodes_;
  int leader_id_ = -1;
};

}  // namespace mini_raft