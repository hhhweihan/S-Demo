#pragma once  // 防止 mini_stl 头文件在同一编译单元中被重复包含。

#include <algorithm>  // 使用 heap 算法和通用算法辅助实现适配器。
#include <cstddef>  // 使用 std::size_t 和 std::ptrdiff_t 表示大小和距离。
#include <functional>  // 使用 std::less 作为优先队列默认比较器。
#include <initializer_list>  // 支持用花括号列表构造容器。
#include <iterator>  // 使用标准迭代器标签与 std::iterator_traits。
#include <memory>  // 使用 std::unique_ptr 管理动态数组。
#include <stdexcept>  // 使用 std::out_of_range 报告非法容器操作。
#include <type_traits>  // 预留类型萃取工具，便于迭代器扩展。
#include <utility>  // 使用 std::move 和 std::swap。
#include <vector>  // 预留标准 vector 对照头，便于适配器扩展。

namespace mini_stl {  // 手写 STL 序列容器放在 mini_stl 命名空间中。

struct input_iterator_tag {};  // 输入迭代器标签，表示只支持单向读取。
struct output_iterator_tag {};  // 输出迭代器标签，表示只支持单向写入。
struct forward_iterator_tag : input_iterator_tag {};  // 前向迭代器标签，继承输入迭代器能力。
struct bidirectional_iterator_tag : forward_iterator_tag {};  // 双向迭代器标签，继承前向迭代器能力并支持回退。
struct random_access_iterator_tag : bidirectional_iterator_tag {};  // 随机访问迭代器标签，继承双向能力并支持跳转。

template <typename Iterator>  // Iterator 是自定义迭代器类型。
struct iterator_traits {  // 从自定义迭代器中提取标准迭代器关联类型。
  using difference_type = typename Iterator::difference_type;  // 提取两个迭代器之间距离的类型。
  using value_type = typename Iterator::value_type;  // 提取迭代器指向的值类型。
  using pointer = typename Iterator::pointer;  // 提取指针类型。
  using reference = typename Iterator::reference;  // 提取引用类型。
  using iterator_category = typename Iterator::iterator_category;  // 提取迭代器类别标签。
};  // iterator_traits 主模板结束。

template <typename T>  // T 是原生指针指向的元素类型。
struct iterator_traits<T*> {  // 为原生指针提供 iterator_traits 特化。
  using difference_type = std::ptrdiff_t;  // 指针距离使用 ptrdiff_t。
  using value_type = T;  // 指针指向的值类型就是 T。
  using pointer = T*;  // 指针类型就是 T*。
  using reference = T&;  // 解引用结果是 T&。
  using iterator_category = random_access_iterator_tag;  // 原生指针满足随机访问迭代器能力。
};  // iterator_traits<T*> 特化结束。

template <typename Iterator>  // Iterator 是被反向包装的基础迭代器类型。
class ReverseIterator {  // 简化版反向迭代器，语义类似 std::reverse_iterator。
 public:  // 暴露标准迭代器关联类型和操作接口。
  using iterator_type = Iterator;  // 保存基础迭代器类型别名。
  using traits = std::iterator_traits<Iterator>;  // 使用标准 iterator_traits 兼容原生指针和自定义迭代器。
  using difference_type = typename traits::difference_type;  // 反向迭代器距离类型。
  using value_type = typename traits::value_type;  // 反向迭代器值类型。
  using pointer = typename traits::pointer;  // 反向迭代器指针类型。
  using reference = typename traits::reference;  // 反向迭代器引用类型。
  using iterator_category = typename traits::iterator_category;  // 反向迭代器类别沿用基础迭代器类别。

  ReverseIterator() = default;  // 默认构造空反向迭代器。
  explicit ReverseIterator(Iterator current) : current_(current) {}  // 用基础迭代器位置构造反向迭代器。

  Iterator base() const { return current_; }  // 返回底层基础迭代器。

