#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <functional>
#include <limits>
#include <list>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mini_hpn {

// 时间轮：add/refresh/cancel/查询均为摊还 O(1)。索引用 unordered_map（哈希，非 std::map
// 的 O(log n)）；每个槽是一条 std::list，索引里连同截止刻度一并存下该连接在槽链表中的
// 迭代器，故刷新/取消都能凭迭代器 O(1) 摘除旧节点，链表其余节点的迭代器不受影响。
class TimerWheel {
 public:
    using Callback = std::function<void(int)>;
    explicit TimerWheel(std::size_t slots, Callback timeout_cb)
        : slots_(slots), timeout_cb_(std::move(timeout_cb)) {}
    void add_or_refresh(int connection_id, std::size_t timeout_ticks) {
        std::size_t expire_tick = now_ + timeout_ticks;
        std::size_t slot = expire_tick % slots_;
        auto it = timers_.find(connection_id);
        if (it != timers_.end()) {
            // 刷新：凭存下的迭代器 O(1) 从旧槽摘除，再挂到新槽，避免旧槽残留过期项。
            wheel_[it->second.slot].erase(it->second.slot_it);
            auto& bucket = wheel_[slot];
            auto pos = bucket.insert(bucket.end(), connection_id);
            it->second = Entry{expire_tick, slot, pos};
        } else {
            auto& bucket = wheel_[slot];
            auto pos = bucket.insert(bucket.end(), connection_id);
            timers_.emplace(connection_id, Entry{expire_tick, slot, pos});
        }
    }
    // 取消一个待触发定时器：O(1) 从所在槽与索引一并摘除。取消不存在的键为安全空操作。
    // 成功取消返回 true，键不存在返回 false。
    bool cancel(int connection_id) {
        auto it = timers_.find(connection_id);
        if (it == timers_.end()) return false;  // 未知键：什么都不做
        wheel_[it->second.slot].erase(it->second.slot_it);  // 凭迭代器 O(1) 摘除
        timers_.erase(it);
        return true;
    }
    void tick() {
        ++now_;
        auto& bucket = wheel_[now_ % slots_];
        for (auto it = bucket.begin(); it != bucket.end();) {
            int id = *it;
            auto ti = timers_.find(id);
            // 只有截止刻度确实到期才触发；未到期项（跨圈情形）保留在槽内待后续圈处理。
            if (ti != timers_.end() && ti->second.expire_tick <= now_) {
                it = bucket.erase(it);  // O(1) 从槽摘除，list::erase 返回下一节点
                timers_.erase(ti);
                timeout_cb_(id);
            } else {
                ++it;
            }
        }
    }
    std::size_t now() const { return now_; }

 private:
    struct Entry {
        std::size_t expire_tick;             // 最新绝对截止刻度（唯一真相）
        std::size_t slot;                    // 当前所在槽下标
        std::list<int>::iterator slot_it;    // 在槽链表中的位置，供 O(1) 摘除
    };
    std::size_t slots_;
    std::size_t now_ = 0;
    std::unordered_map<int, Entry> timers_;          // 连接 -> 截止刻度 + 槽内位置
    std::unordered_map<std::size_t, std::list<int>>  // 槽 -> 该刻度待触发的连接链表
        wheel_;
    Callback timeout_cb_;
};

// 小根堆定时器：适合需要精确到期时间的场景，堆顶即最近截止。
class PrecisionTimerHeap {
 public:
    void add(int id, std::size_t expire_at) { heap_.push({expire_at, id}); }
    std::vector<int> pop_expired(std::size_t now) {
        std::vector<int> expired;
        while (!heap_.empty() && heap_.top().expire_at <= now) {  // now 处到期也算（闭区间）
            expired.push_back(heap_.top().id);
            heap_.pop();
        }
        return expired;
    }

 private:
    struct Timer {
        std::size_t expire_at;
        int id;
        bool operator>(const Timer& other) const { return expire_at > other.expire_at; }
    };
    std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer>>
        heap_;  // greater 使其成为小根堆
};

