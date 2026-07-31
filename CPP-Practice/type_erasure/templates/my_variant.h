#pragma once

#include <cstddef>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace demo::erasure {

inline constexpr std::size_t variant_npos = static_cast<std::size_t>(-1);  // 空状态哨兵下标。

namespace detail {

template <typename T, typename... Ts>
struct is_one_of : std::false_type {};

// 递归：命中 Head 即 true，否则继续查 Tail，用来约束只有合法 alternative 才能进 Variant。
template <typename T, typename Head, typename... Tail>
struct is_one_of<T, Head, Tail...>
    : std::conditional_t<std::is_same_v<T, Head>, std::true_type, is_one_of<T, Tail...>> {};

template <typename T, typename... Ts>
struct index_of;

// 未命中当前 Head 时下探 Tail，结果 +1 把跳过的这一层补回下标。
template <typename T, typename Head, typename... Tail>
struct index_of<T, Head, Tail...>
    : std::integral_constant<std::size_t, 1 + index_of<T, Tail...>::value> {};

template <typename T, typename... Tail>
struct index_of<T, T, Tail...> : std::integral_constant<std::size_t, 0> {};

template <std::size_t I, typename... Ts>
struct type_at;

template <typename Head, typename... Tail>
struct type_at<0, Head, Tail...> {
    using type = Head;
};

template <std::size_t I, typename Head, typename... Tail>
struct type_at<I, Head, Tail...> : type_at<I - 1, Tail...> {};

template <typename T>
using decay_t = typename std::decay<T>::type;

}  // namespace detail

// 手写 Variant：一块 aligned_union 存储 + 运行时 index_ 记录当前是哪种 alternative。
template <typename... Ts>
class Variant {
 public:
    Variant() noexcept = default;

    // is_one_of 约束：只有退化后类型 U 属于 Ts... 时才参与重载，避免抢走拷贝/移动构造。
    template <typename T, typename U = detail::decay_t<T>,
              typename = std::enable_if_t<detail::is_one_of<U, Ts...>::value>>
    Variant(T&& value) {
        emplace<U>(std::forward<T>(value));
    }

    Variant(const Variant& other) {
        if (!other.valueless_by_exception()) {
            copy_from(other);
        }
    }

    Variant(Variant&& other) noexcept {
        if (!other.valueless_by_exception()) {
            move_from(std::move(other));
            other.reset();
        }
    }

    Variant& operator=(const Variant& other) {
        if (this == &other) {
            return *this;
        }
        reset();
        if (!other.valueless_by_exception()) {
            copy_from(other);
        }
        return *this;
    }

    Variant& operator=(Variant&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        reset();
        if (!other.valueless_by_exception()) {
            move_from(std::move(other));
            other.reset();
        }
        return *this;
    }

    template <typename T, typename U = detail::decay_t<T>,
              typename = std::enable_if_t<detail::is_one_of<U, Ts...>::value>>
    Variant& operator=(T&& value) {
        emplace<U>(std::forward<T>(value));
        return *this;
    }

    ~Variant() { reset(); }

    template <typename T, typename... Args>
    T& emplace(Args&&... args) {
        static_assert(detail::is_one_of<T, Ts...>::value, "T must be one of Variant alternatives");
        reset();  // 换值前先销毁旧 alternative，保证同一时刻只有一个存活对象。
        new (&storage_) T(std::forward<Args>(args)...);
        index_ = detail::index_of<T, Ts...>::value;
        return get<T>();
    }

    std::size_t index() const noexcept { return index_; }

    bool valueless_by_exception() const noexcept { return index_ == variant_npos; }

    void reset() noexcept {
        if (!valueless_by_exception()) {
            destroy_table()[index_](&storage_);  // 运行时下标决定调哪种类型的析构。
            index_ = variant_npos;
        }
    }

    template <std::size_t I>
    typename detail::type_at<I, Ts...>::type& get() {
        if (index_ != I) {
            throw std::bad_cast();  // 访问的下标与当前持有类型不符。
        }
        using T = typename detail::type_at<I, Ts...>::type;
        return *reinterpret_cast<T*>(&storage_);
    }

    template <std::size_t I>
    const typename detail::type_at<I, Ts...>::type& get() const {
        if (index_ != I) {
            throw std::bad_cast();
        }
        using T = typename detail::type_at<I, Ts...>::type;
        return *reinterpret_cast<const T*>(&storage_);
    }