  reference operator*() const {  // 解引用反向迭代器。
    Iterator tmp = current_;  // 复制基础迭代器，避免修改当前对象。
    return *--tmp;  // 反向迭代器指向 base() 前一个元素。
  }  // operator* 结束。

  ReverseIterator& operator++() {  // 前置 ++ 让反向迭代器向前移动一个逻辑位置。
    --current_;  // 反向前进等价于基础迭代器后退。
    return *this;  // 返回当前对象引用。
  }  // 前置 ++ 结束。

  ReverseIterator operator++(int) {  // 后置 ++ 返回移动前的旧值。
    ReverseIterator old(*this);  // 保存旧迭代器状态。
    --current_;  // 当前对象按反向语义前进。
    return old;  // 返回旧值。
  }  // 后置 ++ 结束。

  ReverseIterator& operator--() {  // 前置 -- 让反向迭代器向后移动一个逻辑位置。
    ++current_;  // 反向后退等价于基础迭代器前进。
    return *this;  // 返回当前对象引用。
  }  // 前置 -- 结束。

  bool operator==(const ReverseIterator& other) const { return current_ == other.current_; }  // 比较两个反向迭代器的基础位置是否相同。
  bool operator!=(const ReverseIterator& other) const { return !(*this == other); }  // 不等比较复用相等比较取反。

 private:  // 保存反向迭代器内部状态。
  Iterator current_{};  // 指向反向迭代器逻辑元素之后的位置。
};  // ReverseIterator 定义结束。

template <typename T>  // T 是动态数组中保存的元素类型。
class MyVector {  // 简化版 vector，使用连续动态数组和随机访问迭代器。
 public:  // 暴露标准容器常用类型和操作接口。
  using value_type = T;  // 容器元素类型。
  using iterator = T*;  // 用原生指针作为随机访问迭代器。
  using const_iterator = const T*;  // const 版本迭代器。
  using reverse_iterator = ReverseIterator<iterator>;  // 反向迭代器类型。

  MyVector() = default;  // 默认构造空 vector。
  MyVector(std::initializer_list<T> values) {  // 支持花括号列表构造。
    reserve(values.size());  // 先预留足够容量，避免重复扩容。
    for (const auto& value : values) {  // 遍历初始化列表中的每个元素。
      push_back(value);  // 逐个拷贝追加到数组尾部。
    }  // 初始化列表遍历结束。
  }  // initializer_list 构造结束。

  MyVector(const MyVector& other) {  // 拷贝构造 vector。
    reserve(other.size_);  // 预留源对象大小的容量。
    for (const auto& value : other) {  // 遍历源对象元素。
      push_back(value);  // 拷贝追加每个元素。
    }  // 拷贝遍历结束。
  }  // 拷贝构造结束。

  MyVector(MyVector&&) noexcept = default;  // 默认移动构造，转移 unique_ptr 所有权。
  MyVector& operator=(MyVector other) noexcept {  // 使用 copy-and-swap 实现拷贝/移动赋值。
    swap(other);  // 与参数副本交换资源，旧资源随 other 析构释放。
    return *this;  // 返回当前对象以支持链式赋值。
  }  // 赋值运算符结束。

  void push_back(const T& value) {  // 追加左值元素。
    ensure_capacity(size_ + 1);  // 确保至少能容纳新元素。
    data_[size_++] = value;  // 拷贝赋值到尾部并递增大小。
  }  // 左值 push_back 结束。

  void push_back(T&& value) {  // 追加右值元素。
    ensure_capacity(size_ + 1);  // 确保至少能容纳新元素。
    data_[size_++] = std::move(value);  // 移动赋值到尾部并递增大小。
  }  // 右值 push_back 结束。

  void pop_back() {  // 删除尾部元素。
    if (empty()) {  // 空 vector 不能弹出元素。
      throw std::out_of_range("pop_back on empty MyVector");  // 抛出越界异常。
    }  // 空检查结束。
    --size_;  // 简化实现只缩小逻辑大小，数组元素析构交给 unique_ptr<T[]>。
  }  // pop_back 结束。

