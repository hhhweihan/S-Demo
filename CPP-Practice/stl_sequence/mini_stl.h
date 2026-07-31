#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace mini_stl {

// 迭代器分类标签用继承表达能力包含关系（random ⊃ bidirectional ⊃ forward ⊃ input），
// 使算法可用标签分派（tag dispatch）在编译期选择最优实现。
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};

template <typename Iterator>
struct iterator_traits {
    using difference_type = typename Iterator::difference_type;
    using value_type = typename Iterator::value_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};

// 裸指针没有内嵌 typedef，必须靠这个特化让它也能被当作随机访问迭代器使用。
template <typename T>
struct iterator_traits<T*> {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = random_access_iterator_tag;
};

template <typename Iterator>
class ReverseIterator {  // 语义等价 std::reverse_iterator。
 public:
    using iterator_type = Iterator;
    using traits = std::iterator_traits<Iterator>;
    using difference_type = typename traits::difference_type;
    using value_type = typename traits::value_type;
    using pointer = typename traits::pointer;
    using reference = typename traits::reference;
    using iterator_category = typename traits::iterator_category;

    ReverseIterator() = default;
    explicit ReverseIterator(Iterator current) : current_(current) {}

    Iterator base() const { return current_; }

    reference operator*() const {
        // 关键约定：反向迭代器逻辑元素是 base() 的前一个，故解引用要先退一格。
        // 这样 rbegin()==end()、rend()==begin() 的对称关系才成立。
        Iterator tmp = current_;
        return *--tmp;
    }

    ReverseIterator& operator++() {
        --current_;  // 反向前进 = 基础迭代器后退。
        return *this;
    }

    ReverseIterator operator++(int) {
        ReverseIterator old(*this);
        --current_;
        return old;
    }

    ReverseIterator& operator--() {
        ++current_;
        return *this;
    }

    bool operator==(const ReverseIterator& other) const { return current_ == other.current_; }
    bool operator!=(const ReverseIterator& other) const { return !(*this == other); }

 private:
    Iterator current_{};  // 指向逻辑元素之后一位（即 base 位置）。
};

// 第二个模板参数 Alloc 让容器可插拔内存来源：所有裸存储的申请/释放、元素的构造/析构
// 一律经 std::allocator_traits<Alloc> 转发，而非直接 ::operator new / placement-new / ~T()。
// 走 traits 的好处：① 统一了“有 allocate 成员就用、没有就退回默认”的接口；② 元素构造用
// traits::construct（而非裸 placement-new）后，未来若换成会做记账/对齐/调试标记的分配器也无需
// 改动容器；③ MyVector<int> 单参写法仍可用，因为 Alloc 默认 std::allocator<T>。
template <typename T, class Alloc = std::allocator<T>>
class MyVector {  // 连续存储 + 裸指针作随机访问迭代器。
 public:
    using value_type = T;
    using allocator_type = Alloc;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = ReverseIterator<iterator>;

    MyVector() = default;
    MyVector(std::initializer_list<T> values) {
        reserve(values.size());  // 一次预留到位，避免逐个 push_back 反复扩容搬移。
        for (const auto& value : values) {
            push_back(value);
        }
    }

    MyVector(const MyVector& other) {
        reserve(other.size_);
        for (const auto& value : other) {
            push_back(value);
        }
    }

    // 裸存储持有原始指针，默认移动会浅拷贝指针导致重复释放，故手写移动并置空来源。
    MyVector(MyVector&& other) noexcept
        : alloc_(std::move(other.alloc_)),
          data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~MyVector() { destroy(); }

    // copy-and-swap：按值接参，一份实现同时覆盖拷贝赋值与移动赋值，且天然强异常安全。
    MyVector& operator=(MyVector other) noexcept {
        swap(other);
        return *this;
    }

    void push_back(const T& value) {
        ensure_capacity(size_ + 1);
        construct_at(data_ + size_, value);  // 经 traits::construct 在裸槽位就地构造，而非赋值。
        ++size_;
    }

