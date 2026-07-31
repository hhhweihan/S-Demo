#pragma once

// Wire codec for the real-network Raft server (raft_server.h): turns the RPC structs already
// defined in raft_sim.h into length-prefixed frames on a TCP byte stream, and back.
//
// Why this exists: raft_sim.h's Cluster delivers RPCs by moving a C++ struct into a std::multiset
// — no bytes, no boundaries, no reordering-at-the-byte-level. A real TCP socket has none of that:
// it is a byte stream with no message boundaries at all, so two AppendEntries "writes" can arrive
// as one read() or be split across many. Every real RPC layer solves this the same way — a
// self-describing frame: [length][type][body] — so the reader always knows exactly how many bytes
// make one message before it can decode it.
//
// Encoding reuses raft_sim::detail's existing fixed-width little-endian helpers (already used by
// raft_storage.h's PersistentState serialization) instead of inventing a second scheme.

#include <cstdint>
#include <optional>
#include <string>

#include "../raft_sim.h"
#include "../raft_storage.h"

namespace raft_net {

enum class MessageType : std::uint8_t {
    kRequestVoteArgs = 1,
    kRequestVoteReply = 2,
    kAppendEntriesArgs = 3,
    kAppendEntriesReply = 4,
    kInstallSnapshotArgs = 5,
    kInstallSnapshotReply = 6,
};

// --- body encode/decode (one pair per RPC struct) ---------------------------

inline std::string EncodeBody(const raft_sim::RequestVoteArgs& a) {
    std::string out;
    raft_sim::detail::PutI32(&out, a.term);
    raft_sim::detail::PutI32(&out, a.candidateId);
    raft_sim::detail::PutI32(&out, a.lastLogIndex);
    raft_sim::detail::PutI32(&out, a.lastLogTerm);
    out.push_back(a.preVote ? 1 : 0);
    return out;
}
inline bool DecodeBody(const std::string& in, std::size_t* pos, raft_sim::RequestVoteArgs* a) {
    if (!raft_sim::detail::GetI32(in, pos, &a->term)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->candidateId)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->lastLogIndex)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->lastLogTerm)) return false;
    if (*pos >= in.size()) return false;
    a->preVote = in[(*pos)++] != 0;
    return true;
}

inline std::string EncodeBody(const raft_sim::RequestVoteReply& r) {
    std::string out;
    raft_sim::detail::PutI32(&out, r.term);
    out.push_back(r.voteGranted ? 1 : 0);
    out.push_back(r.preVote ? 1 : 0);
    return out;
}
inline bool DecodeBody(const std::string& in, std::size_t* pos, raft_sim::RequestVoteReply* r) {
    if (!raft_sim::detail::GetI32(in, pos, &r->term)) return false;
    if (*pos + 2 > in.size()) return false;
    r->voteGranted = in[(*pos)++] != 0;
    r->preVote = in[(*pos)++] != 0;
    return true;
}

inline std::string EncodeBody(const raft_sim::AppendEntriesArgs& a) {
    std::string out;
    raft_sim::detail::PutI32(&out, a.term);
    raft_sim::detail::PutI32(&out, a.leaderId);
    raft_sim::detail::PutI32(&out, a.prevLogIndex);
    raft_sim::detail::PutI32(&out, a.prevLogTerm);
    raft_sim::detail::PutI32(&out, a.leaderCommit);
    raft_sim::detail::PutI32(&out, a.readId);
    raft_sim::detail::PutU32(&out, static_cast<std::uint32_t>(a.entries.size()));
    for (const auto& e : a.entries) {
        raft_sim::detail::PutI32(&out, e.term);
        raft_sim::detail::PutStr(&out, e.op);
        raft_sim::detail::PutStr(&out, e.key);
        raft_sim::detail::PutStr(&out, e.value);
    }
    return out;
}
inline bool DecodeBody(const std::string& in, std::size_t* pos, raft_sim::AppendEntriesArgs* a) {
    if (!raft_sim::detail::GetI32(in, pos, &a->term)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->leaderId)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->prevLogIndex)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->prevLogTerm)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->leaderCommit)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->readId)) return false;
    std::uint32_t count = 0;
    if (!raft_sim::detail::GetU32(in, pos, &count)) return false;
    a->entries.clear();
    a->entries.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        raft_sim::LogEntry e;
        if (!raft_sim::detail::GetI32(in, pos, &e.term)) return false;
        if (!raft_sim::detail::GetStr(in, pos, &e.op)) return false;
        if (!raft_sim::detail::GetStr(in, pos, &e.key)) return false;
        if (!raft_sim::detail::GetStr(in, pos, &e.value)) return false;
        a->entries.push_back(std::move(e));
    }
    return true;
}