  void reserve(std::size_t capacity) {  // 预留至少 capacity 个元素空间。
    if (capacity <= capacity_) {  // 目标容量不大于当前容量时无需操作。
      return;  // 直接返回。
    }  // 容量检查结束。
    auto next = std::make_unique<T[]>(capacity);  // 分配新的连续数组。
    for (std::size_t index = 0; index < size_; ++index) {  // 遍历已有元素。
      next[index] = std::move(data_[index]);  // 把旧数组元素移动到新数组。
    }  // 元素迁移结束。
    data_ = std::move(next);  // 用新数组替换旧数组。
    capacity_ = capacity;  // 更新容量。
  }  // reserve 结束。

  T& operator[](std::size_t index) { return data_[index]; }  // 返回指定下标元素的可修改引用，不做越界检查。
  const T& operator[](std::size_t index) const { return data_[index]; }  // 返回指定下标元素的只读引用，不做越界检查。
  T& front() { return data_[0]; }  // 返回首元素引用。
  const T& front() const { return data_[0]; }  // 返回首元素只读引用。
  T& back() { return data_[size_ - 1]; }  // 返回尾元素引用。
  const T& back() const { return data_[size_ - 1]; }  // 返回尾元素只读引用。
  std::size_t size() const { return size_; }  // 返回当前元素个数。
  std::size_t capacity() const { return capacity_; }  // 返回当前已分配容量。
  bool empty() const { return size_ == 0; }  // 判断容器是否为空。

  iterator begin() { return data_.get(); }  // 返回首元素迭代器。
  iterator end() { return data_.get() + size_; }  // 返回尾后迭代器。
  const_iterator begin() const { return data_.get(); }  // 返回 const 首元素迭代器。
  const_iterator end() const { return data_.get() + size_; }  // 返回 const 尾后迭代器。
  reverse_iterator rbegin() { return reverse_iterator(end()); }  // 返回反向首迭代器，基础位置是 end。
  reverse_iterator rend() { return reverse_iterator(begin()); }  // 返回反向尾后迭代器，基础位置是 begin。

  void swap(MyVector& other) noexcept {  // 与另一个 MyVector 交换内部资源。
    using std::swap;  // 允许 ADL 找到更合适的 swap。
    swap(data_, other.data_);  // 交换底层数组所有权。
    swap(size_, other.size_);  // 交换元素数量。
    swap(capacity_, other.capacity_);  // 交换容量。
  }  // swap 结束。

 private:  // 以下成员维护底层动态数组和容量增长。
  void ensure_capacity(std::size_t required) {  // 确保容量至少能容纳 required 个元素。
    if (required > capacity_) {  // 当前容量不足时扩容。
      reserve(capacity_ == 0 ? 4 : capacity_ * 2);  // 首次扩到 4，之后容量翻倍。
    }  // 容量足够时无需操作。
  }  // ensure_capacity 结束。

  std::unique_ptr<T[]> data_;  // 管理连续动态数组内存。
  std::size_t size_ = 0;  // 当前元素个数。
  std::size_t capacity_ = 0;  // 当前可容纳元素数量。
};  // MyVector 定义结束。

template <typename T>  // T 是双向链表保存的元素类型。
class MyList {  // 简化版 list，使用带哨兵节点的双向链表。
  struct Node {  // 链表节点，保存元素和前后指针。
    explicit Node(T value) : value(std::move(value)) {}  // 用传入值构造节点元素。
    T value{};  // 当前节点保存的元素值。
    Node* prev = nullptr;  // 指向前一个节点。
    Node* next = nullptr;  // 指向后一个节点。
  };  // Node 定义结束。

 public:  // 暴露迭代器和链表常用接口。
  class iterator {  // 双向链表迭代器，内部保存节点指针。
   public:  // 暴露标准迭代器关联类型和移动/访问操作。
    using difference_type = std::ptrdiff_t;  // 迭代器距离类型。
    using value_type = T;  // 迭代器指向的值类型。
    using pointer = T*;  // 指针类型。
    using reference = T&;  // 引用类型。
    using iterator_category = std::bidirectional_iterator_tag;  // 声明为标准双向迭代器。

