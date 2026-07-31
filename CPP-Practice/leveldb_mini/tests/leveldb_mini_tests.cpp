#include "mini_leveldb.h"

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "lsm/block.h"
#include "lsm/crc32c.h"
#include "lsm/record_log.h"

namespace {

using mini_lsm::Arena;
using mini_lsm::BloomFilter;
using mini_lsm::MemTable;
using mini_lsm::MiniDB;
using mini_lsm::SkipList;
using mini_lsm::SSTable;
using mini_lsm::WAL;

// Each test gets its own scratch directory so parallel test runs never collide.
class TempDir : public ::testing::Test {
 protected:
    void SetUp() override {
        static std::atomic<unsigned> counter{0};
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        dir_ = std::filesystem::temp_directory_path() /
               ("mini_lsm_" + std::to_string(stamp) + "_" + std::to_string(counter.fetch_add(1)));
        std::filesystem::create_directories(dir_);
    }
    void TearDown() override {
        std::error_code ec;
        std::filesystem::remove_all(dir_, ec);
    }
    std::filesystem::path dir_;
};

}  // namespace

TEST(SkipList, InsertAndFind) {
    SkipList<std::string, std::string> list;
    list.Insert("a", "1");
    list.Insert("b", "2");
    ASSERT_TRUE(list.Find("a").has_value());
    EXPECT_EQ(*list.Find("a"), "1");
    EXPECT_EQ(*list.Find("b"), "2");
}

TEST(SkipList, FindMissingReturnsNullopt) {
    SkipList<std::string, std::string> list;
    list.Insert("a", "1");
    EXPECT_FALSE(list.Find("zzz").has_value());
}

TEST(SkipList, InsertSameKeyOverwritesWithoutGrowing) {
    SkipList<std::string, std::string> list;
    list.Insert("k", "old");
    list.Insert("k", "new");
    EXPECT_EQ(*list.Find("k"), "new");
    EXPECT_EQ(list.size(), 1u);  // overwrite updates in place, no new node
}

TEST(SkipList, IteratesInSortedKeyOrder) {
    SkipList<std::string, std::string> list;
    for (const char* k : {"delta", "alpha", "charlie", "bravo"}) list.Insert(k, k);
    std::vector<std::string> keys;
    for (auto it = list.begin(); it != list.end(); ++it) keys.push_back(it.key());
    EXPECT_EQ(keys, (std::vector<std::string>{"alpha", "bravo", "charlie", "delta"}));
}

TEST(Arena, TracksMemoryUsage) {
    Arena arena;
    EXPECT_EQ(arena.memory_usage(), 0u);
    arena.Allocate(64);
    arena.Allocate(16);
    EXPECT_EQ(arena.memory_usage(), 80u);
}

TEST(MemTable, GetReturnsLatestSequenceForKey) {
    MemTable mem;
    mem.Put(1, "k", "v1");
    mem.Put(5, "k", "v3");
    mem.Put(3, "k", "v2");
    ASSERT_TRUE(mem.Get("k").has_value());
    EXPECT_EQ(*mem.Get("k"), "v3");  // highest sequence wins regardless of insert order
}

TEST(MemTable, GetMissingReturnsNullopt) {
    MemTable mem;
    mem.Put(1, "present", "v");
    EXPECT_FALSE(mem.Get("absent").has_value());
}

TEST(MemTable, EntriesByUserKeyAreSortedAndDeduped) {
    MemTable mem;
    mem.Put(1, "b", "vb");
    mem.Put(2, "a", "va");
    mem.Put(3, "a", "va");  // same user key, different sequence -> one entry
    const auto entries = mem.EntriesByUserKey();
    ASSERT_EQ(entries.size(), 2u);
    EXPECT_EQ(entries[0].first, "a");
    EXPECT_EQ(entries[1].first, "b");
}

