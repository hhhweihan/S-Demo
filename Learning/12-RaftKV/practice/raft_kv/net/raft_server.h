#pragma once

// Single-node real Raft server: the same algorithm raft_sim.h's Cluster drives from a god's-eye
// view, but here each RaftServer instance IS one node's entire world — it only ever sees bytes
// arriving on real TCP sockets and real timer expirations, never another node's memory. Run one
// instance per OS process (see raft_net_main.cpp) and the cluster is genuinely distributed on
// 127.0.0.1: a real `kill -9` on one process is indistinguishable, from the survivors' point of
// view, from a real crash in production.
//
// Connection topology: exactly one TCP connection per node pair, reused for traffic in both
// directions (a request and its reply travel the same socket). To avoid both ends racing to dial
// each other, the lower-id node always dials the higher-id node; the higher-id node only accepts.
// Every frame is self-describing (codec.h's `from` field), so which side originally dialed doesn't
// matter for routing replies — a reply is just another SendTo() on whatever fd is on file for that
// peer id.
//
// What's deliberately simple: PUT acknowledges as soon as the leader appends locally (matching
// raft_sim::Cluster::submit()'s convention) rather than waiting for commit — a client that wants a
// durability guarantee polls STATUS/GET. GET is the one place linearizability is actually enforced,
// via ReadIndex (see TryServePendingReads()).

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "codec.h"
#include "event_loop.h"

namespace raft_net {

struct PeerAddr {
    int id;
    std::string host;
    std::uint16_t port;
};

struct ServerConfig {
    int id;
    std::vector<PeerAddr> peers;  // every other node in the cluster, excluding self
    std::uint16_t peerPort;       // this node's listen port for peer-to-peer RPC
    std::uint16_t adminPort;      // this node's listen port for the PUT/GET/STATUS text protocol
    std::shared_ptr<raft_sim::Storage> storage;
    std::chrono::milliseconds electionMin{150};
    std::chrono::milliseconds electionMax{300};
    std::chrono::milliseconds heartbeat{50};
    std::chrono::milliseconds reconnectBackoff{300};
    bool preVote = true;
};

class RaftServer {
 public:
    explicit RaftServer(ServerConfig cfg) : cfg_(std::move(cfg)), rng_(MakeSeed()) {
        for (const auto& p : cfg_.peers) peerIds_.push_back(p.id);
        n_ = static_cast<int>(peerIds_.size()) + 1;

        LoadFromStorage();

        listenPeerFd_ = SetupListenSocket(cfg_.peerPort);
        loop_.Add(listenPeerFd_, EPOLLIN, [this](std::uint32_t) { OnPeerListenReadable(); });
        listenAdminFd_ = SetupListenSocket(cfg_.adminPort);
        loop_.Add(listenAdminFd_, EPOLLIN, [this](std::uint32_t) { OnAdminListenReadable(); });

        electionTimerFd_ = loop_.CreateTimer([this] { OnElectionTimeout(); });
        heartbeatTimerFd_ = loop_.CreateTimer([this] { BroadcastAppendEntries(); });
        ResetElectionTimer();

        // Lower id dials higher id; the higher id side only ever accepts (see class comment).
        for (const auto& p : cfg_.peers) {
            if (cfg_.id < p.id) {
                reconnectTimerFd_[p.id] = loop_.CreateTimer([this, id = p.id] { ConnectToPeer(id); });
                ConnectToPeer(p.id);
            }
        }
    }

    ~RaftServer() {
        for (auto& [fd, conn] : connsByFd_) (void)conn;
        for (auto& [fd, conn] : connsByFd_) ::close(fd);
        for (auto& [fd, conn] : adminConnsByFd_) ::close(fd);
        ::close(listenPeerFd_);
        ::close(listenAdminFd_);
    }

    RaftServer(const RaftServer&) = delete;
    RaftServer& operator=(const RaftServer&) = delete;

    void Run() { loop_.Run(); }
    void Stop() { loop_.Stop(); }
    EventLoop& Loop() { return loop_; }

    int id() const { return cfg_.id; }
    raft_sim::Role role() const { return role_; }
    int term() const { return currentTerm_; }
    int commitIndex() const { return commitIndex_; }
    int lastApplied() const { return lastApplied_; }
    int leaderHint() const { return lastKnownLeaderId_; }

 private:
    // --- one real TCP connection per peer pair ---------------------------------
    struct Conn {
        int fd = -1;
        std::string readBuf;
        std::string writeBuf;
        bool writableArmed = false;
    };
    struct AdminConn {
        int fd = -1;
        std::string readBuf;
    };
    struct PendingRead {
        int adminFd;
        std::string key;
        int readIndex;
        int readId;
    };