    iterator() = default;  // 默认构造空迭代器。
    explicit iterator(Node* node) : node_(node) {}  // 用节点指针构造迭代器。
    reference operator*() const { return node_->value; }  // 解引用得到当前节点元素。
    pointer operator->() const { return &node_->value; }  // 箭头运算符返回当前元素地址。
    iterator& operator++() { node_ = node_->next; return *this; }  // 前置 ++ 移动到后继节点。
    iterator operator++(int) { iterator old(*this); ++(*this); return old; }  // 后置 ++ 返回移动前副本。
    iterator& operator--() { node_ = node_->prev; return *this; }  // 前置 -- 移动到前驱节点。
    iterator operator--(int) { iterator old(*this); --(*this); return old; }  // 后置 -- 返回移动前副本。
    bool operator==(const iterator& other) const { return node_ == other.node_; }  // 相等比较节点指针。
    bool operator!=(const iterator& other) const { return !(*this == other); }  // 不等比较复用相等比较。
   private:  // 迭代器内部状态。
    Node* node_ = nullptr;  // 当前迭代器指向的节点。
  };  // MyList::iterator 定义结束。

  MyList() { init_sentinel(); }  // 默认构造空链表并初始化哨兵节点。
  MyList(std::initializer_list<T> values) : MyList() { for (const auto& value : values) push_back(value); }  // 用初始化列表逐个尾插元素。
  ~MyList() { clear(); delete sentinel_; }  // 析构时先清空普通节点，再释放哨兵节点。
  MyList(const MyList&) = delete;  // 禁止拷贝构造，避免浅拷贝节点指针。
  MyList& operator=(const MyList&) = delete;  // 禁止拷贝赋值，避免浅拷贝节点指针。

  void push_back(const T& value) { insert_before(sentinel_, new Node(value)); }  // 在哨兵前插入节点，即尾插。
  void push_front(const T& value) { insert_before(sentinel_->next, new Node(value)); }  // 在首节点前插入节点，即头插。
  void pop_back() { erase_node(sentinel_->prev); }  // 删除哨兵前的尾节点。
  void pop_front() { erase_node(sentinel_->next); }  // 删除哨兵后的首节点。
  T& front() { return sentinel_->next->value; }  // 返回首元素引用。
  T& back() { return sentinel_->prev->value; }  // 返回尾元素引用。
  bool empty() const { return size_ == 0; }  // 判断链表是否为空。
  std::size_t size() const { return size_; }  // 返回链表元素个数。
  iterator begin() { return iterator(sentinel_->next); }  // 返回首节点迭代器，空表时等于 end。
  iterator end() { return iterator(sentinel_); }  // 返回哨兵节点迭代器作为尾后位置。

  void clear() { while (!empty()) pop_front(); }  // 循环头删直到链表为空。

 private:  // 以下成员负责哨兵节点和链表指针维护。
  void init_sentinel() {  // 初始化循环哨兵节点。
    sentinel_ = new Node(T{});  // 创建不保存有效用户元素的哨兵节点。
    sentinel_->prev = sentinel_;  // 空链表中哨兵前驱指向自己。
    sentinel_->next = sentinel_;  // 空链表中哨兵后继指向自己。
  }  // init_sentinel 结束。
  void insert_before(Node* pos, Node* node) {  // 将 node 插入到 pos 之前。
    node->prev = pos->prev;  // 新节点前驱指向 pos 原来的前驱。
    node->next = pos;  // 新节点后继指向 pos。
    pos->prev->next = node;  // 原前驱的后继改为新节点。
    pos->prev = node;  // pos 的前驱改为新节点。
    ++size_;  // 更新元素数量。
  }  // insert_before 结束。
  void erase_node(Node* node) {  // 删除指定普通节点。
    if (node == sentinel_) throw std::out_of_range("erase sentinel");  // 禁止删除哨兵节点。
    node->prev->next = node->next;  // 前驱绕过当前节点指向后继。
    node->next->prev = node->prev;  // 后继绕过当前节点指向前驱。
    delete node;  // 释放当前节点。
    --size_;  // 更新元素数量。
  }  // erase_node 结束。

