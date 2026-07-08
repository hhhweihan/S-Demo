#pragma once  // 防止头文件被重复包含

#include <coroutine>  // 引入 C++20 协程句柄和挂起类型
#include <chrono>  // 引入时间间隔类型
#include <deque>  // 引入双端队列容器
#include <functional>  // 引入函数对象支持
#include <optional>  // 引入可选值类型
#include <queue>  // 引入队列容器
#include <stdexcept>  // 引入标准异常类型
#include <string>  // 引入字符串类型
#include <thread>  // 引入线程休眠功能
#include <utility>  // 引入移动和交换工具
#include <vector>  // 引入动态数组容器

namespace mini_coro {  // 迷你协程库命名空间

class Scheduler {  // 简单 FIFO 协程调度器
 public:  // 公共调度接口
  void schedule(std::coroutine_handle<> handle) { ready_.push(handle); }  // 将协程句柄加入就绪队列
  void run() {  // 运行所有就绪协程直到队列清空
    while (!ready_.empty()) {  // 持续处理当前就绪队列
      auto handle = ready_.front();  // 取出队首协程句柄
      ready_.pop();  // 从队列移除已取出的句柄
      if (!handle.done()) handle.resume();  // 未完成时恢复协程执行
    }  // 结束就绪队列循环
  }  // 结束调度器运行函数
  std::size_t pending() const { return ready_.size(); }  // 返回等待调度的协程数量
 private:  // 私有状态
  std::queue<std::coroutine_handle<>> ready_;  // 保存就绪协程句柄的队列
};  // 结束 Scheduler 类定义

inline Scheduler*& current_scheduler() {  // 获取当前线程绑定的调度器指针引用
  static thread_local Scheduler* scheduler = nullptr;  // 为每个线程保存独立调度器指针
  return scheduler;  // 返回可修改的调度器指针引用
}  // 结束当前调度器访问函数

class SchedulerScope {  // RAII 方式临时设置当前线程调度器
 public:  // 公共构造和析构接口
  explicit SchedulerScope(Scheduler& scheduler) : previous_(current_scheduler()) { current_scheduler() = &scheduler; }  // 保存旧调度器并设置新调度器
  ~SchedulerScope() { current_scheduler() = previous_; }  // 作用域结束时恢复旧调度器
 private:  // 私有状态
  Scheduler* previous_;  // 保存进入作用域前的调度器指针
};  // 结束 SchedulerScope 类定义

struct YieldAwaiter {  // 表示主动让出执行权的 awaiter
  bool await_ready() const noexcept { return false; }  // 总是挂起以便交还调度器
  void await_suspend(std::coroutine_handle<> handle) const { current_scheduler()->schedule(handle); }  // 挂起时把当前协程重新排队
  void await_resume() const noexcept {}  // 恢复时无需返回值
};  // 结束 YieldAwaiter 结构定义

inline YieldAwaiter yield() { return {}; }  // 创建一个让出执行权的 awaiter

class SleepAwaiter {  // 使用线程休眠模拟异步等待的 awaiter
 public:  // 公共 awaiter 接口
  explicit SleepAwaiter(std::chrono::milliseconds duration) : duration_(duration) {}  // 保存等待时长
  bool await_ready() const noexcept { return duration_.count() == 0; }  // 零时长时无需挂起
  void await_suspend(std::coroutine_handle<> handle) const {  // 挂起后执行阻塞式休眠模拟等待
    std::this_thread::sleep_for(duration_);  // 当前线程休眠指定时长
    current_scheduler()->schedule(handle);  // 休眠结束后重新调度协程
  }  // 结束挂起处理函数
  void await_resume() const noexcept {}  // 恢复时无需返回值
 private:  // 私有状态
  std::chrono::milliseconds duration_;  // 保存等待时长
};  // 结束 SleepAwaiter 类定义

inline SleepAwaiter async_sleep(std::chrono::milliseconds duration) { return SleepAwaiter(duration); }  // 创建睡眠 awaiter

class Task {  // 表示无返回值协程任务的拥有型句柄包装
 public:  // 公共任务接口
  struct promise_type {  // 定义 Task 对应的协程 promise 类型
    Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(*this)); }  // 从 promise 构造任务对象
    std::suspend_always initial_suspend() noexcept { return {}; }  // 创建后先挂起等待显式启动
    std::suspend_always final_suspend() noexcept { return {}; }  // 完成后保持挂起以便外部销毁句柄
    void return_void() noexcept {}  // 支持无返回值协程正常结束
    void unhandled_exception() { throw; }  // 未处理异常直接重新抛出
  };  // 结束 promise_type 结构定义

  explicit Task(std::coroutine_handle<promise_type> handle) : handle_(handle) {}  // 接管协程句柄所有权
  Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}  // 移动构造并清空来源句柄
  Task& operator=(Task&& other) noexcept {  // 移动赋值任务句柄所有权
    if (this != &other) {  // 避免自移动赋值
      if (handle_) handle_.destroy();  // 释放当前持有的协程句柄
      handle_ = std::exchange(other.handle_, {});  // 接管来源句柄并清空来源
    }  // 结束非自赋值处理
    return *this;  // 返回当前任务对象
  }  // 结束移动赋值运算符
  Task(const Task&) = delete;  // 禁止复制任务句柄所有权
  Task& operator=(const Task&) = delete;  // 禁止复制赋值任务句柄所有权
  ~Task() { if (handle_) handle_.destroy(); }  // 析构时销毁仍持有的协程句柄
  void start(Scheduler& scheduler) { scheduler.schedule(handle_); }  // 将任务加入指定调度器
  bool done() const { return !handle_ || handle_.done(); }  // 判断任务是否为空或已完成
 private:  // 私有状态
  std::coroutine_handle<promise_type> handle_;  // 当前任务持有的协程句柄
};  // 结束 Task 类定义