    // --- socket setup -----------------------------------------------------------
    static void MakeNonBlocking(int fd) {
        int flags = ::fcntl(fd, F_GETFL, 0);
        ::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
    static int SetupListenSocket(std::uint16_t port) {
        int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        int one = 1;
        ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr.sin_port = htons(port);
        if (::bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
            throw std::runtime_error(std::string("bind: ") + std::strerror(errno));
        ::listen(fd, 16);
        MakeNonBlocking(fd);
        return fd;
    }
    static void SetNoDelay(int fd) {
        int one = 1;
        ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }
    static std::uint64_t MakeSeed() {
        // Real entropy, not a fixed seed: unlike raft_sim's reproducible chaos tests, a real node's
        // election-timeout randomization must actually differ across processes and restarts.
        std::random_device rd;
        return (static_cast<std::uint64_t>(rd()) << 32) ^ static_cast<std::uint64_t>(rd());
    }

    const PeerAddr& AddrOf(int peerId) const {
        for (const auto& p : cfg_.peers)
            if (p.id == peerId) return p;
        throw std::logic_error("unknown peer id");
    }

    // --- peer connection lifecycle -----------------------------------------------

    void ConnectToPeer(int peerId) {
        const PeerAddr& addr = AddrOf(peerId);
        int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        MakeNonBlocking(fd);
        SetNoDelay(fd);
        sockaddr_in sa{};
        sa.sin_family = AF_INET;
        sa.sin_port = htons(addr.port);
        ::inet_pton(AF_INET, addr.host.c_str(), &sa.sin_addr);
        int rc = ::connect(fd, reinterpret_cast<sockaddr*>(&sa), sizeof(sa));
        if (rc == 0) {
            EstablishConn(peerId, fd);
            return;
        }
        if (errno != EINPROGRESS) {
            ::close(fd);
            ScheduleReconnect(peerId);
            return;
        }
        // Non-blocking connect in flight: wait for EPOLLOUT, then check SO_ERROR.
        loop_.Add(fd, EPOLLOUT, [this, peerId, fd](std::uint32_t) { OnConnectComplete(peerId, fd); });
    }

    void OnConnectComplete(int peerId, int fd) {
        int err = 0;
        socklen_t len = sizeof(err);
        ::getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &len);
        loop_.Remove(fd);
        if (err != 0) {
            ::close(fd);
            ScheduleReconnect(peerId);
            return;
        }
        EstablishConn(peerId, fd);
    }

    void EstablishConn(int peerId, int fd) {
        Conn conn;
        conn.fd = fd;
        connsByFd_[fd] = std::move(conn);
        peerIdToFd_[peerId] = fd;
        loop_.Add(fd, EPOLLIN, [this, fd](std::uint32_t ev) { OnPeerFdEvent(fd, ev); });
    }

    void ScheduleReconnect(int peerId) {
        auto it = reconnectTimerFd_.find(peerId);
        if (it != reconnectTimerFd_.end()) loop_.ResetTimer(it->second, cfg_.reconnectBackoff);
    }

    void CloseConn(int fd) {
        auto it = connsByFd_.find(fd);
        if (it == connsByFd_.end()) return;
        loop_.Remove(fd);
        ::close(fd);
        connsByFd_.erase(it);
        for (auto pit = peerIdToFd_.begin(); pit != peerIdToFd_.end();) {
            if (pit->second == fd) {
                int peerId = pit->first;
                pit = peerIdToFd_.erase(pit);
                if (cfg_.id < peerId) ScheduleReconnect(peerId);  // we own re-dialing this peer
            } else {
                ++pit;
            }
        }
    }

    void OnPeerListenReadable() {
        for (;;) {
            int fd = ::accept4(listenPeerFd_, nullptr, nullptr, SOCK_NONBLOCK);
            if (fd < 0) return;  // EAGAIN: drained the backlog for this readiness notification
            SetNoDelay(fd);
            Conn conn;
            conn.fd = fd;
            connsByFd_[fd] = std::move(conn);
            loop_.Add(fd, EPOLLIN, [this, fd](std::uint32_t ev) { OnPeerFdEvent(fd, ev); });
            // Peer id for this fd is unknown until the first frame's `from` field is decoded
            // (OnPeerFdEvent -> dispatch loop registers it into peerIdToFd_ on first use).
        }
    }

    void OnPeerFdEvent(int fd, std::uint32_t events) {
        if (events & (EPOLLIN | EPOLLHUP | EPOLLERR)) OnPeerFdReadable(fd);
        if (events & EPOLLOUT) OnPeerFdWritable(fd);
    }

    void OnPeerFdReadable(int fd) {
        {
            auto it = connsByFd_.find(fd);
            if (it == connsByFd_.end()) return;
            Conn& conn = it->second;
            char buf[4096];
            for (;;) {
                ssize_t n = ::recv(fd, buf, sizeof(buf), 0);
                if (n > 0) {
                    conn.readBuf.append(buf, static_cast<std::size_t>(n));
                    continue;
                }
                if (n == 0) {
                    CloseConn(fd);
                    return;
                }
                if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                if (errno == EINTR) continue;
                CloseConn(fd);
                return;
            }
        }
        // Dispatch decoded frames. OnFrame() can synchronously tear down THIS connection — a
        // reply's FlushConn() hitting a send() error calls CloseConn(fd), erasing `conn` from
        // connsByFd_ — so re-find the conn each iteration and bail if it's gone rather than
        // holding a reference across the dispatch (which would be use-after-free). Erase the
        // consumed bytes BEFORE dispatching, while the reference is still known-valid.
        for (;;) {
            auto it = connsByFd_.find(fd);
            if (it == connsByFd_.end()) return;  // torn down by a nested dispatch
            Conn& conn = it->second;
            std::size_t consumed = 0;
            bool oversize = false;
            auto frame = TryDecodeFrame(conn.readBuf, &consumed, &oversize);
            if (oversize) {  // bogus huge length prefix — drop the peer instead of buffering it
                CloseConn(fd);
                return;
            }
            if (!frame) break;
            conn.readBuf.erase(0, consumed);
            peerIdToFd_[frame->from] = fd;  // learn/refresh routing for replies to this peer
            OnFrame(*frame);
        }
    }

    void OnPeerFdWritable(int fd) {
        auto it = connsByFd_.find(fd);
        if (it == connsByFd_.end()) return;
        FlushConn(it->second);
    }

    void FlushConn(Conn& conn) {
        while (!conn.writeBuf.empty()) {
            ssize_t n = ::send(conn.fd, conn.writeBuf.data(), conn.writeBuf.size(), 0);
            if (n > 0) {
                conn.writeBuf.erase(0, static_cast<std::size_t>(n));
                continue;
            }
            if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
            if (n < 0 && errno == EINTR) continue;
            CloseConn(conn.fd);
            return;
        }
        bool needWritable = !conn.writeBuf.empty();
        if (needWritable != conn.writableArmed) {
            loop_.Modify(conn.fd, needWritable ? (EPOLLIN | EPOLLOUT) : EPOLLIN);
            conn.writableArmed = needWritable;
        }
    }

    template <typename Body>
    void SendTo(int peerId, MessageType type, const Body& body) {
        auto pit = peerIdToFd_.find(peerId);
        if (pit == peerIdToFd_.end()) return;  // no live connection right now — Raft tolerates loss
        auto cit = connsByFd_.find(pit->second);
        if (cit == connsByFd_.end()) return;
        cit->second.writeBuf += EncodeFrame(cfg_.id, type, body);
        FlushConn(cit->second);
    }

    // --- admin (PUT/GET/STATUS) text protocol ------------------------------------

    void OnAdminListenReadable() {
        for (;;) {
            int fd = ::accept4(listenAdminFd_, nullptr, nullptr, SOCK_NONBLOCK);
            if (fd < 0) return;
            AdminConn conn;
            conn.fd = fd;
            adminConnsByFd_[fd] = conn;
            loop_.Add(fd, EPOLLIN, [this, fd](std::uint32_t) { OnAdminReadable(fd); });
        }
    }

    void OnAdminReadable(int fd) {
        auto it = adminConnsByFd_.find(fd);
        if (it == adminConnsByFd_.end()) return;
        AdminConn& conn = it->second;
        char buf[4096];
        for (;;) {
            ssize_t n = ::recv(fd, buf, sizeof(buf), 0);
            if (n > 0) {
                conn.readBuf.append(buf, static_cast<std::size_t>(n));
                continue;
            }
            if (n == 0 || (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)) {
                loop_.Remove(fd);
                ::close(fd);
                adminConnsByFd_.erase(it);
                return;
            }
            break;
        }
        for (;;) {
            auto pos = conn.readBuf.find('\n');
            if (pos == std::string::npos) break;
            std::string line = conn.readBuf.substr(0, pos);
            conn.readBuf.erase(0, pos + 1);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            HandleAdminLine(fd, line);
        }
    }

    void ReplyAdmin(int fd, const std::string& line) {
        auto it = adminConnsByFd_.find(fd);
        if (it == adminConnsByFd_.end()) return;  // client already gone
        std::string out = line + "\n";
        ::send(fd, out.data(), out.size(), 0);  // best-effort; a full write queue mirrors Conn's
    }

    void HandleAdminLine(int fd, const std::string& line) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd == "STATUS") {
            std::ostringstream oss;
            oss << "ROLE=" << RoleName(role_) << " TERM=" << currentTerm_ << " COMMIT=" << commitIndex_
                << " APPLIED=" << lastApplied_ << " LEADER=" << lastKnownLeaderId_;
            ReplyAdmin(fd, oss.str());
        } else if (cmd == "PUT") {
            std::string key, value;
            iss >> key;
            std::getline(iss, value);
            if (!value.empty() && value.front() == ' ') value.erase(0, 1);
            if (role_ != raft_sim::Role::Leader) {
                ReplyAdmin(fd, "NOT_LEADER " + std::to_string(lastKnownLeaderId_));
                return;
            }
            log_.push_back(raft_sim::LogEntry{currentTerm_, "put", key, value});
            matchIndex_[cfg_.id] = LastLogIndex();
            PersistLog();
            ReplyAdmin(fd, "OK " + std::to_string(LastLogIndex()));
        } else if (cmd == "GET") {
            std::string key;
            iss >> key;
            if (role_ != raft_sim::Role::Leader) {
                ReplyAdmin(fd, "NOT_LEADER " + std::to_string(lastKnownLeaderId_));
                return;
            }
            StartReadIndexFor(fd, key);
        } else {
            ReplyAdmin(fd, "ERR unknown command");
        }
    }

