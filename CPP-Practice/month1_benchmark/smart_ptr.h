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

// 前置声明：控制块、SharedPtr、WeakPtr 互相引用。
template <typename T>
class SharedPtr;
template <typename T>
class WeakPtr;
template <typename T>
class EnableSharedFromThis;

// 控制块基类：类型擦除，让 SharedPtr<T> / WeakPtr<T> 不必知道具体删除逻辑。
//
// 两阶段生命周期（对齐 libstdc++/libc++ 语义）：
//   use_count  —— 强引用数：还有多少 SharedPtr 在“拥有”对象。
//   weak_count —— 弱引用数：所有 WeakPtr 之和，再加 1 代表“强引用整体”这一份。
// 阶段一：use_count 归零 → 只销毁被管理的对象（析构 + 释放对象内存），
//         但控制块本身要保留，因为可能还有 WeakPtr 需要读取 use_count 判断是否过期。
// 阶段二：weak_count 也归零 → 此时没有任何 Shared/Weak 指向控制块，才释放控制块。
// 这样“对象”与“控制块”解耦：弱引用能安全地在对象已死后判断 expired()，
// 而不会访问到已释放的控制块内存。
struct ControlBlockBase {
    std::atomic<int> use_count{1};   // 强引用计数：最后一个强引用负责销毁对象。
    std::atomic<int> weak_count{1};  // 弱引用计数：初始 1 代表“强引用整体”这一份。

    virtual void destroy_object() = 0;      // 阶段一：销毁被管理对象。
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

    // 允许所有 SharedPtr<U> / WeakPtr<U> 互访内部（enable_shared_from_this、lock 需要）。
    template <typename U>
    friend class SharedPtr;
    template <typename U>
    friend class WeakPtr;

    // 私有构造：直接接管一个“已经就绪”的控制块，并把 use_count +1。
    // 供 enable_shared_from_this 的接线复用同一控制块。
    SharedPtr(T* ptr, ControlBlockBase* control) noexcept : ptr_(ptr), control_(control) {
        if (control_ != nullptr) {
            ++control_->use_count;
        }
    }

    // 释放当前强引用：两阶段收尾的核心。
    void release_current() noexcept {
        if (control_ == nullptr) {
            return;
        }

        if (--control_->use_count == 0) {  // 阶段一：最后一个强引用先销毁对象。
            control_->destroy_object();
            // 强引用整体这一份弱计数也随之归还；若无任何 WeakPtr，则进入阶段二。
            if (--control_->weak_count == 0) {
                delete control_;  // 阶段二：再无任何引用，释放控制块本身。
            }
        }

        ptr_ = nullptr;
        control_ = nullptr;
    }

    // 检测被管理对象是否继承 EnableSharedFromThis<U>，若是则回填其内部弱引用，
    // 使 shared_from_this() 能拿到与本 SharedPtr 共享同一控制块的 SharedPtr。
    template <typename U>
    void enable_weak_this(const EnableSharedFromThis<U>* base, U* raw) noexcept {
        if (base != nullptr && base->weak_this_.expired()) {
            // 用共享同一控制块的 SharedPtr<U> 初始化基类里的 WeakPtr<U>（只加 weak_count）。
            base->weak_this_ = SharedPtr<U>(raw, control_);
        }
    }
    // 非 EnableSharedFromThis 类型的兜底重载：什么都不做。
    void enable_weak_this(const volatile void*, const volatile void*) noexcept {}

 public:
    SharedPtr() noexcept = default;

    explicit SharedPtr(T* ptr) : ptr_(ptr) {
        if (ptr_ != nullptr) {  // 空指针不建控制块，use_count() 便返回 0。
            control_ = new ControlBlockPtr<T>(ptr_);
            enable_weak_this(ptr_, ptr_);  // 若对象派生自 EnableSharedFromThis，接线其弱引用。
        }
    }

    ~SharedPtr() { release_current(); }

