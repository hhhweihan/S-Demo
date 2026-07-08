#pragma once  // 防止头文件重复包含

#include <algorithm>  // 使用标准算法
#include <cstddef>  // 使用 size_t
#include <deque>  // 使用双端队列
#include <functional>  // 使用函数对象
#include <limits>  // 保留数值边界工具
#include <map>  // 使用有序映射
#include <optional>  // 使用可选返回值
#include <queue>  // 使用优先队列
#include <stdexcept>  // 使用标准异常
#include <string>  // 使用字符串类型
#include <unordered_map>  // 使用哈希映射
#include <utility>  // 使用移动语义
#include <vector>  // 使用动态数组容器

namespace mini_hpn {  // 高性能网络练习命名空间

class TimerWheel {  // 简化时间轮
 public:  // 暴露时间轮操作接口
  using Callback = std::function<void(int)>;  // 定义超时回调类型
  explicit TimerWheel(std::size_t slots, Callback timeout_cb) : slots_(slots), timeout_cb_(std::move(timeout_cb)) {}  // 初始化槽数和回调
  void add_or_refresh(int connection_id, std::size_t timeout_ticks) {  // 添加或刷新连接超时
    std::size_t expire_tick = now_ + timeout_ticks;  // 计算绝对过期刻度
    timers_[connection_id] = expire_tick;  // 记录连接最新过期刻度
    wheel_[expire_tick % slots_].push_back(connection_id);  // 放入对应时间轮槽
  }  // add_or_refresh 结束
  void tick() {  // 推进一个时钟刻度
    ++now_;  // 更新当前刻度
    auto bucket_index = now_ % slots_;  // 计算当前槽下标
    auto bucket = std::move(wheel_[bucket_index]);  // 取出当前槽的候选连接
    wheel_[bucket_index].clear();  // 清空当前槽
    for (int id : bucket) {  // 遍历候选连接
      auto it = timers_.find(id);  // 查找连接最新过期记录
      if (it != timers_.end() && it->second <= now_) {  // 确认连接确实已过期
        timers_.erase(it);  // 移除定时记录
        timeout_cb_(id);  // 触发超时回调
      }  // 过期确认分支结束
    }  // 当前槽遍历结束
  }  // tick 结束
  std::size_t now() const { return now_; }  // 返回当前刻度
 private:  // 保存时间轮内部状态
  std::size_t slots_;  // 时间轮槽数量
  std::size_t now_ = 0;  // 当前时钟刻度
  std::map<int, std::size_t> timers_;  // 连接到绝对过期刻度的映射
  std::unordered_map<std::size_t, std::vector<int>> wheel_;  // 槽位到连接列表的映射
  Callback timeout_cb_;  // 超时回调函数
};  // TimerWheel 定义结束

class PrecisionTimerHeap {  // 精确定时器小根堆
 public:  // 暴露定时器堆操作接口
  void add(int id, std::size_t expire_at) { heap_.push({expire_at, id}); }  // 添加定时器
  std::vector<int> pop_expired(std::size_t now) {  // 弹出所有已过期定时器
    std::vector<int> expired;  // 保存过期编号
    while (!heap_.empty() && heap_.top().expire_at <= now) {  // 持续处理堆顶过期项
      expired.push_back(heap_.top().id);  // 记录过期编号
      heap_.pop();  // 移除堆顶项
    }  // 过期弹出循环结束
    return expired;  // 返回过期编号列表
  }  // pop_expired 结束
 private:  // 保存定时器堆内部容器
  struct Timer { std::size_t expire_at; int id; bool operator>(const Timer& other) const { return expire_at > other.expire_at; } };  // 定义堆元素和比较规则
  std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer>> heap_;  // 保存按过期时间排序的小根堆
};  // PrecisionTimerHeap 定义结束

struct IoRequest { int id; std::string op; std::string payload; };  // 描述一个 I/O 提交请求
struct IoCompletion { int id; std::string result; };  // 描述一个 I/O 完成结果

class IoUringSimulator {  // io_uring 提交/完成队列模拟器
 public:  // 暴露提交、处理和完成读取接口
  void submit(IoRequest request) { sq_.push_back(std::move(request)); }  // 将请求加入提交队列
  void process() {  // 处理所有提交队列请求
    while (!sq_.empty()) {  // 持续处理直到提交队列为空
      auto request = std::move(sq_.front());  // 取出队首请求
      sq_.pop_front();  // 移除已取出的请求
      cq_.push_back({request.id, request.op + ":" + request.payload});  // 生成完成项
    }  // 提交队列处理循环结束
  }  // process 结束
  std::optional<IoCompletion> peek_completion() {  // 获取一个完成项
    if (cq_.empty()) return std::nullopt;  // 无完成项时返回空值
    IoCompletion completion = std::move(cq_.front());  // 取出队首完成项
    cq_.pop_front();  // 移除已取出的完成项
    return completion;  // 返回完成项
  }  // peek_completion 结束
  std::size_t pending() const { return sq_.size(); }  // 返回提交队列长度
 private:  // 保存提交队列和完成队列
  std::deque<IoRequest> sq_;  // 提交队列
  std::deque<IoCompletion> cq_;  // 完成队列
};  // IoUringSimulator 定义结束

struct Backend {  // 后端服务状态
  std::string address;  // 后端地址
  bool healthy = true;  // 健康状态
  int active = 0;  // 当前活跃连接数
};  // Backend 定义结束

class RoundRobinBalancer {  // 轮询负载均衡器
 public:  // 暴露轮询选择接口
  explicit RoundRobinBalancer(std::vector<Backend>* backends) : backends_(backends) {}  // 绑定共享后端列表
  Backend& pick() {  // 选择一个健康后端
    for (std::size_t attempts = 0; attempts < backends_->size(); ++attempts) {  // 最多尝试所有后端
      Backend& backend = (*backends_)[next_++ % backends_->size()];  // 按轮询顺序取后端
      if (backend.healthy) return backend;  // 返回第一个健康后端
    }  // 尝试所有后端结束
    throw std::runtime_error("no healthy backend");  // 全部不可用时抛出异常
  }  // pick 结束
 private:  // 保存后端列表引用和轮询位置
  std::vector<Backend>* backends_;  // 指向共享后端列表
  std::size_t next_ = 0;  // 下一次轮询起点
};  // RoundRobinBalancer 定义结束

class LeastConnBalancer {  // 最少连接负载均衡器
 public:  // 暴露最少连接选择接口
  explicit LeastConnBalancer(std::vector<Backend>* backends) : backends_(backends) {}  // 绑定共享后端列表
  Backend& pick() {  // 选择健康且连接数最少的后端
    auto it = std::min_element(backends_->begin(), backends_->end(), [](const Backend& lhs, const Backend& rhs) {  // 查找最优后端
      if (lhs.healthy != rhs.healthy) return lhs.healthy > rhs.healthy;  // 健康后端优先
      return lhs.active < rhs.active;  // 活跃连接更少者优先
    });  // 最优后端查找结束
    if (it == backends_->end() || !it->healthy) throw std::runtime_error("no healthy backend");  // 无健康后端时抛出异常
    return *it;  // 返回选中的后端
  }  // pick 结束
 private:  // 保存后端列表引用
  std::vector<Backend>* backends_;  // 指向共享后端列表
};  // LeastConnBalancer 定义结束

class ConnectionPool {  // 简化连接池
 public:  // 暴露连接获取、释放和状态更新接口
  explicit ConnectionPool(std::vector<Backend> backends) : backends_(std::move(backends)), rr_(&backends_), least_(&backends_) {}  // 初始化后端和均衡器
  std::string acquire_round_robin() {  // 使用轮询策略获取连接
    Backend& backend = rr_.pick();  // 选择后端
    ++backend.active;  // 增加活跃连接计数
    return backend.address;  // 返回后端地址
  }  // acquire_round_robin 结束
  std::string acquire_least_conn() {  // 使用最少连接策略获取连接
    Backend& backend = least_.pick();  // 选择后端
    ++backend.active;  // 增加活跃连接计数
    return backend.address;  // 返回后端地址
  }  // acquire_least_conn 结束
  void release(const std::string& address) {  // 释放指定后端连接
    for (auto& backend : backends_) if (backend.address == address && backend.active > 0) --backend.active;  // 命中后减少活跃计数
  }  // release 结束
  void mark_unhealthy(const std::string& address) {  // 标记指定后端不可用
    for (auto& backend : backends_) if (backend.address == address) backend.healthy = false;  // 命中后更新健康状态
  }  // mark_unhealthy 结束
  int active(const std::string& address) const {  // 查询指定后端活跃连接数
    for (const auto& backend : backends_) if (backend.address == address) return backend.active;  // 命中后返回计数
    return 0;  // 未找到时返回 0
  }  // active 结束
 private:  // 保存后端列表和均衡器
  std::vector<Backend> backends_;  // 后端列表
  RoundRobinBalancer rr_;  // 轮询均衡器
  LeastConnBalancer least_;  // 最少连接均衡器
};  // ConnectionPool 定义结束

struct OptimizationReport {  // 平台优化能力报告
  bool zero_copy_supported_by_os = false;  // 操作系统是否支持零拷贝
  bool reuse_port_supported_by_os = false;  // 操作系统是否支持端口复用
  std::string recommendation;  // 平台优化建议
};  // OptimizationReport 定义结束

inline OptimizationReport analyze_platform(bool linux_like) {  // 根据平台类型生成优化报告
  return {linux_like, linux_like, linux_like ? "enable sendfile/splice and SO_REUSEPORT" : "use portable core tests; run Linux/WSL2 for zero-copy benchmarks"};  // 返回对应平台建议
}  // analyze_platform 结束

}  // namespace mini_hpn