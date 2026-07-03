#pragma once  // 防止 Function 头文件在同一编译单元中被重复包含，避免重复定义。

#include <functional>  // 使用 std::invoke 和 std::bad_function_call。
#include <memory>  // 使用 std::unique_ptr 管理擦除后的多态对象。
#include <type_traits>  // 使用 std::enable_if_t、std::decay_t 和 std::is_void_v。
#include <utility>  // 使用 std::move 和 std::forward。

namespace demo::erasure {  // 类型擦除相关组件放在 demo::erasure 命名空间中。

template <typename Signature>  // Signature 是函数签名，例如 Ret(Args...)。
class Function;  // 主模板只做声明，真正实现放在函数类型偏特化中。

template <typename Ret, typename... Args>  // Ret 是返回类型，Args 是参数类型包。
class Function<Ret(Args...)> {  // 手写 std::function 风格的类型擦除包装器。
 public:  // 对外暴露构造、赋值、调用和状态查询接口。
  Function() noexcept = default;  // 默认构造为空 Function，不持有任何可调用对象。

  template <typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Function>>>  // 禁止该模板抢占 Function 自身的拷贝/移动构造。
  Function(F&& function)  // 从任意可调用对象构造 Function。
      : ptr_(std::make_unique<Model<std::decay_t<F>>>(std::forward<F>(function))) {}  // 把具体 callable 放进 Model，再用 Concept 指针擦除类型。

  Function(const Function& other)  // 拷贝构造 Function。
      : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}  // 源对象非空时通过虚函数 clone 复制具体 Model。

  Function(Function&&) noexcept = default;  // 默认移动构造，直接移动 unique_ptr 所有权。

  Function& operator=(const Function& other) {  // 拷贝赋值 Function。
    if (this != &other) {  // 避免自赋值时重复 clone。
      ptr_ = other.ptr_ ? other.ptr_->clone() : nullptr;  // 源对象非空则克隆，源对象为空则置空。
    }  // 非自赋值处理结束。
    return *this;  // 返回当前对象以支持链式赋值。
  }  // 拷贝赋值结束。

  Function& operator=(Function&&) noexcept = default;  // 默认移动赋值，转移 unique_ptr 所有权。

  explicit operator bool() const noexcept {  // 支持 if (function) 判断是否持有 callable。
    return static_cast<bool>(ptr_);  // unique_ptr 非空表示 Function 可调用。
  }  // bool 转换结束。

  Ret operator()(Args... args) const {  // 调用内部保存的可调用对象。
    if (!ptr_) {  // 空 Function 不能被调用。
      throw std::bad_function_call();  // 与 std::function 一样抛出 bad_function_call。
    }  // 空状态检查结束。
    return ptr_->invoke(std::forward<Args>(args)...);  // 通过虚接口转发参数并调用具体 callable。
  }  // 调用运算符结束。

 private:  // 以下实现具体的类型擦除机制。
  struct Concept {  // 抽象基类定义所有 callable 共有的运行时接口。
    virtual ~Concept() = default;  // 虚析构保证通过基类指针删除 Model 时析构完整。
    virtual Ret invoke(Args&&... args) = 0;  // 运行时调用接口，参数类型来自 Function 签名。
    virtual std::unique_ptr<Concept> clone() const = 0;  // 多态复制接口，用于实现 Function 拷贝。
  };  // Concept 定义结束。

  template <typename F>  // F 是具体可调用对象的类型。
  struct Model final : Concept {  // Model 保存一个具体 F，并实现 Concept 虚接口。
    explicit Model(F function) : function_(std::move(function)) {}  // 接收并保存具体 callable。

    Ret invoke(Args&&... args) override {  // 实现运行时调用接口。
      if constexpr (std::is_void_v<Ret>) {  // 返回类型为 void 时不能写 return 表达式。
        std::invoke(function_, std::forward<Args>(args)...);  // 调用 callable 并丢弃 void 结果。
      } else {  // 非 void 返回类型需要把结果返回给调用者。
        return std::invoke(function_, std::forward<Args>(args)...);  // 完美转发参数并返回调用结果。
      }  // if constexpr 结束。
    }  // invoke 实现结束。

    std::unique_ptr<Concept> clone() const override {  // 实现多态复制。
      return std::make_unique<Model<F>>(function_);  // 复制保存的 callable 并返回基类指针。
    }  // clone 实现结束。

    F function_;  // 保存具体 lambda、函数指针或函数对象。
  };  // Model 定义结束。

  std::unique_ptr<Concept> ptr_;  // 指向擦除后的 callable 对象，空指针表示空 Function。
};  // Function<Ret(Args...)> 定义结束。

}  // namespace demo::erasure  // 类型擦除命名空间结束。