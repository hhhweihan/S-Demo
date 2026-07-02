#pragma once  // 防止 Variant 头文件在同一编译单元中被重复包含。

#include <cstddef>  // 使用 std::size_t 表示类型下标。
#include <new>  // 使用 placement new 在原始存储中构造对象。
#include <stdexcept>  // 使用 std::bad_cast 表示访问了错误 alternative。
#include <type_traits>  // 使用标准类型萃取辅助模板选择和存储布局。
#include <utility>  // 使用 std::move 和 std::forward。

namespace demo::erasure {  // 类型擦除相关组件放在 demo::erasure 命名空间中。

inline constexpr std::size_t variant_npos = static_cast<std::size_t>(-1);  // 表示 Variant 当前不持有任何 alternative。

namespace detail {  // detail 命名空间保存 Variant 的模板元函数实现。

template <typename T, typename... Ts>  // T 是待查找类型，Ts 是候选类型包。
struct is_one_of : std::false_type {};  // 默认候选包为空或未命中时返回 false。

template <typename T, typename Head, typename... Tail>  // Head 是当前候选类型，Tail 是剩余候选类型。
struct is_one_of<T, Head, Tail...>  // 递归判断 T 是否出现在候选类型包中。
    : std::conditional_t<std::is_same_v<T, Head>, std::true_type, is_one_of<T, Tail...>> {};  // 当前命中返回 true，否则继续查 Tail。

template <typename T, typename... Ts>  // T 是目标类型，Ts 是候选类型包。
struct index_of;  // 前向声明类型到下标的编译期映射。

template <typename T, typename Head, typename... Tail>  // 当前 Head 不是目标类型时匹配该递归分支。
struct index_of<T, Head, Tail...>  // 递归查找 T 在 Tail 中的位置。
  : std::integral_constant<std::size_t, 1 + index_of<T, Tail...>::value> {};  // 跳过 Head，所以结果需要加 1。

template <typename T, typename... Tail>  // 当前 Head 恰好是目标 T 时匹配该特化。
struct index_of<T, T, Tail...> : std::integral_constant<std::size_t, 0> {};  // 命中当前层，下标为 0。

template <std::size_t I, typename... Ts>  // I 是目标下标，Ts 是候选类型包。
struct type_at;  // 前向声明下标到类型的编译期映射。

template <typename Head, typename... Tail>  // 下标为 0 时命中当前 Head。
struct type_at<0, Head, Tail...> {  // type_at 的递归终止分支。
  using type = Head;  // 暴露当前下标对应的类型。
};  // type_at<0> 定义结束。

template <std::size_t I, typename Head, typename... Tail>  // I 大于 0 时跳过当前 Head。
struct type_at<I, Head, Tail...> : type_at<I - 1, Tail...> {};  // 递归到 Tail 中查找第 I-1 个类型。

template <typename T>  // T 是待退化类型。
using decay_t = typename std::decay<T>::type;  // 使用标准 decay 统一构造实参类型。

}  // namespace detail  // Variant 模板元函数实现结束。

template <typename... Ts>  // Ts 是 Variant 允许保存的所有 alternative 类型。
class Variant {  // 手写 Variant，使用一块最大对齐存储和运行时下标保存其中一种类型。
 public:  // 对外暴露构造、赋值、访问和状态查询接口。
  Variant() noexcept = default;  // 默认构造为空状态，不构造任何 alternative。

  template <typename T, typename U = detail::decay_t<T>,  // T 是构造实参类型，U 是退化后的 alternative 类型。
            typename = std::enable_if_t<detail::is_one_of<U, Ts...>::value>>  // 只有 U 属于 Ts... 时该构造函数才参与重载。
  Variant(T&& value) {  // 从某个 alternative 值构造 Variant。
    emplace<U>(std::forward<T>(value));  // 在内部存储中构造对应 alternative。
  }  // 值构造结束。

  Variant(const Variant& other) {  // 拷贝构造 Variant。
    if (!other.valueless_by_exception()) {  // 源对象有值时才复制 alternative。
      copy_from(other);  // 根据源对象下标调用对应复制函数。
    }  // 源对象为空时当前对象保持空。
  }  // 拷贝构造结束。

