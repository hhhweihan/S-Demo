#include "mini_leveldb.h"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

#include <gtest/gtest.h>

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
    wal.Append(1, "k1", "v1");
    wal.Append(2, "k2", "v2");
    const auto records = wal.Replay();
    ASSERT_EQ(records.size(), 2u);
    EXPECT_EQ(std::get<0>(records[0]), 1u);
    EXPECT_EQ(std::get<1>(records[1]), "k2");
    EXPECT_EQ(std::get<2>(records[1]), "v2");
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
