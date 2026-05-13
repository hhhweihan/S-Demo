#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>

class RWMutex {
public:
    void lock_read() {
        std::unique_lock<std::mutex> lock(mutex_);
        readers_cv_.wait(lock, [this] {
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
        if (readers_ == 0) {
            writers_cv_.notify_one();
        }
    }

    void lock_write() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++waiting_writers_;
        writers_cv_.wait(lock, [this] {
            return !writing_ && readers_ == 0;
        });
        --waiting_writers_;
        writing_ = true;
    }

    void unlock_write() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!writing_) {
            return;
        }

        writing_ = false;
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