template <typename T>  // 声明通道中传输的元素类型
class Channel {  // 简单有界缓冲通道
 public:  // 公共通道接口
  explicit Channel(std::size_t capacity = 0) : capacity_(capacity) {}  // 保存通道容量配置
  bool send(T value) {  // 尝试向通道发送一个值
    if (capacity_ == 0 && !queue_.empty()) return false;  // 零容量模式下已有数据则发送失败
    if (capacity_ > 0 && queue_.size() >= capacity_) return false;  // 有界缓冲已满则发送失败
    queue_.push_back(std::move(value));  // 将值移动到队尾
    return true;  // 表示发送成功
  }  // 结束发送函数
  std::optional<T> recv() {  // 尝试从通道接收一个值
    if (queue_.empty()) return std::nullopt;  // 队列为空时返回空值
    T value = std::move(queue_.front());  // 移出队首元素
    queue_.pop_front();  // 删除已接收的队首元素
    return value;  // 返回接收到的值
  }  // 结束接收函数
  bool empty() const { return queue_.empty(); }  // 判断通道当前是否为空
  std::size_t size() const { return queue_.size(); }  // 返回当前缓冲元素数量
  std::size_t capacity() const { return capacity_; }  // 返回通道容量配置
 private:  // 私有状态
  std::size_t capacity_;  // 最大缓冲容量，零表示只能保留一个待收值
  std::deque<T> queue_;  // 保存通道缓冲数据
};  // 结束 Channel 类模板定义

template <typename T>  // 声明选择接收的通道元素类型
std::optional<T> select_recv(std::vector<Channel<T>*>& channels) {  // 从多个通道中接收首个可用值
  for (auto* channel : channels) {  // 按给定顺序遍历通道指针
    if (auto value = channel->recv()) return value;  // 某通道有值时立即返回
  }  // 结束通道遍历循环
  return std::nullopt;  // 所有通道都无值时返回空值
}  // 结束选择接收函数

class AsyncHttpClientCore {  // 模拟异步 HTTP 客户端核心
 public:  // 公共请求接口
  Task get(std::string url, Channel<std::string>& out) {  // 创建一个模拟 GET 请求任务
    co_await async_sleep(std::chrono::milliseconds(1));  // 模拟短暂异步等待
    out.send("GET " + url + " -> 200 OK");  // 将模拟响应写入输出通道
  }  // 结束 GET 请求任务
};  // 结束 AsyncHttpClientCore 类定义

}  // namespace mini_coro