  Node* sentinel_ = nullptr;  // 循环链表哨兵节点指针。
  std::size_t size_ = 0;  // 当前元素数量，不包含哨兵。
};  // MyList 定义结束。

template <typename T>  // T 是双端队列保存的元素类型。
class MyDeque {  // 简化版 deque，使用环形缓冲区支持头尾操作。
 public:  // 暴露迭代器和双端队列常用接口。
  class iterator {  // MyDeque 的随机访问迭代器，内部保存逻辑下标。
   public:  // 暴露标准迭代器关联类型和随机访问操作。
    using difference_type = std::ptrdiff_t;  // 迭代器距离类型。
    using value_type = T;  // 迭代器指向的值类型。
    using pointer = T*;  // 指针类型。
    using reference = T&;  // 引用类型。
    using iterator_category = std::random_access_iterator_tag;  // 声明为随机访问迭代器。

    iterator() = default;  // 默认构造空迭代器。
    iterator(MyDeque* deque, std::size_t index) : deque_(deque), index_(index) {}  // 绑定所属 deque 和逻辑下标。
    reference operator*() const { return (*deque_)[index_]; }  // 解引用时通过 deque 的逻辑下标访问元素。
    iterator& operator++() { ++index_; return *this; }  // 前置 ++ 移动到下一个逻辑位置。
    iterator operator++(int) { iterator old(*this); ++(*this); return old; }  // 后置 ++ 返回移动前副本。
    iterator& operator--() { --index_; return *this; }  // 前置 -- 移动到上一个逻辑位置。
    iterator& operator+=(difference_type offset) { index_ += offset; return *this; }  // 按偏移向后跳转。
    iterator& operator-=(difference_type offset) { index_ -= offset; return *this; }  // 按偏移向前跳转。
    iterator operator+(difference_type offset) const { iterator copy(*this); return copy += offset; }  // 返回加偏移后的新迭代器。
    iterator operator-(difference_type offset) const { iterator copy(*this); return copy -= offset; }  // 返回减偏移后的新迭代器。
    difference_type operator-(const iterator& other) const { return static_cast<difference_type>(index_) - static_cast<difference_type>(other.index_); }  // 返回两个逻辑下标的距离。
    bool operator==(const iterator& other) const { return deque_ == other.deque_ && index_ == other.index_; }  // 同属同一 deque 且下标相同则相等。
    bool operator!=(const iterator& other) const { return !(*this == other); }  // 不等比较复用相等比较。
    bool operator<(const iterator& other) const { return index_ < other.index_; }  // 按逻辑下标比较顺序。
   private:  // 迭代器内部状态。
    MyDeque* deque_ = nullptr;  // 所属双端队列指针。
    std::size_t index_ = 0;  // 当前逻辑下标。
  };  // MyDeque::iterator 定义结束。

  MyDeque() { reserve(8); }  // 默认构造时分配初始环形缓冲区容量。
  MyDeque(std::initializer_list<T> values) : MyDeque() { for (const auto& value : values) push_back(value); }  // 用初始化列表逐个尾插元素。

  void push_back(const T& value) {  // 在逻辑尾部插入元素。
    ensure_capacity(size_ + 1);  // 确保容量能容纳新元素。
    data_[physical_index(size_)] = value;  // 将逻辑尾部下标转换为物理下标并写入。
    ++size_;  // 更新元素数量。
  }  // push_back 结束。

  void push_front(const T& value) {  // 在逻辑头部插入元素。
    ensure_capacity(size_ + 1);  // 确保容量能容纳新元素。
    head_ = (head_ + capacity_ - 1) % capacity_;  // 环形前移 head，留出新首元素位置。
    data_[head_] = value;  // 在新的物理头位置写入元素。
    ++size_;  // 更新元素数量。
  }  // push_front 结束。