    static const char* RoleName(raft_sim::Role r) {
        switch (r) {
            case raft_sim::Role::Follower:
                return "Follower";
            case raft_sim::Role::Candidate:
                return "Candidate";
            case raft_sim::Role::Leader:
                return "Leader";
        }
        return "?";
    }

    // --- ReadIndex: linearizable GET ---------------------------------------------
    // Mirrors raft_sim::Cluster::requestRead(): snapshot commitIndex, fire an immediate heartbeat
    // round, and only answer once a majority has ack'd it (proving this node is still leader), the
    // current term's no-op is committed (so the state machine reflects everything committed before
    // the read), AND the state machine has applied up to that index. Unlike the simulation's
    // synchronous linearizableGet(), a real GET can't just spin the clock forward — the answer is
    // deferred and delivered from TryServePendingReads() whenever the confirmation actually arrives.
    void StartReadIndexFor(int adminFd, const std::string& key) {
        ++currentReadId_;
        pendingReads_.push_back({adminFd, key, commitIndex_, currentReadId_});
        BroadcastAppendEntries();  // stamps currentReadId_ into this round's AppendEntries
        TryServePendingReads();    // a single-node cluster (Majority()==1) can answer immediately
    }

    // A read round `r` is confirmed once a majority (self + peers) has acked SOME heartbeat carrying
    // readId >= r. readAckedBy_ tracks the highest readId each peer has echoed, so confirmation is
    // monotonic: one qualifying majority reply confirms every still-pending read with a smaller id
    // at once. This is what keeps overlapping GETs from starving each other — an earlier read is no
    // longer stranded just because a later one bumped currentReadId_.
    bool ReadConfirmed(int readId) const {
        int acks = 1;  // self
        for (const auto& [peer, highest] : readAckedBy_)
            if (highest >= readId) ++acks;
        return acks >= Majority();
    }

