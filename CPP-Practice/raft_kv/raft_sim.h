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
//
// Productionization on top of the core simulation:
//   - Persistence via Storage (raft_storage.h): currentTerm/votedFor/log/snapshot are written
//     through a node's Storage on every change. crash() drops ALL volatile state; restart()
//     rebuilds the node purely from Storage::load() — no magic retention — so a bug that
//     forgets to persist something is exposed as lost state after restart.
//   - Snapshot + log compaction: a node snapshots its applied prefix and truncates the log;
//     lastIncludedIndex/lastIncludedTerm make all indexing snapshot-aware. A leader whose log
//     no longer covers a follower's nextIndex ships an InstallSnapshot instead of AppendEntries.
//   - ReadIndex: a linearizable read confirms leadership with a heartbeat round before serving,
//     so a stale (partitioned) leader cannot answer with old data — it returns "unavailable".
//   - Pre-Vote: a candidate first runs a term-less pre-election; only a pre-vote majority lets
//     it bump its term. A partitioned node rejoining cannot disturb a healthy leader.

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "raft_storage.h"

namespace raft_sim {

enum class Role { Follower, Candidate, Leader };

// LogEntry lives in raft_storage.h (shared with the persistence layer).

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
    void restore(std::map<std::string, std::string> data) { data_ = std::move(data); }

    // Deterministic snapshot (map iteration is ordered): serialize for Storage / InstallSnapshot.
    std::string serialize() const {
        std::string out;
        detail::PutU32(&out, static_cast<std::uint32_t>(data_.size()));
        for (const auto& [k, v] : data_) {
            detail::PutStr(&out, k);
            detail::PutStr(&out, v);
        }
        return out;
    }
    static std::map<std::string, std::string> deserialize(const std::string& bytes) {
        std::map<std::string, std::string> data;
        std::size_t pos = 0;
        std::uint32_t count = 0;
        if (!detail::GetU32(bytes, &pos, &count)) return data;
        for (std::uint32_t i = 0; i < count; ++i) {
            std::string k, v;
            if (!detail::GetStr(bytes, &pos, &k)) break;
            if (!detail::GetStr(bytes, &pos, &v)) break;
            data.emplace(std::move(k), std::move(v));
        }
        return data;
    }

 private:
    std::map<std::string, std::string> data_;
};

// --- RPC payloads -----------------------------------------------------------

struct RequestVoteArgs {
    int term;
    int candidateId;
    int lastLogIndex;
    int lastLogTerm;
    bool preVote = false;  // true: term-less pre-election probe (does not bind the voter)
};
struct RequestVoteReply {
    int term;
    bool voteGranted;
    bool preVote = false;
};
struct AppendEntriesArgs {
    int term;
    int leaderId;
    int prevLogIndex;
    int prevLogTerm;
    int leaderCommit;
    std::vector<LogEntry> entries;
    int readId = 0;  // non-zero: this heartbeat also confirms leadership for a ReadIndex round
};
struct AppendEntriesReply {
    int term;
    bool success;
    int matchIndex;  // on success: last index the follower now agrees on
    int readId = 0;  // echoes the args.readId it confirms
};
struct InstallSnapshotArgs {
    int term;
    int leaderId;
    int lastIncludedIndex;
    int lastIncludedTerm;
    std::string data;  // serialized state machine
};
struct InstallSnapshotReply {
    int term;
    int matchIndex;  // follower's lastIncludedIndex after install
};

struct Message {
    int from;
    int to;
    std::variant<RequestVoteArgs, RequestVoteReply, AppendEntriesArgs, AppendEntriesReply,
                 InstallSnapshotArgs, InstallSnapshotReply>
        body;
    std::int64_t deliverAt;
    std::int64_t seq;  // stable tiebreak so ordering is deterministic
};

// --- per-node state ---------------------------------------------------------

struct Node {
    explicit Node(int node_id, std::shared_ptr<Storage> stg)
        : id(node_id), storage(std::move(stg)) {}

