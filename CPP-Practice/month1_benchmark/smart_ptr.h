#pragma once

#include <atomic>
#include <cstddef>
#include <utility>

template<typename T>
class UniquePtr {
  T* ptr_ = nullptr;

public:
  explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {}

  ~UniquePtr() {
    delete ptr_;
  }

  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (this != &other) {
      delete ptr_;
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }

  T* get() const noexcept {
    return ptr_;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const noexcept {
    return ptr_;
  }
};

struct ControlBlockBase {
  std::atomic<int> use_count{1};

  virtual void destroy_object() = 0;
  virtual ~ControlBlockBase() = default;
};

template<typename T>
struct ControlBlockPtr final : ControlBlockBase {
  T* ptr = nullptr;

  explicit ControlBlockPtr(T* ptr_value) : ptr(ptr_value) {}

  void destroy_object() override {
    delete ptr;
    ptr = nullptr;
  }
};

template<typename T>
class SharedPtr {
  T* ptr_ = nullptr;
  ControlBlockBase* control_ = nullptr;

  void release_current() noexcept {
    if (control_ == nullptr) {
      return;
    }

    if (--control_->use_count == 0) {
      control_->destroy_object();
      delete control_;
    }

    ptr_ = nullptr;
    control_ = nullptr;
  }

public:
  SharedPtr() noexcept = default;

  explicit SharedPtr(T* ptr) : ptr_(ptr) {
    if (ptr_ != nullptr) {
      control_ = new ControlBlockPtr<T>(ptr_);
    }
  }

  ~SharedPtr() {
    release_current();
  }

  SharedPtr(const SharedPtr& other) noexcept : ptr_(other.ptr_), control_(other.control_) {
    if (control_ != nullptr) {
      ++control_->use_count;
    }
  }

  SharedPtr& operator=(const SharedPtr& other) noexcept {
    if (this != &other) {
      release_current();
      ptr_ = other.ptr_;
      control_ = other.control_;
      if (control_ != nullptr) {
        ++control_->use_count;
      }
    }
    return *this;
  }

  SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), control_(other.control_) {
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

  T* get() const noexcept {
    return ptr_;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const noexcept {
    return ptr_;
  }

  int use_count() const noexcept {
    return control_ == nullptr ? 0 : control_->use_count.load();
  }
};