    void TryServePendingReads() {
        for (auto it = pendingReads_.begin(); it != pendingReads_.end();) {
            // Gate: leadership confirmed, this term's no-op committed (so the state machine reflects
            // every entry committed before the read — no stale read from a leader still catching
            // up), and the SM applied up to the read's snapshotted commitIndex.
            bool ready = ReadConfirmed(it->readId) && commitIndex_ >= firstIndexOfCurrentTerm_ &&
                         lastApplied_ >= it->readIndex;
            if (ready) {
                auto val = store_.get(it->key);
                ReplyAdmin(it->adminFd, val ? ("VALUE " + *val) : std::string("NOT_FOUND"));
                it = pendingReads_.erase(it);
            } else {
                ++it;
            }
        }
    }

    // --- persistence --------------------------------------------------------------
    void PersistHardState() { cfg_.storage->saveHardState(currentTerm_, votedFor_); }
    void PersistLog() { cfg_.storage->saveLog(log_); }
    void LoadFromStorage() {
        raft_sim::PersistentState ps = cfg_.storage->load();
        currentTerm_ = ps.currentTerm;
        votedFor_ = ps.votedFor;
        lastIncludedIndex_ = ps.lastIncludedIndex;
        lastIncludedTerm_ = ps.lastIncludedTerm;
        log_ = ps.log;
        store_.restore(raft_sim::KVStore::deserialize(ps.snapshot));
        commitIndex_ = ps.lastIncludedIndex;
        lastApplied_ = ps.lastIncludedIndex;
    }