    int id;
    std::shared_ptr<Storage> storage;

    // ---- volatile state (lost on crash) ----
    bool alive = true;
    Role role = Role::Follower;
    int commitIndex = -1;
    int lastApplied = -1;
    KVStore store;
    std::vector<int> nextIndex;   // leader-only
    std::vector<int> matchIndex;  // leader-only
    int votesGranted = 0;
    int preVotesGranted = 0;
    std::int64_t electionDeadline = 0;
    std::int64_t heartbeatDeadline = 0;
    std::int64_t lastLeaderContact = 0;  // last valid AppendEntries/InstallSnapshot time

    // ReadIndex bookkeeping (leader-only, volatile)
    int currentReadId = 0;
    std::vector<char> readAckedBy;  // distinct peers that acked the current read round
    int readIndexValue = -1;
    bool readConfirmed = false;

    // ---- persistent state (mirrored to storage) ----
    int currentTerm = 0;
    int votedFor = -1;
    std::vector<LogEntry> log;   // entries at logical indices [firstIndex()..lastLogIndex()]
    int lastIncludedIndex = -1;  // snapshot boundary; -1 = no snapshot
    int lastIncludedTerm = 0;

    // ---- snapshot-aware indexing ----
    int firstIndex() const { return lastIncludedIndex + 1; }  // first index held in `log`
    int lastLogIndex() const { return lastIncludedIndex + static_cast<int>(log.size()); }
    bool hasEntry(int index) const { return index >= firstIndex() && index <= lastLogIndex(); }
    const LogEntry& entryAt(int index) const { return log[index - firstIndex()]; }
    int termAt(int index) const {
        if (index == lastIncludedIndex) return lastIncludedTerm;
        if (index < firstIndex() || index > lastLogIndex()) return 0;
        return log[index - firstIndex()].term;
    }
};

// --- timing / fault configuration ------------------------------------------

struct Config {
    std::int64_t electionMin = 50;
    std::int64_t electionMax = 100;
    std::int64_t heartbeat = 15;
    std::int64_t delayMin = 2;
    std::int64_t delayMax = 8;
    double dropRate = 0.0;      // per-message, on top of partitions
    double dupRate = 0.0;       // chance a delivered message is duplicated
    int snapshotThreshold = 0;  // applied entries beyond snapshot before compacting (0 = off)
    bool preVote = false;       // run a pre-election before bumping term
};

// Factory so each node gets its own Storage (e.g. a per-node file). Null ⇒ MemStorage.
using StorageFactory = std::function<std::shared_ptr<Storage>(int nodeId)>;

// --- the cluster / simulation driver ---------------------------------------

class Cluster {
 public:
    Cluster(int count, std::uint64_t seed, Config cfg = {}, StorageFactory factory = nullptr)
        : n_(count), cfg_(cfg), rng_(seed), factory_(std::move(factory)) {
        reachable_.assign(n_, std::vector<char>(n_, 1));
        for (int i = 0; i < n_; ++i) nodes_.emplace_back(i, makeStorage(i));
        for (auto& node : nodes_) {
            // A node may already have durable state (e.g. FileStorage pointed at a populated
            // dir); load it so a "fresh" Cluster over existing files behaves like a reboot.
            loadFromStorage(node);
            resetElectionTimer(node);
        }
    }

    // ---- fault injection knobs (all deterministic given the seed) ----
    void crash(int id) {
        // Lose ALL volatile state. Only what reached Storage survives; restart() proves it.
        Node& node = nodes_.at(id);
        std::shared_ptr<Storage> stg = node.storage;
        node = Node(id, stg);
        node.alive = false;
    }
    void restart(int id) {
        Node& node = nodes_.at(id);
        std::shared_ptr<Storage> stg = node.storage;
        node = Node(id, stg);  // volatile state starts empty
        node.alive = true;
        loadFromStorage(node);  // rebuild persistent state + state machine from disk
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
        persistLog(node);
        return node.lastLogIndex();
    }

