#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
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

template <typename T>
class MyVector {  // 连续存储 + 裸指针作随机访问迭代器。
 public:
    using value_type = T;
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

    MyVector(MyVector&&) noexcept = default;
    // copy-and-swap：按值接参，一份实现同时覆盖拷贝赋值与移动赋值，且天然强异常安全。
    MyVector& operator=(MyVector other) noexcept {
        swap(other);
        return *this;
    }

    void push_back(const T& value) {
        ensure_capacity(size_ + 1);
        data_[size_++] = value;
    }

    void push_back(T&& value) {
        ensure_capacity(size_ + 1);
        data_[size_++] = std::move(value);
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("pop_back on empty MyVector");
        }
        // 简化实现：只缩小逻辑大小，元素析构延迟到整个 unique_ptr<T[]> 释放时。
        --size_;
    }

    void reserve(std::size_t capacity) {
        if (capacity <= capacity_) {
            return;
        }
        auto next = std::make_unique<T[]>(capacity);
        for (std::size_t index = 0; index < size_; ++index) {
            next[index] = std::move(data_[index]);
        }
        data_ = std::move(next);
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

    iterator begin() { return data_.get(); }
    iterator end() { return data_.get() + size_; }
    const_iterator begin() const { return data_.get(); }
    const_iterator end() const { return data_.get() + size_; }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    void swap(MyVector& other) noexcept {
        using std::swap;  // 启用 ADL，为自定义类型选到更优的 swap。
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

 private:
    void ensure_capacity(std::size_t required) {
        if (required > capacity_) {
            reserve(capacity_ == 0 ? 4 : capacity_ * 2);  // 翻倍扩容摊还 push_back 到均摊 O(1)。
        }
    }

    std::unique_ptr<T[]> data_;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};

template <typename T>
class MyList {  // 带哨兵节点的循环双向链表。
    struct Node {
        explicit Node(T value) : value(std::move(value)) {}
        T value{};
        Node* prev = nullptr;
        Node* next = nullptr;
    };

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
        delete sentinel_;
    }
    // 禁拷贝：默认拷贝会浅拷贝节点指针，导致两个链表共享并重复释放同一批节点。
    MyList(const MyList&) = delete;
    MyList& operator=(const MyList&) = delete;

    // 哨兵让头尾插入退化为“在某节点前插入”，无需对空链表/首尾做特判。
    void push_back(const T& value) { insert_before(sentinel_, new Node(value)); }
    void push_front(const T& value) { insert_before(sentinel_->next, new Node(value)); }
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
    void init_sentinel() {
        // 哨兵前后都指向自己，形成空的循环链，使插入删除逻辑对空表也成立。
        sentinel_ = new Node(T{});
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
        delete node;
        --size_;
    }

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

    void push_back(const T& value) {
        ensure_capacity(size_ + 1);
        data_[physical_index(size_)] = value;
        ++size_;
    }

    void push_front(const T& value) {
        ensure_capacity(size_ + 1);
        head_ =
            (head_ + capacity_ - 1) % capacity_;  // 环形前移 head（+capacity 防下溢）腾出新首位。
        data_[head_] = value;
        ++size_;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("pop_back on empty MyDeque");
        --size_;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("pop_front on empty MyDeque");
        head_ = (head_ + 1) % capacity_;  // head 环形后移，被弹元素留在原地不析构（简化实现）。
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

 private:
    std::size_t physical_index(std::size_t logical_index) const {
        return (head_ + logical_index) % capacity_;
    }  // 逻辑下标绕 head 映射到物理槽位。
    void ensure_capacity(std::size_t required) {
        if (required <= capacity_) return;
        reserve(capacity_ * 2);
    }
    void reserve(std::size_t capacity) {
        // 扩容顺带把环形布局摊平：按逻辑序拷入新数组并把 head 归零，后续下标运算更简单。
        auto next = std::make_unique<T[]>(capacity);
        for (std::size_t index = 0; index < size_; ++index) {
            next[index] = std::move((*this)[index]);
        }
        data_ = std::move(next);
        capacity_ = capacity;
        head_ = 0;
    }

    std::unique_ptr<T[]> data_;
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
    ~MyForwardList() { clear(); }
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