    // --- snapshot-aware log indexing (mirrors raft_sim::Node) ----------------------
    int FirstIndex() const { return lastIncludedIndex_ + 1; }
    int LastLogIndex() const { return lastIncludedIndex_ + static_cast<int>(log_.size()); }
    bool HasEntry(int index) const { return index >= FirstIndex() && index <= LastLogIndex(); }
    const raft_sim::LogEntry& EntryAt(int index) const { return log_[static_cast<std::size_t>(index - FirstIndex())]; }
    int TermAt(int index) const {
        if (index == lastIncludedIndex_) return lastIncludedTerm_;
        if (index < FirstIndex() || index > LastLogIndex()) return 0;
        return log_[static_cast<std::size_t>(index - FirstIndex())].term;
    }
    int Majority() const { return n_ / 2 + 1; }

    // --- election timing ------------------------------------------------------------
    void ResetElectionTimer() {
        std::uniform_int_distribution<std::int64_t> dist(cfg_.electionMin.count(), cfg_.electionMax.count());
        loop_.ResetTimer(electionTimerFd_, std::chrono::milliseconds(dist(rng_)));
    }

    void StepDown(int term) {
        currentTerm_ = term;
        role_ = raft_sim::Role::Follower;
        votedFor_ = -1;
        readAckedBy_.clear();
        FailPendingReads();  // we're no longer leader — don't leave GET clients blocked forever
        PersistHardState();
        loop_.CancelTimer(heartbeatTimerFd_);
        ResetElectionTimer();
    }

    void FailPendingReads() {
        for (const auto& p : pendingReads_)
            ReplyAdmin(p.adminFd, "NOT_LEADER " + std::to_string(lastKnownLeaderId_));
        pendingReads_.clear();
    }

    void OnElectionTimeout() {
        if (cfg_.preVote)
            StartPreVote();
        else
            StartElection();
    }

    // Term-less probe: ask peers "would you grant term+1?" without bumping our own term/vote. Only
    // a pre-vote majority lets a real, term-incrementing election start — this is what keeps a node
    // that was partitioned (and free-ran its term up while isolated) from disrupting a healthy
    // leader the moment the partition heals.
    void StartPreVote() {
        role_ = raft_sim::Role::Candidate;
        preVotesGranted_ = 1;
        ResetElectionTimer();
        raft_sim::RequestVoteArgs args{currentTerm_ + 1, cfg_.id, LastLogIndex(), TermAt(LastLogIndex()),
                                       /*preVote=*/true};
        for (int peer : peerIds_) SendTo(peer, MessageType::kRequestVoteArgs, args);
    }

    void StartElection() {
        role_ = raft_sim::Role::Candidate;
        ++currentTerm_;
        votedFor_ = cfg_.id;
        votesGranted_ = 1;
        PersistHardState();
        ResetElectionTimer();
        raft_sim::RequestVoteArgs args{currentTerm_, cfg_.id, LastLogIndex(), TermAt(LastLogIndex()),
                                       /*preVote=*/false};
        for (int peer : peerIds_) SendTo(peer, MessageType::kRequestVoteArgs, args);
    }

    void BecomeLeader() {
        role_ = raft_sim::Role::Leader;
        nextIndex_.clear();
        matchIndex_.clear();
        for (int peer : peerIds_) {
            nextIndex_[peer] = LastLogIndex() + 1;
            matchIndex_[peer] = -1;
        }
        matchIndex_[cfg_.id] = LastLogIndex();
        readAckedBy_.clear();
        lastKnownLeaderId_ = cfg_.id;
        // Raft's current-term-commit rule: a fresh leader can't commit prior-term entries until it
        // commits one of its own, so append a no-op immediately to make progress.
        log_.push_back(raft_sim::LogEntry{currentTerm_, "noop", "", ""});
        matchIndex_[cfg_.id] = LastLogIndex();
        firstIndexOfCurrentTerm_ = LastLogIndex();  // reads wait until this no-op commits
        PersistLog();
        loop_.CancelTimer(electionTimerFd_);
        loop_.StartPeriodic(heartbeatTimerFd_, cfg_.heartbeat);
        AdvanceCommit();  // a single-node cluster commits the no-op right here
        BroadcastAppendEntries();
    }