TEST(BloomFilter, NoFalseNegatives) {
    BloomFilter filter;
    const std::vector<std::string> keys = {"apple", "banana", "cherry", "date"};
    for (const auto& k : keys) filter.Add(k);
    for (const auto& k : keys) EXPECT_TRUE(filter.MayContain(k));
}

TEST_F(TempDir, SSTableBuildThenGet) {
    SSTable table;
    table.Build(dir_ / "t.sst", {{"a", "1"}, {"b", "2"}, {"c", "3"}});
    EXPECT_EQ(table.size(), 3u);
    ASSERT_TRUE(table.Get("b").has_value());
    EXPECT_EQ(*table.Get("b"), "2");
    EXPECT_FALSE(table.Get("missing").has_value());
}

TEST_F(TempDir, SSTableFileRoundTrip) {
    const auto path = dir_ / "roundtrip.sst";
    SSTable writer;
    writer.Build(path, {{"x", "10"}, {"y", "20"}, {"z", "30"}});

    SSTable reader;  // reload from disk into a fresh table
    reader.Load(path);
    EXPECT_EQ(reader.size(), 3u);
    ASSERT_TRUE(reader.Get("y").has_value());
    EXPECT_EQ(*reader.Get("y"), "20");
    EXPECT_EQ(*reader.Get("x"), "10");
    EXPECT_FALSE(reader.Get("nope").has_value());
}

TEST_F(TempDir, WalAppendThenReplay) {
    WAL wal(dir_ / "wal.log");
    wal.Append(1, mini_lsm::ValueType::kValue, "k1", "v1");
    wal.Append(2, mini_lsm::ValueType::kValue, "k2", "v2");
    const auto records = wal.Replay();
    ASSERT_EQ(records.size(), 2u);
    EXPECT_EQ(records[0].sequence, 1u);
    EXPECT_EQ(records[1].key, "k2");
    EXPECT_EQ(records[1].value, "v2");
}

TEST_F(TempDir, MiniDbPutGet) {
    MiniDB db(dir_);
    db.Put("name", "mini");
    ASSERT_TRUE(db.Get("name").has_value());
    EXPECT_EQ(*db.Get("name"), "mini");
}

TEST_F(TempDir, MiniDbOverwriteReturnsLatest) {
    MiniDB db(dir_);
    db.Put("k", "first");
    db.Put("k", "second");
    EXPECT_EQ(*db.Get("k"), "second");
}

TEST_F(TempDir, MiniDbGetMissingReturnsNullopt) {
    MiniDB db(dir_);
    db.Put("here", "v");
    EXPECT_FALSE(db.Get("gone").has_value());
}

TEST_F(TempDir, MiniDbFlushPopulatesSSTable) {
    MiniDB db(dir_);
    db.Put("a", "1");
    db.Put("b", "2");
    EXPECT_EQ(db.SSTableSize(), 0u);
    db.Flush();
    EXPECT_EQ(db.SSTableSize(), 2u);
    EXPECT_EQ(*db.Get("a"), "1");  // still readable after flush
}

// Durability round-trip: a fresh DB over the same directory rebuilds its
// MemTable by replaying the WAL.
TEST_F(TempDir, MiniDbRecoversFromWal) {
    {
        MiniDB db(dir_);
        db.Put("persist", "yes");
        db.Put("count", "42");
    }
    MiniDB reopened(dir_);
    reopened.Recover();
    ASSERT_TRUE(reopened.Get("persist").has_value());
    EXPECT_EQ(*reopened.Get("persist"), "yes");
    EXPECT_EQ(*reopened.Get("count"), "42");
}

// ---------------------------------------------------------------------------
// F3 崩溃安全化新增用例：CRC32C / Block / WAL 尾部恢复 / tombstone / compaction /
// 文件号 / kill-9 持久性。
// ---------------------------------------------------------------------------