    // ---- linearizable read (ReadIndex) ----
    // Start a read round on the presumed leader: snapshot commitIndex and fire a heartbeat that
    // a majority must ack (proving no newer leader exists). Returns the readIndex, or -1.
    int requestRead(int leaderId) {
        if (leaderId < 0) return -1;
        Node& l = nodes_.at(leaderId);
        if (!l.alive || l.role != Role::Leader) return -1;
        ++l.currentReadId;
        l.readIndexValue = l.commitIndex;
        l.readAckedBy.assign(n_, 0);
        l.readAckedBy[l.id] = 1;  // counts self
        l.readConfirmed = (majority() == 1);
        broadcastAppendEntries(l);  // stamps currentReadId into the heartbeats
        return l.readIndexValue;
    }
    // The read may be served once leadership is confirmed AND the state machine has caught up to
    // the readIndex. A leader that lost quorum never confirms → the read stays unavailable.
    bool readReady(int leaderId, int readIndex) const {
        if (leaderId < 0) return false;
        const Node& l = nodes_.at(leaderId);
        return l.alive && l.role == Role::Leader && l.readConfirmed && l.lastApplied >= readIndex;
    }
    // Convenience: run a ReadIndex round to completion and return the value, or nullopt if the
    // node could not confirm leadership within the budget (stale/partitioned leader).
    std::optional<std::string> linearizableGet(int leaderId, const std::string& key,
                                               std::int64_t maxMs) {
        int readIndex = requestRead(leaderId);
        if (readIndex < 0) return std::nullopt;
        if (!runUntil([&] { return readReady(leaderId, readIndex); }, maxMs)) return std::nullopt;
        return nodes_.at(leaderId).store.get(key);
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
    std::shared_ptr<Storage> makeStorage(int id) {
        return factory_ ? factory_(id) : std::make_shared<MemStorage>();
    }

    // ---- persistence helpers ----
    void persistHardState(Node& node) {
        node.storage->saveHardState(node.currentTerm, node.votedFor);
    }
    void persistLog(Node& node) { node.storage->saveLog(node.log); }
    void loadFromStorage(Node& node) {
        PersistentState ps = node.storage->load();
        node.currentTerm = ps.currentTerm;
        node.votedFor = ps.votedFor;
        node.lastIncludedIndex = ps.lastIncludedIndex;
        node.lastIncludedTerm = ps.lastIncludedTerm;
        node.log = ps.log;
        // Snapshot is applied state by definition; volatile commit/apply resume from its boundary
        // and are re-advanced by the leader's AppendEntries after rejoining.
        node.store.restore(KVStore::deserialize(ps.snapshot));
        node.commitIndex = ps.lastIncludedIndex;
        node.lastApplied = ps.lastIncludedIndex;
    }

    // ---- core stepping ----
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
                onElectionTimeout(node);
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
        node.readConfirmed = false;
        persistHardState(node);
        resetElectionTimer(node);
    }

    // On timeout: with Pre-Vote enabled, first run a term-less probe; otherwise elect directly.
    void onElectionTimeout(Node& node) {
        if (cfg_.preVote)
            startPreVote(node);
        else
            startElection(node);
    }

    // Pre-Vote: ask peers "would you grant a vote in term+1?" WITHOUT bumping our term or vote.
    // Only a pre-vote majority triggers the real, term-incrementing election. This is what stops
    // a partitioned node (whose term ran far ahead in isolation) from disrupting a healthy leader
    // on reconnect: healthy followers still hearing heartbeats reject the pre-vote.
    void startPreVote(Node& node) {
        node.role = Role::Candidate;
        node.preVotesGranted = 1;  // votes for itself, hypothetically
        resetElectionTimer(node);
        RequestVoteArgs args{node.currentTerm + 1, node.id, node.lastLogIndex(),
                             node.termAt(node.lastLogIndex()), /*preVote=*/true};
        for (int peer = 0; peer < n_; ++peer)
            if (peer != node.id) send(node.id, peer, args);
    }