    void push_back(T&& value) {
        ensure_capacity(size_ + 1);
        construct_at(data_ + size_, std::move(value));
        ++size_;
    }

    // 变参就地构造：直接把参数转发给 traits::construct，省去先建临时再拷贝/移动的一步。
    template <class... Args>
    reference emplace_back(Args&&... args) {
        ensure_capacity(size_ + 1);
        construct_at(data_ + size_, std::forward<Args>(args)...);
        ++size_;
        return data_[size_ - 1];
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("pop_back on empty MyVector");
        }
        --size_;
        destroy_at(data_ + size_);  // 经 traits::destroy 显式析构被弹出元素，裸存储不会自动销毁。
    }

    // 在 pos 前插入：先算下标（扩容会使 pos 失效，故用偏移量而非指针记录位置）。
    iterator insert(iterator pos, const T& value) { return emplace(pos, value); }
    iterator insert(iterator pos, T&& value) { return emplace(pos, std::move(value)); }

    // emplace 承担 insert 的全部搬移逻辑：末尾插入退化为就地构造；中间插入则先把尾元素移动
    // 构造进新槽位，再逐个向后移动赋值腾出空档，最后把新值放到空档处。
    template <class... Args>
    iterator emplace(iterator pos, Args&&... args) {
        std::size_t index = static_cast<std::size_t>(pos - data_);
        ensure_capacity(size_ + 1);  // 可能重新分配，index 是偏移量故仍有效。
        if (index < size_) {
            construct_at(data_ + size_, std::move(data_[size_ - 1]));  // 新的尾槽位就地构造。
            for (std::size_t i = size_ - 1; i > index; --i) {
                data_[i] = std::move(data_[i - 1]);  // 已构造区间用移动赋值右移一格。
            }
            data_[index] = T(std::forward<Args>(args)...);  // 空档处赋入新值。
        } else {
            construct_at(data_ + size_, std::forward<Args>(args)...);  // 末尾直接就地构造。
        }
        ++size_;
        return data_ + index;
    }

    // 删除单个元素：后续元素整体左移一格覆盖，最后析构多出来的尾元素。
    iterator erase(iterator pos) {
        std::size_t index = static_cast<std::size_t>(pos - data_);
        for (std::size_t i = index; i + 1 < size_; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        destroy_at(data_ + size_ - 1);  // 左移后末尾出现重复元素，须析构避免泄漏。
        --size_;
        return data_ + index;
    }

    // 删除区间 [first, last)：把 last 之后的元素左移 count 格，再析构末尾多出的 count 个。
    iterator erase(iterator first, iterator last) {
        std::size_t f = static_cast<std::size_t>(first - data_);
        std::size_t l = static_cast<std::size_t>(last - data_);
        std::size_t count = l - f;
        if (count == 0) return data_ + f;
        for (std::size_t i = l; i < size_; ++i) {
            data_[i - count] = std::move(data_[i]);
        }
        for (std::size_t i = size_ - count; i < size_; ++i) destroy_at(data_ + i);
        size_ -= count;
        return data_ + f;
    }

    // 缩小时析构多余元素，增大时对新槽位默认构造（要求 T 可默认构造）。
    void resize(std::size_t n) {
        if (n < size_) {
            for (std::size_t i = n; i < size_; ++i) destroy_at(data_ + i);
            size_ = n;
        } else if (n > size_) {
            reserve(n);
            for (; size_ < n; ++size_) construct_at(data_ + size_);
        }
    }

    // 增大时用 value 拷贝构造新槽位。
    void resize(std::size_t n, const T& value) {
        if (n < size_) {
            for (std::size_t i = n; i < size_; ++i) destroy_at(data_ + i);
            size_ = n;
        } else if (n > size_) {
            reserve(n);
            for (; size_ < n; ++size_) construct_at(data_ + size_, value);
        }
    }

    void clear() noexcept {
        for (std::size_t index = 0; index < size_; ++index) destroy_at(data_ + index);
        size_ = 0;
    }

    void reserve(std::size_t capacity) {
        if (capacity <= capacity_) {
            return;
        }
        T* next = allocate(capacity);
        std::size_t index = 0;
        try {
            for (; index < size_; ++index) {
                // move_if_noexcept：移动可能抛出且元素可拷贝时退回拷贝，保住强异常保证。
                construct_at(next + index, std::move_if_noexcept(data_[index]));
            }
        } catch (...) {
            // 搬移中途失败：销毁已构造的新元素并释放新缓冲，旧缓冲原封不动（强保证）。
            for (std::size_t built = 0; built < index; ++built) destroy_at(next + built);
            deallocate(next, capacity);
            throw;
        }
        for (std::size_t old = 0; old < size_; ++old) destroy_at(data_ + old);  // 销毁旧缓冲元素。
        deallocate(data_, capacity_);
        data_ = next;
        capacity_ = capacity;
    }

    T& operator[](std::size_t index) {
        return data_[index];
    }  // 不做越界检查，语义对齐 std::vector::operator[]。
    const T& operator[](std::size_t index) const { return data_[index]; }
    T& front() { return data_[0]; }
    const T& front() const { return data_[0]; }
    T& back() { return data_[size_ - 1]; }
    const T& back() const { return data_[size_ - 1]; }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    void swap(MyVector& other) noexcept {
        using std::swap;  // 启用 ADL，为自定义类型选到更优的 swap。
        swap(alloc_, other.alloc_);
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

 private:
    using AllocTraits = std::allocator_traits<Alloc>;

    // 只申请原始存储、不构造元素：走 traits::allocate 而非 ::operator new，让分配来源可插拔。
    T* allocate(std::size_t count) {
        if (count == 0) return nullptr;
        return AllocTraits::allocate(alloc_, count);
    }
    // traits::deallocate 需要元素个数（分配器可能据此记账），故释放时把对应容量传回。
    void deallocate(T* ptr, std::size_t count) noexcept {
        if (ptr) AllocTraits::deallocate(alloc_, ptr, count);
    }
    // 元素构造/析构统一经 traits，而非裸 placement-new / ~T()，便于换用带记账的分配器。
    template <class... Args>
    void construct_at(T* ptr, Args&&... args) {
        AllocTraits::construct(alloc_, ptr, std::forward<Args>(args)...);
    }
    void destroy_at(T* ptr) noexcept { AllocTraits::destroy(alloc_, ptr); }
    void destroy() noexcept {
        for (std::size_t index = 0; index < size_; ++index) destroy_at(data_ + index);
        deallocate(data_, capacity_);
    }
    void ensure_capacity(std::size_t required) {
        if (required > capacity_) {
            reserve(capacity_ == 0 ? 4 : capacity_ * 2);  // 翻倍扩容摊还 push_back 到均摊 O(1)。
        }
    }

    Alloc alloc_{};  // 分配器实例：无状态分配器（如 std::allocator）为空基本零开销。
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};

// 链表同样接受 Alloc 参数。但节点型容器有个关键差异：用户给的是 allocator<T>，而我们真正要
// 分配的是 Node（内含 T + 两个指针），二者大小/类型都不同。故必须用 rebind_alloc<Node> 把分配器
// “改绑”到 Node 类型上，再经 traits 申请/释放/构造/析构节点——这正是标准容器内部对 list/map
// 等节点结构的通用做法。
template <typename T, class Alloc = std::allocator<T>>
class MyList {  // 带哨兵节点的循环双向链表。
    struct Node {
        explicit Node(T value) : value(std::move(value)) {}
        T value{};
        Node* prev = nullptr;
        Node* next = nullptr;
    };

    // 把用户的 allocator<T> 改绑到 Node 上：链表分配的是节点而非裸 T，二者类型不同。
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using NodeAllocTraits = std::allocator_traits<NodeAlloc>;

 public:
    class iterator {
     public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() = default;
        explicit iterator(Node* node) : node_(node) {}
        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }
        iterator& operator++() {
            node_ = node_->next;
            return *this;
        }
        iterator operator++(int) {
            iterator old(*this);
            ++(*this);
            return old;
        }
        iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }
        iterator operator--(int) {
            iterator old(*this);
            --(*this);
            return old;
        }
        bool operator==(const iterator& other) const { return node_ == other.node_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

     private:
        Node* node_ = nullptr;
    };

    MyList() { init_sentinel(); }
    MyList(std::initializer_list<T> values) : MyList() {
        for (const auto& value : values) push_back(value);
    }
    ~MyList() {
        clear();
        destroy_node(sentinel_);
    }
    // 禁拷贝：默认拷贝会浅拷贝节点指针，导致两个链表共享并重复释放同一批节点。
    MyList(const MyList&) = delete;
    MyList& operator=(const MyList&) = delete;

    // 哨兵让头尾插入退化为“在某节点前插入”，无需对空链表/首尾做特判。
    void push_back(const T& value) { insert_before(sentinel_, create_node(value)); }
    void push_front(const T& value) { insert_before(sentinel_->next, create_node(value)); }
    void pop_back() { erase_node(sentinel_->prev); }
    void pop_front() { erase_node(sentinel_->next); }
    T& front() { return sentinel_->next->value; }
    T& back() { return sentinel_->prev->value; }
    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
    iterator begin() { return iterator(sentinel_->next); }
    iterator end() { return iterator(sentinel_); }  // 哨兵即尾后位置，空链表时 begin()==end()。

    void clear() {
        while (!empty()) pop_front();
    }

 private:
    // 申请一个节点的裸存储并就地构造：allocate/construct 均经改绑后的 NodeAllocTraits，
    // 而非裸 new Node——这样节点内存也纳入用户分配器的管辖。构造失败要回退释放，避免泄漏。
    template <class... Args>
    Node* create_node(Args&&... args) {
        Node* node = NodeAllocTraits::allocate(node_alloc_, 1);
        try {
            NodeAllocTraits::construct(node_alloc_, node, std::forward<Args>(args)...);
        } catch (...) {
            NodeAllocTraits::deallocate(node_alloc_, node, 1);
            throw;
        }
        return node;
    }
    // 先析构再释放，对应 create_node 的“构造 + 分配”，两步都走 traits。
    void destroy_node(Node* node) noexcept {
        NodeAllocTraits::destroy(node_alloc_, node);
        NodeAllocTraits::deallocate(node_alloc_, node, 1);
    }
    void init_sentinel() {
        // 哨兵前后都指向自己，形成空的循环链，使插入删除逻辑对空表也成立。
        sentinel_ = create_node(T{});
        sentinel_->prev = sentinel_;
        sentinel_->next = sentinel_;
    }
    void insert_before(Node* pos, Node* node) {
        node->prev = pos->prev;
        node->next = pos;
        pos->prev->next = node;
        pos->prev = node;
        ++size_;
    }
    void erase_node(Node* node) {
        if (node == sentinel_)
            throw std::out_of_range("erase sentinel");  // 删哨兵会破坏循环结构，属非法操作。
        node->prev->next = node->next;
        node->next->prev = node->prev;
        destroy_node(node);
        --size_;
    }

    NodeAlloc node_alloc_{};  // 改绑到 Node 的分配器实例：链表内存全部由它管辖。
    Node* sentinel_ = nullptr;
    std::size_t size_ = 0;  // 不含哨兵。
};