    SharedPtr(const SharedPtr& other) noexcept : ptr_(other.ptr_), control_(other.control_) {
        if (control_ != nullptr) {
            ++control_->use_count;  // 多一个强引用。
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

    void reset() noexcept { release_current(); }

    T* get() const noexcept { return ptr_; }

    T& operator*() const { return *ptr_; }

    T* operator->() const noexcept { return ptr_; }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    int use_count() const noexcept { return control_ == nullptr ? 0 : control_->use_count.load(); }
};

// WeakPtr：观察者，不拥有对象，只持有对控制块的弱引用（bump weak_count）。
// 关键作用——打破循环引用：若两个对象用 SharedPtr 互指，强计数永远不会归零，
// 对象泄漏；把其中一条边换成 WeakPtr，就不再贡献 use_count，
// 于是最后一个外部 SharedPtr 析构时 use_count 能归零，对象被正常销毁。
template <typename T>
class WeakPtr {
    T* ptr_ = nullptr;
    ControlBlockBase* control_ = nullptr;

    template <typename U>
    friend class WeakPtr;
    template <typename U>
    friend class SharedPtr;

    // 释放当前弱引用：可能触发阶段二（对象已死且再无弱引用）。
    void release_current() noexcept {
        if (control_ != nullptr) {
            if (--control_->weak_count == 0) {
                delete control_;  // 阶段二：控制块寿终正寝。
            }
        }
        ptr_ = nullptr;
        control_ = nullptr;
    }

 public:
    WeakPtr() noexcept = default;

    // 从 SharedPtr 构造：只增弱计数，绝不碰强计数，故不会延长对象寿命。
    WeakPtr(const SharedPtr<T>& shared) noexcept : ptr_(shared.ptr_), control_(shared.control_) {
        if (control_ != nullptr) {
            ++control_->weak_count;
        }
    }

    WeakPtr(const WeakPtr& other) noexcept : ptr_(other.ptr_), control_(other.control_) {
        if (control_ != nullptr) {
            ++control_->weak_count;
        }
    }

    WeakPtr(WeakPtr&& other) noexcept : ptr_(other.ptr_), control_(other.control_) {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    ~WeakPtr() { release_current(); }

    WeakPtr& operator=(const WeakPtr& other) noexcept {
        if (this != &other) {
            release_current();
            ptr_ = other.ptr_;
            control_ = other.control_;
            if (control_ != nullptr) {
                ++control_->weak_count;
            }
        }
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            release_current();
            ptr_ = other.ptr_;
            control_ = other.control_;
            other.ptr_ = nullptr;
            other.control_ = nullptr;
        }
        return *this;
    }

    // 允许用 SharedPtr 重新绑定弱引用（enable_shared_from_this 接线会用到）。
    WeakPtr& operator=(const SharedPtr<T>& shared) noexcept {
        release_current();
        ptr_ = shared.ptr_;
        control_ = shared.control_;
        if (control_ != nullptr) {
            ++control_->weak_count;
        }
        return *this;
    }

    void reset() noexcept { release_current(); }

    // 强计数归零即视为过期：此时对象已被销毁（阶段一完成）。
    bool expired() const noexcept {
        return control_ == nullptr || control_->use_count.load() == 0;
    }

    int use_count() const noexcept { return control_ == nullptr ? 0 : control_->use_count.load(); }

    // 提升为 SharedPtr：仅当对象仍存活时成功，否则返回空指针。
    // 用 CAS 循环“只在 use_count>0 时 +1”，避免与最后一个强引用的销毁竞态。
    SharedPtr<T> lock() const noexcept {
        SharedPtr<T> result;
        if (control_ == nullptr) {
            return result;
        }
        int count = control_->use_count.load(std::memory_order_relaxed);
        while (count != 0) {
            if (control_->use_count.compare_exchange_weak(count, count + 1)) {
                // 抢到一个强引用：直接填字段，不再走会二次自增的构造路径。
                result.ptr_ = ptr_;
                result.control_ = control_;
                return result;
            }
            // compare_exchange_weak 失败时会把最新值写回 count，循环重试。
        }
        return result;  // 对象已过期，返回空 SharedPtr。
    }
};

// EnableSharedFromThis：让对象能安全地交出一个与既有控制块共享的 SharedPtr。
// 原理：对象内部藏一个 WeakPtr<T>，在首个 SharedPtr<T> 接管该对象时被回填；
// shared_from_this() 只是把这个 WeakPtr lock() 出来，因此绝不会新建控制块、
// 也就不会出现“同一对象两套引用计数导致重复销毁”的经典错误。
template <typename T>
class EnableSharedFromThis {
    mutable WeakPtr<T> weak_this_;  // 由 SharedPtr 的接线逻辑回填。

    template <typename U>
    friend class SharedPtr;

 protected:
    EnableSharedFromThis() noexcept = default;
    EnableSharedFromThis(const EnableSharedFromThis&) noexcept = default;
    EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept {
        // 拷贝对象不应连带弱引用：新对象将由它自己的 SharedPtr 接线。
        return *this;
    }
    ~EnableSharedFromThis() = default;

 public:
    SharedPtr<T> shared_from_this() { return weak_this_.lock(); }
    WeakPtr<T> weak_from_this() noexcept { return weak_this_; }
};
