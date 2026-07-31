#pragma once

// Minimal single-threaded epoll event loop for the real-network Raft server (raft_server.h).
//
// Why epoll instead of one-thread-per-connection: a Raft node holds a long-lived connection to
// every peer plus a listening socket, and the number of peers is small and fixed — but the point
// of this module is to actually exercise the same readiness-notification model production network
// servers use, not to hide it behind blocking I/O. Level-triggered (LT) mode is used throughout:
// simpler correctness story than edge-triggered (a readable fd keeps being reported readable until
// fully drained, so a handler that reads "some" data rather than "all of it" this round can't lose
// an event) — the ET vs LT tradeoff itself is exercised for real in the network_reactor module.
//
// Timers (election timeout, heartbeat) are first-class here via timerfd, not an epoll_wait()
// timeout: a real server has more than one deadline in flight at once, and timerfd lets each one
// be an ordinary fd the same dispatch loop already knows how to handle.
//
// Shutdown uses the standard self-pipe trick (here: eventfd) rather than relying on EINTR from a
// signal handler racing epoll_wait — Stop() just writes to an fd, which is as safe to call from a
// signal handler as it is from another thread, and the loop notices it on the next iteration like
// any other readable fd.

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace raft_net {

class EventLoop {
 public:
    using Callback = std::function<void(std::uint32_t events)>;

    EventLoop() {
        epfd_ = ::epoll_create1(EPOLL_CLOEXEC);
        if (epfd_ < 0) throw std::runtime_error(std::string("epoll_create1: ") + std::strerror(errno));
        stopFd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (stopFd_ < 0) throw std::runtime_error(std::string("eventfd: ") + std::strerror(errno));
        Add(stopFd_, EPOLLIN, [this](std::uint32_t) {
            std::uint64_t val = 0;
            while (::read(stopFd_, &val, sizeof(val)) > 0) {
            }
            running_ = false;
        });
    }

    ~EventLoop() {
        for (auto& [fd, cb] : callbacks_) (void)cb;
        ::close(stopFd_);
        ::close(epfd_);
    }

    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;

    // Registers fd for `events` (EPOLLIN/EPOLLOUT, LT). `cb` is invoked with the fired events mask
    // whenever epoll_wait reports it ready.
    void Add(int fd, std::uint32_t events, Callback cb) {
        epoll_event ev{};
        ev.events = events;
        ev.data.fd = fd;
        if (::epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) < 0)
            throw std::runtime_error(std::string("epoll_ctl ADD: ") + std::strerror(errno));
        callbacks_[fd] = std::move(cb);
    }

    // Changes the interest set for an already-registered fd (e.g. arm EPOLLOUT while a non-blocking
    // connect()/write() is pending, then drop it again once the socket drains).
    void Modify(int fd, std::uint32_t events) {
        epoll_event ev{};
        ev.events = events;
        ev.data.fd = fd;
        if (::epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) < 0)
            throw std::runtime_error(std::string("epoll_ctl MOD: ") + std::strerror(errno));
    }

    // Unregisters fd. Does NOT close it — the caller owns the fd's lifetime (a peer connection's
    // fd is often closed and immediately replaced by a reconnect, which Remove()+Add() models
    // directly).
    void Remove(int fd) {
        ::epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
        callbacks_.erase(fd);
    }

    // Creates a disarmed timerfd registered with the loop; `onFire` runs each time it expires.
    // Returned fd is a normal timer handle for Reset()/Cancel()/DestroyTimer().
    int CreateTimer(std::function<void()> onFire) {
        int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        if (fd < 0) throw std::runtime_error(std::string("timerfd_create: ") + std::strerror(errno));
        Add(fd, EPOLLIN, [fd, onFire = std::move(onFire)](std::uint32_t) {
            std::uint64_t expirations = 0;
            ::read(fd, &expirations, sizeof(expirations));  // drain; LT would otherwise refire
            onFire();
        });
        return fd;
    }

    // Re-arms a timer to fire once after `delay` (one-shot: no repeat interval). Raft's election
    // timeout is reset like this on every valid heartbeat/vote grant — a fresh random delay each
    // time, not a fixed period.
    void ResetTimer(int timerFd, std::chrono::milliseconds delay) {
        itimerspec spec{};
        ToTimespec(delay, &spec.it_value);
        ::timerfd_settime(timerFd, 0, &spec, nullptr);
    }

    // Arms a timer to fire every `interval` starting after `interval` (leader heartbeat cadence).
    void StartPeriodic(int timerFd, std::chrono::milliseconds interval) {
        itimerspec spec{};
        ToTimespec(interval, &spec.it_value);
        ToTimespec(interval, &spec.it_interval);
        ::timerfd_settime(timerFd, 0, &spec, nullptr);
    }

    // Disarms without unregistering — the fd stays valid and can be Reset()/Started again.
    void CancelTimer(int timerFd) {
        itimerspec spec{};
        ::timerfd_settime(timerFd, 0, &spec, nullptr);
    }

    void DestroyTimer(int timerFd) {
        Remove(timerFd);
        ::close(timerFd);
    }

    // Blocks in epoll_wait for up to `timeoutMs` (-1 = forever), dispatches all ready fds once.
    // Exposed separately from Run() so tests can drive the loop deterministically.
    void RunOnce(int timeoutMs) {
        events_.resize(std::max<std::size_t>(16, callbacks_.size()));
        int n = ::epoll_wait(epfd_, events_.data(), static_cast<int>(events_.size()), timeoutMs);
        if (n < 0) {
            if (errno == EINTR) return;
            throw std::runtime_error(std::string("epoll_wait: ") + std::strerror(errno));
        }
        for (int i = 0; i < n; ++i) {
            int fd = events_[static_cast<std::size_t>(i)].data.fd;
            auto it = callbacks_.find(fd);
            if (it == callbacks_.end()) continue;  // removed by an earlier callback this batch
            it->second(events_[static_cast<std::size_t>(i)].events);
        }
    }

    // Runs until Stop() is called (from this thread, another thread, or a signal handler).
    void Run() {
        running_ = true;
        while (running_) RunOnce(-1);
    }

    void Stop() {
        std::uint64_t one = 1;
        ::write(stopFd_, &one, sizeof(one));
    }

 private:
    static void ToTimespec(std::chrono::milliseconds ms, timespec* ts) {
        ts->tv_sec = ms.count() / 1000;
        ts->tv_nsec = (ms.count() % 1000) * 1000000;
        // timerfd treats an all-zero it_value as "disarm"; a zero delay must still fire, so bump
        // it to the smallest representable positive interval.
        if (ts->tv_sec == 0 && ts->tv_nsec == 0) ts->tv_nsec = 1;
    }

    int epfd_ = -1;
    int stopFd_ = -1;
    std::atomic<bool> running_{false};
    std::unordered_map<int, Callback> callbacks_;
    std::vector<epoll_event> events_;
};

}  // namespace raft_net
