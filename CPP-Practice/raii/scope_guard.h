#pragma once  // 防止头文件在同一编译单元中被重复包含。

#define SCOPE_GUARD_CONCAT_INNER(a, b) a##b  // 使用预处理器 token paste 把两个标记拼成一个名字。
#define SCOPE_GUARD_CONCAT(a, b) SCOPE_GUARD_CONCAT_INNER(a, b)  // 多包一层宏，确保 __LINE__ 先展开再拼接。
#define DEFER(code) auto SCOPE_GUARD_CONCAT(_defer_, __LINE__) = make_scope_guard([&] { code; })  // 创建带唯一变量名的作用域守卫，离开作用域时执行 code。

#include <type_traits>  // 使用类型萃取判断可移动/可拷贝构造是否 noexcept。
#include <utility>  // 使用 std::forward 和 std::move 完成完美转发与移动语义。

template<typename F>  // F 表示离开作用域时需要执行的可调用对象类型。
class ScopeGuard {  // ScopeGuard 用 RAII 管理一段退出作用域时自动运行的清理逻辑。
  F func_;  // 保存用户传入的清理函数、lambda 或函数对象。
  bool dismissed_ = false;  // 标记守卫是否已经取消，默认处于激活状态。

public:  // 对外暴露构造、移动、析构和状态控制接口。
  explicit ScopeGuard(F&& func) noexcept(std::is_nothrow_move_constructible_v<F>)  // 接收右值可调用对象，并按 F 的移动构造 noexcept 性质声明异常规格。
      : func_(std::forward<F>(func)) {}  // 通过完美转发把可调用对象构造到成员中。

  explicit ScopeGuard(const F& func) noexcept(std::is_nothrow_copy_constructible_v<F>)  // 接收左值可调用对象，并按 F 的拷贝构造 noexcept 性质声明异常规格。
      : func_(func) {}  // 复制左值可调用对象，保证调用者的原对象不被移动。

  ScopeGuard(const ScopeGuard&) = delete;  // 禁止拷贝构造，避免两个守卫重复执行同一清理动作。
  ScopeGuard& operator=(const ScopeGuard&) = delete;  // 禁止拷贝赋值，保持清理动作的唯一所有权。

  ScopeGuard(ScopeGuard&& other) noexcept(std::is_nothrow_move_constructible_v<F>)  // 允许移动构造，把清理职责转移到新对象。
      : func_(std::move(other.func_)),  // 移动源对象中保存的可调用对象。
        dismissed_(other.dismissed_) {  // 同步源对象当时的激活/取消状态。
    other.dismiss();  // 取消源对象，防止移动后析构时重复执行清理逻辑。
  }  // 移动构造结束，新对象拥有唯一清理职责。

  ScopeGuard& operator=(ScopeGuard&& other) = delete;  // 禁止移动赋值，避免替换已有守卫时产生清理顺序歧义。

  ~ScopeGuard() noexcept {  // 析构函数在作用域退出时自动触发，承担 RAII 清理入口。
    if (!dismissed_) {  // 只有守卫仍处于激活状态时才执行清理逻辑。
      func_();  // 调用保存的清理函数。
    }  // 条件块结束，已取消的守卫不会执行任何动作。
  }  // 析构完成后作用域清理结束。

  void dismiss() noexcept {  // 手动取消守卫，使析构时不再执行清理函数。
    dismissed_ = true;  // 把状态置为已取消。
  }  // 取消操作结束。

  bool active() const noexcept {  // 查询守卫当前是否仍然会在析构时执行。
    return !dismissed_;  // 未取消就表示处于激活状态。
  }  // 状态查询结束。
};  // ScopeGuard 定义结束。

template<typename F>  // 工厂函数根据实参自动推导可调用对象类型。
auto make_scope_guard(F&& func) {  // 构造 ScopeGuard，避免调用处显式书写模板参数。
  using GuardFunc = std::decay_t<F>;  // 去掉引用和 cv 限定，得到适合保存到成员中的值类型。
  return ScopeGuard<GuardFunc>(std::forward<F>(func));  // 完美转发实参并返回拥有清理动作的守卫对象。
}  // 工厂函数结束。