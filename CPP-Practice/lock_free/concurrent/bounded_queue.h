#pragma once  // 防止头文件重复包含

#include <condition_variable>  // 使用条件变量等待容量和数据
#include <cstddef>  // 使用 std::size_t
#include <mutex>  // 使用互斥锁保护状态
#include <queue>  // 使用标准队列存储元素
#include <stdexcept>  // 使用异常报告非法状态
#include <utility>  // 使用 std::move

template <typename T>  // 队列元素类型模板
class BoundedQueue {  // 有界阻塞队列
public:  // 公开接口区
    explicit BoundedQueue(std::size_t capacity)  // 构造指定容量队列
        : capacity_(capacity) {  // 保存容量上限
        if (capacity_ == 0) {  // 禁止零容量队列
            throw std::invalid_argument("capacity must be > 0");  // 抛出非法容量异常
        }  // 结束容量检查
    }  // 结束构造函数

    void push(T value) {  // 阻塞写入一个元素
        std::unique_lock<std::mutex> lock(mutex_);  // 加锁并允许等待时释放
        not_full_.wait(lock, [this] { return shutdown_ || queue_.size() < capacity_; });  // 等待未满或关闭

        if (shutdown_) {  // 检查是否已关闭
            throw std::runtime_error("push on shutdown BoundedQueue");  // 关闭后拒绝写入
        }  // 结束关闭检查

        queue_.push(std::move(value));  // 移动元素入队
        lock.unlock();  // 先释放锁再通知
        not_empty_.notify_one();  // 唤醒一个等待消费者
    }  // 结束 push

    T pop() {  // 阻塞弹出一个元素
        std::unique_lock<std::mutex> lock(mutex_);  // 加锁并允许等待时释放
        not_empty_.wait(lock, [this] { return shutdown_ || !queue_.empty(); });  // 等待有数据或关闭

        if (queue_.empty()) {  // 关闭且无数据时无法弹出
            throw std::runtime_error("pop from shutdown BoundedQueue");  // 抛出关闭弹出异常
        }  // 结束空队列检查

        T value = std::move(queue_.front());  // 移出队首元素
        queue_.pop();  // 移除队首槽位
        lock.unlock();  // 先释放锁再通知
        not_full_.notify_one();  // 唤醒一个等待生产者
        return value;  // 返回弹出的值
    }  // 结束 pop

    bool try_push(T value) {  // 非阻塞尝试写入
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护队列
        if (shutdown_ || queue_.size() >= capacity_) {  // 检查关闭或满队列
            return false;  // 当前不能写入
        }  // 结束可写检查

        queue_.push(std::move(value));  // 移动元素入队
        not_empty_.notify_one();  // 通知一个等待消费者
        return true;  // 写入成功
    }  // 结束 try_push

    bool try_pop(T& out) {  // 非阻塞尝试弹出
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护队列
        if (queue_.empty()) {  // 检查是否为空
            return false;  // 无数据时弹出失败
        }  // 结束空队列检查

        out = std::move(queue_.front());  // 移出队首元素
        queue_.pop();  // 移除队首槽位
        not_full_.notify_one();  // 通知一个等待生产者
        return true;  // 弹出成功
    }  // 结束 try_pop

    std::size_t size() const {  // 获取当前元素数量
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取大小
        return queue_.size();  // 返回队列长度
    }  // 结束 size

    bool empty() const {  // 判断队列是否为空
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取状态
        return queue_.empty();  // 返回是否为空
    }  // 结束 empty

    void shutdown() {  // 关闭队列并唤醒等待者
        {  // 限定锁作用域
            std::lock_guard<std::mutex> lock(mutex_);  // 加锁修改关闭标志
            shutdown_ = true;  // 标记队列已关闭
        }  // 释放互斥锁
        not_empty_.notify_all();  // 唤醒所有消费者
        not_full_.notify_all();  // 唤醒所有生产者
    }  // 结束 shutdown

private:  // 私有成员区
    const std::size_t capacity_;  // 队列容量上限
    mutable std::mutex mutex_;  // 保护队列状态的互斥锁
    std::condition_variable not_empty_;  // 等待非空状态的条件变量
    std::condition_variable not_full_;  // 等待非满状态的条件变量
    std::queue<T> queue_;  // 底层元素队列
    bool shutdown_ = false;  // 关闭状态标志
};  // 结束 BoundedQueue
