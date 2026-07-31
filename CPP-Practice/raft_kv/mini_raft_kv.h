#pragma once

#include <algorithm>
#include <cstddef>
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
    int term = 0;  // 所属任期：Raft 靠 (term, index) 唯一定位并比较日志新旧
    std::string op;
    std::string key;
    std::string value;
};

// KV 状态机：只被已提交日志按序驱动，apply 必须是确定性的，否则各副本状态会发散。
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
    void restore(std::map<std::string, std::string> data) {
        data_ = std::move(data);
    }  // 快照恢复入口
 private:
    std::map<std::string, std::string> data_;  // 用 map 保证快照序列化时的确定性顺序
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

    void crash() {
        alive_ = false;
        role_ = Role::Follower;
    }
    void restart() {
        alive_ = true;
        role_ = Role::Follower;
        voted_for_ = -1;
    }
    // 任期只增不减：见到更高任期一律退回 Follower 并清空本任期投票记录。
    void become_follower(int term) {
        current_term_ = std::max(current_term_, term);
        role_ = Role::Follower;
        voted_for_ = -1;
    }
    void become_leader() { role_ = Role::Leader; }

    bool request_vote(int term, int candidate_id, int candidate_last_log_index) {
        if (!alive_) return false;
        if (term < current_term_) return false;           // 拒绝过期任期的候选者
        if (term > current_term_) become_follower(term);  // 更高任期：先跟随再考虑投票
        // 选举限制：候选者日志不能比本地旧，否则可能选出丢失已提交条目的 Leader。
        const bool log_ok = candidate_last_log_index >= static_cast<int>(log_.size()) - 1;
        // 每任期至多投一票（voted_for_ 约束），保证同一任期不会选出两个 Leader。
        if ((voted_for_ == -1 || voted_for_ == candidate_id) && log_ok) {
            voted_for_ = candidate_id;
            return true;
        }
        return false;
    }

    bool append_entries(int term, const std::vector<LogEntry>& leader_log, int leader_commit) {
        if (!alive_) return false;
        if (term < current_term_) return false;  // 拒绝过期 Leader，防止脑裂时旧 Leader 覆盖新数据
        become_follower(term);
        log_ = leader_log;         // 教学简化：整表覆盖，等价于“以 Leader 日志为准”
        commit_to(leader_commit);  // 提交只推进到 Leader 告知的位置，不超前
        return true;
    }

    LogEntry append_local(std::string op, std::string key, std::string value) {
        LogEntry entry{current_term_, std::move(op), std::move(key),
                       std::move(value)};  // 打上当前任期
        log_.push_back(entry);
        return entry;
    }

    void commit_to(int index) {
        // 提交单调递增，且只应用真实存在的条目；每推进一格就把该条目喂给状态机，
        // 保证 commit_index 与已 apply 的状态严格对应。
        while (commit_index_ < index && commit_index_ + 1 < static_cast<int>(log_.size())) {
            ++commit_index_;
            store_.apply(log_[commit_index_]);
        }
    }

    // 持久化 term/commit/log：WAL 式追加是崩溃后能恢复已提交状态的前提。
    void persist(const std::filesystem::path& path) const {
        std::filesystem::create_directories(path.parent_path());
        std::ofstream out(path, std::ios::trunc);
        out << current_term_ << ' ' << commit_index_ << '\n';  // 首行存元信息
        for (const auto& entry : log_)
            out << entry.term << '\t' << entry.op << '\t' << entry.key << '\t' << entry.value
                << '\n';
    }

    void recover(const std::filesystem::path& path) {
        std::ifstream in(path);
        if (!in) return;  // 无文件视为全新节点，静默返回
        log_.clear();
        store_.restore({});
        in >> current_term_ >> commit_index_;
        std::string rest;
        std::getline(in, rest);  // 消费首行残余换行，后续 getline 才对齐日志行
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream input(line);
            LogEntry entry;
            std::string term;
            std::getline(input, term, '\t');
            std::getline(input, entry.op, '\t');
            std::getline(input, entry.key, '\t');
            std::getline(input, entry.value);  // 值可能含空格，读到行尾
            entry.term = std::stoi(term);
            log_.push_back(entry);
        }
        // 重放而非信任落盘 KV：把 commit_index 归零再 commit_to，让状态机由日志重建，
        // 确保恢复出的状态与提交历史一致。
        int saved_commit = commit_index_;
        commit_index_ = -1;
        commit_to(saved_commit);
    }

    std::map<std::string, std::string> snapshot() const { return store_.data(); }
    // 安装快照后丢弃已被覆盖的日志，并把 commit_index 跳到快照包含的最后位置，
    // 用于让落后过多的 Follower 快速追平而不必逐条补日志。
    void install_snapshot(std::map<std::string, std::string> data, int last_included_index) {
        store_.restore(std::move(data));
        log_.clear();
        commit_index_ = last_included_index;
    }

 private:
    int id_;
    Role role_ = Role::Follower;
    int current_term_ = 0;
    int voted_for_ = -1;  // -1 表示本任期尚未投票
    bool alive_ = true;
    std::vector<LogEntry> log_;
    int commit_index_ = -1;  // -1 表示尚无已提交条目（下标从 0 起）
    KVStore store_;
};

