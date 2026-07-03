#pragma once  // 防止头文件重复包含

#include <atomic>  // 使用原子读写索引
#include <cstddef>  // 使用 std::size_t
#include <optional>  // 使用可空槽位保存元素
#include <utility>  // 使用 std::move
#include <vector>  // 使用连续缓冲区

template <typename T>  // 队列元素类型模板
class SPSCQueue {  // 单生产者单消费者无锁环形队列
public:  // 公开接口区
    explicit SPSCQueue(std::size_t capacity)  // 构造指定有效容量
        : capacity_(capacity + 1),  // 预留一个空槽区分满和空
          buffer_(capacity_) {}  // 初始化环形缓冲区

    bool push(T value) {  // 尝试写入一个元素
        const std::size_t tail = tail_.load(std::memory_order_relaxed);  // 读取当前写索引
        const std::size_t next = increment(tail);  // 计算写入后的索引
        if (next == head_.load(std::memory_order_acquire)) {  // 检查队列是否已满
            return false;  // 满队列时写入失败
        }  // 结束满队列判断

        buffer_[tail] = std::move(value);  // 将元素移动到当前槽位
        tail_.store(next, std::memory_order_release);  // 发布新的写索引
        return true;  // 写入成功
    }  // 结束 push

    bool pop(T& out) {  // 尝试弹出一个元素
        const std::size_t head = head_.load(std::memory_order_relaxed);  // 读取当前读索引
        if (head == tail_.load(std::memory_order_acquire)) {  // 检查队列是否为空
            return false;  // 空队列时弹出失败
        }  // 结束空队列判断

        out = std::move(*buffer_[head]);  // 移出当前槽位元素
        buffer_[head].reset();  // 清空已消费槽位
        head_.store(increment(head), std::memory_order_release);  // 发布新的读索引
        return true;  // 弹出成功
    }  // 结束 pop

    bool empty() const {  // 判断队列是否为空
        return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_acquire);  // 读写索引相等表示空
    }  // 结束 empty

    std::size_t capacity() const {  // 返回用户可用容量
        return capacity_ - 1;  // 扣除哨兵空槽
    }  // 结束 capacity

private:  // 私有实现区
    std::size_t increment(std::size_t index) const {  // 计算环形后继索引
        return (index + 1) % capacity_;  // 按容量取模回绕
    }  // 结束 increment

    const std::size_t capacity_;  // 包含哨兵槽的实际容量
    std::vector<std::optional<T>> buffer_;  // 环形存储槽数组
    alignas(64) std::atomic<std::size_t> head_{0};  // 缓存行对齐的读索引
    alignas(64) std::atomic<std::size_t> tail_{0};  // 缓存行对齐的写索引
};  // 结束 SPSCQueue