    void startElection(Node& node) {
        node.role = Role::Candidate;
        ++node.currentTerm;
        node.votedFor = node.id;
        node.votesGranted = 1;
        persistHardState(node);
        resetElectionTimer(node);
        RequestVoteArgs args{node.currentTerm, node.id, node.lastLogIndex(),
                             node.termAt(node.lastLogIndex()), /*preVote=*/false};
        for (int peer = 0; peer < n_; ++peer)
            if (peer != node.id) send(node.id, peer, args);
    }

    void becomeLeader(Node& node) {
        node.role = Role::Leader;
        node.nextIndex.assign(n_, node.lastLogIndex() + 1);
        node.matchIndex.assign(n_, -1);
        node.matchIndex[node.id] = node.lastLogIndex();
        node.readConfirmed = false;
        recordLeader(node.currentTerm, node.id);
        // A fresh leader commits nothing from prior terms until it commits one of its own
        // entries (Raft's no-op / current-term rule); append a no-op to make progress.
        node.log.push_back(LogEntry{node.currentTerm, "noop", "", ""});
        node.matchIndex[node.id] = node.lastLogIndex();
        persistLog(node);
        node.heartbeatDeadline = now_;  // send immediately
    }

    void broadcastAppendEntries(Node& leader) {
        leader.heartbeatDeadline = now_ + cfg_.heartbeat;
        for (int peer = 0; peer < n_; ++peer) {
            if (peer == leader.id) continue;
            // If the follower needs entries we've already compacted into our snapshot, ship the
            // snapshot instead of AppendEntries.
            if (leader.nextIndex[peer] <= leader.lastIncludedIndex) {
                sendInstallSnapshot(leader, peer);
                continue;
            }
            int prev = leader.nextIndex[peer] - 1;
            AppendEntriesArgs args;
            args.term = leader.currentTerm;
            args.leaderId = leader.id;
            args.prevLogIndex = prev;
            args.prevLogTerm = leader.termAt(prev);
            args.leaderCommit = leader.commitIndex;
            for (int i = leader.nextIndex[peer]; i <= leader.lastLogIndex(); ++i)
                args.entries.push_back(leader.entryAt(i));
            args.readId = leader.readConfirmed ? 0 : leader.currentReadId;
            send(leader.id, peer, args);
        }
    }

    void sendInstallSnapshot(Node& leader, int peer) {
        InstallSnapshotArgs args;
        args.term = leader.currentTerm;
        args.leaderId = leader.id;
        args.lastIncludedIndex = leader.lastIncludedIndex;
        args.lastIncludedTerm = leader.lastIncludedTerm;
        args.data = leader.store.serialize();
        send(leader.id, peer, args);
    }

    void deliver(const Message& msg) {
        Node& to = nodes_[msg.to];
        if (!to.alive) return;
        std::visit([&](const auto& body) { handle(to, msg.from, body); }, msg.body);
    }

    void handle(Node& node, int from, const RequestVoteArgs& args) {
        if (args.preVote) {
            handlePreVote(node, from, args);
            return;
        }
        if (args.term > node.currentTerm) stepDown(node, args.term);
        bool grant = false;
        if (args.term >= node.currentTerm) {
            bool logOk = args.lastLogTerm > node.termAt(node.lastLogIndex()) ||
                         (args.lastLogTerm == node.termAt(node.lastLogIndex()) &&
                          args.lastLogIndex >= node.lastLogIndex());
            if ((node.votedFor == -1 || node.votedFor == args.candidateId) && logOk) {
                grant = true;
                node.votedFor = args.candidateId;
                persistHardState(node);
                resetElectionTimer(node);
            }
        }
        send(node.id, from, RequestVoteReply{node.currentTerm, grant, /*preVote=*/false});
    }