inline std::string EncodeBody(const raft_sim::AppendEntriesReply& r) {
    std::string out;
    raft_sim::detail::PutI32(&out, r.term);
    out.push_back(r.success ? 1 : 0);
    raft_sim::detail::PutI32(&out, r.matchIndex);
    raft_sim::detail::PutI32(&out, r.readId);
    return out;
}
inline bool DecodeBody(const std::string& in, std::size_t* pos, raft_sim::AppendEntriesReply* r) {
    if (!raft_sim::detail::GetI32(in, pos, &r->term)) return false;
    if (*pos >= in.size()) return false;
    r->success = in[(*pos)++] != 0;
    if (!raft_sim::detail::GetI32(in, pos, &r->matchIndex)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &r->readId)) return false;
    return true;
}

inline std::string EncodeBody(const raft_sim::InstallSnapshotArgs& a) {
    std::string out;
    raft_sim::detail::PutI32(&out, a.term);
    raft_sim::detail::PutI32(&out, a.leaderId);
    raft_sim::detail::PutI32(&out, a.lastIncludedIndex);
    raft_sim::detail::PutI32(&out, a.lastIncludedTerm);
    raft_sim::detail::PutStr(&out, a.data);
    return out;
}
inline bool DecodeBody(const std::string& in, std::size_t* pos, raft_sim::InstallSnapshotArgs* a) {
    if (!raft_sim::detail::GetI32(in, pos, &a->term)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->leaderId)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->lastIncludedIndex)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &a->lastIncludedTerm)) return false;
    if (!raft_sim::detail::GetStr(in, pos, &a->data)) return false;
    return true;
}

inline std::string EncodeBody(const raft_sim::InstallSnapshotReply& r) {
    std::string out;
    raft_sim::detail::PutI32(&out, r.term);
    raft_sim::detail::PutI32(&out, r.matchIndex);
    return out;
}
inline bool DecodeBody(const std::string& in, std::size_t* pos, raft_sim::InstallSnapshotReply* r) {
    if (!raft_sim::detail::GetI32(in, pos, &r->term)) return false;
    if (!raft_sim::detail::GetI32(in, pos, &r->matchIndex)) return false;
    return true;
}

// --- frame: [u32 length][u32 from][u8 type][body] ---------------------------
// `length` counts everything after itself (from + type + body), so a reader with N buffered
// bytes can tell — before decoding anything — whether a whole frame is available yet.

// Hard cap on a single frame's declared payload length. A corrupt or malicious sender could
// otherwise put an arbitrary u32 in the length prefix and make the reader buffer gigabytes before
// it ever holds a "complete" frame — an easy OOM. Anything larger is a protocol error the caller
// handles by closing the connection, not by buffering. A few MB comfortably covers the largest
// legitimate frame here (an InstallSnapshot carrying the whole state machine).
constexpr std::size_t kMaxFrame = 8 * 1024 * 1024;

template <typename Body>
inline std::string EncodeFrame(int from, MessageType type, const Body& body) {
    std::string payload;
    raft_sim::detail::PutI32(&payload, from);
    payload.push_back(static_cast<char>(static_cast<std::uint8_t>(type)));
    payload.append(EncodeBody(body));

    std::string framed;
    raft_sim::detail::PutU32(&framed, static_cast<std::uint32_t>(payload.size()));
    framed.append(payload);
    return framed;
}

struct DecodedFrame {
    int from;
    MessageType type;
    std::string body;  // still-encoded body; caller decodes with DecodeBody() for the known type
};

// Tries to pull one complete frame off the front of `buf`. Returns nullopt if `buf` doesn't yet
// hold a whole frame (the caller should read more bytes and retry) — this is what makes the codec
// safe against TCP splitting one logical message across multiple recv()s, or coalescing several
// into one. On success, `*consumed` is set to the number of bytes to erase from the front of `buf`.
// If the declared payload length exceeds kMaxFrame, `*oversize` (when provided) is set and nullopt
// is returned so the caller can drop the connection instead of buffering an absurd length.
inline std::optional<DecodedFrame> TryDecodeFrame(const std::string& buf, std::size_t* consumed,
                                                  bool* oversize = nullptr) {
    if (oversize) *oversize = false;
    constexpr std::size_t kLenFieldSize = 4;
    if (buf.size() < kLenFieldSize) return std::nullopt;
    std::size_t pos = 0;
    std::uint32_t payloadLen = 0;
    raft_sim::detail::GetU32(buf, &pos, &payloadLen);
    if (payloadLen > kMaxFrame) {  // bogus/huge length prefix — refuse before buffering it
        if (oversize) *oversize = true;
        return std::nullopt;
    }
    if (buf.size() < kLenFieldSize + payloadLen) return std::nullopt;  // partial frame

    const std::string payload = buf.substr(kLenFieldSize, payloadLen);
    std::size_t ppos = 0;
    DecodedFrame frame;
    if (!raft_sim::detail::GetI32(payload, &ppos, &frame.from)) return std::nullopt;
    if (ppos >= payload.size()) return std::nullopt;
    frame.type = static_cast<MessageType>(static_cast<std::uint8_t>(payload[ppos++]));
    frame.body = payload.substr(ppos);
    *consumed = kLenFieldSize + payloadLen;
    return frame;
}

}  // namespace raft_net
