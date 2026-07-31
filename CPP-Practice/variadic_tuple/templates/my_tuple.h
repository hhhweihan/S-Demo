#pragma once

#include <cstddef>
#include <ostream>
#include <type_traits>
#include <utility>

namespace demo::tuple {

template <typename... Ts>
struct Tuple;

// 空包全特化：递归继承链的终点，不存任何元素。
template <>
struct Tuple<> {
    constexpr Tuple() noexcept = default;
};

// 每层存一个 Head 并继承装着剩余元素的 Tuple<Tail...>，靠继承链把 N 个元素摊成 N 层。
template <typename Head, typename... Tail>
struct Tuple<Head, Tail...> : Tuple<Tail...> {
    using Base = Tuple<Tail...>;

    Head value;

    constexpr Tuple() = default;

    // H/Ts 独立于 Head/Tail 以支持完美转发；sizeof... 约束保证实参个数与本层+剩余层匹配，
    // 同时防止单实参时它抢占拷贝/移动构造。基类先于成员初始化(顺序由声明决定,与写法无关)。
    template <typename H, typename... Ts,
              typename = std::enable_if_t<sizeof...(Ts) == sizeof...(Tail)>>
    constexpr explicit Tuple(H&& head, Ts&&... tail)
        : Base(std::forward<Ts>(tail)...), value(std::forward<H>(head)) {}
};

template <std::size_t N, typename... Ts>
struct TupleElement;

template <typename Head, typename... Tail>
struct TupleElement<0, Head, Tail...> {
    using type = Head;
    // node = 真正存着该 value 的那一层继承类型，get 靠它 static_cast 到正确的层。
    using node = Tuple<Head, Tail...>;
};

// N>0 时剥掉 Head 递归到 N-1，直到命中上面的 0 号特化。
template <std::size_t N, typename Head, typename... Tail>
struct TupleElement<N, Head, Tail...> : TupleElement<N - 1, Tail...> {};

template <std::size_t N, typename... Ts>
using tuple_element_t = typename TupleElement<N, Ts...>::type;

template <std::size_t N, typename... Ts>
using tuple_node_t = typename TupleElement<N, Ts...>::node;

template <typename... Ts>
constexpr std::size_t tuple_size_v = sizeof...(Ts);

template <std::size_t N, typename... Ts>
constexpr tuple_element_t<N, Ts...>& get(Tuple<Ts...>& tuple) noexcept {
    using Node = tuple_node_t<N, Ts...>;
    // 向下转型到"存第 N 个 value 的那层"，再取该层的 value——继承链让索引访问变成一次转型。
    return static_cast<Node&>(tuple).value;
}

template <std::size_t N, typename... Ts>
constexpr const tuple_element_t<N, Ts...>& get(const Tuple<Ts...>& tuple) noexcept {
    using Node = tuple_node_t<N, Ts...>;
    return static_cast<const Node&>(tuple).value;
}

template <std::size_t N, typename... Ts>
constexpr tuple_element_t<N, Ts...>&& get(Tuple<Ts...>&& tuple) noexcept {
    // 复用左值版定位元素，再 move 成右值引用，避免重复写一遍转型逻辑。
    return std::move(get<N>(tuple));
}

template <typename F, typename TupleType, std::size_t... Is>
constexpr decltype(auto) apply_impl(F&& function, TupleType&& tuple, std::index_sequence<Is...>) {
    // 展开 get<Is>(tuple)... 成实参逐个转发；decltype(auto) 保留 function 的引用/值返回类别。
    return std::forward<F>(function)(get<Is>(std::forward<TupleType>(tuple))...);
}

template <typename F, typename... Ts>
constexpr decltype(auto) apply(F&& function, Tuple<Ts...>& tuple) {
    return apply_impl(std::forward<F>(function), tuple, std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename F, typename... Ts>
constexpr decltype(auto) apply(F&& function, const Tuple<Ts...>& tuple) {
    return apply_impl(std::forward<F>(function), tuple, std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename... Ls, typename... Rs, std::size_t... Is>
constexpr bool equal_impl(const Tuple<Ls...>& lhs, const Tuple<Rs...>& rhs,
                          std::index_sequence<Is...>) {
    return ((get<Is>(lhs) == get<Is>(rhs)) && ...);
}

template <typename... Ls, typename... Rs>
constexpr bool operator==(const Tuple<Ls...>& lhs, const Tuple<Rs...>& rhs) {
    // 元素个数不同的两个 Tuple 允许比较，直接判不等，避免 equal_impl 里索引越界。
    if constexpr (sizeof...(Ls) != sizeof...(Rs)) {
        return false;
    } else {
        return equal_impl(lhs, rhs, std::make_index_sequence<sizeof...(Ls)>{});
    }
}

template <typename... Ls, typename... Rs>
constexpr bool operator!=(const Tuple<Ls...>& lhs, const Tuple<Rs...>& rhs) {
    return !(lhs == rhs);
}

template <typename... Ts, std::size_t... Is>
void print_impl(std::ostream& os, const Tuple<Ts...>& tuple, std::index_sequence<Is...>) {
    os << '(';
    ((os << (Is == 0 ? "" : ", ") << get<Is>(tuple)), ...);
    os << ')';
}

template <typename... Ts>
void print(std::ostream& os, const Tuple<Ts...>& tuple) {
    print_impl(os, tuple, std::make_index_sequence<sizeof...(Ts)>{});
}

}  // namespace demo::tuple