  Variant(Variant&& other) noexcept {  // 移动构造 Variant。
    if (!other.valueless_by_exception()) {  // 源对象有值时才移动 alternative。
      move_from(std::move(other));  // 根据源对象下标调用对应移动构造函数。
      other.reset();  // 清空源对象，避免移动后仍持有旧 alternative。
    }  // 源对象为空时当前对象保持空。
  }  // 移动构造结束。

  Variant& operator=(const Variant& other) {  // 拷贝赋值 Variant。
    if (this == &other) {  // 检查自赋值。
      return *this;  // 自赋值直接返回当前对象。
    }  // 自赋值检查结束。
    reset();  // 先销毁当前持有的 alternative。
    if (!other.valueless_by_exception()) {  // 源对象有值时复制它。
      copy_from(other);  // 根据源对象下标复制对应 alternative。
    }  // 源对象为空时当前对象保持空。
    return *this;  // 返回当前对象以支持链式赋值。
  }  // 拷贝赋值结束。

  Variant& operator=(Variant&& other) noexcept {  // 移动赋值 Variant。
    if (this == &other) {  // 检查自移动赋值。
      return *this;  // 自移动直接返回当前对象。
    }  // 自移动检查结束。
    reset();  // 先销毁当前持有的 alternative。
    if (!other.valueless_by_exception()) {  // 源对象有值时移动它。
      move_from(std::move(other));  // 根据源对象下标移动对应 alternative。
      other.reset();  // 清空源对象。
    }  // 源对象为空时当前对象保持空。
    return *this;  // 返回当前对象以支持链式赋值。
  }  // 移动赋值结束。

  template <typename T, typename U = detail::decay_t<T>,  // T 是赋值实参类型，U 是退化后的 alternative 类型。
            typename = std::enable_if_t<detail::is_one_of<U, Ts...>::value>>  // 只有 U 属于 Ts... 时该赋值函数才参与重载。
  Variant& operator=(T&& value) {  // 从某个 alternative 值赋值。
    emplace<U>(std::forward<T>(value));  // 销毁旧值并构造新 alternative。
    return *this;  // 返回当前对象以支持链式赋值。
  }  // 值赋值结束。

  ~Variant() {  // 析构 Variant 时销毁当前 alternative。
    reset();  // 如果当前有值，则按下标调用对应析构函数。
  }  // 析构结束。

  template <typename T, typename... Args>  // T 是要构造的 alternative，Args 是构造参数类型包。
  T& emplace(Args&&... args) {  // 在 Variant 中原地构造指定 alternative。
    static_assert(detail::is_one_of<T, Ts...>::value, "T must be one of Variant alternatives");  // 编译期限制 T 必须属于 Ts...。
    reset();  // 先销毁当前已有 alternative。
    new (&storage_) T(std::forward<Args>(args)...);  // 在原始存储中 placement new 构造 T。
    index_ = detail::index_of<T, Ts...>::value;  // 记录当前持有类型在 Ts... 中的下标。
    return get<T>();  // 返回新构造对象的引用。
  }  // emplace 结束。

  std::size_t index() const noexcept {  // 查询当前 alternative 下标。
    return index_;  // 返回内部记录的下标，空状态时为 variant_npos。
  }  // index 结束。

  bool valueless_by_exception() const noexcept {  // 查询 Variant 是否处于无值状态。
    return index_ == variant_npos;  // 下标为 npos 表示当前没有构造任何 alternative。
  }  // valueless_by_exception 结束。

  void reset() noexcept {  // 销毁当前 alternative 并进入空状态。
    if (!valueless_by_exception()) {  // 只有当前有值时才需要析构。
      destroy_table()[index_](&storage_);  // 根据下标从析构函数表中取函数并调用。
      index_ = variant_npos;  // 析构完成后标记为空。
    }  // 空状态不需要操作。
  }  // reset 结束。

  template <std::size_t I>  // I 是要访问的 alternative 下标。
  typename detail::type_at<I, Ts...>::type& get() {  // 按下标返回可修改引用。
    if (index_ != I) {  // 当前持有类型和请求下标不一致时访问非法。
      throw std::bad_cast();  // 抛出 bad_cast 表示类型访问错误。
    }  // 下标检查结束。
    using T = typename detail::type_at<I, Ts...>::type;  // 根据下标取得对应 alternative 类型。
    return *reinterpret_cast<T*>(&storage_);  // 将原始存储解释为 T* 并解引用。
  }  // 非 const 下标 get 结束。

