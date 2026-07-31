#pragma once

// Deterministic discrete-event Raft simulation for fault-injection testing.
//
// Why this exists (vs. the god's-eye mini_raft_kv.h): the teaching version replicates
// synchronously by directly reading every node's state, so it can never exhibit the
// failures Raft actually has to survive — lost/reordered/duplicated messages, network
// partitions, split votes, stale leaders. This harness drives real per-node RPC handlers
// (RequestVote / AppendEntries with the prevLog consistency check, nextIndex/matchIndex,
// and term step-down) over a virtual-time network that can drop, delay, duplicate,
// reorder, and partition traffic. Everything is single-threaded and seeded, so a failing
// scenario is exactly reproducible from (seed, operations) — the property tests rely on
// that. Safety invariants are checked after every step so a violation aborts at the seed
// that caused it, not several rounds later.

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace raft_sim {

enum class Role { Follower, Candidate, Leader };

struct LogEntry {
    int term = 0;
    std::string op;  // "put" | "delete" | "noop"
    std::string key;
    std::string value;

    bool operator==(const LogEntry& other) const {
        return term == other.term && op == other.op && key == other.key && value == other.value;
    }
};

// --- KV state machine -------------------------------------------------------

class KVStore {
 public:
    void apply(const LogEntry& entry) {
        if (entry.op == "put")
            data_[entry.key] = entry.value;
        else if (entry.op == "delete")
            data_.erase(entry.key);
        // "noop" intentionally touches nothing (leaders append one on election).
    }
    std::optional<std::string> get(const std::string& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) return std::nullopt;
        return it->second;
    }
    const std::map<std::string, std::string>& data() const { return data_; }

 private:
    std::map<std::string, std::string> data_;
};

// --- RPC payloads -----------------------------------------------------------

struct RequestVoteArgs {
    int term;
    int candidateId;
    int lastLogIndex;
    int lastLogTerm;
};
struct RequestVoteReply {
    int term;
    bool voteGranted;
};
struct AppendEntriesArgs {
    int term;
    int leaderId;
    int prevLogIndex;
    int prevLogTerm;
    int leaderCommit;
    std::vector<LogEntry> entries;
};
struct AppendEntriesReply {
    int term;
    bool success;
    int matchIndex;  // on success: last index the follower now agrees on
};

struct Message {
    int from;
    int to;
    std::variant<RequestVoteArgs, RequestVoteReply, AppendEntriesArgs, AppendEntriesReply> body;
    std::int64_t deliverAt;
    std::int64_t seq;  // stable tiebreak so ordering is deterministic
};

// --- per-node state ---------------------------------------------------------

struct Node {
    explicit Node(int node_id) : id(node_id) {}

    int id;
    bool alive = true;
    Role role = Role::Follower;
    int currentTerm = 0;
    int votedFor = -1;
    std::vector<LogEntry> log;  // 0-based; termAt(-1) == 0 by convention
    int commitIndex = -1;
    int lastApplied = -1;
    KVStore store;

    // leader-only bookkeeping
    std::vector<int> nextIndex;
    std::vector<int> matchIndex;
    int votesGranted = 0;

    std::int64_t electionDeadline = 0;   // followers/candidates
    std::int64_t heartbeatDeadline = 0;  // leaders

    int lastLogIndex() const { return static_cast<int>(log.size()) - 1; }
    int termAt(int index) const {
        if (index < 0 || index >= static_cast<int>(log.size())) return 0;
        return log[index].term;
    }
};

// --- timing / fault configuration ------------------------------------------

struct Config {
    std::int64_t electionMin = 50;
    std::int64_t electionMax = 100;
    std::int64_t heartbeat = 15;
    std::int64_t delayMin = 2;
    std::int64_t delayMax = 8;
    double dropRate = 0.0;  // per-message, on top of partitions
    double dupRate = 0.0;   // chance a delivered message is duplicated
};

// --- the cluster / simulation driver ---------------------------------------

class Cluster {
 public:
    Cluster(int count, std::uint64_t seed, Config cfg = {}) : n_(count), cfg_(cfg), rng_(seed) {
        reachable_.assign(n_, std::vector<char>(n_, 1));
        for (int i = 0; i < n_; ++i) nodes_.emplace_back(i);
        for (auto& node : nodes_) resetElectionTimer(node);
    }

