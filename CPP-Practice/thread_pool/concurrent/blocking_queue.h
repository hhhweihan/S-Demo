#pragma once  // 保证头文件只被包含一次

#include <condition_variable>  // 引入条件变量支持
#include <cstddef>  // 引入 std::size_t
#include <mutex>  // 引入互斥锁支持
#include <queue>  // 引入标准队列容器
#include <stdexcept>  // 引入标准异常类型
#include <utility>  // 引入移动语义工具

template <typename T>  // 定义阻塞队列元素类型
class BlockingQueue {  // 声明线程安全阻塞队列
public:  // 对外公开队列操作
    void push(T value) {  // 向队列写入一个元素
        {  // 限定加锁作用域
            std::lock_guard<std::mutex> lock(mutex_);  // 加锁保护共享队列
            if (shutdown_) {  // 拒绝关闭后的写入
                throw std::runtime_error("push on shutdown BlockingQueue");  // 抛出关闭写入异常
            }  // 结束关闭检查
            queue_.push(std::move(value));  // 移动元素进入队尾
        }  // 释放队列互斥锁
        cv_.notify_one();  // 唤醒一个等待消费者
    }  // 结束 push

    T pop() {  // 阻塞弹出一个元素
        std::unique_lock<std::mutex> lock(mutex_);  // 获取可等待的互斥锁
        cv_.wait(lock, [this] { return shutdown_ || !queue_.empty(); });  // 等待关闭或队列非空

        if (queue_.empty()) {  // 关闭且无元素时无法弹出
            throw std::runtime_error("pop from shutdown BlockingQueue");  // 抛出关闭弹出异常
        }  // 结束空队列检查

        T value = std::move(queue_.front());  // 移出队首元素
        queue_.pop();  // 移除已取出的队首节点
        return value;  // 返回取出的元素
    }  // 结束 pop

    bool try_pop(T& out) {  // 尝试非阻塞弹出元素
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁检查队列状态
        if (queue_.empty()) {  // 队列为空时立即失败
            return false;  // 返回未取到元素
        }  // 结束空队列检查
        out = std::move(queue_.front());  // 将队首元素移出到输出参数
        queue_.pop();  // 删除队首节点
        return true;  // 返回取出成功
    }  // 结束 try_pop

    std::size_t size() const {  // 查询当前队列大小
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取队列大小
        return queue_.size();  // 返回元素数量
    }  // 结束 size

    bool empty() const {  // 判断队列是否为空
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁读取队列状态
        return queue_.empty();  // 返回是否为空
    }  // 结束 empty

    void shutdown() {  // 关闭队列并唤醒等待者
        {  // 限定关闭标志加锁作用域
            std::lock_guard<std::mutex> lock(mutex_);  // 加锁修改关闭状态
            shutdown_ = true;  // 标记队列已经关闭
        }  // 释放关闭状态互斥锁
        cv_.notify_all();  // 唤醒所有等待线程
    }  // 结束 shutdown

private:  // 内部同步状态
    mutable std::mutex mutex_;  // 保护队列和关闭标志
    std::condition_variable cv_;  // 协调生产者和消费者等待
    std::queue<T> queue_;  // 保存排队元素
    bool shutdown_ = false;  // 记录队列是否关闭
};  // 结束 BlockingQueue 定义