struct IoRequest {
    int id;
    std::string op;
    std::string payload;
};
struct IoCompletion {
    int id;
    std::string result;
};

// 模拟 io_uring 的 SQ/CQ 双队列：提交请求进 SQ，内核处理后结果进 CQ。
// 用队列替代真实系统调用，让上层异步逻辑在非 Linux 平台也能测试。
class IoUringSimulator {
 public:
    void submit(IoRequest request) { sq_.push_back(std::move(request)); }
    void process() {
        while (!sq_.empty()) {
            auto request = std::move(sq_.front());
            sq_.pop_front();
            cq_.push_back({request.id, request.op + ":" + request.payload});
        }
    }
    std::optional<IoCompletion> peek_completion() {
        if (cq_.empty()) return std::nullopt;
        IoCompletion completion = std::move(cq_.front());
        cq_.pop_front();
        return completion;
    }
    std::size_t pending() const { return sq_.size(); }

 private:
    std::deque<IoRequest> sq_;
    std::deque<IoCompletion> cq_;
};

struct Backend {
    std::string address;
    bool healthy = true;
    int active = 0;  // 当前活跃连接数，least-conn 依据
};

class RoundRobinBalancer {
 public:
    explicit RoundRobinBalancer(std::vector<Backend>* backends) : backends_(backends) {}
    Backend& pick() {
        // 最多绕一圈：全不健康时才放弃，避免死循环。
        for (std::size_t attempts = 0; attempts < backends_->size(); ++attempts) {
            Backend& backend = (*backends_)[next_++ % backends_->size()];
            if (backend.healthy) return backend;
        }
        throw std::runtime_error("no healthy backend");
    }

 private:
    std::vector<Backend>* backends_;  // 与连接池共享，故用指针而非拷贝
    std::size_t next_ = 0;
};

class LeastConnBalancer {
 public:
    explicit LeastConnBalancer(std::vector<Backend>* backends) : backends_(backends) {}
    Backend& pick() {
        auto it = std::min_element(
            backends_->begin(), backends_->end(), [](const Backend& lhs, const Backend& rhs) {
                if (lhs.healthy != rhs.healthy)
                    return lhs.healthy > rhs.healthy;  // 健康的排在前，保证 min 不会选到病态节点
                return lhs.active < rhs.active;
            });
        if (it == backends_->end() || !it->healthy) throw std::runtime_error("no healthy backend");
        return *it;
    }

 private:
    std::vector<Backend>* backends_;
};

class ConnectionPool {
 public:
    explicit ConnectionPool(std::vector<Backend> backends)
        : backends_(std::move(backends)), rr_(&backends_), least_(&backends_) {}
    std::string acquire_round_robin() {
        Backend& backend = rr_.pick();
        ++backend.active;
        return backend.address;
    }
    std::string acquire_least_conn() {
        Backend& backend = least_.pick();
        ++backend.active;
        return backend.address;
    }
    void release(const std::string& address) {
        for (auto& backend : backends_)
            if (backend.address == address && backend.active > 0)
                --backend.active;  // 计数下限 0，防止重复 release 出现负数
    }
    void mark_unhealthy(const std::string& address) {
        for (auto& backend : backends_)
            if (backend.address == address) backend.healthy = false;
    }
    int active(const std::string& address) const {
        for (const auto& backend : backends_)
            if (backend.address == address) return backend.active;
        return 0;
    }

 private:
    std::vector<Backend> backends_;  // 唯一所有者；两个均衡器持有其地址
    RoundRobinBalancer rr_;
    LeastConnBalancer least_;
};

struct OptimizationReport {
    bool zero_copy_supported_by_os = false;
    bool reuse_port_supported_by_os = false;
    std::string recommendation;
};

// 零拷贝与 SO_REUSEPORT 依赖 Linux 内核能力；非 Linux 只能给出可移植退路建议。
inline OptimizationReport analyze_platform(bool linux_like) {
    return {linux_like, linux_like,
            linux_like ? "enable sendfile/splice and SO_REUSEPORT"
                       : "use portable core tests; run Linux/WSL2 for zero-copy benchmarks"};
}

}  // namespace mini_hpn