template <typename T>
class MyDeque {  // 环形缓冲区，头尾插入均摊 O(1)。
 public:
    class iterator {
     public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

        iterator() = default;
        // 迭代器只存逻辑下标，把“逻辑→物理”换算交给 deque，扩容重排后迭代器语义不变。
        iterator(MyDeque* deque, std::size_t index) : deque_(deque), index_(index) {}
        reference operator*() const { return (*deque_)[index_]; }
        iterator& operator++() {
            ++index_;
            return *this;
        }
        iterator operator++(int) {
            iterator old(*this);
            ++(*this);
            return old;
        }
        iterator& operator--() {
            --index_;
            return *this;
        }
        iterator& operator+=(difference_type offset) {
            index_ += offset;
            return *this;
        }
        iterator& operator-=(difference_type offset) {
            index_ -= offset;
            return *this;
        }
        iterator operator+(difference_type offset) const {
            iterator copy(*this);
            return copy += offset;
        }
        iterator operator-(difference_type offset) const {
            iterator copy(*this);
            return copy -= offset;
        }
        difference_type operator-(const iterator& other) const {
            return static_cast<difference_type>(index_) -
                   static_cast<difference_type>(other.index_);
        }
        bool operator==(const iterator& other) const {
            return deque_ == other.deque_ && index_ == other.index_;
        }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        bool operator<(const iterator& other) const { return index_ < other.index_; }