    // Pre-vote grant is advisory: it does NOT bump currentTerm or set votedFor. Grant only if the
    // candidate's log is at least as up-to-date AND we have not heard from a leader recently (so a
    // node inside a healthy majority never helps a would-be disruptor cross the pre-vote bar).
    void handlePreVote(Node& node, int from, const RequestVoteArgs& args) {
        bool leaderStale = (now_ - node.lastLeaderContact) >= cfg_.electionMin;
        bool logOk = args.lastLogTerm > node.termAt(node.lastLogIndex()) ||
                     (args.lastLogTerm == node.termAt(node.lastLogIndex()) &&
                      args.lastLogIndex >= node.lastLogIndex());
        bool grant = leaderStale && logOk && args.term > node.currentTerm;
        send(node.id, from, RequestVoteReply{node.currentTerm, grant, /*preVote=*/true});
    }

    void handle(Node& node, int from, const RequestVoteReply& reply) {
        if (reply.preVote) {
            if (reply.term > node.currentTerm) {
                stepDown(node, reply.term);
                return;
            }
            if (node.role != Role::Candidate) return;
            if (reply.voteGranted && ++node.preVotesGranted >= majority())
                startElection(node);  // pre-vote passed → real, term-incrementing election
            (void)from;
            return;
        }
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
            send(node.id, from, AppendEntriesReply{node.currentTerm, false, -1, args.readId});
            return;
        }
        // Valid leader for this term: (re)establish follower state and back off elections.
        if (args.term > node.currentTerm) {
            node.currentTerm = args.term;
            node.votedFor = -1;
            persistHardState(node);
        }
        node.role = Role::Follower;
        node.lastLeaderContact = now_;
        resetElectionTimer(node);

        // Log consistency check at prevLogIndex (only meaningful at/after our snapshot boundary;
        // anything below lastIncludedIndex is committed and matches by Log Matching).
        if (args.prevLogIndex >= node.lastIncludedIndex &&
            (args.prevLogIndex > node.lastLogIndex() ||
             node.termAt(args.prevLogIndex) != args.prevLogTerm)) {
            send(node.id, from, AppendEntriesReply{node.currentTerm, false, -1, args.readId});
            return;
        }

        // Append, truncating only on a real conflict (never blindly overwrite). Skip any entries
        // already covered by our snapshot.
        bool changed = false;
        int index = args.prevLogIndex;
        for (const auto& entry : args.entries) {
            ++index;
            if (index < node.firstIndex()) continue;  // predates our snapshot; already durable
            if (index <= node.lastLogIndex()) {
                if (node.termAt(index) != entry.term) {
                    node.log.resize(index - node.firstIndex());
                    node.log.push_back(entry);
                    changed = true;
                }
            } else {
                node.log.push_back(entry);
                changed = true;
            }
        }
        if (changed) persistLog(node);

