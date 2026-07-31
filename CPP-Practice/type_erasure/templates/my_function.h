#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace demo::erasure {

template <typename Signature>
class Function;

// 手写 std::function 风格擦除器：把任意可调用对象藏在 Concept/Model 继承体系后，只暴露签名。
template <typename Ret, typename... Args>
class Function<Ret(Args...)> {
 public:
    Function() noexcept = default;

    // enable_if 排除 F 就是 Function 自身的情形,否则这个转发构造会抢走本该走拷贝/移动构造的调用。
    template <typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Function>>>
    Function(F&& function)
        : ptr_(std::make_unique<Model<std::decay_t<F>>>(std::forward<F>(function))) {}

    // unique_ptr 不可拷贝,所以深拷贝靠虚函数 clone 复制出具体 Model。
    Function(const Function& other) : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}

    Function(Function&&) noexcept = default;

    Function& operator=(const Function& other) {
        if (this != &other) {
            ptr_ = other.ptr_ ? other.ptr_->clone() : nullptr;
        }
        return *this;
    }

    Function& operator=(Function&&) noexcept = default;

    explicit operator bool() const noexcept { return static_cast<bool>(ptr_); }

    Ret operator()(Args... args) const {
        if (!ptr_) {
            throw std::bad_function_call();  // 与 std::function 语义一致：调用空对象抛此异常。
        }
        return ptr_->invoke(std::forward<Args>(args)...);
    }

 private:
    // Concept 是擦除后的统一接口：所有具体 callable 都以 Concept* 存放，运行时经虚函数分发。
    struct Concept {
        virtual ~Concept() = default;  // 经基类指针删除 Model 时保证析构完整。
        virtual Ret invoke(Args&&... args) = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;  // 供 Function 深拷贝。
    };

    // Model 绑定一个具体类型 F,把虚接口落到对 F 的真实调用/复制上。
    template <typename F>
    struct Model final : Concept {
        explicit Model(F function) : function_(std::move(function)) {}

        Ret invoke(Args&&... args) override {
            // void 与非 void 返回分开写,以覆盖任意 Ret（含 void）。
            if constexpr (std::is_void_v<Ret>) {
                std::invoke(function_, std::forward<Args>(args)...);
            } else {
                return std::invoke(function_, std::forward<Args>(args)...);
            }
        }

        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model<F>>(function_);
        }

        F function_;
    };

    std::unique_ptr<Concept> ptr_;  // 空指针即空 Function。
};

}  // namespace demo::erasure
