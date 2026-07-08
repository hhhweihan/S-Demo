#pragma once  // 防止头文件重复包含

#include <atomic>  // 使用原子头指针
#include <cstddef>  // 使用 std::size_t
#include <memory>  // 使用 shared_ptr 承载数据
#include <mutex>  // 使用互斥锁保护退休链表
#include <utility>  // 使用 std::move
#include <vector>  // 使用数组保存延迟释放节点

template <typename T>  // 栈元素类型模板
class LockFreeStack {  // 基础无锁栈实现
public:  // 公开接口区
    LockFreeStack() = default;  // 使用默认构造

    ~LockFreeStack() {  // 析构时清理所有节点
        clear_live_nodes();  // 清理仍在栈中的节点
        clear_retired_nodes();  // 清理已退休节点
    }  // 结束析构函数

    LockFreeStack(const LockFreeStack&) = delete;  // 禁止拷贝构造
    LockFreeStack& operator=(const LockFreeStack&) = delete;  // 禁止拷贝赋值

    void push(T value) {  // 压入一个元素
        Node* new_node = new Node(std::move(value));  // 分配并初始化新节点
        new_node->next = head_.load(std::memory_order_relaxed);  // 先指向当前栈顶
        while (!head_.compare_exchange_weak(new_node->next,  // 循环尝试替换栈顶
                                            new_node,  // 成功时发布新节点
                                            std::memory_order_release,  // 成功时释放写入
                                            std::memory_order_relaxed)) {  // 失败时只需宽松读取
        }  // CAS 失败时 new_node->next 会被更新为新栈顶
    }  // 结束 push

    bool pop(T& out) {  // 尝试弹出一个元素
        Node* old_head = head_.load(std::memory_order_acquire);  // 读取当前栈顶
        while (old_head != nullptr &&  // 栈非空时尝试弹出
               !head_.compare_exchange_weak(old_head,  // 比较当前栈顶
                                            old_head->next,  // 成功时切到下一个节点
                                            std::memory_order_acq_rel,  // 成功时获取释放
                                            std::memory_order_acquire)) {  // 失败时重新获取栈顶
        }  // CAS 失败时 old_head 会被刷新

        if (old_head == nullptr) {  // 检查是否为空栈
            return false;  // 空栈弹出失败
        }  // 结束空栈检查

        out = *old_head->data;  // 拷贝弹出的数据
        retire_node(old_head);  // 延迟释放弹出的节点
        return true;  // 弹出成功
    }  // 结束 pop

    bool empty() const {  // 判断栈是否为空
        return head_.load(std::memory_order_acquire) == nullptr;  // 栈顶为空表示空栈
    }  // 结束 empty

private:  // 私有实现区
    struct Node {  // 栈节点结构
        explicit Node(T value)  // 构造节点并保存数据
            : data(std::make_shared<T>(std::move(value))) {}  // 数据存入 shared_ptr

        std::shared_ptr<T> data;  // 节点保存的数据
        Node* next = nullptr;  // 指向下一个节点
    };  // 结束 Node

    void retire_node(Node* node) {  // 将节点加入延迟释放列表
        std::lock_guard<std::mutex> lock(retired_mutex_);  // 加锁保护退休列表
        retired_.push_back(node);  // 保存待释放节点
    }  // 结束 retire_node

    void clear_live_nodes() {  // 清空仍在栈中的节点
        Node* current = head_.exchange(nullptr, std::memory_order_acq_rel);  // 取走整条活动链
        while (current != nullptr) {  // 遍历活动节点
            Node* next = current->next;  // 保存后继节点
            delete current;  // 释放当前节点
            current = next;  // 前进到下一个节点
        }  // 结束活动链遍历
    }  // 结束 clear_live_nodes

