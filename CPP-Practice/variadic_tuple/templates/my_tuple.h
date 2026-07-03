#pragma once  // 防止该模板头文件在同一编译单元中被重复包含，保证模板只声明一次。

#include <cstddef>  // 使用 std::size_t 表示编译期索引和元素数量。
#include <ostream>  // 使用 std::ostream 实现 Tuple 的打印函数。
#include <type_traits>  // 使用类型萃取工具辅助模板元编程。
#include <utility>  // 使用 std::forward、std::move 和 std::index_sequence。

namespace demo::tuple {  // 把自定义 Tuple 放在 demo::tuple 命名空间中，避免和 std::tuple 冲突。

template <typename... Ts>  // Ts 表示 Tuple 中保存的所有元素类型。
struct Tuple;  // 前向声明主模板，后续用空包和非空包分别特化。

template <>  // 空类型包的全特化，作为递归继承链的终点。
struct Tuple<> {  // 空 Tuple 不保存任何元素。
  constexpr Tuple() noexcept = default;  // 空 Tuple 可以 constexpr 默认构造且不抛异常。
};  // 空 Tuple 定义结束。

template <typename Head, typename... Tail>  // Head 是当前层元素类型，Tail 是剩余元素类型包。
struct Tuple<Head, Tail...> : Tuple<Tail...> {  // 通过继承 Tuple<Tail...> 递归保存后续元素。
  using Base = Tuple<Tail...>;  // 给基类起别名，便于构造函数初始化递归基类。

  Head value;  // 当前递归层保存一个 Head 类型的元素。

  constexpr Tuple() = default;  // 允许默认构造当前层和所有基类层。