TEST(Crc32c, KnownAnswerVectors) {
    // CRC-32C(Castagnoli) 的标准校验向量："123456789" -> 0xE3069283，空串 -> 0。
    EXPECT_EQ(mini_lsm::crc32c::Value(std::string("123456789")), 0xE3069283u);
    EXPECT_EQ(mini_lsm::crc32c::Value(std::string()), 0u);
    // 单字节差异必然改变校验和。
    EXPECT_NE(mini_lsm::crc32c::Value(std::string("hello")),
              mini_lsm::crc32c::Value(std::string("hellp")));
}

TEST(Block, RoundTripWithPrefixCompressionAndBinarySearch) {
    mini_lsm::BlockBuilder builder;
    builder.Reset();
    // 足够多且带公共前缀的 key，跨过 restart 间隔，验证前缀压缩 + restart 二分。
    std::vector<std::pair<std::string, std::string>> kv;
    for (int i = 0; i < 100; ++i) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "user:%05d", i);
        kv.emplace_back(buf, "value-" + std::to_string(i));
    }
    for (const auto& [k, v] : kv) builder.Add(k, v);
    mini_lsm::Block block(builder.Finish());

    for (const auto& [k, v] : kv) {
        auto got = block.Get(k);
        ASSERT_TRUE(got.has_value()) << "missing key " << k;
        EXPECT_EQ(*got, v);
    }
    EXPECT_FALSE(block.Get("user:99999").has_value());    // 越过末尾
    EXPECT_FALSE(block.Get("aaaa").has_value());          // 早于开头
    EXPECT_FALSE(block.Get("user:000005x").has_value());  // 不精确匹配
}

TEST_F(TempDir, WalDropsTruncatedTail) {
    const auto path = dir_ / "trunc.log";
    {
        mini_lsm::RecordWriter writer;
        ASSERT_TRUE(writer.Open(path.string()));
        ASSERT_TRUE(writer.AddRecord("first"));
        ASSERT_TRUE(writer.AddRecord("second"));
        ASSERT_TRUE(writer.AddRecord("third"));
        ASSERT_TRUE(writer.Sync());
    }
    // 手工截掉文件末尾若干字节，模拟"写到一半断电"的残缺尾部 record。
    const auto full = std::filesystem::file_size(path);
    std::filesystem::resize_file(path, full - 3);

    const auto records = mini_lsm::RecordReader::Replay(path.string());
    ASSERT_EQ(records.size(), 2u);  // 完整的前两条幸存，截断的第三条被丢弃
    EXPECT_EQ(records[0], "first");
    EXPECT_EQ(records[1], "second");
}

TEST_F(TempDir, WalStopsAtCorruptedRecord) {
    const auto path = dir_ / "corrupt.log";
    {
        mini_lsm::RecordWriter writer;
        ASSERT_TRUE(writer.Open(path.string()));
        ASSERT_TRUE(writer.AddRecord("alpha"));
        ASSERT_TRUE(writer.AddRecord("bravo"));
        ASSERT_TRUE(writer.AddRecord("charlie"));
        ASSERT_TRUE(writer.Sync());
    }
    // 翻转第二条 record payload 里的一个字节：CRC 不再匹配，回放应止于此。
    // 帧布局 length(4)+type(1)+crc(4)+payload；第一条 payload="alpha"(5B)，帧长 9+5=14。
    // 第二条 payload 起点 = 14 + 9 = 23。翻一个 payload 字节。
    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
    ASSERT_TRUE(file.is_open());
    file.seekp(23);
    char c = 0;
    file.read(&c, 1);
    file.seekp(23);
    c = static_cast<char>(c ^ 0xFF);
    file.write(&c, 1);
    file.close();

    const auto records = mini_lsm::RecordReader::Replay(path.string());
    ASSERT_EQ(records.size(), 1u);  // 只保住损坏前的第一条
    EXPECT_EQ(records[0], "alpha");
}