    void BroadcastAppendEntries() {
        if (role_ != raft_sim::Role::Leader) return;
        for (int peer : peerIds_) {
            if (nextIndex_[peer] <= lastIncludedIndex_) {
                SendInstallSnapshot(peer);
                continue;
            }
            int prev = nextIndex_[peer] - 1;
            raft_sim::AppendEntriesArgs args;
            args.term = currentTerm_;
            args.leaderId = cfg_.id;
            args.prevLogIndex = prev;
            args.prevLogTerm = TermAt(prev);
            args.leaderCommit = commitIndex_;
            for (int i = nextIndex_[peer]; i <= LastLogIndex(); ++i) args.entries.push_back(EntryAt(i));
            // Stamp the latest read round while any GET is outstanding; a peer echoing it confirms
            // leadership for every pending read (their ids are all <= currentReadId_).
            args.readId = pendingReads_.empty() ? 0 : currentReadId_;
            SendTo(peer, MessageType::kAppendEntriesArgs, args);
        }
    }

    void SendInstallSnapshot(int peer) {
        raft_sim::InstallSnapshotArgs args;
        args.term = currentTerm_;
        args.leaderId = cfg_.id;
        args.lastIncludedIndex = lastIncludedIndex_;
        args.lastIncludedTerm = lastIncludedTerm_;
        args.data = store_.serialize();
        SendTo(peer, MessageType::kInstallSnapshotArgs, args);
    }

    // --- frame dispatch --------------------------------------------------------------
    void OnFrame(const DecodedFrame& frame) {
        std::size_t pos = 0;
        switch (frame.type) {
            case MessageType::kRequestVoteArgs: {
                raft_sim::RequestVoteArgs a{};
                if (DecodeBody(frame.body, &pos, &a)) Handle(frame.from, a);
                break;
            }
            case MessageType::kRequestVoteReply: {
                raft_sim::RequestVoteReply r{};
                if (DecodeBody(frame.body, &pos, &r)) Handle(frame.from, r);
                break;
            }
            case MessageType::kAppendEntriesArgs: {
                raft_sim::AppendEntriesArgs a;
                if (DecodeBody(frame.body, &pos, &a)) Handle(frame.from, a);
                break;
            }
            case MessageType::kAppendEntriesReply: {
                raft_sim::AppendEntriesReply r{};
                if (DecodeBody(frame.body, &pos, &r)) Handle(frame.from, r);
                break;
            }
            case MessageType::kInstallSnapshotArgs: {
                raft_sim::InstallSnapshotArgs a;
                if (DecodeBody(frame.body, &pos, &a)) Handle(frame.from, a);
                break;
            }
            case MessageType::kInstallSnapshotReply: {
                raft_sim::InstallSnapshotReply r{};
                if (DecodeBody(frame.body, &pos, &r)) Handle(frame.from, r);
                break;
            }
        }
    }

    // --- RPC handling (mirrors raft_sim::Cluster's per-node logic) -------------------

    void Handle(int from, const raft_sim::RequestVoteArgs& args) {
        if (args.preVote) {
            HandlePreVote(from, args);
            return;
        }
        if (args.term > currentTerm_) StepDown(args.term);
        bool grant = false;
        if (args.term >= currentTerm_) {
            bool logOk = args.lastLogTerm > TermAt(LastLogIndex()) ||
                         (args.lastLogTerm == TermAt(LastLogIndex()) && args.lastLogIndex >= LastLogIndex());
            if ((votedFor_ == -1 || votedFor_ == args.candidateId) && logOk) {
                grant = true;
                votedFor_ = args.candidateId;
                PersistHardState();
                ResetElectionTimer();
            }
        }
        SendTo(from, MessageType::kRequestVoteReply, raft_sim::RequestVoteReply{currentTerm_, grant, false});
    }

    void HandlePreVote(int from, const raft_sim::RequestVoteArgs& args) {
        bool leaderStale = SteadyNowMs() - lastLeaderContactMs_ >= cfg_.electionMin.count();
        bool logOk = args.lastLogTerm > TermAt(LastLogIndex()) ||
                     (args.lastLogTerm == TermAt(LastLogIndex()) && args.lastLogIndex >= LastLogIndex());
        bool grant = leaderStale && logOk && args.term > currentTerm_;
        SendTo(from, MessageType::kRequestVoteReply, raft_sim::RequestVoteReply{currentTerm_, grant, true});
    }