    void clear_retired_nodes() {  // 清空延迟释放节点
        std::lock_guard<std::mutex> lock(retired_mutex_);  // 加锁保护退休列表
        for (Node* node : retired_) {  // 遍历待释放节点
            delete node;  // 释放节点内存
        }  // 结束退休节点遍历
        retired_.clear();  // 清空退休列表
    }  // 结束 clear_retired_nodes

    std::atomic<Node*> head_{nullptr};  // 原子栈顶指针
    std::mutex retired_mutex_;  // 保护退休列表的互斥锁
    std::vector<Node*> retired_;  // 延迟释放的节点列表
};  // 结束 LockFreeStack

template <typename T>  // 栈元素类型模板
class ReclaimingLockFreeStack {  // 带延迟回收的无锁栈
public:  // 公开接口区
    ReclaimingLockFreeStack() = default;  // 使用默认构造

    ~ReclaimingLockFreeStack() {  // 析构时清理所有节点
        clear_live_nodes();  // 清理活动节点
        delete_nodes(to_be_deleted_.exchange(nullptr, std::memory_order_acq_rel));  // 清理待删除链表
    }  // 结束析构函数

    ReclaimingLockFreeStack(const ReclaimingLockFreeStack&) = delete;  // 禁止拷贝构造
    ReclaimingLockFreeStack& operator=(const ReclaimingLockFreeStack&) = delete;  // 禁止拷贝赋值

    void push(T value) {  // 压入一个元素
        Node* new_node = new Node(std::move(value));  // 分配新节点
        new_node->next = head_.load(std::memory_order_relaxed);  // 指向当前栈顶
        while (!head_.compare_exchange_weak(new_node->next,  // 循环尝试发布新栈顶
                                            new_node,  // 成功时写入新节点
                                            std::memory_order_release,  // 成功时释放数据写入
                                            std::memory_order_relaxed)) {  // 失败时宽松读取
        }  // CAS 失败时自动更新 next
    }  // 结束 push

    std::shared_ptr<T> pop() {  // 尝试弹出并返回共享数据
        threads_in_pop_.fetch_add(1, std::memory_order_acq_rel);  // 标记当前线程进入 pop

        Node* old_head = head_.load(std::memory_order_acquire);  // 读取当前栈顶
        while (old_head != nullptr &&  // 栈非空时尝试弹出
               !head_.compare_exchange_weak(old_head,  // 比较栈顶指针
                                            old_head->next,  // 成功时切到后继节点
                                            std::memory_order_acq_rel,  // 成功时获取释放
                                            std::memory_order_acquire)) {  // 失败时获取新栈顶
        }  // CAS 失败时刷新 old_head

        std::shared_ptr<T> result;  // 保存弹出的数据
        if (old_head != nullptr) {  // 只有弹到节点才取数据
            result.swap(old_head->data);  // 将节点数据转移到结果中
        }  // 结束数据转移

        try_reclaim(old_head);  // 尝试安全回收旧节点
        return result;  // 返回弹出数据或空指针
    }  // 结束 pop

    bool empty() const {  // 判断栈是否为空
        return head_.load(std::memory_order_acquire) == nullptr;  // 栈顶为空表示空栈
    }  // 结束 empty

private:  // 私有实现区
    struct Node {  // 栈节点结构
        explicit Node(T value)  // 构造节点并保存数据
            : data(std::make_shared<T>(std::move(value))) {}  // 数据存入 shared_ptr

        std::shared_ptr<T> data;  // 节点保存的数据
        Node* next = nullptr;  // 指向下一个节点或待删链表节点
    };  // 结束 Node

    static void delete_nodes(Node* nodes) {  // 删除一整条节点链
        while (nodes != nullptr) {  // 遍历链表
            Node* next = nodes->next;  // 保存后继节点
            delete nodes;  // 释放当前节点
            nodes = next;  // 前进到下一个节点
        }  // 结束链表遍历
    }  // 结束 delete_nodes