    // ---- fault injection knobs (all deterministic given the seed) ----
    void crash(int id) {
        Node& node = nodes_.at(id);
        node.alive = false;
        node.role = Role::Follower;
    }
    void restart(int id) {
        // Volatile state is lost on restart; persistent state (currentTerm, votedFor, log)
        // survives — that is exactly what Raft assumes stable storage guarantees.
        Node& node = nodes_.at(id);
        node.alive = true;
        node.role = Role::Follower;
        node.commitIndex = -1;
        node.lastApplied = -1;
        node.store = KVStore{};
        replayCommittedAfterRestart(node);
        resetElectionTimer(node);
    }
    void partition(const std::vector<int>& groupA, const std::vector<int>& groupB) {
        for (int a : groupA)
            for (int b : groupB) {
                reachable_[a][b] = 0;
                reachable_[b][a] = 0;
            }
    }
    void heal() {
        for (int i = 0; i < n_; ++i)
            for (int j = 0; j < n_; ++j) reachable_[i][j] = 1;
    }

    // ---- client interface ----
    // Appends to whoever currently believes it is leader; returns the log index, or -1.
    int submit(const std::string& op, const std::string& key, const std::string& value) {
        int leader = currentLeader();
        if (leader < 0) return -1;
        Node& node = nodes_[leader];
        node.log.push_back(LogEntry{node.currentTerm, op, key, value});
        node.matchIndex[leader] = node.lastLogIndex();
        return node.lastLogIndex();
    }

    // ---- observation ----
    int currentLeader() const {
        int best = -1, bestTerm = -1;
        for (const auto& node : nodes_)
            if (node.alive && node.role == Role::Leader && node.currentTerm > bestTerm) {
                bestTerm = node.currentTerm;
                best = node.id;
            }
        return best;
    }
    const Node& node(int id) const { return nodes_.at(id); }
    int size() const { return n_; }
    std::int64_t now() const { return now_; }
    const std::vector<LogEntry>& committedLog() const { return committedLog_; }
    // Index i is committed cluster-wide once it appears in the agreed committed prefix.
    bool isCommitted(int index) const {
        return index >= 0 && index < static_cast<int>(committedLog_.size());
    }

    // ---- driving time ----
    void runFor(std::int64_t ms) {
        std::int64_t target = now_ + ms;
        while (now_ < target) step(target);
    }
    // Runs until pred() holds or the budget is exhausted; returns whether pred held.
    bool runUntil(const std::function<bool()>& pred, std::int64_t maxMs) {
        std::int64_t target = now_ + maxMs;
        while (now_ < target) {
            if (pred()) return true;
            step(target);
        }
        return pred();
    }

 private:
    // Advance virtual time to the next scheduled event (timer or message), no later than
    // `cap`, process everything due at that instant, then re-check safety invariants.
    void step(std::int64_t cap) {
        std::int64_t next = cap;
        for (const auto& node : nodes_) {
            if (!node.alive) continue;
            std::int64_t deadline =
                node.role == Role::Leader ? node.heartbeatDeadline : node.electionDeadline;
            next = std::min(next, deadline);
        }
        if (!wire_.empty()) next = std::min(next, wire_.begin()->deliverAt);
        now_ = next;

        for (auto& node : nodes_) {
            if (!node.alive) continue;
            if (node.role == Role::Leader) {
                if (node.heartbeatDeadline <= now_) broadcastAppendEntries(node);
            } else if (node.electionDeadline <= now_) {
                startElection(node);
            }
        }

        while (!wire_.empty() && wire_.begin()->deliverAt <= now_) {
            Message msg = *wire_.begin();
            wire_.erase(wire_.begin());
            deliver(msg);
        }

        checkInvariants();
    }

    // ---- election / replication logic ----

    void resetElectionTimer(Node& node) {
        std::uniform_int_distribution<std::int64_t> dist(cfg_.electionMin, cfg_.electionMax);
        node.electionDeadline = now_ + dist(rng_);
    }

    void stepDown(Node& node, int term) {
        node.currentTerm = term;
        node.role = Role::Follower;
        node.votedFor = -1;
        resetElectionTimer(node);
    }

    void startElection(Node& node) {
        node.role = Role::Candidate;
        ++node.currentTerm;
        node.votedFor = node.id;
        node.votesGranted = 1;
        resetElectionTimer(node);
        RequestVoteArgs args{node.currentTerm, node.id, node.lastLogIndex(),
                             node.termAt(node.lastLogIndex())};
        for (int peer = 0; peer < n_; ++peer)
            if (peer != node.id) send(node.id, peer, args);
    }