    void Handle(int from, const raft_sim::RequestVoteReply& reply) {
        if (reply.preVote) {
            if (reply.term > currentTerm_) {
                StepDown(reply.term);
                return;
            }
            if (role_ != raft_sim::Role::Candidate) return;
            if (reply.voteGranted && ++preVotesGranted_ >= Majority()) StartElection();
            return;
        }
        if (reply.term > currentTerm_) {
            StepDown(reply.term);
            return;
        }
        if (role_ != raft_sim::Role::Candidate || reply.term != currentTerm_) return;
        if (reply.voteGranted && ++votesGranted_ >= Majority()) BecomeLeader();
        (void)from;
    }

    void Handle(int from, const raft_sim::AppendEntriesArgs& args) {
        if (args.term < currentTerm_) {
            SendTo(from, MessageType::kAppendEntriesReply,
                   raft_sim::AppendEntriesReply{currentTerm_, false, -1, args.readId});
            return;
        }
        if (args.term > currentTerm_) {
            currentTerm_ = args.term;
            votedFor_ = -1;
            PersistHardState();
        }
        role_ = raft_sim::Role::Follower;
        lastKnownLeaderId_ = args.leaderId;
        lastLeaderContactMs_ = SteadyNowMs();
        ResetElectionTimer();

        if (args.prevLogIndex >= lastIncludedIndex_ &&
            (args.prevLogIndex > LastLogIndex() || TermAt(args.prevLogIndex) != args.prevLogTerm)) {
            SendTo(from, MessageType::kAppendEntriesReply,
                   raft_sim::AppendEntriesReply{currentTerm_, false, -1, args.readId});
            return;
        }

        bool changed = false;
        int index = args.prevLogIndex;
        for (const auto& entry : args.entries) {
            ++index;
            if (index < FirstIndex()) continue;  // predates our snapshot; already durable
            if (index <= LastLogIndex()) {
                if (TermAt(index) != entry.term) {
                    log_.resize(static_cast<std::size_t>(index - FirstIndex()));
                    log_.push_back(entry);
                    changed = true;
                }
            } else {
                log_.push_back(entry);
                changed = true;
            }
        }
        if (changed) PersistLog();

        int lastNew = args.prevLogIndex + static_cast<int>(args.entries.size());
        if (args.leaderCommit > commitIndex_) commitIndex_ = std::min(args.leaderCommit, lastNew);
        commitIndex_ = std::max(commitIndex_, lastIncludedIndex_);
        ApplyCommitted();
        SendTo(from, MessageType::kAppendEntriesReply,
               raft_sim::AppendEntriesReply{currentTerm_, true, std::max(lastNew, lastIncludedIndex_),
                                            args.readId});
    }

    void Handle(int from, const raft_sim::AppendEntriesReply& reply) {
        if (reply.term > currentTerm_) {
            StepDown(reply.term);
            return;
        }
        if (role_ != raft_sim::Role::Leader || reply.term != currentTerm_) return;
        if (reply.success) {
            matchIndex_[from] = std::max(matchIndex_[from], reply.matchIndex);
            nextIndex_[from] = matchIndex_[from] + 1;
            if (reply.readId != 0) {
                // Record the highest ReadIndex round this peer has confirmed; a single reply can
                // thus satisfy every pending read with id <= reply.readId (see ReadConfirmed()).
                int& highest = readAckedBy_[from];
                if (reply.readId > highest) {
                    highest = reply.readId;
                    TryServePendingReads();
                }
            }
            AdvanceCommit();
        } else {
            nextIndex_[from] = std::max(0, nextIndex_[from] - 1);
        }
    }

