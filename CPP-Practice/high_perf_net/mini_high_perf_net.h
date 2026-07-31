#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <functional>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mini_hpn {

// 时间轮：O(1) 管理海量连接的空闲超时。刷新连接只是写入新槽，旧槽里的过期项靠
// tick 时对比 timers_ 里的最新截止刻度来甄别，无需从旧槽删除（惰性失效）。
class TimerWheel {
 public:
    using Callback = std::function<void(int)>;
    explicit TimerWheel(std::size_t slots, Callback timeout_cb)
        : slots_(slots), timeout_cb_(std::move(timeout_cb)) {}
    void add_or_refresh(int connection_id, std::size_t timeout_ticks) {
        std::size_t expire_tick = now_ + timeout_ticks;
        timers_[connection_id] = expire_tick;  // 覆盖旧截止刻度即为“刷新”
        wheel_[expire_tick % slots_].push_back(connection_id);
    }
    void tick() {
        ++now_;
        auto bucket_index = now_ % slots_;
        auto bucket = std::move(wheel_[bucket_index]);
        wheel_[bucket_index].clear();
        for (int id : bucket) {
            auto it = timers_.find(id);
            // 关键：只有 timers_ 里的截止刻度确实到期才触发。被刷新过的连接其旧槽记录
            // 会因截止刻度 > now_ 而被跳过，避免刷新后仍在旧刻度误触发超时。
            if (it != timers_.end() && it->second <= now_) {
                timers_.erase(it);
                timeout_cb_(id);
            }
        }
    }
    std::size_t now() const { return now_; }

 private:
    std::size_t slots_;
    std::size_t now_ = 0;
    std::map<int, std::size_t> timers_;  // 连接 -> 最新绝对截止刻度（唯一真相）
    std::unordered_map<std::size_t, std::vector<int>>
        wheel_;  // 槽 -> 候选连接（可能含已被刷新的过期项）
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
