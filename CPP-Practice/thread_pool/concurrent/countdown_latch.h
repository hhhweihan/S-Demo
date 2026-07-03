#pragma once  // 保证头文件只被包含一次

#include <chrono>  // 引入时间类型支持
#include <cstddef>  // 引入 std::size_t
#include <condition_variable>  // 引入条件变量支持
#include <mutex>  // 引入互斥锁支持
#include <stdexcept>  // 引入标准异常类型

class CountDownLatch {  // 声明倒计时同步器
public:  // 对外公开同步接口
    explicit CountDownLatch(std::size_t count)  // 使用初始计数构造
        : count_(count) {}  // 保存初始计数

    void count_up(std::size_t delta = 1) {  // 增加等待计数
        if (delta == 0) {  // 忽略零增量
            return;  // 直接返回
        }  // 结束零增量检查

        std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护计数
        count_ += delta;  // 增加剩余计数
    }  // 结束 count_up

    void count_down(std::size_t delta = 1) {  // 减少等待计数
        if (delta == 0) {  // 忽略零减量
            return;  // 直接返回
        }  // 结束零减量检查

        std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护计数
        if (delta > count_) {  // 防止计数下溢
            throw std::runtime_error("CountDownLatch underflow");  // 抛出下溢异常
        }  // 结束下溢检查

        count_ -= delta;  // 扣减剩余计数
        if (count_ == 0) {  // 计数归零时释放等待者
            cv_.notify_all();  // 唤醒所有等待线程
        }  // 结束归零检查
    }  // 结束 count_down

    void wait() {  // 阻塞等待计数归零
        std::unique_lock<std::mutex> lock(mutex_);  // 获取可等待互斥锁
        cv_.wait(lock, [this] { return count_ == 0; });  // 等待计数为零
    }  // 结束 wait

    template <typename Rep, typename Period>  // 接受任意 chrono 时长类型
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout) {  // 带超时等待计数归零
        std::unique_lock<std::mutex> lock(mutex_);  // 获取可等待互斥锁
        return cv_.wait_for(lock, timeout, [this] { return count_ == 0; });  // 返回是否在超时前归零
    }  // 结束 wait_for

    std::size_t count() const {  // 查询当前剩余计数
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取计数
        return count_;  // 返回剩余计数
    }  // 结束 count

private:  // 内部计数状态
    mutable std::mutex mutex_;  // 保护计数状态
    std::condition_variable cv_;  // 等待计数归零的条件变量
    std::size_t count_ = 0;  // 当前剩余计数
};  // 结束 CountDownLatch 定义