    void chain_pending_nodes(Node* first, Node* last) {  // 将节点链挂到待删除列表
        last->next = to_be_deleted_.load(std::memory_order_relaxed);  // 让尾节点接上当前待删链
        while (!to_be_deleted_.compare_exchange_weak(last->next,  // 循环尝试更新待删头
                                                     first,  // 成功时写入新链头
                                                     std::memory_order_release,  // 成功时发布链表
                                                     std::memory_order_relaxed)) {  // 失败时宽松重试
        }  // CAS 失败时尾节点 next 会被刷新
    }  // 结束 chain_pending_nodes

    void chain_pending_nodes(Node* nodes) {  // 将未知尾节点的链挂起
        Node* last = nodes;  // 从链头开始寻找尾节点
        while (last != nullptr && last->next != nullptr) {  // 遍历到链尾
            last = last->next;  // 前进一个节点
        }  // 结束尾节点查找
        if (nodes != nullptr && last != nullptr) {  // 确认链表非空
            chain_pending_nodes(nodes, last);  // 使用首尾节点挂入待删链
        }  // 结束非空检查
    }  // 结束 chain_pending_nodes

    void chain_pending_node(Node* node) {  // 挂起单个待删节点
        chain_pending_nodes(node, node);  // 单节点首尾相同
    }  // 结束 chain_pending_node

    void try_reclaim(Node* old_head) {  // 尝试回收刚弹出的节点
        if (old_head == nullptr) {  // 没有弹出节点时只退出 pop 区域
            threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel);  // 减少正在 pop 的线程数
            return;  // 无节点需要回收
        }  // 结束空节点检查

        if (threads_in_pop_.load(std::memory_order_acquire) == 1) {  // 只有当前线程在 pop 时可回收
            Node* nodes_to_delete = to_be_deleted_.exchange(nullptr, std::memory_order_acq_rel);  // 取走待删链
            if (threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel) == 1) {  // 再次确认无人进入 pop
                delete_nodes(nodes_to_delete);  // 安全删除待删链
            } else if (nodes_to_delete != nullptr) {  // 若期间有其他 pop 线程进入
                chain_pending_nodes(nodes_to_delete);  // 重新挂回待删链
            }  // 结束待删链处理
            delete old_head;  // 删除当前弹出的节点
        } else {  // 仍有其他线程可能读取旧节点
            chain_pending_node(old_head);  // 延迟回收当前节点
            threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel);  // 离开 pop 区域
        }  // 结束回收分支
    }  // 结束 try_reclaim

    void clear_live_nodes() {  // 清理活动链表
        Node* current = head_.exchange(nullptr, std::memory_order_acq_rel);  // 取走当前栈顶链
        delete_nodes(current);  // 删除活动链表
    }  // 结束 clear_live_nodes

    std::atomic<Node*> head_{nullptr};  // 原子栈顶指针
    std::atomic<unsigned> threads_in_pop_{0};  // 正在执行 pop 的线程计数
    std::atomic<Node*> to_be_deleted_{nullptr};  // 延迟删除节点链表头
};  // 结束 ReclaimingLockFreeStack

template <typename T>  // 栈元素类型模板
class TaggedPointerStack {  // 带版本标签的无锁栈
public:  // 公开接口区
    TaggedPointerStack() = default;  // 使用默认构造

    ~TaggedPointerStack() {  // 析构时清理节点
        clear_live_nodes();  // 清理活动节点
        clear_retired_nodes();  // 清理退休节点
    }  // 结束析构函数

    TaggedPointerStack(const TaggedPointerStack&) = delete;  // 禁止拷贝构造
    TaggedPointerStack& operator=(const TaggedPointerStack&) = delete;  // 禁止拷贝赋值

