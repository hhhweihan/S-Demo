#pragma once  // 防止 Optional 头文件在同一编译单元中被重复包含。

#include <new>  // 使用 placement new 在原始存储上构造对象。
#include <optional>  // 使用 std::bad_optional_access 作为空值访问异常。
#include <type_traits>  // 使用 aligned_storage 和 noexcept 类型萃取。
#include <utility>  // 使用 std::move 和 std::forward 支持移动与完美转发。

namespace demo::erasure {  // 类型擦除相关组件放在 demo::erasure 命名空间中。

template <typename T>  // T 是 Optional 可能保存的值类型。
class Optional {  // 手写 Optional，演示延迟构造、手动析构和有值状态管理。
 public:  // 对外暴露构造、赋值、访问和重置接口。
  Optional() noexcept = default;  // 默认构造为空 Optional，不构造 T。

  Optional(const T& value) {  // 从左值 T 构造 Optional。
    construct(value);  // 在内部原始存储中拷贝构造 T。
  }  // 左值构造结束。

  Optional(T&& value) {  // 从右值 T 构造 Optional。
    construct(std::move(value));  // 在内部原始存储中移动构造 T。
  }  // 右值构造结束。

  Optional(const Optional& other) {  // 拷贝构造 Optional。
    if (other.has_value_) {  // 只有源对象有值时才需要构造 T。
      construct(other.value());  // 拷贝源对象中保存的值。
    }  // 源对象为空时保持当前对象为空。
  }  // 拷贝构造结束。

  Optional(Optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {  // 移动构造 Optional，并按 T 的移动构造异常规格声明 noexcept。
    if (other.has_value_) {  // 只有源对象有值时才移动构造。
      construct(std::move(other.value()));  // 把源对象中的值移动到当前存储中。
      other.reset();  // 清空源对象，保持移动后的 Optional 不再持有值。
    }  // 源对象为空时当前对象也保持为空。
  }  // 移动构造结束。

  Optional& operator=(const Optional& other) {  // 拷贝赋值 Optional。
    if (this == &other) {  // 检查自赋值，避免不必要操作。
      return *this;  // 自赋值直接返回当前对象。
    }  // 自赋值检查结束。
    if (other.has_value_) {  // 源对象有值时需要复制该值。
      assign_or_construct(other.value());  // 当前有值则赋值，当前为空则构造。
    } else {  // 源对象为空时当前对象也应变为空。
      reset();  // 销毁当前可能持有的值。
    }  // 源对象状态处理结束。
    return *this;  // 返回当前对象以支持链式赋值。
  }  // 拷贝赋值结束。

  Optional& operator=(Optional&& other) noexcept(  // 移动赋值 Optional。
      std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>) {  // noexcept 取决于 T 的移动构造和移动赋值。
    if (this == &other) {  // 检查自移动赋值。
      return *this;  // 自移动时直接返回当前对象。
    }  // 自移动检查结束。
    if (other.has_value_) {  // 源对象有值时移动该值。
      assign_or_construct(std::move(other.value()));  // 当前有值则移动赋值，当前为空则移动构造。
      other.reset();  // 清空源对象，避免移动后仍报告有值。
    } else {  // 源对象为空时当前对象也应为空。
      reset();  // 销毁当前可能持有的值。
    }  // 源对象状态处理结束。
    return *this;  // 返回当前对象以支持链式赋值。
  }  // 移动赋值结束。

  Optional& operator=(const T& value) {  // 从左值 T 赋值。
    assign_or_construct(value);  // 当前有值则拷贝赋值，当前为空则拷贝构造。
    return *this;  // 返回当前对象。
  }  // 左值赋值结束。

  Optional& operator=(T&& value) {  // 从右值 T 赋值。
    assign_or_construct(std::move(value));  // 当前有值则移动赋值，当前为空则移动构造。
    return *this;  // 返回当前对象。
  }  // 右值赋值结束。

  ~Optional() {  // 析构 Optional 时释放可能存在的 T。
    reset();  // 如果当前有值，则手动调用 T 的析构函数。
  }  // 析构结束。

  bool has_value() const noexcept {  // 查询 Optional 是否持有值。
    return has_value_;  // 返回内部有值标记。
  }  // has_value 结束。

  explicit operator bool() const noexcept {  // 支持 if (optional) 形式判断是否有值。
    return has_value();  // 复用 has_value 查询结果。
  }  // bool 转换结束。

  T& value() {  // 返回可修改的内部值引用。
    if (!has_value_) {  // 空 Optional 不能访问 value。
      throw std::bad_optional_access();  // 抛出标准 optional 空值访问异常。
    }  // 空值检查结束。
    return *ptr();  // 将原始存储解释为 T* 并解引用。
  }  // 非 const value 结束。

  const T& value() const {  // 返回只读的内部值引用。
    if (!has_value_) {  // 空 Optional 不能访问 value。
      throw std::bad_optional_access();  // 抛出标准 optional 空值访问异常。
    }  // 空值检查结束。
    return *ptr();  // 将原始存储解释为 const T* 并解引用。
  }  // const value 结束。

  T value_or(T default_value) const {  // 有值时返回内部值副本，否则返回默认值。
    return has_value_ ? *ptr() : std::move(default_value);  // 根据有值状态在存储值和默认值之间选择。
  }  // value_or 结束。

  void reset() noexcept {  // 清空 Optional 并销毁内部对象。
    if (has_value_) {  // 只有当前确实构造过 T 时才需要析构。
      ptr()->~T();  // 手动调用 T 的析构函数。
      has_value_ = false;  // 更新状态为空。
    }  // 空 Optional 不需要操作。
  }  // reset 结束。

 private:  // 以下成员负责原始存储和构造/访问细节。
  using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;  // 为 T 分配大小和对齐都合适的未初始化存储。

  template <typename U>  // U 是传入值的引用折叠类型。
  void construct(U&& value) {  // 在内部原始存储中构造 T。
    new (&storage_) T(std::forward<U>(value));  // placement new 将对象直接构造到 storage_ 地址上。
    has_value_ = true;  // 构造成功后标记为有值。
  }  // construct 结束。

  template <typename U>  // U 是传入值的引用折叠类型。
  void assign_or_construct(U&& value) {  // 根据当前状态选择赋值或构造。
    if (has_value_) {  // 已经有值时直接给已有 T 赋值。
      *ptr() = std::forward<U>(value);  // 完美转发值给 T 的赋值运算符。
    } else {  // 当前为空时需要先构造 T。
      construct(std::forward<U>(value));  // 在原始存储中构造新值。
    }  // 状态分支结束。
  }  // assign_or_construct 结束。

  T* ptr() noexcept {  // 获取指向内部 T 对象的指针。
    return reinterpret_cast<T*>(&storage_);  // 将原始存储地址解释为 T*。
  }  // 非 const ptr 结束。

  const T* ptr() const noexcept {  // 获取指向内部 T 对象的只读指针。
    return reinterpret_cast<const T*>(&storage_);  // 将原始存储地址解释为 const T*。
  }  // const ptr 结束。

  Storage storage_;  // 保存 T 的未初始化原始存储。
  bool has_value_ = false;  // 记录 storage_ 中当前是否已经构造了 T。
};  // Optional 定义结束。

}  // namespace demo::erasure  // 类型擦除命名空间结束。