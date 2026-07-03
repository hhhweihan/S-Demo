#include "smart_ptr.h"  // 引入本目录智能指针实现

#include "../memory_pool/fixed_allocator.h"  // 引入固定块内存分配器
#include "../memory_pool/pool_allocator.h"  // 引入通用池分配器

#include <array>  // 使用固定长度数组
#include <chrono>  // 使用高精度计时
#include <cstddef>  // 使用 std::size_t
#include <iomanip>  // 使用输出格式控制
#include <iostream>  // 使用标准输出
#include <random>  // 使用随机数分布
#include <string>  // 使用字符串名称
#include <vector>  // 预留动态数组支持

namespace {  // 限制基准测试辅助符号作用域

constexpr int kIterations = 1'000'000;  // 定义每项测试循环次数
constexpr std::array<std::size_t, 8> kMixedSizes = {8, 16, 24, 32, 64, 128, 256, 512};  // 定义混合分配尺寸集合

struct BenchResult {  // 保存单次基准测试结果
  std::string name;  // 记录测试名称
  double elapsed_ms;  // 记录耗时毫秒数
};  // 结束结果结构体

struct Payload32 {  // 定义约 32 字节测试负载
  std::array<int, 8> data{};  // 保存整数负载数据
};  // 结束测试负载结构体

template<typename Fn>  // 声明可接收任意可调用对象的模板
BenchResult run_benchmark(const std::string& name, Fn&& fn) {  // 运行指定名称的基准测试
  const auto start = std::chrono::high_resolution_clock::now();  // 记录开始时间点
  fn();  // 执行待测逻辑
  const auto end = std::chrono::high_resolution_clock::now();  // 记录结束时间点
  const auto elapsed = std::chrono::duration<double, std::milli>(end - start).count();  // 计算毫秒耗时
  return BenchResult{name, elapsed};  // 返回测试名称和耗时
}  // 结束通用基准测试函数

double run_fixed_allocator_benchmark() {  // 测量固定分配器性能
  volatile int sink = 0;  // 防止循环被优化掉
  FixedAllocator allocator;  // 创建固定块分配器
  allocator.init(32, static_cast<std::size_t>(kIterations));  // 初始化 32 字节块池

  const BenchResult result = run_benchmark("FixedAllocator 32B", [&]() {  // 统计固定分配器循环耗时
    for (int index = 0; index < kIterations; ++index) {  // 重复执行分配和释放
      void* raw = allocator.allocate();  // 从固定池申请内存
      auto* value = static_cast<int*>(raw);  // 将原始内存视作整数指针
      *value = index;  // 写入当前索引
      sink += *value;  // 累加读取值以保留副作用
      allocator.deallocate(raw);  // 归还内存块
    }  // 结束固定分配器循环
  });  // 完成固定分配器测试

  if (sink == 0) {  // 避免编译器完全忽略 sink
    std::cout << "sink=" << sink << "\n";  // 输出异常累计值
  }  // 结束 sink 检查

  return result.elapsed_ms;  // 返回固定分配器耗时
}  // 结束固定分配器基准函数

double run_pool_allocator_benchmark() {  // 测量通用池分配器性能
  volatile std::size_t sink = 0;  // 防止混合分配循环被优化
  PoolAllocator allocator;  // 创建通用池分配器
  std::mt19937 rng(20260422);  // 创建固定种子的随机引擎
  std::uniform_int_distribution<int> dist(0, static_cast<int>(kMixedSizes.size() - 1));  // 创建尺寸下标分布

  const BenchResult result = run_benchmark("PoolAllocator mixed", [&]() {  // 统计混合尺寸分配耗时
    for (int index = 0; index < kIterations; ++index) {  // 重复执行随机尺寸分配
      const std::size_t size = kMixedSizes[static_cast<std::size_t>(dist(rng))];  // 选择当前分配尺寸
      void* raw = allocator.allocate(size);  // 申请指定大小内存
      auto* bytes = static_cast<unsigned char*>(raw);  // 将内存视作字节数组
      bytes[0] = static_cast<unsigned char>(size);  // 写入尺寸低字节
      sink += bytes[0];  // 累加字节值以保留副作用
      allocator.deallocate(raw);  // 释放当前内存块
    }  // 结束混合尺寸循环
  });  // 完成通用池测试

  allocator.flush_current_thread_cache();  // 清理当前线程缓存

  if (sink == 0) {  // 避免编译器完全忽略 sink
    std::cout << "sink=" << sink << "\n";  // 输出异常累计值
  }  // 结束 sink 检查

  return result.elapsed_ms;  // 返回通用池分配器耗时
}  // 结束通用池基准函数

double run_shared_ptr_benchmark() {  // 测量 SharedPtr 创建销毁性能
  volatile int sink = 0;  // 防止智能指针循环被优化

  const BenchResult result = run_benchmark("SharedPtr create/destroy", [&]() {  // 统计共享指针构造析构耗时
    for (int index = 0; index < kIterations; ++index) {  // 重复创建并销毁共享指针
      SharedPtr<Payload32> ptr(new Payload32{});  // 创建管理测试负载的共享指针
      ptr->data[0] = index;  // 写入负载首元素
      sink += ptr->data[0];  // 累加读取值以保留副作用
    }  // 结束共享指针循环
  });  // 完成共享指针测试

  if (sink == 0) {  // 避免编译器完全忽略 sink
    std::cout << "sink=" << sink << "\n";  // 输出异常累计值
  }  // 结束 sink 检查

  return result.elapsed_ms;  // 返回共享指针耗时
}  // 结束共享指针基准函数

double run_unique_ptr_benchmark() {  // 测量 UniquePtr 创建销毁性能
  volatile int sink = 0;  // 防止独占指针循环被优化

  const BenchResult result = run_benchmark("UniquePtr create/destroy", [&]() {  // 统计独占指针构造析构耗时
    for (int index = 0; index < kIterations; ++index) {  // 重复创建并销毁独占指针
      UniquePtr<Payload32> ptr(new Payload32{});  // 创建管理测试负载的独占指针
      ptr->data[0] = index;  // 写入负载首元素
      sink += ptr->data[0];  // 累加读取值以保留副作用
    }  // 结束独占指针循环
  });  // 完成独占指针测试

  if (sink == 0) {  // 避免编译器完全忽略 sink
    std::cout << "sink=" << sink << "\n";  // 输出异常累计值
  }  // 结束 sink 检查

  return result.elapsed_ms;  // 返回独占指针耗时
}  // 结束独占指针基准函数

}  // namespace

int main() {  // 程序入口
  const double fixed_allocator_ms = run_fixed_allocator_benchmark();  // 运行固定分配器基准
  const double pool_allocator_ms = run_pool_allocator_benchmark();  // 运行通用池分配器基准
  const double shared_ptr_ms = run_shared_ptr_benchmark();  // 运行共享指针基准
  const double unique_ptr_ms = run_unique_ptr_benchmark();  // 运行独占指针基准

  std::cout << std::fixed << std::setprecision(3);  // 设置毫秒输出精度
  std::cout << "Month 1 benchmark summary\n";  // 输出汇总标题
  std::cout << "FixedAllocator 1000000 ops: " << fixed_allocator_ms << " ms\n";  // 输出固定分配器结果
  std::cout << "PoolAllocator mixed 1000000 ops: " << pool_allocator_ms << " ms\n";  // 输出通用池结果
  std::cout << "SharedPtr 1000000 ops: " << shared_ptr_ms << " ms\n";  // 输出共享指针结果
  std::cout << "UniquePtr 1000000 ops: " << unique_ptr_ms << " ms\n";  // 输出独占指针结果
  return 0;  // 返回成功状态
}  // 结束程序入口