## share_ptr是线程安全的吗
回答：
- 如果多个线程同时拷贝同一个shared_ptr对象，不会有问题，因为shared_ptr的引用技术是线程安全的。
- 如果多个线程同时修改同一个shared_ptr对象，不是线程安全的。
- 如果多个线程同时读写shared_ptr指向的内存对象，不是线程安全的。

### 关键点
- 引用计数的安全性：std::shared_ptr的引用计数是原子的，这意味着增加或减少引用计数的操作是线程安全的，即使这些操作在多个线程中同时进行也不会出现问题。
- 赋值操作的不安全性：std::shared_ptr的赋值操作(包括拷贝构造和赋值运算符)涉及多个步骤，这些操作在多个线程中同时进行时可能不安全，因为它们不是原子的。
- 对象访问的不安全性：std::shared_ptr不会为被管理的对象提供任何线程安全的保障。如果多个线程尝试读取或修改被管理对象的状态，必须使用锁或其他同步机制来确保线程安全。
- 构造和析构的不安全性：如果多个线程同时创建或销毁std::shared_ptr实例，虽然引用计数的增减是线程安全的，但与对象关联的构造和析构操作可能需要额外的同步来避免竞态条件。

### std::shared_ptr主要由一下关键部分组成
- 控制块：存储引用计数(use_count:跟踪有多少std::shared_ptr实例指向同个对象)和弱引用计数(weak_count:跟踪有多少std::weak_ptr实例指向同一个控制块)。
- 智能指针对象：持有指向控制块的指针和指向托管对象的指针。

### 核心操作
- 构造和析构。
- 复制构造和赋值。
- 释放对象(当引用计数降到0时)。
- 增加和减少引用计数。
- 获取和设置指向的原始指针。

### 使用样例
```C++
#include <iostream>
#include <memory>
 
struct BigObj {
    BigObj() {
        std::cout << "big object has been constructed" << std::endl;
    }
    ~BigObj() {
        std::cout << "big object has been destructed" << std::endl;
    }
};
 
void test_ref() {
    std::shared_ptr<BigObj> sp1 = std::make_shared<BigObj>();   // 调用了BigObj构造
    std::cout << sp1.use_count() << std::endl;  // 1
    std::shared_ptr<BigObj> sp2 = sp1;   
    std::cout << sp2.use_count() << std::endl;  // 2
    std::shared_ptr<BigObj> sp3 = sp2; 
    std::cout << sp3.use_count() << std::endl;  // 3
    std::cout << sp1.use_count() << std::endl;  // 3    
}
 
void test_ref1() {
    std::shared_ptr<BigObj> sp1 = std::make_shared<BigObj>();   // 调用了BigObj构造
    std::cout << sp1.use_count() << std::endl;  // 1
 
    {
        std::shared_ptr<BigObj> sp2 = sp1;  
        std::cout << sp1.use_count() << std::endl; // 2   
    }
    std::cout << sp1.use_count() << std::endl;  // 1
    BigObj* ptr = sp1.get(); 
 
    sp1 = nullptr;
    std::cout << sp1.use_count() << std::endl;  // 0
}
 
int main() {
 
    test_ref();   // 程序生命周期结束，即使引用计数不是0，智能指针也进行资源析构
    test_ref1();  // 当引用计数变为0，它会调用控制块中存储的析构函数来销毁资源。
 
}
```

### 样例输出结果
```C++
big object has been constructed
1
2
3
3
big object has been destructed
big object has been constructed
1
2
1
big object has been destructed
0
```

### shared_ptr的析构操作
- 减少其内部控制块中的引用计数。
- 如果引用计数变为0，它会调用控制块中存储的析构函数来销毁资源。
- 最后，它会减少控制块的弱引用计数，并如果弱引用计数也变为0，那么整个控制块将被销毁。
因此，当一个函数结束时，如果该函数内定义的std::shared_ptr是唯一引用某个对象的实例，那么该对象将在函数结束时被自动销毁。这是std::shared_ptr提供的一种自动资源管理机制，有助于防止内存泄漏和资源泄露。

### shared_ptr简单代码示例
```C++
template<typename T>
class shared_ptr {
private:
    T* ptr; // 指向分配的内存资源
    size_t* ref_count; // 引用计数

public:
    // 构造函数
    explicit shared_ptr(T* p = nullptr) : ptr(p), ref_count(new size_t(1)) {}

    // 拷贝构造函数
    shared_ptr(const shared_ptr<T>& other) : ptr(other.ptr), ref_count(other.ref_count) {
        (*ref_count)++;
    }

    // 移动构造函数
    shared_ptr(shared_ptr<T>&& other) noexcept : ptr(other.ptr), ref_count(other.ref_count) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    // 析构函数
    ~shared_ptr() {
        release();
    }

    // 重载赋值运算符
    shared_ptr<T>& operator=(const shared_ptr<T>& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            (*ref_count)++;
        }
        return *this;
    }

    // 重载移动赋值运算符
    shared_ptr<T>& operator=(shared_ptr<T>&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    // 获取指针指向的对象
    T* get() const { return ptr; }

    // 获取引用计数
    size_t use_count() const { return (ref_count != nullptr) ? *ref_count : 0; }

    // 重载箭头操作符
    T* operator->() const { return ptr; }

    // 解引用操作符
    T& operator*() const { return *ptr; }

    // 释放资源
    void release() {
        if (ref_count != nullptr) {
            (*ref_count)--;
            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
        }
    }

    // 重置指针
    void reset(T* p = nullptr) {
        release();
        ptr = p;
        ref_count = new size_t(1);
    }
};
```