  template <typename H, typename... Ts,  // H/Ts 接收构造参数，和 Head/Tail 分离以支持完美转发。
            typename = std::enable_if_t<sizeof...(Ts) == sizeof...(Tail)>>  // 确保传入的 tail 参数个数和剩余元素个数一致。
  constexpr explicit Tuple(H&& head, Ts&&... tail)  // 构造非空 Tuple 的当前层和递归基类层。
      : Base(std::forward<Ts>(tail)...), value(std::forward<H>(head)) {}  // 先构造基类保存 tail，再保存当前 head。
};  // 非空 Tuple 定义结束。

template <std::size_t N, typename... Ts>  // N 是要查找的索引，Ts 是 Tuple 的类型包。
struct TupleElement;  // 前向声明索引到元素类型/节点类型的元函数。

template <typename Head, typename... Tail>  // 索引为 0 时，当前 Head 就是目标元素。
struct TupleElement<0, Head, Tail...> {  // TupleElement 的递归终止特化。
  using type = Head;  // 暴露第 0 个元素的类型。
  using node = Tuple<Head, Tail...>;  // 暴露保存该元素 value 的 Tuple 继承层类型。
};  // 索引命中当前层的元函数定义结束。

template <std::size_t N, typename Head, typename... Tail>  // N 大于 0 时跳过当前 Head。
struct TupleElement<N, Head, Tail...> : TupleElement<N - 1, Tail...> {};  // 递归到 Tail 中查找第 N-1 个元素。

template <std::size_t N, typename... Ts>  // 简化第 N 个元素类型的写法。
using tuple_element_t = typename TupleElement<N, Ts...>::type;  // 取出 TupleElement 暴露的 type。

template <std::size_t N, typename... Ts>  // 简化保存第 N 个元素的继承层类型写法。
using tuple_node_t = typename TupleElement<N, Ts...>::node;  // 取出 TupleElement 暴露的 node。

template <typename... Ts>  // Ts 是 Tuple 的元素类型包。
constexpr std::size_t tuple_size_v = sizeof...(Ts);  // 编译期计算 Tuple 元素个数。

template <std::size_t N, typename... Ts>  // N 指定要访问的元素索引。
constexpr tuple_element_t<N, Ts...>& get(Tuple<Ts...>& tuple) noexcept {  // 为非常量左值 Tuple 返回可修改引用。
  using Node = tuple_node_t<N, Ts...>;  // 找到实际保存第 N 个 value 的继承层。
  return static_cast<Node&>(tuple).value;  // 向下转成对应层并返回该层的 value。
}  // 非 const 左值 get 结束。

template <std::size_t N, typename... Ts>  // N 指定要访问的元素索引。
constexpr const tuple_element_t<N, Ts...>& get(const Tuple<Ts...>& tuple) noexcept {  // 为 const Tuple 返回只读引用。
  using Node = tuple_node_t<N, Ts...>;  // 找到实际保存第 N 个 value 的继承层。
  return static_cast<const Node&>(tuple).value;  // 转成 const 继承层并返回只读 value。
}  // const 左值 get 结束。

template <std::size_t N, typename... Ts>  // N 指定要访问的元素索引。
constexpr tuple_element_t<N, Ts...>&& get(Tuple<Ts...>&& tuple) noexcept {  // 为右值 Tuple 返回右值引用。
  return std::move(get<N>(tuple));  // 复用左值 get 定位元素，再把结果转换成右值引用。
}  // 右值 get 结束。

template <typename F, typename TupleType, std::size_t... Is>  // F 是可调用对象，TupleType 是 Tuple 引用类型，Is 是索引包。
constexpr decltype(auto) apply_impl(F&& function, TupleType&& tuple,  // 根据索引包把 Tuple 元素展开成函数实参。
                                    std::index_sequence<Is...>) {  // std::index_sequence 承载 0 到 N-1 的编译期索引。
  return std::forward<F>(function)(get<Is>(std::forward<TupleType>(tuple))...);  // 对每个索引调用 get，再展开调用 function。
}  // apply 的底层展开实现结束。

template <typename F, typename... Ts>  // F 是可调用对象，Ts 是 Tuple 的元素类型。
constexpr decltype(auto) apply(F&& function, Tuple<Ts...>& tuple) {  // 对非常量 Tuple 调用函数。
  return apply_impl(std::forward<F>(function), tuple,  // 转发可调用对象，并把 Tuple 作为左值传入。
                    std::make_index_sequence<sizeof...(Ts)>{});  // 生成与元素个数等长的索引序列。
}  // 非 const apply 结束。

template <typename F, typename... Ts>  // F 是可调用对象，Ts 是 Tuple 的元素类型。
constexpr decltype(auto) apply(F&& function, const Tuple<Ts...>& tuple) {  // 对 const Tuple 调用函数。
  return apply_impl(std::forward<F>(function), tuple,  // 转发可调用对象，并保留 Tuple 的 const 属性。
                    std::make_index_sequence<sizeof...(Ts)>{});  // 生成索引序列用于逐元素展开。
}  // const apply 结束。

template <typename... Ls, typename... Rs, std::size_t... Is>  // Ls/Rs 是左右 Tuple 的元素类型包，Is 是比较索引包。
constexpr bool equal_impl(const Tuple<Ls...>& lhs, const Tuple<Rs...>& rhs,  // 按索引逐个比较两个 Tuple 的元素。
                          std::index_sequence<Is...>) {  // 索引序列控制折叠表达式展开范围。
  return ((get<Is>(lhs) == get<Is>(rhs)) && ...);  // 所有对应元素都相等时返回 true。
}  // 相等比较展开实现结束。

template <typename... Ls, typename... Rs>  // Ls/Rs 分别表示左右 Tuple 的元素类型包。
constexpr bool operator==(const Tuple<Ls...>& lhs, const Tuple<Rs...>& rhs) {  // 定义两个 Tuple 的相等比较。
  if constexpr (sizeof...(Ls) != sizeof...(Rs)) {  // 编译期判断两边元素个数是否不同。
    return false;  // 元素个数不同则不可能相等。
  } else {  // 元素个数相同才需要逐元素比较。
    return equal_impl(lhs, rhs, std::make_index_sequence<sizeof...(Ls)>{});  // 生成索引序列并比较每个元素。
  }  // if constexpr 结束。
}  // operator== 结束。

template <typename... Ls, typename... Rs>  // Ls/Rs 分别表示左右 Tuple 的元素类型包。
constexpr bool operator!=(const Tuple<Ls...>& lhs, const Tuple<Rs...>& rhs) {  // 定义两个 Tuple 的不等比较。
  return !(lhs == rhs);  // 复用 operator==，取反得到不等结果。
}  // operator!= 结束。

template <typename... Ts, std::size_t... Is>  // Ts 是元素类型包，Is 是输出索引包。
void print_impl(std::ostream& os, const Tuple<Ts...>& tuple,  // 按索引把 Tuple 内容写入输出流。
                std::index_sequence<Is...>) {  // 索引序列决定展开哪些元素。
  os << '(';  // 输出左括号作为 Tuple 文本表示的开头。
  ((os << (Is == 0 ? "" : ", ") << get<Is>(tuple)), ...);  // 用折叠表达式输出每个元素并处理逗号分隔。
  os << ')';  // 输出右括号作为 Tuple 文本表示的结尾。
}  // 打印展开实现结束。

template <typename... Ts>  // Ts 是 Tuple 的元素类型包。
void print(std::ostream& os, const Tuple<Ts...>& tuple) {  // 对外提供 Tuple 打印接口。
  print_impl(os, tuple, std::make_index_sequence<sizeof...(Ts)>{});  // 生成索引序列并委托给 print_impl。
}  // print 结束。

}  // namespace demo::tuple  // 自定义 Tuple 命名空间结束。