     private:
        MyDeque* deque_ = nullptr;
        std::size_t index_ = 0;
    };

    MyDeque() { reserve(8); }
    MyDeque(std::initializer_list<T> values) : MyDeque() {
        for (const auto& value : values) push_back(value);
    }

    MyDeque(const MyDeque& other) {
        reserve(other.capacity_ == 0 ? 8 : other.capacity_);  // 预留后按逻辑序逐个就地构造。
        for (std::size_t index = 0; index < other.size_; ++index) push_back(other[index]);
    }

    // 裸存储持有原始指针，默认移动会浅拷贝导致重复释放，故手写移动并置空来源。
    MyDeque(MyDeque&& other) noexcept
        : data_(other.data_), head_(other.head_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.head_ = 0;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~MyDeque() { destroy(); }

    // copy-and-swap：一份实现覆盖拷贝赋值与移动赋值。
    MyDeque& operator=(MyDeque other) noexcept {
        swap(other);
        return *this;
    }

    void push_back(const T& value) {
        ensure_capacity(size_ + 1);
        ::new (static_cast<void*>(data_ + physical_index(size_))) T(value);  // 裸槽位就地构造。
        ++size_;
    }

    void push_front(const T& value) {
        ensure_capacity(size_ + 1);
        head_ =
            (head_ + capacity_ - 1) % capacity_;  // 环形前移 head（+capacity 防下溢）腾出新首位。
        ::new (static_cast<void*>(data_ + head_)) T(value);
        ++size_;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("pop_back on empty MyDeque");
        data_[physical_index(size_ - 1)].~T();  // 显式析构被弹出的尾元素。
        --size_;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("pop_front on empty MyDeque");
        data_[head_].~T();                 // 显式析构被弹出的首元素，裸存储不会自动销毁。
        head_ = (head_ + 1) % capacity_;   // head 环形后移。
        --size_;
    }

    T& operator[](std::size_t index) { return data_[physical_index(index)]; }
    const T& operator[](std::size_t index) const { return data_[physical_index(index)]; }
    T& front() { return (*this)[0]; }
    T& back() { return (*this)[size_ - 1]; }
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size_); }

    void swap(MyDeque& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(head_, other.head_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

 private:
    std::size_t physical_index(std::size_t logical_index) const {
        return (head_ + logical_index) % capacity_;
    }  // 逻辑下标绕 head 映射到物理槽位。
    static T* allocate(std::size_t count) {
        if (count == 0) return nullptr;
        return static_cast<T*>(::operator new(count * sizeof(T)));
    }
    static void deallocate(T* ptr) noexcept { ::operator delete(ptr); }
    void destroy() noexcept {
        for (std::size_t index = 0; index < size_; ++index) data_[physical_index(index)].~T();
        deallocate(data_);
    }
    void ensure_capacity(std::size_t required) {
        if (required <= capacity_) return;
        reserve(capacity_ == 0 ? 8 : capacity_ * 2);
    }
    void reserve(std::size_t capacity) {
        // 扩容顺带把环形布局摊平：按逻辑序就地构造进新数组并把 head 归零，后续下标运算更简单。
        T* next = allocate(capacity);
        std::size_t index = 0;
        try {
            for (; index < size_; ++index) {
                ::new (static_cast<void*>(next + index)) T(std::move_if_noexcept((*this)[index]));
            }
        } catch (...) {
            for (std::size_t built = 0; built < index; ++built) next[built].~T();
            deallocate(next);
            throw;
        }
        for (std::size_t old = 0; old < size_; ++old) (*this)[old].~T();  // 销毁旧环形缓冲元素。
        deallocate(data_);
        data_ = next;
        capacity_ = capacity;
        head_ = 0;
    }

    T* data_ = nullptr;
    std::size_t head_ = 0;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};

template <typename T>
class MyForwardList {  // 单链表，只支持头插与前向遍历。
    struct Node {
        T value;
        Node* next = nullptr;
    };

 public:
    class iterator {
     public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::forward_iterator_tag;
        explicit iterator(Node* node = nullptr)
            : node_(node) {}  // 默认构造的空指针迭代器充当 end。
        reference operator*() const { return node_->value; }
        iterator& operator++() {
            node_ = node_->next;
            return *this;
        }
        bool operator==(const iterator& other) const { return node_ == other.node_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

     private:
        Node* node_ = nullptr;
    };
    MyForwardList() = default;
    ~MyForwardList() { clear(); }

    // 深拷贝：按 other 的顺序逐节点新建，绝不共享 head_，避免两个链表重复释放同一批节点。
    MyForwardList(const MyForwardList& other) {
        Node** tail = &head_;  // 用二级指针把新节点依次挂到链尾，保持原有顺序。
        for (Node* cur = other.head_; cur != nullptr; cur = cur->next) {
            *tail = new Node{cur->value, nullptr};
            tail = &(*tail)->next;
        }
        size_ = other.size_;
    }

    MyForwardList(MyForwardList&& other) noexcept : head_(other.head_), size_(other.size_) {
        other.head_ = nullptr;  // 置空来源，移动后不再指向被接管的节点链。
        other.size_ = 0;
    }

    // copy-and-swap：拷贝实参走深拷贝构造，移动实参走移动构造，一份实现覆盖两种赋值。
    MyForwardList& operator=(MyForwardList other) noexcept {
        swap(other);
        return *this;
    }

    void swap(MyForwardList& other) noexcept {
        using std::swap;
        swap(head_, other.head_);
        swap(size_, other.size_);
    }

    void push_front(const T& value) {
        head_ = new Node{value, head_};
        ++size_;
    }  // 头插 O(1)：新节点 next 接原 head。
    void pop_front() {
        Node* old = head_;
        head_ = head_->next;
        delete old;
        --size_;
    }
    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(); }
    void clear() {
        while (head_) pop_front();
    }

 private:
    Node* head_ = nullptr;
    std::size_t size_ = 0;
};

// 适配器只暴露受限接口，底层容器可替换：栈用尾部即可，故默认 MyVector。
template <typename T, typename Container = MyVector<T>>
class MyStack {
 public:
    void push(const T& value) { container_.push_back(value); }
    void pop() { container_.pop_back(); }
    T& top() { return container_.back(); }
    bool empty() const { return container_.empty(); }
    std::size_t size() const { return container_.size(); }

 private:
    Container container_;
};

// 队列需高效头删，故选双向链表 MyList 而非 vector（vector 头删是 O(n)）。
template <typename T>
class MyQueue {
 public:
    void push(const T& value) { list_.push_back(value); }
    void pop() { list_.pop_front(); }
    T& front() { return list_.front(); }
    bool empty() const { return list_.empty(); }
    std::size_t size() const { return list_.size(); }

 private:
    MyList<T> list_;
};

template <typename T, typename Compare = std::less<T>>
class MyPriorityQueue {  // 基于 MyVector + 标准堆算法；Compare 决定大顶/小顶。
 public:
    void push(const T& value) {
        data_.push_back(value);
        std::push_heap(data_.begin(), data_.end(), compare_);
    }
    void pop() {
        std::pop_heap(data_.begin(), data_.end(), compare_);
        data_.pop_back();
    }  // pop_heap 把堆顶换到末尾，再删除。
    const T& top() const { return data_.front(); }
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }

 private:
    MyVector<T> data_;
    Compare compare_{};
};

}  // namespace mini_stl