        int lastNew = args.prevLogIndex + static_cast<int>(args.entries.size());
        if (args.leaderCommit > node.commitIndex)
            node.commitIndex = std::min(args.leaderCommit, lastNew);
        node.commitIndex = std::max(node.commitIndex, node.lastIncludedIndex);
        applyCommitted(node);
        maybeSnapshot(node);
        send(node.id, from,
             AppendEntriesReply{node.currentTerm, true, std::max(lastNew, node.lastIncludedIndex),
                                args.readId});
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
            // ReadIndex: confirm leadership once a MAJORITY of DISTINCT peers ack this round.
            if (reply.readId != 0 && reply.readId == node.currentReadId && !node.readConfirmed) {
                if (from >= 0 && from < static_cast<int>(node.readAckedBy.size()))
                    node.readAckedBy[from] = 1;
                int acks = 0;
                for (char c : node.readAckedBy)
                    if (c) ++acks;
                if (acks >= majority()) node.readConfirmed = true;
            }
            advanceCommit(node);
        } else {
            node.nextIndex[from] = std::max(0, node.nextIndex[from] - 1);
        }
    }

    void handle(Node& node, int from, const InstallSnapshotArgs& args) {
        if (args.term < node.currentTerm) {
            send(node.id, from, InstallSnapshotReply{node.currentTerm, -1});
            return;
        }
        if (args.term > node.currentTerm) {
            node.currentTerm = args.term;
            node.votedFor = -1;
            persistHardState(node);
        }
        node.role = Role::Follower;
        node.lastLeaderContact = now_;
        resetElectionTimer(node);

        if (args.lastIncludedIndex <= node.lastIncludedIndex) {
            // We already have this snapshot (or better).
            send(node.id, from, InstallSnapshotReply{node.currentTerm, node.lastIncludedIndex});
            return;
        }

        // Retain any log suffix that is consistent with the snapshot point; otherwise discard all.
        if (node.hasEntry(args.lastIncludedIndex) &&
            node.termAt(args.lastIncludedIndex) == args.lastIncludedTerm) {
            std::vector<LogEntry> keep;
            for (int i = args.lastIncludedIndex + 1; i <= node.lastLogIndex(); ++i)
                keep.push_back(node.entryAt(i));
            node.log = std::move(keep);
        } else {
            node.log.clear();
        }
        node.lastIncludedIndex = args.lastIncludedIndex;
        node.lastIncludedTerm = args.lastIncludedTerm;
        node.store.restore(KVStore::deserialize(args.data));
        node.commitIndex = std::max(node.commitIndex, node.lastIncludedIndex);
        node.lastApplied = std::max(node.lastApplied, node.lastIncludedIndex);
        node.storage->saveSnapshot(node.lastIncludedIndex, node.lastIncludedTerm,
                                   node.store.serialize(), node.log);
        send(node.id, from, InstallSnapshotReply{node.currentTerm, node.lastIncludedIndex});
    }

    void handle(Node& node, int from, const InstallSnapshotReply& reply) {
        if (reply.term > node.currentTerm) {
            stepDown(node, reply.term);
            return;
        }
        if (node.role != Role::Leader || reply.term != node.currentTerm) return;
        if (reply.matchIndex >= 0) {
            node.matchIndex[from] = std::max(node.matchIndex[from], reply.matchIndex);
            node.nextIndex[from] = node.matchIndex[from] + 1;
            advanceCommit(node);
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
                maybeSnapshot(leader);
                break;
            }
        }
    }

    void applyCommitted(Node& node) {
        while (node.lastApplied < node.commitIndex) {
            ++node.lastApplied;
            const LogEntry& entry = node.entryAt(node.lastApplied);
            node.store.apply(entry);
            recordCommitted(node.lastApplied, entry);
        }
    }

    // Compact the applied prefix into a snapshot and truncate the log, bounding its growth. Only
    // applied (hence committed) entries are snapshotted, so the state machine image is safe.
    void maybeSnapshot(Node& node) {
        if (cfg_.snapshotThreshold <= 0) return;
        if (node.lastApplied - node.lastIncludedIndex < cfg_.snapshotThreshold) return;
        int newLast = node.lastApplied;
        int newTerm = node.termAt(newLast);
        int drop = newLast - node.lastIncludedIndex;  // entries to remove from the front
        if (drop <= 0) return;
        node.log.erase(node.log.begin(), node.log.begin() + drop);
        node.lastIncludedIndex = newLast;
        node.lastIncludedTerm = newTerm;
        node.storage->saveSnapshot(node.lastIncludedIndex, node.lastIncludedTerm,
                                   node.store.serialize(), node.log);
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
        // Only compare indices both nodes still hold (snapshots may have compacted a prefix).
        for (int a = 0; a < n_; ++a)
            for (int b = a + 1; b < n_; ++b) {
                int lo = std::max(nodes_[a].firstIndex(), nodes_[b].firstIndex());
                int hi = std::min(nodes_[a].lastLogIndex(), nodes_[b].lastLogIndex());
                for (int i = lo; i <= hi; ++i)
                    if (nodes_[a].termAt(i) == nodes_[b].termAt(i) &&
                        !(nodes_[a].entryAt(i) == nodes_[b].entryAt(i)))
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
    StorageFactory factory_;
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
