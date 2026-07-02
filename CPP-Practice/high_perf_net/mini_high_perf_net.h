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

class TimerWheel {
 public:
  using Callback = std::function<void(int)>;
  explicit TimerWheel(std::size_t slots, Callback timeout_cb) : slots_(slots), timeout_cb_(std::move(timeout_cb)) {}
  void add_or_refresh(int connection_id, std::size_t timeout_ticks) {
    std::size_t expire_tick = now_ + timeout_ticks;
    timers_[connection_id] = expire_tick;
    wheel_[expire_tick % slots_].push_back(connection_id);
  }
  void tick() {
    ++now_;
    auto bucket_index = now_ % slots_;
    auto bucket = std::move(wheel_[bucket_index]);
    wheel_[bucket_index].clear();
    for (int id : bucket) {
      auto it = timers_.find(id);
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
  std::map<int, std::size_t> timers_;
  std::unordered_map<std::size_t, std::vector<int>> wheel_;
  Callback timeout_cb_;
};

class PrecisionTimerHeap {
 public:
  void add(int id, std::size_t expire_at) { heap_.push({expire_at, id}); }
  std::vector<int> pop_expired(std::size_t now) {
    std::vector<int> expired;
    while (!heap_.empty() && heap_.top().expire_at <= now) {
      expired.push_back(heap_.top().id);
      heap_.pop();
    }
    return expired;
  }
 private:
  struct Timer { std::size_t expire_at; int id; bool operator>(const Timer& other) const { return expire_at > other.expire_at; } };
  std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer>> heap_;
};

struct IoRequest { int id; std::string op; std::string payload; };
struct IoCompletion { int id; std::string result; };

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
  int active = 0;
};

class RoundRobinBalancer {
 public:
  explicit RoundRobinBalancer(std::vector<Backend>* backends) : backends_(backends) {}
  Backend& pick() {
    for (std::size_t attempts = 0; attempts < backends_->size(); ++attempts) {
      Backend& backend = (*backends_)[next_++ % backends_->size()];
      if (backend.healthy) return backend;
    }
    throw std::runtime_error("no healthy backend");
  }
 private:
  std::vector<Backend>* backends_;
  std::size_t next_ = 0;
};

class LeastConnBalancer {
 public:
  explicit LeastConnBalancer(std::vector<Backend>* backends) : backends_(backends) {}
  Backend& pick() {
    auto it = std::min_element(backends_->begin(), backends_->end(), [](const Backend& lhs, const Backend& rhs) {
      if (lhs.healthy != rhs.healthy) return lhs.healthy > rhs.healthy;
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
  explicit ConnectionPool(std::vector<Backend> backends) : backends_(std::move(backends)), rr_(&backends_), least_(&backends_) {}
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
    for (auto& backend : backends_) if (backend.address == address && backend.active > 0) --backend.active;
  }
  void mark_unhealthy(const std::string& address) {
    for (auto& backend : backends_) if (backend.address == address) backend.healthy = false;
  }
  int active(const std::string& address) const {
    for (const auto& backend : backends_) if (backend.address == address) return backend.active;
    return 0;
  }
 private:
  std::vector<Backend> backends_;
  RoundRobinBalancer rr_;
  LeastConnBalancer least_;
};

struct OptimizationReport {
  bool zero_copy_supported_by_os = false;
  bool reuse_port_supported_by_os = false;
  std::string recommendation;
};

inline OptimizationReport analyze_platform(bool linux_like) {
  return {linux_like, linux_like, linux_like ? "enable sendfile/splice and SO_REUSEPORT" : "use portable core tests; run Linux/WSL2 for zero-copy benchmarks"};
}

}  // namespace mini_hpn