TEST_F(TempDir, TombstoneShadowsFlushedValue) {
    MiniDB db(dir_);
    db.Put("k", "v1");
    db.Flush();  // v1 落到 L0
    EXPECT_EQ(*db.Get("k"), "v1");
    db.Delete("k");                         // 内存里的墓碑
    EXPECT_FALSE(db.Get("k").has_value());  // 墓碑遮蔽已落盘的旧值
    db.Flush();                             // 墓碑也落到新的 L0
    EXPECT_FALSE(db.Get("k").has_value());
}

TEST_F(TempDir, CompactionReclaimsTombstonesAtBottomLevel) {
    MiniDB db(dir_);
    // 写入并删除一个 key，再写若干 key，反复 flush 直到触发 compaction。
    db.Put("gone", "x");
    db.Put("live", "y");
    db.Delete("gone");
    for (int i = 0; i < 5; ++i) {
        db.Put("f" + std::to_string(i), std::to_string(i));
        db.Flush();  // 每次 flush 一个 L0；到阈值自动触发 L0→L1 compaction
    }
    db.CompactToBottom();  // 显式再压一次，把剩余 L0 也并入 L1，状态确定
    // compaction 已把墓碑在最底层回收：gone 不存在，live 仍在。
    EXPECT_FALSE(db.Get("gone").has_value());
    EXPECT_EQ(*db.Get("live"), "y");
    EXPECT_EQ(db.Level0FileCount(), 0u);  // 全部并入 L1
    EXPECT_EQ(db.Level1FileCount(), 1u);  // 合并成单个 L1 文件

    // 重开恢复后仍一致。
    MiniDB reopened(dir_);
    reopened.Recover();
    EXPECT_FALSE(reopened.Get("gone").has_value());
    EXPECT_EQ(*reopened.Get("live"), "y");
}

TEST_F(TempDir, RecoversAcrossFlushAndCompaction) {
    {
        MiniDB db(dir_);
        for (int i = 0; i < 20; ++i) {
            db.Put("key" + std::to_string(i), "val" + std::to_string(i));
            if (i % 4 == 3) db.Flush();
        }
        db.Put("tail", "unflushed");  // 留在 WAL，未 flush
    }
    MiniDB reopened(dir_);
    reopened.Recover();
    for (int i = 0; i < 20; ++i) {
        auto got = reopened.Get("key" + std::to_string(i));
        ASSERT_TRUE(got.has_value()) << "lost key" << i;
        EXPECT_EQ(*got, "val" + std::to_string(i));
    }
    EXPECT_EQ(*reopened.Get("tail"), "unflushed");  // WAL 恢复未落盘的写
}

// kill-9 持久性：子进程写入并 fsync 若干 acked 写后，未 Flush 就 _exit(0)（模拟崩溃，
// 且必须 _exit 而非 exit/return，避免 gtest atexit 在子进程重跑）。父进程重开库 Recover，
// 断言所有 acked 写幸存——证明 WAL fsync 提供的真持久性。
TEST_F(TempDir, Kill9DurabilityAckedWritesSurvive) {
    constexpr int kAcked = 50;
    const pid_t pid = fork();
    ASSERT_NE(pid, -1);
    if (pid == 0) {
        // 子进程：写入并落盘，然后"猝死"。
        MiniDB db(dir_);
        for (int i = 0; i < kAcked; ++i) {
            db.Put("k" + std::to_string(i), "v" + std::to_string(i));
        }
        _exit(0);  // 不析构、不 Flush、不跑 atexit —— 仅依赖 WAL 已 fsync 的记录
    }
    int status = 0;
    ASSERT_EQ(::waitpid(pid, &status, 0), pid);

    MiniDB reopened(dir_);
    reopened.Recover();
    for (int i = 0; i < kAcked; ++i) {
        auto got = reopened.Get("k" + std::to_string(i));
        ASSERT_TRUE(got.has_value()) << "lost acked write k" << i;
        EXPECT_EQ(*got, "v" + std::to_string(i));
    }
}
