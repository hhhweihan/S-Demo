#include <cstdlib>  // 使用退出函数
#include <filesystem>  // 使用文件系统路径
#include <iostream>  // 使用标准输入输出
#include <string>  // 使用字符串
#include <vector>  // 使用动态数组

#include "mini_leveldb.h"  // 引入迷你 LevelDB 实现

namespace {  // 限定本文件内部测试工具

void expect(bool condition, const std::string& message) {  // 检查测试条件并输出结果
  if (!condition) {  // 条件失败时进入错误分支
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息
    std::exit(1);  // 立即终止进程
  }  // 结束失败分支
  std::cout << "[PASS] " << message << '\n';  // 输出通过信息
}  // 结束测试断言函数

void test_infrastructure() {  // 测试基础设施组件
  mini_lsm::Slice slice("leveldb");  // 创建字符串视图封装
  mini_lsm::Arena arena;  // 创建简单内存分配器
  void* block = arena.Allocate(128);  // 分配一块测试内存
  expect(slice.ToString() == "leveldb", "Slice references external bytes");  // 验证 Slice 内容
  expect(block != nullptr && arena.memory_usage() == 128, "Arena tracks allocated bytes");  // 验证 Arena 计量
  expect(mini_lsm::Status::OK().ok(), "Status::OK reports success");  // 验证成功状态
}  // 结束基础设施测试

void test_skiplist() {  // 测试跳表有序性和查找
  mini_lsm::SkipList<int, std::string> list;  // 创建整数到字符串的跳表
  for (int value : {5, 3, 8, 1, 7}) list.Insert(value, std::to_string(value));  // 插入乱序键值
  std::vector<int> keys;  // 保存迭代得到的键
  for (auto it = list.begin(); it != list.end(); ++it) keys.push_back(it.key());  // 收集跳表键序列
  expect((keys == std::vector<int>{1, 3, 5, 7, 8}), "SkipList iterates keys in sorted order");  // 验证升序遍历
  expect(list.Find(7).value_or("") == "7", "SkipList finds an inserted key");  // 验证已插入键可查
}  // 结束跳表测试

void test_memtable_sstable_bloom() {  // 测试内存表、SSTable 和布隆过滤器
  const auto dir = std::filesystem::path("CPP-Practice/leveldb_mini/build/db");  // 设置测试数据库目录
  std::filesystem::remove_all(dir);  // 清理旧测试目录
  std::filesystem::create_directories(dir);  // 创建测试目录
  mini_lsm::MiniDB db(dir);  // 创建迷你数据库实例
  db.Put("alpha", "1");  // 写入 alpha 旧值
  db.Put("beta", "2");  // 写入 beta 值
  db.Put("alpha", "3");  // 写入 alpha 新值
  expect(db.Get("alpha").value_or("") == "3", "MemTable returns the latest sequence for a key");  // 验证读取最新值
  db.Flush();  // 将内存表刷入 SSTable
  expect(db.SSTableSize() == 2, "SSTable stores sorted unique user keys after flush");  // 验证去重后条目数
  expect(db.Get("missing") == std::nullopt, "BloomFilter/SSTable path rejects a missing key");  // 验证缺失键返回空
}  // 结束存储路径测试

void test_wal_recovery() {  // 测试 WAL 恢复流程
  const auto dir = std::filesystem::path("CPP-Practice/leveldb_mini/build/recover_db");  // 设置恢复测试目录
  std::filesystem::remove_all(dir);  // 清理旧恢复目录
  {  // 限定数据库写入生命周期
    mini_lsm::MiniDB db(dir);  // 创建待恢复数据库
    db.Put("raft", "log");  // 写入第一条日志记录
    db.Put("lsm", "tree");  // 写入第二条日志记录
  }  // 结束写入生命周期
  mini_lsm::MiniDB recovered(dir);  // 创建用于恢复的数据库对象
  recovered.Recover();  // 回放 WAL 记录
  expect(recovered.MemSize() == 2, "WAL replay restores records into MemTable");  // 验证恢复记录数
  expect(recovered.Get("raft").value_or("") == "log", "Recovered DB can read WAL-restored value");  // 验证恢复后可读
}  // 结束 WAL 恢复测试

}  // namespace

int main() {  // 程序入口
  std::cout << "== Month 07 mini LevelDB demo ==\n";  // 输出测试标题
  test_infrastructure();  // 运行基础设施测试
  test_skiplist();  // 运行跳表测试
  test_memtable_sstable_bloom();  // 运行存储路径测试
  test_wal_recovery();  // 运行 WAL 恢复测试
  std::cout << "All Month 07 mini LevelDB tests passed.\n";  // 输出全部通过信息
  return 0;  // 返回成功状态
}  // 结束程序入口