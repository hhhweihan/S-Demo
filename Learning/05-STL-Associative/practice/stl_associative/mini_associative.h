#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace mini_stl {

enum class Color { Red, Black };

// Alloc 默认 std::allocator：老写法 RBTree<int,int> 仍能编译；传入 PoolAllocator 即走内存池。
template <typename Key, typename Value, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, Value>>>
class RBTree {  // 红黑树：插入/删除均做标准旋转+变色修复（见 erase / fix_after_delete）。
 public:
    struct Node {
        Key key;
        Value value;
        Color color =
            Color::Red;  // 新节点先染红：不改变任何路径的黑高，把违规局限为“红-红”，好修。
        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    class iterator {
     public:
        using difference_type = std::ptrdiff_t;
        using value_type = Node;
        using pointer = Node*;
        using reference = Node&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() = default;
        iterator(Node* node, const RBTree* tree) : node_(node), tree_(tree) {}
        reference operator*() const { return *node_; }
        pointer operator->() const { return node_; }
        iterator& operator++() {
            node_ = tree_->successor(node_);
            return *this;
        }  // ++ 走中序后继，故遍历即升序。
        iterator operator++(int) {
            iterator old(*this);
            ++(*this);
            return old;
        }
        bool operator==(const iterator& other) const { return node_ == other.node_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

     private:
        Node* node_ = nullptr;          // nullptr 表示 end。
        const RBTree* tree_ = nullptr;  // successor 需要沿 parent 上溯，故迭代器要持有树。
    };

    RBTree() = default;
    ~RBTree() { clear(root_); }
    // 禁拷贝：默认拷贝浅拷指针会导致两棵树共享节点并二次释放。
    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;

    // insert 是 upsert：命中已有 key 时覆盖 value 并返回 false（未新增），否则插入并修复后返回
    // true。
    std::pair<iterator, bool> insert(const Key& key, const Value& value) {
        Node* parent = nullptr;
        Node* current = root_;
        while (current) {
            parent = current;  // 记录最后一个非空节点，作为新叶的挂接点。
            if (compare_(key, current->key)) {
                current = current->left;
            } else if (compare_(current->key, key)) {
                current = current->right;
            } else {
                current->value = value;  // key 等价 → 覆盖，不新增节点。
                return {iterator(current, this), false};
            }
        }

        Node* node = create_node(key, value);  // 经 allocator_traits 分配+构造，不再裸 new。
        node->parent = parent;
        if (!parent) {
            root_ = node;
        } else if (compare_(key, parent->key)) {
            parent->left = node;
        } else {
            parent->right = node;
        }
        ++size_;
        fix_after_insert(node);  // 新红节点可能造成红-红，需旋转/变色恢复红黑性质。
        return {iterator(node, this), true};
    }

    // 标准红黑删除：先做 BST 删除（0/1 子直接改接，2 子用中序后继顶替），
    // 若“真正移除的节点”是黑色则黑高被破坏，交给 fix_after_delete 消解双黑。
    bool erase(const Key& key) {
        Node* z = find_node(key);
        if (!z) return false;  // 不存在则不算删除。
        // y：实际从树中“物理下线”的节点；x：顶替 y 位置、可能承接一层额外黑的节点（可为空）。
        Node* y = z;
        Color y_original_color = y->color;
        Node* x = nullptr;
        Node* x_parent = nullptr;  // x 可能为空（叶下的 nil），故单独记录其父以定位修复起点。
        if (!z->left) {
            x = z->right;
            x_parent = z->parent;
            transplant(z, z->right);  // 无左子：右子（或空）直接顶上。
        } else if (!z->right) {
            x = z->left;
            x_parent = z->parent;
            transplant(z, z->left);  // 无右子：左子顶上。
        } else {
            y = minimum(z->right);  // 两子：取右子树最小者（中序后继）来顶替 z。
            y_original_color = y->color;
            x = y->right;  // y 无左子，其右子（或空）将接替 y 的位置。
            if (y->parent == z) {
                x_parent = y;  // y 就是 z 的右子：x 之父即 y（x 为空时也要能定位）。
            } else {
                x_parent = y->parent;
                transplant(y, y->right);  // 先把 y 从深处摘出，其右子补位。
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);  // y 顶替 z 的位置，并继承 z 的左子与颜色（颜色继承使 z 处黑高不变）。
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        destroy_node(z);  // 经 allocator_traits 析构+回收，不再裸 delete。
        --size_;
        if (y_original_color == Color::Black)
            fix_after_delete(x, x_parent);  // 删掉的是黑节点才会破坏黑高，需修复。
        return true;
    }

    Node* find_node(const Key& key) const {
        Node* current = root_;
        while (current) {  // 标准 BST 下降查找。
            if (compare_(key, current->key))
                current = current->left;
            else if (compare_(current->key, key))
                current = current->right;
            else
                return current;
        }
        return nullptr;
    }

    iterator find(const Key& key) const { return iterator(find_node(key), this); }
    iterator lower_bound(const Key& key) const {
        return iterator(bound(key, false), this);
    }  // 第一个 >= key。
    iterator upper_bound(const Key& key) const {
        return iterator(bound(key, true), this);
    }  // 第一个 > key。
    std::pair<iterator, iterator> equal_range(const Key& key) const {
        return {lower_bound(key), upper_bound(key)};
    }
    iterator begin() const { return iterator(minimum(root_), this); }  // 最左节点 = 最小 key。
    iterator end() const { return iterator(nullptr, this); }
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    int height() const { return height(root_); }

    bool validate() const {
        if (!root_) return true;
        if (root_->color != Color::Black) return false;  // 性质：根必为黑。
        int black_height = -1;  // -1 = 尚未确定期望黑高，首条到底路径来定标。
        return validate_node(root_, 0, black_height, nullptr, nullptr);
    }

 private:
    // 关键：节点式容器分配的是 Node 而非 Key/Value，故必须把外部 Alloc 重绑到 Node 类型。
    // allocator_traits::rebind_alloc 正是为此而生——同一族 allocator 换个 value_type 复用其分配策略。
    using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using NodeAllocTraits = std::allocator_traits<NodeAlloc>;

    // 统一走 allocator_traits：allocate 拿裸内存，construct 就地构造 Node（含颜色/指针初值）。
    Node* create_node(const Key& key, const Value& value) {
        Node* node = NodeAllocTraits::allocate(node_alloc_, 1);
        NodeAllocTraits::construct(node_alloc_, node, Node{key, value});
        return node;
    }
    // 与 create_node 对称：先 destroy 调析构，再 deallocate 归还内存（池化时回收进空闲链）。
    void destroy_node(Node* node) {
        NodeAllocTraits::destroy(node_alloc_, node);
        NodeAllocTraits::deallocate(node_alloc_, node, 1);
    }

    static Color color_of(Node* node) {
        return node ? node->color : Color::Black;
    }  // 空节点视为黑（叶为黑的约定），省去大量空判。
    static void set_color(Node* node, Color color) {
        if (node) node->color = color;
    }

    // 左旋：右孩子上位成为子树根，node 降为其左孩子；保持 BST 有序性，仅改变树形以降高。
    void rotate_left(Node* node) {
        Node* right = node->right;
        node->right = right->left;
        if (right->left) right->left->parent = node;
        right->parent = node->parent;
        if (!node->parent)
            root_ = right;
        else if (node == node->parent->left)
            node->parent->left = right;
        else
            node->parent->right = right;
        right->left = node;
        node->parent = right;
    }

    void rotate_right(Node* node) {  // 左旋的镜像。
        Node* left = node->left;
        node->left = left->right;
        if (left->right) left->right->parent = node;
        left->parent = node->parent;
        if (!node->parent)
            root_ = left;
        else if (node == node->parent->right)
            node->parent->right = left;
        else
            node->parent->left = left;
        left->right = node;
        node->parent = left;
    }

    // 插入修复：唯一可能违反的性质是“红节点不能有红孩子”。循环把红-red 冲突逐层上推，
    // 按叔叔颜色分三种情形处理，直到根或父为黑；最后强制根为黑。
    void fix_after_insert(Node* node) {
        set_color(node, Color::Red);
        while (node != root_ && color_of(node->parent) == Color::Red) {
            Node* parent = node->parent;
            Node* grand = parent->parent;  // 父为红 → 祖父必存在且为黑，旋转变色围绕它。
            if (parent == grand->left) {
                Node* uncle = grand->right;
                if (color_of(uncle) == Color::Red) {
                    // 情形 1：叔红。父叔转黑、祖父转红，把冲突整体上移一层，无需旋转。
                    set_color(parent, Color::Black);
                    set_color(uncle, Color::Black);
                    set_color(grand, Color::Red);
                    node = grand;
                } else {
                    if (node == parent->right) {
                        // 情形 2：叔黑且当前是内侧孩子，先左旋父节点转成外侧（情形 3）。
                        node = parent;
                        rotate_left(node);
                        parent = node->parent;
                        grand = parent->parent;
                    }
                    // 情形 3：叔黑且外侧。父转黑、祖父转红后右旋祖父，红-红消解且黑高不变。
                    set_color(parent, Color::Black);
                    set_color(grand, Color::Red);
                    rotate_right(grand);
                }
            } else {  // 父在祖父右侧，下面是左右对称的镜像处理。
                Node* uncle = grand->left;
                if (color_of(uncle) == Color::Red) {
                    set_color(parent, Color::Black);
                    set_color(uncle, Color::Black);
                    set_color(grand, Color::Red);
                    node = grand;
                } else {
                    if (node == parent->left) {
                        node = parent;
                        rotate_right(node);
                        parent = node->parent;
                        grand = parent->parent;
                    }
                    set_color(parent, Color::Black);
                    set_color(grand, Color::Red);
                    rotate_left(grand);
                }
            }
        }
        set_color(
            root_,
            Color::Black);  // 情形 1 上推可能把根染红，统一收尾置黑（黑高对所有路径 +1，仍合法）。
    }

    // 用子树 v 替换以 u 为根的子树在其父下的挂接（不动 u 自身的孩子指针）。v 可为空。
    void transplant(Node* u, Node* v) {
        if (!u->parent)
            root_ = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    // 删除修复：x 承接了一层“额外黑”（双黑）。沿兄弟 w 的颜色/孩子颜色分四情形把额外黑上推或消解。
    // x 可能为空，故额外传入其父 parent 来定位；有效红黑树里双黑节点的兄弟必非空。
    void fix_after_delete(Node* x, Node* parent) {
        while (x != root_ && color_of(x) == Color::Black) {
            if (x == parent->left) {
                Node* w = parent->right;  // 兄弟。
                if (color_of(w) == Color::Red) {
                    // 情形 1：兄红。变色后左旋父，转化为兄黑的情形 2/3/4。
                    set_color(w, Color::Black);
                    set_color(parent, Color::Red);
                    rotate_left(parent);
                    w = parent->right;
                }
                if (color_of(w->left) == Color::Black && color_of(w->right) == Color::Black) {
                    // 情形 2：兄黑且两侄皆黑。兄染红，把额外黑上移到父，继续向上处理。
                    set_color(w, Color::Red);
                    x = parent;
                    parent = x->parent;
                } else {
                    if (color_of(w->right) == Color::Black) {
                        // 情形 3：兄黑、近侄红远侄黑。变色后右旋兄，转成远侄红的情形 4。
                        set_color(w->left, Color::Black);
                        set_color(w, Color::Red);
                        rotate_right(w);
                        w = parent->right;
                    }
                    // 情形 4：兄黑且远侄红。兄继承父色、父与远侄转黑，左旋父，额外黑消解，收尾。
                    set_color(w, color_of(parent));
                    set_color(parent, Color::Black);
                    set_color(w->right, Color::Black);
                    rotate_left(parent);
                    x = root_;  // 置 x=root 令循环终止。
                }
            } else {  // x 在右侧，下面为左右对称镜像。
                Node* w = parent->left;
                if (color_of(w) == Color::Red) {
                    set_color(w, Color::Black);
                    set_color(parent, Color::Red);
                    rotate_right(parent);
                    w = parent->left;
                }
                if (color_of(w->right) == Color::Black && color_of(w->left) == Color::Black) {
                    set_color(w, Color::Red);
                    x = parent;
                    parent = x->parent;
                } else {
                    if (color_of(w->left) == Color::Black) {
                        set_color(w->right, Color::Black);
                        set_color(w, Color::Red);
                        rotate_left(w);
                        w = parent->left;
                    }
                    set_color(w, color_of(parent));
                    set_color(parent, Color::Black);
                    set_color(w->left, Color::Black);
                    rotate_right(parent);
                    x = root_;
                }
            }
        }
        set_color(x, Color::Black);  // 收尾：红节点顶替则染黑吸收额外黑；根多出的黑对全路径等价。
    }

    // lower/upper_bound 合一：一路向左收窄候选。upper 用 key<cur、lower 用 !(cur<key)
    // 判断“可作候选”。
    Node* bound(const Key& key, bool upper) const {
        Node* current = root_;
        Node* candidate = nullptr;
        while (current) {
            const bool goes_left =
                upper ? compare_(key, current->key) : !compare_(current->key, key);
            if (goes_left) {
                candidate = current;  // 当前满足边界，记下后继续往左找更小的合格者。
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return candidate;
    }

    Node* minimum(Node* node) const {
        if (!node) return nullptr;
        while (node->left) node = node->left;
        return node;
    }

    Node* successor(Node* node) const {
        if (!node) return nullptr;
        if (node->right) return minimum(node->right);  // 有右子树：后继是右子树最小节点。
        // 无右子树：上溯到第一个“当前节点在其左子树”的祖先，它才是中序后继。
        Node* parent = node->parent;
        while (parent && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    int height(Node* node) const {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    }

    // 递归校验四项：BST 有序（min/max 边界收窄）、无红-红、各路径黑高一致、（根黑由调用方查）。
    bool validate_node(Node* node, int black_count, int& expected, const Key* min_key,
                       const Key* max_key) const {
        if (!node) {
            if (expected == -1) expected = black_count;  // 首条到底路径确定期望黑高。
            return black_count == expected;              // 其余路径必须与之相等。
        }
        if (min_key && !compare_(*min_key, node->key)) return false;
        if (max_key && !compare_(node->key, *max_key)) return false;
        if (node->color == Color::Red &&
            (color_of(node->left) == Color::Red || color_of(node->right) == Color::Red))
            return false;
        if (node->color == Color::Black) ++black_count;
        return validate_node(node->left, black_count, expected, min_key,
                             &node->key) &&  // 左子树上界收紧为本节点 key。
               validate_node(node->right, black_count, expected, &node->key,
                             max_key);  // 右子树下界收紧为本节点 key。
    }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        destroy_node(node);  // 后序释放：先子后父，避免访问已删指针。
    }

    Node* root_ = nullptr;
    std::size_t size_ = 0;
    Compare compare_{};
    NodeAlloc node_alloc_{};  // 重绑到 Node 的 allocator 实例：所有节点分配/回收都经它。
};

template <typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class MySet {  // RBTree 的薄封装：key 即 value 的有序唯一集合。
 public:
    using Tree = RBTree<Key, Key, Compare, Alloc>;  // Alloc 透传给树，再由树重绑到 Node。
    class iterator {
     public:
        // Standard iterator typedefs so std::iterator_traits recognises this as an
        // input/forward iterator — without them range constructors (e.g. vector(first,last))
        // and range algorithms reject it. Elements are immutable (set semantics).
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = const Key*;
        using reference = const Key&;
        explicit iterator(typename Tree::iterator iter = {}) : iter_(iter) {}
        const Key& operator*() const {
            return iter_->key;
        }  // 只读暴露 key：set 元素不可变，改 key 会破坏排序。
        iterator& operator++() {
            ++iter_;
            return *this;
        }
        bool operator==(const iterator& other) const { return iter_ == other.iter_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

     private:
        typename Tree::iterator iter_;
    };
    bool insert(const Key& key) {
        return tree_.insert(key, key).second;
    }  // 返回 true 表示新增（已存在则 false）。
    bool erase(const Key& key) { return tree_.erase(key); }
    bool contains(const Key& key) const { return tree_.find_node(key) != nullptr; }
    iterator begin() const { return iterator(tree_.begin()); }
    iterator end() const { return iterator(tree_.end()); }
    std::size_t size() const { return tree_.size(); }

 private:
    Tree tree_;
};

template <typename Key, typename Value, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, Value>>>
class MyMap {  // RBTree 封装的有序唯一键映射。
 public:
    using Tree = RBTree<Key, Value, Compare, Alloc>;  // Alloc 透传给树，再由树重绑到 Node。
    Value& operator[](const Key& key) {
        // insert() upserts, so it would clobber an existing value with Value{}. Only default-
        // construct when the key is absent; otherwise return the existing slot untouched.
        if (auto* node = tree_.find_node(key)) return node->value;
        return tree_.insert(key, Value{}).first->value;
    }
    bool insert(const Key& key, const Value& value) { return tree_.insert(key, value).second; }
    bool erase(const Key& key) { return tree_.erase(key); }
    Value* find(const Key& key) {
        auto* node = tree_.find_node(key);
        return node ? &node->value : nullptr;
    }
    typename Tree::iterator lower_bound(const Key& key) const { return tree_.lower_bound(key); }
    typename Tree::iterator upper_bound(const Key& key) const { return tree_.upper_bound(key); }
    std::pair<typename Tree::iterator, typename Tree::iterator> equal_range(const Key& key) const {
        return tree_.equal_range(key);
    }
    typename Tree::iterator begin() const { return tree_.begin(); }
    typename Tree::iterator end() const { return tree_.end(); }
    std::size_t size() const { return tree_.size(); }

 private:
    Tree tree_;
};

template <typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class MyMultiSet {  // 教学版：用有序 vector 而非树，突出“允许重复 key”这一点。
 public:
    void insert(const Key& key) {
        values_.push_back(key);
        std::sort(values_.begin(), values_.end(), compare_);
    }  // 每次插入后重排以维持有序（O(n log n)，仅教学用）。
    std::size_t count(const Key& key) const {
        auto range = std::equal_range(values_.begin(), values_.end(), key, compare_);
        return static_cast<std::size_t>(range.second - range.first);
    }  // equal_range 圈出全部等价 key。
    auto begin() const { return values_.begin(); }
    auto end() const { return values_.end(); }

 private:
    // 底层是连续 vector，故把 Alloc 重绑到 Key（vector 的元素类型正是 Key）。
    using KeyAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Key>;
    std::vector<Key, KeyAlloc> values_;
    Compare compare_{};
};

template <typename Key, typename Value, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<Key, Value>>>
class MyMultiMap {  // 教学版：vector 存 key/value，允许同 key 多值。
 public:
    void insert(const Key& key, const Value& value) {
        values_.push_back({key, value});
        // 用 stable_sort：同 key 的多个 value 必须保持插入先后顺序（multimap 的等价顺序约定）。
        std::stable_sort(values_.begin(), values_.end(), [this](const auto& lhs, const auto& rhs) {
            return compare_(lhs.first, rhs.first);
        });
    }
    std::size_t count(const Key& key) const {
        return static_cast<std::size_t>(
            std::count_if(values_.begin(), values_.end(), [&](const auto& entry) {
                return !compare_(key, entry.first) && !compare_(entry.first, key);
            }));  // 两向都不小于 == 等价。
    }

 private:
    // vector 元素是 pair<Key,Value>，故把 Alloc 重绑到该 pair 类型。
    using PairAlloc =
        typename std::allocator_traits<Alloc>::template rebind_alloc<std::pair<Key, Value>>;
    std::vector<std::pair<Key, Value>, PairAlloc> values_;
    Compare compare_{};
};

template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename Equal = std::equal_to<Key>>
class MyUnorderedMap {  // 开链哈希表：vector<list<pair>>，冲突挂链。
 public:
    // 至少保留 1 个桶：bucket_index 用 hash % buckets_.size()，0 桶会触发除零 UB。
    explicit MyUnorderedMap(std::size_t buckets = 8) : buckets_(buckets == 0 ? 1 : buckets) {}
    Value& operator[](const Key& key) {
        if ((size_ + 1.0) / buckets_.size() > max_load_factor_)
            rehash(buckets_.size() * 2);  // 插入前预判负载：超阈值先扩桶，避免链变长拖慢查找。
        auto& bucket = buckets_[bucket_index(key)];
        for (auto& entry : bucket)
            if (equal_(entry.first, key)) return entry.second;  // 命中则返回现有槽（upsert 语义）。
        bucket.push_back({key, Value{}});
        ++size_;
        return bucket.back().second;
    }
    Value* find(const Key& key) {
        auto& bucket = buckets_[bucket_index(key)];
        for (auto& entry : bucket)
            if (equal_(entry.first, key)) return &entry.second;
        return nullptr;
    }
    bool erase(const Key& key) {
        auto& bucket = buckets_[bucket_index(key)];
        for (auto iter = bucket.begin(); iter != bucket.end(); ++iter) {
            if (equal_(iter->first, key)) {
                bucket.erase(iter);
                --size_;
                return true;
            }
        }
        return false;
    }
    void rehash(std::size_t bucket_count) {
        if (bucket_count == 0) bucket_count = 1;  // 同理禁止 0 桶，避免后续取模除零。
        // 桶数变了，所有条目的 index = hash % bucket_count 都要重算并迁移；move 避免深拷贝。
        std::vector<std::list<std::pair<Key, Value>>> next(bucket_count);
        for (auto& bucket : buckets_) {
            for (auto& entry : bucket)
                next[hash_(entry.first) % bucket_count].push_back(std::move(entry));
        }
        buckets_ = std::move(next);
    }
    double load_factor() const { return static_cast<double>(size_) / buckets_.size(); }
    double max_load_factor() const { return max_load_factor_; }
    std::size_t bucket_count() const { return buckets_.size(); }
    std::size_t size() const { return size_; }

 private:
    std::size_t bucket_index(const Key& key) const { return hash_(key) % buckets_.size(); }
    std::vector<std::list<std::pair<Key, Value>>> buckets_;
    std::size_t size_ = 0;
    double max_load_factor_ = 0.75;  // 经验阈值：均摊查找 O(1) 与空间占用的折中。
    Hash hash_{};
    Equal equal_{};
};

// 以下四个算法是对标准库的教学转发封装，重点在展示接口契约而非重新实现。
template <typename Iterator>
void heap_sort(Iterator first, Iterator last) {
    std::make_heap(first, last);  // 先建堆，再反复弹堆顶 → 原地升序。
    std::sort_heap(first, last);
}

template <typename Iterator,
          typename Compare = std::less<typename std::iterator_traits<Iterator>::value_type>>
void stable_sort(Iterator first, Iterator last, Compare compare = Compare{}) {
    std::stable_sort(first, last, compare);  // 等价元素保持相对顺序。
}

template <typename Iterator,
          typename Compare = std::less<typename std::iterator_traits<Iterator>::value_type>>
void partial_sort(Iterator first, Iterator middle, Iterator last, Compare compare = Compare{}) {
    std::partial_sort(first, middle, last,
                      compare);  // 只保证 [first, middle) 是全局最小的有序前缀。
}

template <typename Iterator, typename T>
bool binary_search(Iterator first, Iterator last, const T& value) {
    return std::binary_search(first, last, value);  // 前提：范围已按同一序有序。
}

// 定长块内存池 allocator：对单对象（count==1）分配走空闲链复用，避免频繁 new/delete；
// count>1（如 vector 的连续数组）无法用定长块池化，回退 ::operator new/delete。
// 空闲块原地内嵌 next 指针复用为链表节点，故仅当 sizeof(T) 足够容纳指针时才启用池化。
template <typename T>
class PoolAllocator {
 public:
    using value_type = T;
    PoolAllocator() = default;
    template <typename U>
    PoolAllocator(const PoolAllocator<U>&) {
    }  // rebind 转换构造：容器要为内部节点类型重绑 allocator。

    T* allocate(std::size_t count) {
        if (count == 1 && kPoolable) {
            if (FreeNode* node = free_list()) {  // 命中空闲链：弹出复用，省一次系统分配。
                free_list() = node->next;
                return reinterpret_cast<T*>(node);
            }
        }
        return static_cast<T*>(::operator new(sizeof(T) * count));
    }
    void deallocate(T* pointer, std::size_t count) noexcept {
        if (count == 1 && kPoolable) {  // 单对象归还空闲链而非交还系统，供后续复用。
            FreeNode* node = reinterpret_cast<FreeNode*>(pointer);
            node->next = free_list();
            free_list() = node;
            return;
        }
        ::operator delete(pointer);
    }

 private:
    struct FreeNode {
        FreeNode* next;
    };
    // 块须能内嵌 next 指针（大小与对齐都够）才可池化；int 等小类型退化为纯转发，保证安全。
    static constexpr bool kPoolable =
        sizeof(T) >= sizeof(FreeNode) && alignof(T) >= alignof(FreeNode);
    // 每种 T 一条静态空闲链：实例本身无状态，任一实例都可释放另一实例分配的块（满足等价要求）。
    static FreeNode*& free_list() {
        static FreeNode* head = nullptr;
        return head;
    }
};

// 所有实例视为等价 → 任一 PoolAllocator 分配的内存可被另一实例释放（无状态 allocator 的要求）。
template <typename T, typename U>
bool operator==(const PoolAllocator<T>&, const PoolAllocator<U>&) {
    return true;
}
template <typename T, typename U>
bool operator!=(const PoolAllocator<T>&, const PoolAllocator<U>&) {
    return false;
}

}  // namespace mini_stl