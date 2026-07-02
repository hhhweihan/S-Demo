#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "mini_leveldb.h"

namespace {

void expect(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "[FAIL] " << message << '\n';
    std::exit(1);
  }
  std::cout << "[PASS] " << message << '\n';
}

void test_infrastructure() {
  mini_lsm::Slice slice("leveldb");
  mini_lsm::Arena arena;
  void* block = arena.Allocate(128);
  expect(slice.ToString() == "leveldb", "Slice references external bytes");
  expect(block != nullptr && arena.memory_usage() == 128, "Arena tracks allocated bytes");
  expect(mini_lsm::Status::OK().ok(), "Status::OK reports success");
}

void test_skiplist() {
  mini_lsm::SkipList<int, std::string> list;
  for (int value : {5, 3, 8, 1, 7}) list.Insert(value, std::to_string(value));
  std::vector<int> keys;
  for (auto it = list.begin(); it != list.end(); ++it) keys.push_back(it.key());
  expect((keys == std::vector<int>{1, 3, 5, 7, 8}), "SkipList iterates keys in sorted order");
  expect(list.Find(7).value_or("") == "7", "SkipList finds an inserted key");
}

void test_memtable_sstable_bloom() {
  const auto dir = std::filesystem::path("CPP-Practice/leveldb_mini/build/db");
  std::filesystem::remove_all(dir);
  std::filesystem::create_directories(dir);
  mini_lsm::MiniDB db(dir);
  db.Put("alpha", "1");
  db.Put("beta", "2");
  db.Put("alpha", "3");
  expect(db.Get("alpha").value_or("") == "3", "MemTable returns the latest sequence for a key");
  db.Flush();
  expect(db.SSTableSize() == 2, "SSTable stores sorted unique user keys after flush");
  expect(db.Get("missing") == std::nullopt, "BloomFilter/SSTable path rejects a missing key");
}

void test_wal_recovery() {
  const auto dir = std::filesystem::path("CPP-Practice/leveldb_mini/build/recover_db");
  std::filesystem::remove_all(dir);
  {
    mini_lsm::MiniDB db(dir);
    db.Put("raft", "log");
    db.Put("lsm", "tree");
  }
  mini_lsm::MiniDB recovered(dir);
  recovered.Recover();
  expect(recovered.MemSize() == 2, "WAL replay restores records into MemTable");
  expect(recovered.Get("raft").value_or("") == "log", "Recovered DB can read WAL-restored value");
}

}  // namespace

int main() {
  std::cout << "== Month 07 mini LevelDB demo ==\n";
  test_infrastructure();
  test_skiplist();
  test_memtable_sstable_bloom();
  test_wal_recovery();
  std::cout << "All Month 07 mini LevelDB tests passed.\n";
  return 0;
}