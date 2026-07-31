#pragma once

// 两层宏保证 __LINE__ 先展开再 token-paste；否则拼出的名字会是字面量 "_defer___LINE__"。
#define SCOPE_GUARD_CONCAT_INNER(a, b) a##b
#define SCOPE_GUARD_CONCAT(a, b) SCOPE_GUARD_CONCAT_INNER(a, b)
// 用 __LINE__ 生成唯一变量名，允许同一作用域内多次 DEFER 不撞名。
#define DEFER(code) auto SCOPE_GUARD_CONCAT(_defer_, __LINE__) = make_scope_guard([&] { code; })

#include <type_traits>
#include <utility>

template <typename F>
class ScopeGuard {
    F func_;
    bool dismissed_ = false;

 public:
    // 两个构造分别吃右值/左值：左值走拷贝，避免动到调用者原对象。noexcept 随 F 的对应操作而定。
    explicit ScopeGuard(F&& func) noexcept(std::is_nothrow_move_constructible_v<F>)
        : func_(std::forward<F>(func)) {}

    explicit ScopeGuard(const F& func) noexcept(std::is_nothrow_copy_constructible_v<F>)
        : func_(func) {}

    // 禁拷贝：两个守卫会重复执行同一清理动作。
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard&& other) noexcept(std::is_nothrow_move_constructible_v<F>)
        : func_(std::move(other.func_)), dismissed_(other.dismissed_) {
        other.dismiss();  // 取消源，否则移动后两个对象都会触发清理。
    }

    // 禁移动赋值：替换已激活的守卫会让清理时机变得含糊。
    ScopeGuard& operator=(ScopeGuard&& other) = delete;

    ~ScopeGuard() noexcept {
        if (!dismissed_) {  // 已 dismiss 的守卫不执行任何动作。
            func_();
        }
    }

    // 提供逃生舱：条件满足时取消清理（如操作成功后不必回滚）。
    void dismiss() noexcept { dismissed_ = true; }

    bool active() const noexcept { return !dismissed_; }
};

// 工厂函数：借实参推导免去调用处写模板参数，并 decay 掉引用/cv 以按值保存。
template <typename F>
auto make_scope_guard(F&& func) {
    using GuardFunc = std::decay_t<F>;
    return ScopeGuard<GuardFunc>(std::forward<F>(func));
}