    void push(T value) {  // 压入一个元素
        Node* new_node = new Node(std::move(value));  // 分配新节点
        Head old_head = head_.load(std::memory_order_relaxed);  // 读取旧的带标签栈顶
        Head new_head{};  // 准备新的带标签栈顶

        do {  // 至少尝试一次 CAS
            new_node->next = old_head.ptr;  // 新节点接到旧栈顶前
            new_head.ptr = new_node;  // 新头指向新节点
            new_head.tag = old_head.tag + 1;  // 版本标签递增
        } while (!head_.compare_exchange_weak(old_head,  // 比较旧头并更新
                                              new_head,  // 成功时写入新头
                                              std::memory_order_release,  // 成功时释放写入
                                              std::memory_order_relaxed));  // 失败时宽松读取
    }  // 结束 push

    bool pop(T& out) {  // 尝试弹出一个元素
        Head old_head = head_.load(std::memory_order_acquire);  // 读取当前带标签栈顶
        Head new_head{};  // 准备新的带标签栈顶

        while (old_head.ptr != nullptr) {  // 栈非空时持续尝试
            new_head.ptr = old_head.ptr->next;  // 新头切到后继节点
            new_head.tag = old_head.tag + 1;  // 版本标签递增
            if (head_.compare_exchange_weak(old_head,  // 尝试替换带标签头
                                            new_head,  // 成功时写入新头
                                            std::memory_order_acq_rel,  // 成功时获取释放
                                            std::memory_order_acquire)) {  // 失败时获取新头
                out = *old_head.ptr->data;  // 拷贝弹出数据
                retire_node(old_head.ptr);  // 延迟释放旧节点
                return true;  // 弹出成功
            }  // 结束 CAS 成功分支
        }  // 结束弹出循环

        return false;  // 空栈弹出失败
    }  // 结束 pop

    bool empty() const {  // 判断栈是否为空
        return head_.load(std::memory_order_acquire).ptr == nullptr;  // 头指针为空表示空栈
    }  // 结束 empty

private:  // 私有实现区
    struct Node {  // 栈节点结构
        explicit Node(T value)  // 构造节点并保存数据
            : data(std::make_shared<T>(std::move(value))) {}  // 数据存入 shared_ptr

        std::shared_ptr<T> data;  // 节点保存的数据
        Node* next = nullptr;  // 指向下一个节点
    };  // 结束 Node

    struct Head {  // 带标签的栈顶描述
        Node* ptr = nullptr;  // 栈顶节点指针
        std::size_t tag = 0;  // ABA 防护版本标签
    };  // 结束 Head

    static_assert(std::is_trivially_copyable<Head>::value, "Head must be trivially copyable");  // 确保 Head 可被原子复制

    void retire_node(Node* node) {  // 将节点加入退休列表
        std::lock_guard<std::mutex> lock(retired_mutex_);  // 加锁保护退休列表
        retired_.push_back(node);  // 保存待释放节点
    }  // 结束 retire_node

    void clear_live_nodes() {  // 清理活动节点链
        Head current = head_.exchange(Head{}, std::memory_order_acq_rel);  // 取走当前带标签头
        Node* node = current.ptr;  // 获取活动链头节点
        while (node != nullptr) {  // 遍历活动链
            Node* next = node->next;  // 保存后继节点
            delete node;  // 释放当前节点
            node = next;  // 前进到下一个节点
        }  // 结束活动链遍历
    }  // 结束 clear_live_nodes

    void clear_retired_nodes() {  // 清理退休节点列表
        std::lock_guard<std::mutex> lock(retired_mutex_);  // 加锁保护退休列表
        for (Node* node : retired_) {  // 遍历退休节点
            delete node;  // 释放节点内存
        }  // 结束退休节点遍历
        retired_.clear();  // 清空退休列表
    }  // 结束 clear_retired_nodes

    std::atomic<Head> head_{};  // 原子带标签栈顶
    std::mutex retired_mutex_;  // 保护退休列表的互斥锁
    std::vector<Node*> retired_;  // 待延迟释放节点列表
};  // 结束 TaggedPointerStack
