#pragma once  // 防止头文件被重复包含

#include <atomic>  // 使用原子引用计数
#include <cstddef>  // 预留标准大小类型支持
#include <utility>  // 预留移动工具支持

template<typename T>  // 声明独占指针模板
class UniquePtr {  // 定义简化版独占智能指针
  T* ptr_ = nullptr;  // 保存被管理对象指针

public:  // 开始公开接口
  explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {}  // 从裸指针构造独占指针

  ~UniquePtr() {  // 析构独占指针
    delete ptr_;  // 释放被管理对象
  }  // 结束析构函数

  UniquePtr(const UniquePtr&) = delete;  // 禁止拷贝构造
  UniquePtr& operator=(const UniquePtr&) = delete;  // 禁止拷贝赋值

  UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {  // 移动构造并接管资源
    other.ptr_ = nullptr;  // 清空来源指针
  }  // 结束移动构造

  UniquePtr& operator=(UniquePtr&& other) noexcept {  // 移动赋值并接管资源
    if (this != &other) {  // 避免自移动赋值
      delete ptr_;  // 释放当前资源
      ptr_ = other.ptr_;  // 接管来源资源
      other.ptr_ = nullptr;  // 清空来源指针
    }  // 结束自赋值检查
    return *this;  // 返回当前对象
  }  // 结束移动赋值

  T* get() const noexcept {  // 获取裸指针
    return ptr_;  // 返回被管理对象地址
  }  // 结束裸指针访问

  T& operator*() const {  // 解引用被管理对象
    return *ptr_;  // 返回对象引用
  }  // 结束解引用运算符

  T* operator->() const noexcept {  // 访问被管理对象成员
    return ptr_;  // 返回对象指针
  }  // 结束箭头运算符
};  // 结束 UniquePtr 定义

struct ControlBlockBase {  // 定义共享指针控制块基类
  std::atomic<int> use_count{1};  // 保存共享引用计数

  virtual void destroy_object() = 0;  // 定义销毁托管对象接口
  virtual ~ControlBlockBase() = default;  // 允许通过基类析构控制块
};  // 结束控制块基类

template<typename T>  // 声明裸指针控制块模板
struct ControlBlockPtr final : ControlBlockBase {  // 定义保存裸指针的控制块
  T* ptr = nullptr;  // 保存被共享管理的对象指针

  explicit ControlBlockPtr(T* ptr_value) : ptr(ptr_value) {}  // 初始化托管对象指针

  void destroy_object() override {  // 销毁托管对象
    delete ptr;  // 释放对象内存
    ptr = nullptr;  // 清空对象指针
  }  // 结束对象销毁函数
};  // 结束裸指针控制块

template<typename T>  // 声明共享指针模板
class SharedPtr {  // 定义简化版共享智能指针
  T* ptr_ = nullptr;  // 保存被管理对象指针
  ControlBlockBase* control_ = nullptr;  // 保存共享控制块指针

  void release_current() noexcept {  // 释放当前共享引用
    if (control_ == nullptr) {  // 检查是否没有控制块
      return;  // 空指针无需释放
    }  // 结束空控制块检查

    if (--control_->use_count == 0) {  // 递减计数并判断是否为最后引用
      control_->destroy_object();  // 销毁托管对象
      delete control_;  // 释放控制块
    }  // 结束最后引用处理

    ptr_ = nullptr;  // 清空对象指针
    control_ = nullptr;  // 清空控制块指针
  }  // 结束当前引用释放

public:  // 开始公开接口
  SharedPtr() noexcept = default;  // 默认构造空共享指针

  explicit SharedPtr(T* ptr) : ptr_(ptr) {  // 从裸指针构造共享指针
    if (ptr_ != nullptr) {  // 仅为非空指针创建控制块
      control_ = new ControlBlockPtr<T>(ptr_);  // 分配并初始化控制块
    }  // 结束非空检查
  }  // 结束裸指针构造

  ~SharedPtr() {  // 析构共享指针
    release_current();  // 释放当前共享引用
  }  // 结束析构函数

  SharedPtr(const SharedPtr& other) noexcept : ptr_(other.ptr_), control_(other.control_) {  // 拷贝构造共享引用
    if (control_ != nullptr) {  // 仅非空控制块需要计数
      ++control_->use_count;  // 增加共享引用计数
    }  // 结束控制块检查
  }  // 结束拷贝构造

  SharedPtr& operator=(const SharedPtr& other) noexcept {  // 拷贝赋值共享引用
    if (this != &other) {  // 避免自拷贝赋值
      release_current();  // 先释放当前引用
      ptr_ = other.ptr_;  // 复制对象指针
      control_ = other.control_;  // 复制控制块指针
      if (control_ != nullptr) {  // 仅非空控制块需要计数
        ++control_->use_count;  // 增加共享引用计数
      }  // 结束控制块检查
    }  // 结束自赋值检查
    return *this;  // 返回当前对象
  }  // 结束拷贝赋值

  SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), control_(other.control_) {  // 移动构造共享指针
    other.ptr_ = nullptr;  // 清空来源对象指针
    other.control_ = nullptr;  // 清空来源控制块指针
  }  // 结束移动构造

  SharedPtr& operator=(SharedPtr&& other) noexcept {  // 移动赋值共享指针
    if (this != &other) {  // 避免自移动赋值
      release_current();  // 先释放当前引用
      ptr_ = other.ptr_;  // 接管来源对象指针
      control_ = other.control_;  // 接管来源控制块指针
      other.ptr_ = nullptr;  // 清空来源对象指针
      other.control_ = nullptr;  // 清空来源控制块指针
    }  // 结束自赋值检查
    return *this;  // 返回当前对象
  }  // 结束移动赋值

  T* get() const noexcept {  // 获取裸指针
    return ptr_;  // 返回被管理对象地址
  }  // 结束裸指针访问

  T& operator*() const {  // 解引用被管理对象
    return *ptr_;  // 返回对象引用
  }  // 结束解引用运算符

  T* operator->() const noexcept {  // 访问被管理对象成员
    return ptr_;  // 返回对象指针
  }  // 结束箭头运算符

  int use_count() const noexcept {  // 查询共享引用计数
    return control_ == nullptr ? 0 : control_->use_count.load();  // 返回空指针或控制块计数
  }  // 结束引用计数查询
};  // 结束 SharedPtr 定义