  void pop_back() {  // 删除逻辑尾部元素。
    if (empty()) throw std::out_of_range("pop_back on empty MyDeque");  // 空 deque 不能弹出尾部。
    --size_;  // 简化实现只缩小逻辑大小。
  }  // pop_back 结束。

  void pop_front() {  // 删除逻辑头部元素。
    if (empty()) throw std::out_of_range("pop_front on empty MyDeque");  // 空 deque 不能弹出头部。
    head_ = (head_ + 1) % capacity_;  // 环形后移 head 到新的首元素位置。
    --size_;  // 更新元素数量。
  }  // pop_front 结束。

  T& operator[](std::size_t index) { return data_[physical_index(index)]; }  // 按逻辑下标返回可修改元素引用。
  const T& operator[](std::size_t index) const { return data_[physical_index(index)]; }  // 按逻辑下标返回只读元素引用。
  T& front() { return (*this)[0]; }  // 返回逻辑首元素。
  T& back() { return (*this)[size_ - 1]; }  // 返回逻辑尾元素。
  std::size_t size() const { return size_; }  // 返回当前元素个数。
  bool empty() const { return size_ == 0; }  // 判断 deque 是否为空。
  iterator begin() { return iterator(this, 0); }  // 返回逻辑首位置迭代器。
  iterator end() { return iterator(this, size_); }  // 返回逻辑尾后位置迭代器。

 private:  // 以下成员维护环形缓冲区布局。
  std::size_t physical_index(std::size_t logical_index) const { return (head_ + logical_index) % capacity_; }  // 将逻辑下标映射为物理数组下标。
  void ensure_capacity(std::size_t required) {  // 确保环形缓冲区容量足够。
    if (required <= capacity_) return;  // 容量足够时直接返回。
    reserve(capacity_ * 2);  // 容量不足时翻倍扩容。
  }  // ensure_capacity 结束。
  void reserve(std::size_t capacity) {  // 分配新的连续缓冲区并整理逻辑顺序。
    auto next = std::make_unique<T[]>(capacity);  // 分配新数组。
    for (std::size_t index = 0; index < size_; ++index) {  // 按逻辑顺序遍历已有元素。
      next[index] = std::move((*this)[index]);  // 移动到新数组的连续位置。
    }  // 元素迁移结束。
    data_ = std::move(next);  // 用新数组替换旧数组。
    capacity_ = capacity;  // 更新容量。
    head_ = 0;  // 扩容后元素从物理下标 0 开始连续存放。
  }  // reserve 结束。