  template <std::size_t I>  // I 是要访问的 alternative 下标。
  const typename detail::type_at<I, Ts...>::type& get() const {  // 按下标返回只读引用。
    if (index_ != I) {  // 当前持有类型和请求下标不一致时访问非法。
      throw std::bad_cast();  // 抛出 bad_cast 表示类型访问错误。
    }  // 下标检查结束。
    using T = typename detail::type_at<I, Ts...>::type;  // 根据下标取得对应 alternative 类型。
    return *reinterpret_cast<const T*>(&storage_);  // 将原始存储解释为 const T* 并解引用。
  }  // const 下标 get 结束。

  template <typename T>  // T 是要访问的 alternative 类型。
  T& get() {  // 按类型返回可修改引用。
    return get<detail::index_of<T, Ts...>::value>();  // 先把类型映射为下标，再调用下标版本 get。
  }  // 非 const 类型 get 结束。

  template <typename T>  // T 是要访问的 alternative 类型。
  const T& get() const {  // 按类型返回只读引用。
    return get<detail::index_of<T, Ts...>::value>();  // 先把类型映射为下标，再调用 const 下标版本 get。
  }  // const 类型 get 结束。

 private:  // 以下成员负责底层存储、操作表和 visit 访问权限。
  using Storage = std::aligned_union_t<0, Ts...>;  // 分配能容纳任意 Ts 且对齐满足所有 Ts 的原始存储。
  using DestroyFn = void (*)(void*);  // 析构函数表项类型，接收存储地址并销毁对应类型。
  using CopyFn = void (*)(void*, const void*);  // 复制函数表项类型，接收目标存储和源存储。
  using MoveFn = void (*)(void*, void*);  // 移动函数表项类型，接收目标存储和源存储。

  static const DestroyFn* destroy_table() {  // 返回按 Ts... 顺序排列的析构函数表。
    static const DestroyFn table[] = {  // 静态表只初始化一次，生命周期覆盖整个程序。
        [](void* storage) { reinterpret_cast<Ts*>(storage)->~Ts(); }...};  // 为每个 Ts 生成一个析构 lambda。
    return table;  // 返回表首地址，调用方用 index_ 选择对应函数。
  }  // destroy_table 结束。

  static const CopyFn* copy_table() {  // 返回按 Ts... 顺序排列的复制构造函数表。
    static const CopyFn table[] = {  // 静态表只初始化一次。
        [](void* dst, const void* src) { new (dst) Ts(*reinterpret_cast<const Ts*>(src)); }...};  // 为每个 Ts 生成一个 placement copy lambda。
    return table;  // 返回表首地址，调用方用源 index_ 选择对应函数。
  }  // copy_table 结束。

  static const MoveFn* move_table() {  // 返回按 Ts... 顺序排列的移动构造函数表。
    static const MoveFn table[] = {  // 静态表只初始化一次。
        [](void* dst, void* src) { new (dst) Ts(std::move(*reinterpret_cast<Ts*>(src))); }...};  // 为每个 Ts 生成一个 placement move lambda。
    return table;  // 返回表首地址，调用方用源 index_ 选择对应函数。
  }  // move_table 结束。

  void copy_from(const Variant& other) {  // 从另一个有值 Variant 复制当前 alternative。
    copy_table()[other.index_](&storage_, &other.storage_);  // 根据源下标复制构造对应类型。
    index_ = other.index_;  // 复制成功后同步当前下标。
  }  // copy_from 结束。

  void move_from(Variant&& other) {  // 从另一个有值 Variant 移动当前 alternative。
    move_table()[other.index_](&storage_, &other.storage_);  // 根据源下标移动构造对应类型。
    index_ = other.index_;  // 移动成功后同步当前下标。
  }  // move_from 结束。

  Storage storage_;  // 保存当前 alternative 的原始存储。
  std::size_t index_ = variant_npos;  // 保存当前 alternative 下标，默认为空状态。

