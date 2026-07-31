#include "net/codec.h"

#include <string>

#include <gtest/gtest.h>

using namespace raft_net;
using raft_sim::AppendEntriesArgs;
using raft_sim::AppendEntriesReply;
using raft_sim::InstallSnapshotArgs;
using raft_sim::InstallSnapshotReply;
using raft_sim::LogEntry;
using raft_sim::RequestVoteArgs;
using raft_sim::RequestVoteReply;

namespace {

// Round-trips one frame through Encode -> TryDecodeFrame -> DecodeBody and checks every field
// survives. `from`/`type` are asserted on the frame; the body is compared field-by-field by the
// caller-supplied predicate since each RPC struct has no operator==.
template <typename Body, typename Eq>
void ExpectRoundTrip(int from, MessageType type, const Body& body, Eq eq) {
    const std::string framed = EncodeFrame(from, type, body);
    std::size_t consumed = 0;
    auto decoded = TryDecodeFrame(framed, &consumed);
    ASSERT_TRUE(decoded.has_value());
    EXPECT_EQ(consumed, framed.size());
    EXPECT_EQ(decoded->from, from);
    EXPECT_EQ(decoded->type, type);

    Body out{};
    std::size_t pos = 0;
    ASSERT_TRUE(DecodeBody(decoded->body, &pos, &out));
    eq(body, out);
}

TEST(RaftNetCodec, RequestVoteArgsRoundTrips) {
    RequestVoteArgs a{7, 3, 41, 6, /*preVote=*/true};
    ExpectRoundTrip(3, MessageType::kRequestVoteArgs, a, [](const auto& in, const auto& out) {
        EXPECT_EQ(in.term, out.term);
        EXPECT_EQ(in.candidateId, out.candidateId);
        EXPECT_EQ(in.lastLogIndex, out.lastLogIndex);
        EXPECT_EQ(in.lastLogTerm, out.lastLogTerm);
        EXPECT_EQ(in.preVote, out.preVote);
    });
}

TEST(RaftNetCodec, RequestVoteReplyRoundTrips) {
    RequestVoteReply r{9, false, true};
    ExpectRoundTrip(1, MessageType::kRequestVoteReply, r, [](const auto& in, const auto& out) {
        EXPECT_EQ(in.term, out.term);
        EXPECT_EQ(in.voteGranted, out.voteGranted);
        EXPECT_EQ(in.preVote, out.preVote);
    });
}

TEST(RaftNetCodec, AppendEntriesArgsRoundTripsWithMultipleEntries) {
    AppendEntriesArgs a;
    a.term = 4;
    a.leaderId = 0;
    a.prevLogIndex = 10;
    a.prevLogTerm = 3;
    a.leaderCommit = 9;
    a.readId = 5;
    a.entries = {LogEntry{4, "put", "k1", "v1"}, LogEntry{4, "delete", "k2", ""},
                 LogEntry{4, "noop", "", ""}};
    ExpectRoundTrip(0, MessageType::kAppendEntriesArgs, a, [](const auto& in, const auto& out) {
        EXPECT_EQ(in.term, out.term);
        EXPECT_EQ(in.leaderId, out.leaderId);
        EXPECT_EQ(in.prevLogIndex, out.prevLogIndex);
        EXPECT_EQ(in.prevLogTerm, out.prevLogTerm);
        EXPECT_EQ(in.leaderCommit, out.leaderCommit);
        EXPECT_EQ(in.readId, out.readId);
        ASSERT_EQ(in.entries.size(), out.entries.size());
        for (std::size_t i = 0; i < in.entries.size(); ++i) EXPECT_TRUE(in.entries[i] == out.entries[i]);
    });
}

TEST(RaftNetCodec, AppendEntriesArgsRoundTripsWithNoEntries) {
    AppendEntriesArgs a;
    a.term = 1;
    a.leaderId = 2;
    a.prevLogIndex = -1;
    a.prevLogTerm = 0;
    a.leaderCommit = -1;
    ExpectRoundTrip(2, MessageType::kAppendEntriesArgs, a, [](const auto& in, const auto& out) {
        EXPECT_EQ(in.term, out.term);
        EXPECT_TRUE(out.entries.empty());
    });
}

TEST(RaftNetCodec, AppendEntriesReplyRoundTrips) {
    AppendEntriesReply r{6, true, 12, 0};
    ExpectRoundTrip(4, MessageType::kAppendEntriesReply, r, [](const auto& in, const auto& out) {
        EXPECT_EQ(in.term, out.term);
        EXPECT_EQ(in.success, out.success);
        EXPECT_EQ(in.matchIndex, out.matchIndex);
        EXPECT_EQ(in.readId, out.readId);
    });
}

TEST(RaftNetCodec, InstallSnapshotArgsRoundTripsWithBinaryData) {
    InstallSnapshotArgs a;
    a.term = 8;
    a.leaderId = 1;
    a.lastIncludedIndex = 99;
    a.lastIncludedTerm = 7;
    // 含嵌入 NUL 的二进制负载；长度由数组推导（sizeof-1 去掉编译器追加的结尾 NUL），
    // 避免手写长度写错读越界（原来硬编码 20 > 实际 17 字节，ASan 会报 global-buffer-overflow）。
    static const char kBlob[] = "\x00\x01\xff binary blob \x00";
    a.data = std::string(kBlob, sizeof(kBlob) - 1);
    ExpectRoundTrip(1, MessageType::kInstallSnapshotArgs, a, [](const auto& in, const auto& out) {
        EXPECT_EQ(in.term, out.term);
        EXPECT_EQ(in.lastIncludedIndex, out.lastIncludedIndex);
        EXPECT_EQ(in.lastIncludedTerm, out.lastIncludedTerm);
        EXPECT_EQ(in.data, out.data);
    });
}

TEST(RaftNetCodec, InstallSnapshotReplyRoundTrips) {
    InstallSnapshotReply r{3, 55};
    ExpectRoundTrip(2, MessageType::kInstallSnapshotReply, r, [](const auto& in, const auto& out) {
        EXPECT_EQ(in.term, out.term);
        EXPECT_EQ(in.matchIndex, out.matchIndex);
    });
}

// The core reason this codec exists: TCP has no message boundaries, so a partial frame (e.g. one
// still missing its last byte) must be reported as "not ready yet", never mis-decoded.
TEST(RaftNetCodec, PartialFrameIsNotDecoded) {
    RequestVoteArgs a{1, 0, -1, 0, false};
    const std::string framed = EncodeFrame(0, MessageType::kRequestVoteArgs, a);
    for (std::size_t cut = 0; cut < framed.size(); ++cut) {
        std::size_t consumed = 0;
        EXPECT_FALSE(TryDecodeFrame(framed.substr(0, cut), &consumed).has_value())
            << "spuriously decoded at cut=" << cut;
    }
}

// Two frames coalesced into one buffer (as TCP is free to do) must decode as two, in order, with
// TryDecodeFrame's `consumed` correctly pointing past the first before the second is attempted.
TEST(RaftNetCodec, TwoCoalescedFramesDecodeInOrder) {
    RequestVoteArgs a{1, 0, -1, 0, false};
    RequestVoteReply b{2, true, false};
    std::string buf = EncodeFrame(0, MessageType::kRequestVoteArgs, a);
    buf += EncodeFrame(1, MessageType::kRequestVoteReply, b);

    std::size_t consumed1 = 0;
    auto first = TryDecodeFrame(buf, &consumed1);
    ASSERT_TRUE(first.has_value());
    EXPECT_EQ(first->type, MessageType::kRequestVoteArgs);
    buf.erase(0, consumed1);

    std::size_t consumed2 = 0;
    auto second = TryDecodeFrame(buf, &consumed2);
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(second->type, MessageType::kRequestVoteReply);
    EXPECT_EQ(consumed2, buf.size());
}

}  // namespace