    void Handle(int from, const raft_sim::InstallSnapshotArgs& args) {
        if (args.term < currentTerm_) {
            SendTo(from, MessageType::kInstallSnapshotReply, raft_sim::InstallSnapshotReply{currentTerm_, -1});
            return;
        }
        if (args.term > currentTerm_) {
            currentTerm_ = args.term;
            votedFor_ = -1;
            PersistHardState();
        }
        role_ = raft_sim::Role::Follower;
        lastKnownLeaderId_ = args.leaderId;
        lastLeaderContactMs_ = SteadyNowMs();
        ResetElectionTimer();

        if (args.lastIncludedIndex <= lastIncludedIndex_) {
            SendTo(from, MessageType::kInstallSnapshotReply,
                   raft_sim::InstallSnapshotReply{currentTerm_, lastIncludedIndex_});
            return;
        }

        if (HasEntry(args.lastIncludedIndex) && TermAt(args.lastIncludedIndex) == args.lastIncludedTerm) {
            std::vector<raft_sim::LogEntry> keep;
            for (int i = args.lastIncludedIndex + 1; i <= LastLogIndex(); ++i) keep.push_back(EntryAt(i));
            log_ = std::move(keep);
        } else {
            log_.clear();
        }
        lastIncludedIndex_ = args.lastIncludedIndex;
        lastIncludedTerm_ = args.lastIncludedTerm;
        store_.restore(raft_sim::KVStore::deserialize(args.data));
        // restore() reset the state machine to the snapshot boundary, so commit/apply must snap
        // back to it too — not max(). If this follower had applied past lastIncludedIndex, keeping
        // a higher lastApplied would leave the rolled-back entries permanently un-re-applied.
        commitIndex_ = lastIncludedIndex_;
        lastApplied_ = lastIncludedIndex_;
        cfg_.storage->saveSnapshot(lastIncludedIndex_, lastIncludedTerm_, store_.serialize(), log_);
        SendTo(from, MessageType::kInstallSnapshotReply,
               raft_sim::InstallSnapshotReply{currentTerm_, lastIncludedIndex_});
    }

    void Handle(int from, const raft_sim::InstallSnapshotReply& reply) {
        if (reply.term > currentTerm_) {
            StepDown(reply.term);
            return;
        }
        if (role_ != raft_sim::Role::Leader || reply.term != currentTerm_) return;
        if (reply.matchIndex >= 0) {
            matchIndex_[from] = std::max(matchIndex_[from], reply.matchIndex);
            nextIndex_[from] = matchIndex_[from] + 1;
            AdvanceCommit();
        }
    }

    void AdvanceCommit() {
        for (int idx = LastLogIndex(); idx > commitIndex_; --idx) {
            if (TermAt(idx) != currentTerm_) continue;  // only count current-term entries
            int replicas = 0;
            for (int peer : peerIds_)
                if (matchIndex_[peer] >= idx) ++replicas;
            if (matchIndex_[cfg_.id] >= idx) ++replicas;
            if (replicas >= Majority()) {
                commitIndex_ = idx;
                ApplyCommitted();
                break;
            }
        }
    }

    void ApplyCommitted() {
        while (lastApplied_ < commitIndex_) {
            ++lastApplied_;
            store_.apply(EntryAt(lastApplied_));
        }
        TryServePendingReads();
    }

    static std::int64_t SteadyNowMs() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
    }

    // --- configuration & wiring ---
    ServerConfig cfg_;
    std::mt19937_64 rng_;
    int n_ = 1;
    std::vector<int> peerIds_;
    EventLoop loop_;
    int listenPeerFd_ = -1;
    int listenAdminFd_ = -1;
    int electionTimerFd_ = -1;
    int heartbeatTimerFd_ = -1;
    std::unordered_map<int, int> reconnectTimerFd_;  // peer id -> timer fd (only for peers we dial)
    std::unordered_map<int, Conn> connsByFd_;
    std::unordered_map<int, int> peerIdToFd_;  // peer id -> live fd, absent if currently disconnected
    std::unordered_map<int, AdminConn> adminConnsByFd_;

    // --- persistent Raft state (mirrors raft_sim::Node) ---
    int currentTerm_ = 0;
    int votedFor_ = -1;
    std::vector<raft_sim::LogEntry> log_;
    int lastIncludedIndex_ = -1;
    int lastIncludedTerm_ = 0;

    // --- volatile Raft state ---
    raft_sim::Role role_ = raft_sim::Role::Follower;
    int commitIndex_ = -1;
    int lastApplied_ = -1;
    raft_sim::KVStore store_;
    std::unordered_map<int, int> nextIndex_;   // leader-only
    std::unordered_map<int, int> matchIndex_;  // leader-only
    int votesGranted_ = 0;
    int preVotesGranted_ = 0;
    std::int64_t lastLeaderContactMs_ = 0;
    int lastKnownLeaderId_ = -1;

    // --- ReadIndex bookkeeping (leader-only, volatile) ---
    int currentReadId_ = 0;
    int firstIndexOfCurrentTerm_ = -1;         // index of this term's no-op; read gate boundary
    std::unordered_map<int, int> readAckedBy_;  // peer id -> highest ReadIndex round it has acked
    std::vector<PendingRead> pendingReads_;
};

}  // namespace raft_net
