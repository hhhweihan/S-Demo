#pragma once  // 防止头文件重复包含

#include <condition_variable>  // 使用条件变量阻塞等待
#include <cstddef>  // 使用 std::size_t
#include <mutex>  // 使用互斥锁保护队列
#include <queue>  // 使用标准队列存储元素
#include <stdexcept>  // 使用运行时异常
#include <utility>  // 使用 std::move

template <typename T>  // 队列元素类型模板
class BlockingQueue {  // 无界阻塞队列
public:  // 公开接口区
    void push(T value) {  // 写入一个元素
        {  // 限定锁的作用域
            std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护共享队列
            if (shutdown_) {  // 检查队列是否已关闭
                throw std::runtime_error("push on shutdown BlockingQueue");  // 关闭后拒绝写入
            }  // 结束关闭检查
            queue_.push(std::move(value));  // 移动元素进入队列
        }  // 释放互斥锁
        cv_.notify_one();  // 唤醒一个等待消费者
    }  // 结束 push

    T pop() {  // 阻塞弹出一个元素
        std::unique_lock<std::mutex> lock(mutex_);  // 加锁并允许条件变量释放
        cv_.wait(lock, [this] { return shutdown_ || !queue_.empty(); });  // 等待关闭或有数据

        if (queue_.empty()) {  // 关闭且没有数据时无法弹出
            throw std::runtime_error("pop from shutdown BlockingQueue");  // 抛出关闭弹出异常
        }  // 结束空队列检查

        T value = std::move(queue_.front());  // 取出队首元素
        queue_.pop();  // 移除队首槽位
        return value;  // 返回弹出的值
    }  // 结束 pop

    bool try_pop(T& out) {  // 非阻塞尝试弹出
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护队列
        if (queue_.empty()) {  // 检查是否无数据
            return false;  // 空队列弹出失败
        }  // 结束空队列检查

        out = std::move(queue_.front());  // 移出队首元素
        queue_.pop();  // 移除队首槽位
        return true;  // 弹出成功
    }  // 结束 try_pop

    std::size_t size() const {  // 获取当前元素数量
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取队列大小
        return queue_.size();  // 返回队列长度
    }  // 结束 size

    bool empty() const {  // 判断队列是否为空
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取队列状态
        return queue_.empty();  // 返回是否为空
    }  // 结束 empty

    void shutdown() {  // 关闭队列并唤醒等待者
        {  // 限定锁的作用域
            std::lock_guard<std::mutex> lock(mutex_);  // 加锁修改关闭标志
            shutdown_ = true;  // 标记队列已关闭
        }  // 释放互斥锁
        cv_.notify_all();  // 唤醒所有等待线程
    }  // 结束 shutdown

private:  // 私有成员区
    mutable std::mutex mutex_;  // 保护内部队列的互斥锁
    std::condition_variable cv_;  // 消费者等待数据的条件变量
    std::queue<T> queue_;  // 存储元素的底层队列
    bool shutdown_ = false;  // 关闭状态标志
};  // 结束 BlockingQueue