    void becomeLeader(Node& node) {
        node.role = Role::Leader;
        node.nextIndex.assign(n_, node.lastLogIndex() + 1);
        node.matchIndex.assign(n_, -1);
        node.matchIndex[node.id] = node.lastLogIndex();
        recordLeader(node.currentTerm, node.id);
        // A fresh leader commits nothing from prior terms until it commits one of its own
        // entries (Raft's no-op / current-term rule); append a no-op to make progress.
        node.log.push_back(LogEntry{node.currentTerm, "noop", "", ""});
        node.matchIndex[node.id] = node.lastLogIndex();
        node.heartbeatDeadline = now_;  // send immediately
    }

    void broadcastAppendEntries(Node& leader) {
        leader.heartbeatDeadline = now_ + cfg_.heartbeat;
        for (int peer = 0; peer < n_; ++peer) {
            if (peer == leader.id) continue;
            int prev = leader.nextIndex[peer] - 1;
            AppendEntriesArgs args;
            args.term = leader.currentTerm;
            args.leaderId = leader.id;
            args.prevLogIndex = prev;
            args.prevLogTerm = leader.termAt(prev);
            args.leaderCommit = leader.commitIndex;
            for (int i = leader.nextIndex[peer]; i <= leader.lastLogIndex(); ++i)
                args.entries.push_back(leader.log[i]);
            send(leader.id, peer, args);
        }
    }

    void deliver(const Message& msg) {
        Node& to = nodes_[msg.to];
        if (!to.alive) return;
        std::visit([&](const auto& body) { handle(to, msg.from, body); }, msg.body);
    }

    void handle(Node& node, int from, const RequestVoteArgs& args) {
        if (args.term > node.currentTerm) stepDown(node, args.term);
        bool grant = false;
        if (args.term >= node.currentTerm) {
            bool logOk = args.lastLogTerm > node.termAt(node.lastLogIndex()) ||
                         (args.lastLogTerm == node.termAt(node.lastLogIndex()) &&
                          args.lastLogIndex >= node.lastLogIndex());
            if ((node.votedFor == -1 || node.votedFor == args.candidateId) && logOk) {
                grant = true;
                node.votedFor = args.candidateId;
                resetElectionTimer(node);
            }
        }
        send(node.id, from, RequestVoteReply{node.currentTerm, grant});
    }

    void handle(Node& node, int from, const RequestVoteReply& reply) {
        if (reply.term > node.currentTerm) {
            stepDown(node, reply.term);
            return;
        }
        if (node.role != Role::Candidate || reply.term != node.currentTerm) return;
        if (reply.voteGranted && ++node.votesGranted >= majority()) becomeLeader(node);
        (void)from;
    }

    void handle(Node& node, int from, const AppendEntriesArgs& args) {
        if (args.term < node.currentTerm) {
            send(node.id, from, AppendEntriesReply{node.currentTerm, false, -1});
            return;
        }
        // Valid leader for this term: (re)establish follower state and back off elections.
        if (args.term > node.currentTerm) node.currentTerm = args.term;
        node.role = Role::Follower;
        node.votedFor = (node.votedFor == -1 ? from : node.votedFor);
        resetElectionTimer(node);

        // Log consistency check at prevLogIndex.
        if (args.prevLogIndex >= 0 && (args.prevLogIndex > node.lastLogIndex() ||
                                       node.termAt(args.prevLogIndex) != args.prevLogTerm)) {
            send(node.id, from, AppendEntriesReply{node.currentTerm, false, -1});
            return;
        }

        // Append, truncating only on a real conflict (never blindly overwrite).
        int index = args.prevLogIndex;
        for (const auto& entry : args.entries) {
            ++index;
            if (index <= node.lastLogIndex()) {
                if (node.termAt(index) != entry.term) {
                    node.log.resize(index);
                    node.log.push_back(entry);
                }
            } else {
                node.log.push_back(entry);
            }
        }
        int lastNew = args.prevLogIndex + static_cast<int>(args.entries.size());
        if (args.leaderCommit > node.commitIndex)
            node.commitIndex = std::min(args.leaderCommit, lastNew);
        applyCommitted(node);
        send(node.id, from, AppendEntriesReply{node.currentTerm, true, lastNew});
    }

    void handle(Node& node, int from, const AppendEntriesReply& reply) {
        if (reply.term > node.currentTerm) {
            stepDown(node, reply.term);
            return;
        }
        if (node.role != Role::Leader || reply.term != node.currentTerm) return;
        if (reply.success) {
            node.matchIndex[from] = std::max(node.matchIndex[from], reply.matchIndex);
            node.nextIndex[from] = node.matchIndex[from] + 1;
            advanceCommit(node);
        } else {
            node.nextIndex[from] = std::max(0, node.nextIndex[from] - 1);
        }
    }