// 单进程模拟集群：用直接函数调用替代 RPC，便于确定性地测试选举与复制的安全性。
class RaftCluster {
 public:
    explicit RaftCluster(int count) {
        for (int id = 0; id < count; ++id) nodes_.push_back(RaftNode(id));
    }

    bool elect(int candidate_id) {
        RaftNode& candidate = nodes_.at(candidate_id);
        if (!candidate.alive()) return false;  // 宕机节点不能当选
        int next_term = max_term() + 1;        // 新选举必须用更高任期
        candidate.become_follower(next_term);
        int votes = 1;  // 候选者先投自己一票
        for (auto& node : nodes_) {
            if (node.id() == candidate_id) continue;
            if (node.request_vote(next_term, candidate_id,
                                  static_cast<int>(candidate.log().size()) - 1))
                ++votes;
        }
        if (votes >= majority()) {  // 只有拿到多数派才当选，保证任一任期至多一个 Leader
            for (auto& node : nodes_)
                if (node.id() != candidate_id && node.alive()) node.become_follower(next_term);
            candidate.become_leader();
            leader_id_ = candidate_id;
            return true;
        }
        return false;
    }

    bool put(const std::string& key, const std::string& value) {
        return replicate("put", key, value);
    }
    bool erase(const std::string& key) { return replicate("delete", key, ""); }
    std::optional<std::string> get(const std::string& key) const {
        if (leader_id_ < 0) return std::nullopt;  // 无 Leader 时读不可用（强一致读只走 Leader）
        return nodes_[leader_id_].store().get(key);
    }
    void crash_leader() {
        if (leader_id_ >= 0) {
            nodes_[leader_id_].crash();
            leader_id_ = -1;
        }
    }  // 崩溃后集群暂无 Leader
    RaftNode& node(int id) { return nodes_.at(id); }
    const RaftNode& node(int id) const { return nodes_.at(id); }
    int leader_id() const { return leader_id_; }

 private:
    bool replicate(const std::string& op, const std::string& key, const std::string& value) {
        if (leader_id_ < 0) return false;
        RaftNode& leader = nodes_[leader_id_];
        if (!leader.alive() || leader.role() != Role::Leader)
            return false;                     // 只有活着的 Leader 能发起复制
        leader.append_local(op, key, value);  // 先落 Leader 本地日志，再向外复制
        int ack = 1;                          // Leader 自身算一份确认
        for (auto& node : nodes_) {
            if (node.id() == leader_id_) continue;
            if (node.append_entries(leader.term(), leader.log(),
                                    static_cast<int>(leader.log().size()) - 1))
                ++ack;
        }
        if (ack >= majority()) {  // 达多数派才提交——这是“已提交=不丢失”安全性的关键门槛
            leader.commit_to(static_cast<int>(leader.log().size()) - 1);
            // 再跑一轮把新 commit_index 下推给 Follower，使其状态机同步应用。
            for (auto& node : nodes_)
                if (node.id() != leader_id_ && node.alive())
                    node.append_entries(leader.term(), leader.log(), leader.commit_index());
            return true;
        }
        return false;  // 未达多数派：不提交，写入失败
    }
    int majority() const {
        return static_cast<int>(nodes_.size() / 2 + 1);
    }  // N 节点多数派 = N/2+1
    int max_term() const {
        int result = 0;
        for (const auto& node : nodes_) result = std::max(result, node.term());
        return result;
    }

    std::vector<RaftNode> nodes_;
    int leader_id_ = -1;
};

}  // namespace mini_raft
