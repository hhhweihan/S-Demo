#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>

// 写者优先的读写锁：只要有写者在等，新读者就得让路（lock_read 谓词里检查 waiting_writers_），
// 以此避免读者源源不断把写者饿死。代价是读者可能被等待的写者反向饿死，这是刻意的权衡。
class RWMutex {
 public:
    void lock_read() {
        std::unique_lock<std::mutex> lock(mutex_);
        readers_cv_.wait(lock, [this] {
            // 有等待写者时也阻塞新读者——这正是"写者优先"策略的落点。
            return !writing_ && waiting_writers_ == 0;
        });
        ++readers_;
    }

    void unlock_read() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (readers_ == 0) {
            return;
        }

        --readers_;
        // 只有最后一个读者离场才可能让写者进入，故仅在归零时唤醒。
        if (readers_ == 0) {
            writers_cv_.notify_one();
        }
    }

    void lock_write() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 先登记等待写者，这样后续到来的读者会在谓词处让路（见 lock_read）。
        ++waiting_writers_;
        writers_cv_.wait(lock, [this] { return !writing_ && readers_ == 0; });
        --waiting_writers_;
        writing_ = true;
    }

    void unlock_write() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!writing_) {
            return;
        }

        writing_ = false;
        // 有写者在等就把锁交棒给写者（延续写者优先）；否则一次性放行全部读者。
        if (waiting_writers_ > 0) {
            writers_cv_.notify_one();
        } else {
            readers_cv_.notify_all();
        }
    }

 private:
    std::mutex mutex_;
    std::condition_variable readers_cv_;
    std::condition_variable writers_cv_;
    std::size_t readers_ = 0;
    std::size_t waiting_writers_ = 0;
    bool writing_ = false;
};