    void advanceCommit(Node& leader) {
        for (int idx = leader.lastLogIndex(); idx > leader.commitIndex; --idx) {
            // Raft only commits an entry from the current term by counting replicas.
            if (leader.termAt(idx) != leader.currentTerm) continue;
            int replicas = 0;
            for (int peer = 0; peer < n_; ++peer)
                if (leader.matchIndex[peer] >= idx) ++replicas;
            if (replicas >= majority()) {
                leader.commitIndex = idx;
                applyCommitted(leader);
                break;
            }
        }
    }

    void applyCommitted(Node& node) {
        while (node.lastApplied < node.commitIndex) {
            ++node.lastApplied;
            const LogEntry& entry = node.log[node.lastApplied];
            node.store.apply(entry);
            recordCommitted(node.lastApplied, entry);
        }
    }

    void replayCommittedAfterRestart(Node& node) {
        // Reconstruct the state machine from the (persistent) log up to what we can prove
        // committed; conservatively replay nothing beyond the known committed prefix.
        for (int i = 0; i < static_cast<int>(committedLog_.size()) && i <= node.lastLogIndex();
             ++i) {
            node.store.apply(node.log[i]);
            node.lastApplied = i;
            node.commitIndex = i;
        }
    }

    // ---- network ----
    template <typename Body>
    void send(int from, int to, const Body& body) {
        if (!nodes_[from].alive) return;
        if (!reachable_[from][to]) return;  // partitioned
        std::uniform_real_distribution<double> coin(0.0, 1.0);
        if (coin(rng_) < cfg_.dropRate) return;
        enqueue(from, to, body);
        if (coin(rng_) < cfg_.dupRate) enqueue(from, to, body);  // duplicate delivery
    }
    template <typename Body>
    void enqueue(int from, int to, const Body& body) {
        std::uniform_int_distribution<std::int64_t> dist(cfg_.delayMin, cfg_.delayMax);
        wire_.insert(Message{from, to, body, now_ + dist(rng_), seq_++});
    }

    int majority() const { return n_ / 2 + 1; }

    // ---- safety invariants (checked after every step) ----
    void recordLeader(int term, int id) {
        auto it = leaderByTerm_.find(term);
        // Election Safety: at most one leader per term.
        if (it != leaderByTerm_.end() && it->second != id)
            fail("election safety: two leaders in term " + std::to_string(term));
        leaderByTerm_[term] = id;
    }
    void recordCommitted(int index, const LogEntry& entry) {
        if (index < static_cast<int>(committedLog_.size())) {
            // State Machine Safety / Leader Completeness: a committed index never changes.
            if (!(committedLog_[index] == entry))
                fail("state machine safety: committed index " + std::to_string(index) +
                     " diverged");
        } else if (index == static_cast<int>(committedLog_.size())) {
            committedLog_.push_back(entry);
        }
        // index > size can't happen: entries commit in order.
    }
    void checkInvariants() {
        // Log Matching: if two logs share the same term at an index, all prior entries match.
        for (int a = 0; a < n_; ++a)
            for (int b = a + 1; b < n_; ++b) {
                int lim = std::min(nodes_[a].lastLogIndex(), nodes_[b].lastLogIndex());
                for (int i = 0; i <= lim; ++i)
                    if (nodes_[a].termAt(i) == nodes_[b].termAt(i) &&
                        !(nodes_[a].log[i] == nodes_[b].log[i]))
                        fail("log matching: index " + std::to_string(i) +
                             " differs for equal term");
            }
    }
    [[noreturn]] void fail(const std::string& why) {
        throw std::logic_error("Raft invariant violated: " + why);
    }

    int n_;
    Config cfg_;
    std::mt19937_64 rng_;
    std::int64_t now_ = 0;
    std::int64_t seq_ = 0;
    std::vector<Node> nodes_;
    std::vector<std::vector<char>> reachable_;

    struct ByTime {
        bool operator()(const Message& a, const Message& b) const {
            if (a.deliverAt != b.deliverAt) return a.deliverAt < b.deliverAt;
            return a.seq < b.seq;
        }
    };
    std::multiset<Message, ByTime> wire_;

    std::map<int, int> leaderByTerm_;     // term -> leader id (election safety)
    std::vector<LogEntry> committedLog_;  // agreed committed prefix (state machine safety)
};

}  // namespace raft_sim
