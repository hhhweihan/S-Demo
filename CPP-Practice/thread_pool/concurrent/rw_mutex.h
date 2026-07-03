#pragma once  // 保证头文件只被包含一次

#include <condition_variable>  // 引入条件变量支持
#include <cstddef>  // 引入 std::size_t
#include <mutex>  // 引入互斥锁支持

class RWMutex {  // 声明读写互斥锁
public:  // 对外公开加锁接口
    void lock_read() {  // 获取读锁
        std::unique_lock<std::mutex> lock(mutex_);  // 获取可等待互斥锁
        readers_cv_.wait(lock, [this] {  // 等待读者可进入
            return !writing_ && waiting_writers_ == 0;  // 无写者且无等待写者时允许读
        });  // 完成读锁等待
        ++readers_;  // 增加活跃读者数
    }  // 结束 lock_read

    void unlock_read() {  // 释放读锁
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁修改读者数
        if (readers_ == 0) {  // 防御无读者释放
            return;  // 直接返回
        }  // 结束无读者检查

        --readers_;  // 减少活跃读者数
        if (readers_ == 0) {  // 最后一个读者离开
            writers_cv_.notify_one();  // 唤醒一个等待写者
        }  // 结束读者归零检查
    }  // 结束 unlock_read

    void lock_write() {  // 获取写锁
        std::unique_lock<std::mutex> lock(mutex_);  // 获取可等待互斥锁
        ++waiting_writers_;  // 记录一个等待写者
        writers_cv_.wait(lock, [this] {  // 等待写者可进入
            return !writing_ && readers_ == 0;  // 无写者且无读者时允许写
        });  // 完成写锁等待
        --waiting_writers_;  // 移除等待写者记录
        writing_ = true;  // 标记当前正在写入
    }  // 结束 lock_write

    void unlock_write() {  // 释放写锁
        std::lock_guard<std::mutex> lock(mutex_);  // 加锁修改写状态
        if (!writing_) {  // 防御无写者释放
            return;  // 直接返回
        }  // 结束无写者检查

        writing_ = false;  // 清除写入标志
        if (waiting_writers_ > 0) {  // 优先唤醒等待写者
            writers_cv_.notify_one();  // 唤醒一个写者
        } else {  // 没有等待写者时释放读者
            readers_cv_.notify_all();  // 唤醒所有读者
        }  // 结束唤醒分支
    }  // 结束 unlock_write

private:  // 内部锁状态
    std::mutex mutex_;  // 保护读写锁内部状态
    std::condition_variable readers_cv_;  // 等待读锁的条件变量
    std::condition_variable writers_cv_;  // 等待写锁的条件变量
    std::size_t readers_ = 0;  // 当前活跃读者数量
    std::size_t waiting_writers_ = 0;  // 当前等待写者数量
    bool writing_ = false;  // 当前是否有写者持锁
};  // 结束 RWMutex 定义