    template <typename T>
    T& get() {
        return get<detail::index_of<T, Ts...>::value>();  // 按类型访问先编译期映射成下标。
    }

    template <typename T>
    const T& get() const {
        return get<detail::index_of<T, Ts...>::value>();
    }

 private:
    using Storage = std::aligned_union_t<0, Ts...>;  // 容得下最大 Ts 且满足全体对齐的原始存储。
    using DestroyFn = void (*)(void*);
    using CopyFn = void (*)(void*, const void*);
    using MoveFn = void (*)(void*, void*);

    // 运行时无法按类型 switch，故为每个 Ts 生成一段操作 lambda 建成跳表，用 index_ 下标索引。
    // 静态局部表只初始化一次,生命周期覆盖全程序。
    static const DestroyFn* destroy_table() {
        static const DestroyFn table[] = {
            [](void* storage) { reinterpret_cast<Ts*>(storage)->~Ts(); }...};
        return table;
    }

    static const CopyFn* copy_table() {
        static const CopyFn table[] = {
            [](void* dst, const void* src) { new (dst) Ts(*reinterpret_cast<const Ts*>(src)); }...};
        return table;
    }

    static const MoveFn* move_table() {
        static const MoveFn table[] = {
            [](void* dst, void* src) { new (dst) Ts(std::move(*reinterpret_cast<Ts*>(src))); }...};
        return table;
    }

    void copy_from(const Variant& other) {
        copy_table()[other.index_](&storage_, &other.storage_);
        index_ = other.index_;
    }

    void move_from(Variant&& other) {
        move_table()[other.index_](&storage_, &other.storage_);
        index_ = other.index_;
    }

    Storage storage_;
    std::size_t index_ = variant_npos;

    // visit 系列需要直接把 storage_ 解释为具体 alternative，故声明为友元开放私有存储。
    template <typename Visitor, typename... Us>
    friend decltype(auto) visit(Visitor&& visitor, Variant<Us...>& variant);

    template <typename Visitor, typename... Us>
    friend decltype(auto) visit(Visitor&& visitor, const Variant<Us...>& variant);

    template <typename Visitor, typename VariantType, typename T>
    friend decltype(auto) visit_one(Visitor&& visitor, VariantType& variant);

    template <typename Visitor, typename VariantType, typename T>
    friend decltype(auto) visit_one_const(Visitor&& visitor, const VariantType& variant);
};

template <typename Visitor, typename VariantType, typename T>
decltype(auto) visit_one(Visitor&& visitor, VariantType& variant) {
    return std::forward<Visitor>(visitor)(*reinterpret_cast<T*>(&variant.storage_));
}

template <typename Visitor, typename VariantType, typename T>
decltype(auto) visit_one_const(Visitor&& visitor, const VariantType& variant) {
    return std::forward<Visitor>(visitor)(*reinterpret_cast<const T*>(&variant.storage_));
}

template <typename Visitor, typename... Ts>
decltype(auto) visit(Visitor&& visitor, Variant<Ts...>& variant) {
    if (variant.valueless_by_exception()) {
        throw std::bad_cast();
    }
    // 用第 0 个 alternative 推导返回类型：本实现要求 visitor 对所有 alternative 返回同一类型。
    using Ret = decltype(std::forward<Visitor>(visitor)(variant.template get<0>()));
    using Fn = Ret (*)(Visitor&&, Variant<Ts...>&);
    static const Fn table[] = {&visit_one<Visitor, Variant<Ts...>, Ts>...};
    return table[variant.index_](std::forward<Visitor>(visitor), variant);  // 按运行时下标跳转。
}

template <typename Visitor, typename... Ts>
decltype(auto) visit(Visitor&& visitor, const Variant<Ts...>& variant) {
    if (variant.valueless_by_exception()) {
        throw std::bad_cast();
    }
    using Ret = decltype(std::forward<Visitor>(visitor)(variant.template get<0>()));
    using Fn = Ret (*)(Visitor&&, const Variant<Ts...>&);
    static const Fn table[] = {&visit_one_const<Visitor, Variant<Ts...>, Ts>...};
    return table[variant.index_](std::forward<Visitor>(visitor), variant);
}

}  // namespace demo::erasure
