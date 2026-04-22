#pragma once

#define SCOPE_GUARD_CONCAT_INNER(a, b) a##b
#define SCOPE_GUARD_CONCAT(a, b) SCOPE_GUARD_CONCAT_INNER(a, b)
#define DEFER(code) auto SCOPE_GUARD_CONCAT(_defer_, __LINE__) = make_scope_guard([&] { code; })

#include <type_traits>
#include <utility>

template<typename F>
class ScopeGuard {
  F func_;
  bool dismissed_ = false;

public:
  explicit ScopeGuard(F&& func) noexcept(std::is_nothrow_move_constructible_v<F>)
      : func_(std::forward<F>(func)) {}

  explicit ScopeGuard(const F& func) noexcept(std::is_nothrow_copy_constructible_v<F>)
      : func_(func) {}

  ScopeGuard(const ScopeGuard&) = delete;
  ScopeGuard& operator=(const ScopeGuard&) = delete;

  ScopeGuard(ScopeGuard&& other) noexcept(std::is_nothrow_move_constructible_v<F>)
      : func_(std::move(other.func_)),
        dismissed_(other.dismissed_) {
    other.dismiss();
  }

  ScopeGuard& operator=(ScopeGuard&& other) = delete;

  ~ScopeGuard() noexcept {
    if (!dismissed_) {
      func_();
    }
  }

  void dismiss() noexcept {
    dismissed_ = true;
  }

  bool active() const noexcept {
    return !dismissed_;
  }
};

template<typename F>
auto make_scope_guard(F&& func) {
  using GuardFunc = std::decay_t<F>;
  return ScopeGuard<GuardFunc>(std::forward<F>(func));
}