  template <typename Visitor, typename... Us>  // Visitor 是访问器类型，Us 是目标 Variant 的类型包。
  friend decltype(auto) visit(Visitor&& visitor, Variant<Us...>& variant);  // 允许非 const visit 访问私有 storage_。

  template <typename Visitor, typename... Us>  // Visitor 是访问器类型，Us 是目标 Variant 的类型包。
  friend decltype(auto) visit(Visitor&& visitor, const Variant<Us...>& variant);  // 允许 const visit 访问私有 storage_。

  template <typename Visitor, typename VariantType, typename T>  // T 是当前下标对应的 alternative 类型。
  friend decltype(auto) visit_one(Visitor&& visitor, VariantType& variant);  // 允许 visit_one 直接解释 storage_。

  template <typename Visitor, typename VariantType, typename T>  // T 是当前下标对应的 alternative 类型。
  friend decltype(auto) visit_one_const(Visitor&& visitor, const VariantType& variant);  // 允许 const visit_one 直接解释 storage_。
};  // Variant 定义结束。

template <typename Visitor, typename VariantType, typename T>  // Visitor 是访问器，VariantType 是 Variant 类型，T 是当前 alternative。
decltype(auto) visit_one(Visitor&& visitor, VariantType& variant) {  // 对非 const Variant 的一个具体 alternative 执行访问器。
  return std::forward<Visitor>(visitor)(*reinterpret_cast<T*>(&variant.storage_));  // 把 storage_ 解释为 T 并调用 visitor。
}  // visit_one 结束。

template <typename Visitor, typename VariantType, typename T>  // Visitor 是访问器，VariantType 是 Variant 类型，T 是当前 alternative。
decltype(auto) visit_one_const(Visitor&& visitor, const VariantType& variant) {  // 对 const Variant 的一个具体 alternative 执行访问器。
  return std::forward<Visitor>(visitor)(*reinterpret_cast<const T*>(&variant.storage_));  // 把 storage_ 解释为 const T 并调用 visitor。
}  // visit_one_const 结束。

template <typename Visitor, typename... Ts>  // Visitor 是访问器类型，Ts 是 Variant 的 alternative 类型包。
decltype(auto) visit(Visitor&& visitor, Variant<Ts...>& variant) {  // 根据运行时下标分发到当前 alternative。
  if (variant.valueless_by_exception()) {  // 空 Variant 不能被访问。
    throw std::bad_cast();  // 抛出 bad_cast 表示当前没有可访问对象。
  }  // 空状态检查结束。
  using Ret = decltype(std::forward<Visitor>(visitor)(variant.template get<0>()));  // 用第 0 个 alternative 推导 visitor 返回类型。
  using Fn = Ret (*)(Visitor&&, Variant<Ts...>&);  // 定义访问跳表函数指针类型。
  static const Fn table[] = {&visit_one<Visitor, Variant<Ts...>, Ts>...};  // 为每个 Ts 生成一个 visit_one 表项。
  return table[variant.index_](std::forward<Visitor>(visitor), variant);  // 按运行时下标调用对应表项。
}  // 非 const visit 结束。

template <typename Visitor, typename... Ts>  // Visitor 是访问器类型，Ts 是 Variant 的 alternative 类型包。
decltype(auto) visit(Visitor&& visitor, const Variant<Ts...>& variant) {  // 根据运行时下标访问 const Variant。
  if (variant.valueless_by_exception()) {  // 空 Variant 不能被访问。
    throw std::bad_cast();  // 抛出 bad_cast 表示当前没有可访问对象。
  }  // 空状态检查结束。
  using Ret = decltype(std::forward<Visitor>(visitor)(variant.template get<0>()));  // 用第 0 个 alternative 推导 visitor 返回类型。
  using Fn = Ret (*)(Visitor&&, const Variant<Ts...>&);  // 定义 const 访问跳表函数指针类型。
  static const Fn table[] = {&visit_one_const<Visitor, Variant<Ts...>, Ts>...};  // 为每个 Ts 生成一个 const visit_one 表项。
  return table[variant.index_](std::forward<Visitor>(visitor), variant);  // 按运行时下标调用对应表项。
}  // const visit 结束。

}  // namespace demo::erasure  // 类型擦除命名空间结束。