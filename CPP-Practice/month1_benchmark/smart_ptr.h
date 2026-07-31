#pragma once

#include <atomic>
#include <cstddef>
#include <utility>

template <typename T>
class UniquePtr {
    T* ptr_ = nullptr;

 public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {}

    ~UniquePtr() { delete ptr_; }

    // 独占语义：禁拷贝，只允许移动转移所有权。
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;  // 置空源，防止其析构时误删已转移的对象。
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {  // 自移动保护：否则下一行会删掉自己要接管的资源。
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T* get() const noexcept { return ptr_; }

    T& operator*() const { return *ptr_; }

    T* operator->() const noexcept { return ptr_; }
};

// 控制块基类：类型擦除，让 SharedPtr<T> 不必知道具体删除逻辑。
struct ControlBlockBase {
    std::atomic<int> use_count{1};  // 原子计数，支持多线程共享同一对象。

    virtual void destroy_object() = 0;
    virtual ~ControlBlockBase() = default;  // 虚析构：经基类指针 delete 时也能正确清理。
};

template <typename T>
struct ControlBlockPtr final : ControlBlockBase {
    T* ptr = nullptr;

    explicit ControlBlockPtr(T* ptr_value) : ptr(ptr_value) {}

    void destroy_object() override {
        delete ptr;
        ptr = nullptr;
    }
};

template <typename T>
class SharedPtr {
    T* ptr_ = nullptr;
    ControlBlockBase* control_ = nullptr;

    void release_current() noexcept {
        if (control_ == nullptr) {
            return;
        }

        if (--control_->use_count == 0) {  // 最后一个引用负责销毁对象和控制块。
            control_->destroy_object();
            delete control_;
        }

        ptr_ = nullptr;
        control_ = nullptr;
    }

 public:
    SharedPtr() noexcept = default;

    explicit SharedPtr(T* ptr) : ptr_(ptr) {
        if (ptr_ != nullptr) {  // 空指针不建控制块，use_count() 便返回 0。
            control_ = new ControlBlockPtr<T>(ptr_);
        }
    }

    ~SharedPtr() { release_current(); }

    SharedPtr(const SharedPtr& other) noexcept : ptr_(other.ptr_), control_(other.control_) {
        if (control_ != nullptr) {
            ++control_->use_count;  // 多一个引用。
        }
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            release_current();  // 先放掉旧引用，再接管新引用。
            ptr_ = other.ptr_;
            control_ = other.control_;
            if (control_ != nullptr) {
                ++control_->use_count;
            }
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), control_(other.control_) {
        // 移动不改引用计数：源让出所有权即可。
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release_current();
            ptr_ = other.ptr_;
            control_ = other.control_;
            other.ptr_ = nullptr;
            other.control_ = nullptr;
        }
        return *this;
    }

    T* get() const noexcept { return ptr_; }

    T& operator*() const { return *ptr_; }

    T* operator->() const noexcept { return ptr_; }

    int use_count() const noexcept { return control_ == nullptr ? 0 : control_->use_count.load(); }
};
