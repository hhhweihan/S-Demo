#pragma once

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

template <typename T>
class LockFreeStack {
public:
    LockFreeStack() = default;

    ~LockFreeStack() {
        clear_live_nodes();
        clear_retired_nodes();
    }

    LockFreeStack(const LockFreeStack&) = delete;
    LockFreeStack& operator=(const LockFreeStack&) = delete;

    void push(T value) {
        Node* new_node = new Node(std::move(value));
        new_node->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(new_node->next,
                                            new_node,
                                            std::memory_order_release,
                                            std::memory_order_relaxed)) {
        }
    }

    bool pop(T& out) {
        Node* old_head = head_.load(std::memory_order_acquire);
        while (old_head != nullptr &&
               !head_.compare_exchange_weak(old_head,
                                            old_head->next,
                                            std::memory_order_acq_rel,
                                            std::memory_order_acquire)) {
        }

        if (old_head == nullptr) {
            return false;
        }

        out = *old_head->data;
        retire_node(old_head);
        return true;
    }

    bool empty() const {
        return head_.load(std::memory_order_acquire) == nullptr;
    }

private:
    struct Node {
        explicit Node(T value)
            : data(std::make_shared<T>(std::move(value))) {}

        std::shared_ptr<T> data;
        Node* next = nullptr;
    };

    void retire_node(Node* node) {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        retired_.push_back(node);
    }

    void clear_live_nodes() {
        Node* current = head_.exchange(nullptr, std::memory_order_acq_rel);
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void clear_retired_nodes() {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        for (Node* node : retired_) {
            delete node;
        }
        retired_.clear();
    }

    std::atomic<Node*> head_{nullptr};
    std::mutex retired_mutex_;
    std::vector<Node*> retired_;
};

template <typename T>
class ReclaimingLockFreeStack {
public:
    ReclaimingLockFreeStack() = default;

    ~ReclaimingLockFreeStack() {
        clear_live_nodes();
        delete_nodes(to_be_deleted_.exchange(nullptr, std::memory_order_acq_rel));
    }

    ReclaimingLockFreeStack(const ReclaimingLockFreeStack&) = delete;
    ReclaimingLockFreeStack& operator=(const ReclaimingLockFreeStack&) = delete;

    void push(T value) {
        Node* new_node = new Node(std::move(value));
        new_node->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(new_node->next,
                                            new_node,
                                            std::memory_order_release,
                                            std::memory_order_relaxed)) {
        }
    }

    std::shared_ptr<T> pop() {
        threads_in_pop_.fetch_add(1, std::memory_order_acq_rel);

        Node* old_head = head_.load(std::memory_order_acquire);
        while (old_head != nullptr &&
               !head_.compare_exchange_weak(old_head,
                                            old_head->next,
                                            std::memory_order_acq_rel,
                                            std::memory_order_acquire)) {
        }

        std::shared_ptr<T> result;
        if (old_head != nullptr) {
            result.swap(old_head->data);
        }

        try_reclaim(old_head);
        return result;
    }

    bool empty() const {
        return head_.load(std::memory_order_acquire) == nullptr;
    }

private:
    struct Node {
        explicit Node(T value)
            : data(std::make_shared<T>(std::move(value))) {}

        std::shared_ptr<T> data;
        Node* next = nullptr;
    };

    static void delete_nodes(Node* nodes) {
        while (nodes != nullptr) {
            Node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void chain_pending_nodes(Node* first, Node* last) {
        last->next = to_be_deleted_.load(std::memory_order_relaxed);
        while (!to_be_deleted_.compare_exchange_weak(last->next,
                                                     first,
                                                     std::memory_order_release,
                                                     std::memory_order_relaxed)) {
        }
    }

    void chain_pending_nodes(Node* nodes) {
        Node* last = nodes;
        while (last != nullptr && last->next != nullptr) {
            last = last->next;
        }
        if (nodes != nullptr && last != nullptr) {
            chain_pending_nodes(nodes, last);
        }
    }

    void chain_pending_node(Node* node) {
        chain_pending_nodes(node, node);
    }

    void try_reclaim(Node* old_head) {
        if (old_head == nullptr) {
            threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel);
            return;
        }

        if (threads_in_pop_.load(std::memory_order_acquire) == 1) {
            Node* nodes_to_delete = to_be_deleted_.exchange(nullptr, std::memory_order_acq_rel);
            if (threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete_nodes(nodes_to_delete);
            } else if (nodes_to_delete != nullptr) {
                chain_pending_nodes(nodes_to_delete);
            }
            delete old_head;
        } else {
            chain_pending_node(old_head);
            threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel);
        }
    }

    void clear_live_nodes() {
        Node* current = head_.exchange(nullptr, std::memory_order_acq_rel);
        delete_nodes(current);
    }

    std::atomic<Node*> head_{nullptr};
    std::atomic<unsigned> threads_in_pop_{0};
    std::atomic<Node*> to_be_deleted_{nullptr};
};

template <typename T>
class TaggedPointerStack {
public:
    TaggedPointerStack() = default;

    ~TaggedPointerStack() {
        clear_live_nodes();
        clear_retired_nodes();
    }

    TaggedPointerStack(const TaggedPointerStack&) = delete;
    TaggedPointerStack& operator=(const TaggedPointerStack&) = delete;

    void push(T value) {
        Node* new_node = new Node(std::move(value));
        Head old_head = head_.load(std::memory_order_relaxed);
        Head new_head{};

        do {
            new_node->next = old_head.ptr;
            new_head.ptr = new_node;
            new_head.tag = old_head.tag + 1;
        } while (!head_.compare_exchange_weak(old_head,
                                              new_head,
                                              std::memory_order_release,
                                              std::memory_order_relaxed));
    }

    bool pop(T& out) {
        Head old_head = head_.load(std::memory_order_acquire);
        Head new_head{};

        while (old_head.ptr != nullptr) {
            new_head.ptr = old_head.ptr->next;
            new_head.tag = old_head.tag + 1;
            if (head_.compare_exchange_weak(old_head,
                                            new_head,
                                            std::memory_order_acq_rel,
                                            std::memory_order_acquire)) {
                out = *old_head.ptr->data;
                retire_node(old_head.ptr);
                return true;
            }
        }

        return false;
    }

    bool empty() const {
        return head_.load(std::memory_order_acquire).ptr == nullptr;
    }

private:
    struct Node {
        explicit Node(T value)
            : data(std::make_shared<T>(std::move(value))) {}

        std::shared_ptr<T> data;
        Node* next = nullptr;
    };

    struct Head {
        Node* ptr = nullptr;
        std::size_t tag = 0;
    };

    static_assert(std::is_trivially_copyable<Head>::value, "Head must be trivially copyable");

    void retire_node(Node* node) {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        retired_.push_back(node);
    }

    void clear_live_nodes() {
        Head current = head_.exchange(Head{}, std::memory_order_acq_rel);
        Node* node = current.ptr;
        while (node != nullptr) {
            Node* next = node->next;
            delete node;
            node = next;
        }
    }

    void clear_retired_nodes() {
        std::lock_guard<std::mutex> lock(retired_mutex_);
        for (Node* node : retired_) {
            delete node;
        }
        retired_.clear();
    }

    std::atomic<Head> head_{};
    std::mutex retired_mutex_;
    std::vector<Node*> retired_;
};
