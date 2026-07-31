#pragma once

#include <new>
#include <optional>
#include <type_traits>
#include <utility>

namespace demo::erasure {

// 手写 Optional：核心是一块原始存储 + has_value_ 标记，T 只在有值时才真正构造/析构。
template <typename T>
class Optional {
 public:
    Optional() noexcept = default;

    Optional(const T& value) { construct(value); }

    Optional(T&& value) { construct(std::move(value)); }

    Optional(const Optional& other) {
        if (other.has_value_) {
            construct(other.value());
        }
    }

    // noexcept 随 T 的移动构造而定；移后清空源，维持"移走的 Optional 不再持值"。
    Optional(Optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
        if (other.has_value_) {
            construct(std::move(other.value()));
            other.reset();
        }
    }

    Optional& operator=(const Optional& other) {
        if (this == &other) {
            return *this;
        }
        if (other.has_value_) {
            assign_or_construct(other.value());
        } else {
            reset();
        }
        return *this;
    }

    Optional& operator=(Optional&& other) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                   std::is_nothrow_move_assignable_v<T>) {
        if (this == &other) {
            return *this;
        }
        if (other.has_value_) {
            assign_or_construct(std::move(other.value()));
            other.reset();
        } else {
            reset();
        }
        return *this;
    }

    Optional& operator=(const T& value) {
        assign_or_construct(value);
        return *this;
    }

    Optional& operator=(T&& value) {
        assign_or_construct(std::move(value));
        return *this;
    }

    ~Optional() {
        reset();  // 原始存储不会自动析构 T，必须显式清理。
    }

    bool has_value() const noexcept { return has_value_; }

    explicit operator bool() const noexcept { return has_value(); }

    T& value() {
        if (!has_value_) {
            throw std::bad_optional_access();
        }
        return *ptr();
    }

    const T& value() const {
        if (!has_value_) {
            throw std::bad_optional_access();
        }
        return *ptr();
    }

    T value_or(T default_value) const { return has_value_ ? *ptr() : std::move(default_value); }

    void reset() noexcept {
        // 只有确实构造过 T 才手动调析构；否则对未初始化存储调 ~T 是 UB。
        if (has_value_) {
            ptr()->~T();
            has_value_ = false;
        }
    }

 private:
    // 大小/对齐都匹配 T 的未初始化存储：让 Optional 能不构造 T 就先占好位置。
    using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;

    template <typename U>
    void construct(U&& value) {
        new (&storage_) T(std::forward<U>(value));  // placement new 直接在 storage_ 上构造。
        has_value_ = true;
    }

    // 已有值走赋值(复用 T 的 operator=)，空则构造——不能对未初始化存储做赋值。
    template <typename U>
    void assign_or_construct(U&& value) {
        if (has_value_) {
            *ptr() = std::forward<U>(value);
        } else {
            construct(std::forward<U>(value));
        }
    }

    // std::launder：storage_ 是 aligned_storage,真正的 T 是靠 placement new 建在其上的;
    // 直接 reinterpret_cast 存储地址得到的指针编译器不保证指向那个新对象,须经 launder 提示。
    T* ptr() noexcept { return std::launder(reinterpret_cast<T*>(&storage_)); }

    const T* ptr() const noexcept { return std::launder(reinterpret_cast<const T*>(&storage_)); }

    Storage storage_;
    bool has_value_ = false;  // 不变量：为 true 当且仅当 storage_ 中有一个存活的 T。
};

}  // namespace demo::erasure