  std::unique_ptr<T[]> data_;  // 管理环形缓冲区数组。
  std::size_t head_ = 0;  // 逻辑首元素对应的物理下标。
  std::size_t size_ = 0;  // 当前元素个数。
  std::size_t capacity_ = 0;  // 当前缓冲区容量。
};  // MyDeque 定义结束。

template <typename T>  // T 是单向链表保存的元素类型。
class MyForwardList {  // 简化版 forward_list，只支持头插和前向遍历。
  struct Node { T value; Node* next = nullptr; };  // 单向链表节点，保存值和后继指针。
 public:  // 暴露前向迭代器和链表基本接口。
  class iterator {  // 单向链表前向迭代器。
   public:  // 暴露标准迭代器关联类型和基础操作。
    using difference_type = std::ptrdiff_t;  // 迭代器距离类型。
    using value_type = T;  // 迭代器指向的值类型。
    using pointer = T*;  // 指针类型。
    using reference = T&;  // 引用类型。
    using iterator_category = std::forward_iterator_tag;  // 声明为标准前向迭代器。
    explicit iterator(Node* node = nullptr) : node_(node) {}  // 用节点指针构造迭代器，默认表示 end。
    reference operator*() const { return node_->value; }  // 解引用得到当前节点元素。
    iterator& operator++() { node_ = node_->next; return *this; }  // 前置 ++ 移动到后继节点。
    bool operator==(const iterator& other) const { return node_ == other.node_; }  // 相等比较节点指针。
    bool operator!=(const iterator& other) const { return !(*this == other); }  // 不等比较复用相等比较。
   private:  // 迭代器内部状态。
    Node* node_ = nullptr;  // 当前节点指针，nullptr 表示尾后位置。
  };  // MyForwardList::iterator 定义结束。
  ~MyForwardList() { clear(); }  // 析构时释放所有节点。
  void push_front(const T& value) { head_ = new Node{value, head_}; ++size_; }  // 头插新节点并更新大小。
  void pop_front() { Node* old = head_; head_ = head_->next; delete old; --size_; }  // 删除头节点并释放内存。
  bool empty() const { return size_ == 0; }  // 判断链表是否为空。
  std::size_t size() const { return size_; }  // 返回当前元素数量。
  iterator begin() { return iterator(head_); }  // 返回首节点迭代器。
  iterator end() { return iterator(); }  // 返回空指针尾后迭代器。
  void clear() { while (head_) pop_front(); }  // 循环删除头节点直到链表为空。
 private:  // 以下成员维护链表头和大小。
  Node* head_ = nullptr;  // 指向首节点，空表时为 nullptr。
  std::size_t size_ = 0;  // 当前元素数量。
};  // MyForwardList 定义结束。

template <typename T, typename Container = MyVector<T>>  // T 是栈元素类型，Container 是底层顺序容器类型。
class MyStack {  // 简化版 stack 适配器，默认基于 MyVector。
 public:  // 暴露 LIFO 栈接口。
  void push(const T& value) { container_.push_back(value); }  // 入栈等价于底层容器尾插。
  void pop() { container_.pop_back(); }  // 出栈等价于删除底层容器尾元素。
  T& top() { return container_.back(); }  // 栈顶对应底层容器尾元素。
  bool empty() const { return container_.empty(); }  // 查询栈是否为空。
  std::size_t size() const { return container_.size(); }  // 查询栈元素数量。
 private:  // 保存底层容器。
  Container container_;  // 实际存储栈元素的容器。
};  // MyStack 定义结束。

template <typename T>  // T 是队列元素类型。
class MyQueue {  // 简化版 queue 适配器，基于 MyList 实现 FIFO。
 public:  // 暴露 FIFO 队列接口。
  void push(const T& value) { list_.push_back(value); }  // 入队等价于链表尾插。
  void pop() { list_.pop_front(); }  // 出队等价于链表头删。
  T& front() { return list_.front(); }  // 队首对应链表首元素。
  bool empty() const { return list_.empty(); }  // 查询队列是否为空。
  std::size_t size() const { return list_.size(); }  // 查询队列元素数量。
 private:  // 保存底层链表。
  MyList<T> list_;  // 实际存储队列元素的双向链表。
};  // MyQueue 定义结束。

template <typename T, typename Compare = std::less<T>>  // T 是元素类型，Compare 控制堆优先级。
class MyPriorityQueue {  // 简化版 priority_queue，基于 MyVector 和标准堆算法。
 public:  // 暴露优先队列接口。
  void push(const T& value) { data_.push_back(value); std::push_heap(data_.begin(), data_.end(), compare_); }  // 追加元素后上滤维护堆。
  void pop() { std::pop_heap(data_.begin(), data_.end(), compare_); data_.pop_back(); }  // 把堆顶移到尾部后删除。
  const T& top() const { return data_.front(); }  // 堆顶元素位于底层数组首位。
  bool empty() const { return data_.empty(); }  // 查询优先队列是否为空。
  std::size_t size() const { return data_.size(); }  // 查询元素数量。
 private:  // 保存堆数据和比较器。
  MyVector<T> data_;  // 底层数组，按堆结构排列元素。
  Compare compare_{};  // 比较器对象，决定大顶堆或小顶堆语义。
};  // MyPriorityQueue 定义结束。

}  // namespace mini_stl  // 手写 STL 序列容器命名空间结束。