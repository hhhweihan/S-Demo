#pragma once  // 防止 mini_associative 头文件在同一编译单元中被重复包含。

#include <algorithm>  // 使用排序、堆、二分和稳定排序等标准算法封装。
#include <cmath>  // 预留数学工具，便于树高和复杂度示例扩展。
#include <cstddef>  // 使用 std::size_t 和 std::ptrdiff_t 表示大小和迭代器距离。
#include <functional>  // 使用 std::less、std::hash、std::equal_to 等默认策略。
#include <initializer_list>  // 预留初始化列表支持。
#include <list>  // 使用 std::list 作为哈希桶链表。
#include <memory>  // 预留智能指针工具，便于 allocator 扩展。
#include <stdexcept>  // 预留异常类型，便于容器接口扩展。
#include <string>  // 预留字符串 key 示例支持。
#include <type_traits>  // 预留类型萃取工具，便于 allocator/算法扩展。
#include <utility>  // 使用 std::pair、std::move 等工具。
#include <vector>  // 使用 std::vector 存储重建条目、multi 容器和哈希桶数组。

namespace mini_stl {  // 手写 STL 关联容器和算法放在 mini_stl 命名空间中。

enum class Color { Red, Black };  // 红黑树节点颜色，红/黑性质依赖该状态。

template <typename Key, typename Value, typename Compare = std::less<Key>>  // Key 是排序键，Value 是保存值，Compare 是键比较器。
class RBTree {  // 教学版红黑树，实现插入、查找、边界查询和中序迭代。
 public:  // 暴露节点、迭代器和树操作接口。
  struct Node {  // 红黑树节点结构。
    Key key;  // 节点排序键。
    Value value;  // 节点关联值。
    Color color = Color::Red;  // 新节点默认红色，插入修复后调整。
    Node* parent = nullptr;  // 父节点指针。
    Node* left = nullptr;  // 左子节点指针。
    Node* right = nullptr;  // 右子节点指针。
  };  // Node 定义结束。

  class iterator {  // 红黑树中序迭代器。
   public:  // 暴露标准迭代器关联类型和遍历操作。
    using difference_type = std::ptrdiff_t;  // 迭代器距离类型。
    using value_type = Node;  // 迭代器值类型是树节点。
    using pointer = Node*;  // 指针类型。
    using reference = Node&;  // 引用类型。
    using iterator_category = std::bidirectional_iterator_tag;  // 声明为双向迭代器类别。

    iterator() = default;  // 默认构造空迭代器。
    iterator(Node* node, const RBTree* tree) : node_(node), tree_(tree) {}  // 绑定当前节点和所属树。
    reference operator*() const { return *node_; }  // 解引用返回当前节点。
    pointer operator->() const { return node_; }  // 箭头运算符返回当前节点指针。
    iterator& operator++() { node_ = tree_->successor(node_); return *this; }  // 前置 ++ 移动到中序后继。
    iterator operator++(int) { iterator old(*this); ++(*this); return old; }  // 后置 ++ 返回移动前副本。
    bool operator==(const iterator& other) const { return node_ == other.node_; }  // 相等比较节点指针。
    bool operator!=(const iterator& other) const { return !(*this == other); }  // 不等比较复用相等比较。
   private:  // 迭代器内部状态。
    Node* node_ = nullptr;  // 当前节点，nullptr 表示 end。
    const RBTree* tree_ = nullptr;  // 所属树，用于查找 successor。
  };  // RBTree::iterator 定义结束。

  RBTree() = default;  // 默认构造空红黑树。
  ~RBTree() { clear(root_); }  // 析构时递归释放所有节点。
  RBTree(const RBTree&) = delete;  // 禁止拷贝构造，避免浅拷贝节点指针。
  RBTree& operator=(const RBTree&) = delete;  // 禁止拷贝赋值，避免浅拷贝节点指针。

  std::pair<iterator, bool> insert(const Key& key, const Value& value) {  // 插入 key/value，已存在 key 时更新 value。
    Node* parent = nullptr;  // 记录搜索过程中最后一个非空节点，作为新节点父节点。
    Node* current = root_;  // 从根节点开始二叉搜索。
    while (current) {  // 沿搜索路径查找插入位置或已有 key。
      parent = current;  // 更新候选父节点。
      if (compare_(key, current->key)) {  // 待插入 key 更小时走左子树。
        current = current->left;  // 移动到左子节点。
      } else if (compare_(current->key, key)) {  // 待插入 key 更大时走右子树。
        current = current->right;  // 移动到右子节点。
      } else {  // 两边都不小于说明 key 等价。
        current->value = value;  // 更新已有节点的值。
        return {iterator(current, this), false};  // 返回已有节点迭代器，并标记未插入新节点。
      }  // key 比较分支结束。
    }  // 搜索结束，current 为空表示找到插入位置。

    Node* node = new Node{key, value};  // 创建默认红色的新节点。
    node->parent = parent;  // 设置新节点父指针。
    if (!parent) {  // 树原本为空时新节点成为根。
      root_ = node;  // 更新根节点。
    } else if (compare_(key, parent->key)) {  // 新 key 小于父节点 key 时挂到左侧。
      parent->left = node;  // 连接为父节点左孩子。
    } else {  // 新 key 大于父节点 key 时挂到右侧。
      parent->right = node;  // 连接为父节点右孩子。
    }  // 新节点挂接结束。
    ++size_;  // 更新节点数量。
    fix_after_insert(node);  // 修复红黑树颜色和旋转性质。
    return {iterator(node, this), true};  // 返回新节点迭代器，并标记插入成功。
  }  // insert 结束。

  bool erase(const Key& key) {  // 教学版删除：收集其他节点后重建整棵树。
    std::vector<std::pair<Key, Value>> entries;  // 保存除目标 key 外的所有 key/value。
    bool removed = false;  // 记录是否真的找到了要删除的 key。
    collect_except(root_, key, entries, removed);  // 中序收集并跳过第一个匹配 key。
    clear(root_);  // 释放旧树节点。
    root_ = nullptr;  // 清空根指针。
    size_ = 0;  // 重置节点数量。
    for (const auto& entry : entries) {  // 遍历保留下来的条目。
      insert(entry.first, entry.second);  // 重新插入，借助插入修复恢复红黑性质。
    }  // 重建结束。
    return removed;  // 返回是否删除了目标 key。
  }  // erase 结束。

  Node* find_node(const Key& key) const {  // 查找指定 key 对应的节点。
    Node* current = root_;  // 从根节点开始搜索。
    while (current) {  // 沿二叉搜索树路径向下查找。
      if (compare_(key, current->key)) current = current->left;  // key 更小时进入左子树。
      else if (compare_(current->key, key)) current = current->right;  // key 更大时进入右子树。
      else return current;  // 两边都不小于说明 key 等价，返回当前节点。
    }  // 搜索到空节点说明不存在。
    return nullptr;  // 未找到时返回空指针。
  }  // find_node 结束。

  iterator find(const Key& key) const { return iterator(find_node(key), this); }  // 返回 key 对应节点的迭代器。
  iterator lower_bound(const Key& key) const { return iterator(bound(key, false), this); }  // 返回第一个不小于 key 的节点。
  iterator upper_bound(const Key& key) const { return iterator(bound(key, true), this); }  // 返回第一个大于 key 的节点。
  std::pair<iterator, iterator> equal_range(const Key& key) const { return {lower_bound(key), upper_bound(key)}; }  // 返回等价区间边界。
  iterator begin() const { return iterator(minimum(root_), this); }  // 返回最小 key 节点迭代器。
  iterator end() const { return iterator(nullptr, this); }  // 返回尾后迭代器。
  std::size_t size() const { return size_; }  // 返回节点数量。
  bool empty() const { return size_ == 0; }  // 判断树是否为空。
  int height() const { return height(root_); }  // 返回树高度。

  bool validate() const {  // 验证红黑树基本性质和二叉搜索树顺序。
    if (!root_) return true;  // 空树视为合法。
    if (root_->color != Color::Black) return false;  // 红黑树根节点必须为黑色。
    int black_height = -1;  // 第一次到达叶子时记录期望黑高。
    return validate_node(root_, 0, black_height, nullptr, nullptr);  // 递归检查每个节点和路径黑高。
  }  // validate 结束。

 private:  // 以下成员实现红黑树旋转、修复、遍历和校验细节。
  static Color color_of(Node* node) { return node ? node->color : Color::Black; }  // 空节点按黑色处理，简化红黑性质判断。
  static void set_color(Node* node, Color color) { if (node) node->color = color; }  // 非空节点才设置颜色，避免空指针访问。

  void rotate_left(Node* node) {  // 围绕 node 做左旋，把右孩子提升到 node 位置。
    Node* right = node->right;  // 保存即将上提的右孩子。
    node->right = right->left;  // right 的左子树转接为 node 的右子树。
    if (right->left) right->left->parent = node;  // 更新被转接子树的父指针。
    right->parent = node->parent;  // right 接管 node 原来的父节点。
    if (!node->parent) root_ = right;  // node 原来是根时，right 成为新根。
    else if (node == node->parent->left) node->parent->left = right;  // node 是左孩子时更新父节点左指针。
    else node->parent->right = right;  // node 是右孩子时更新父节点右指针。
    right->left = node;  // node 成为 right 的左孩子。
    node->parent = right;  // 更新 node 的父指针。
  }  // 左旋结束。

  void rotate_right(Node* node) {  // 围绕 node 做右旋，把左孩子提升到 node 位置。
    Node* left = node->left;  // 保存即将上提的左孩子。
    node->left = left->right;  // left 的右子树转接为 node 的左子树。
    if (left->right) left->right->parent = node;  // 更新被转接子树的父指针。
    left->parent = node->parent;  // left 接管 node 原来的父节点。
    if (!node->parent) root_ = left;  // node 原来是根时，left 成为新根。
    else if (node == node->parent->right) node->parent->right = left;  // node 是右孩子时更新父节点右指针。
    else node->parent->left = left;  // node 是左孩子时更新父节点左指针。
    left->right = node;  // node 成为 left 的右孩子。
    node->parent = left;  // 更新 node 的父指针。
  }  // 右旋结束。

  void fix_after_insert(Node* node) {  // 插入红色节点后修复红黑树性质。
    set_color(node, Color::Red);  // 新插入节点先按红色处理，避免增加黑高。
    while (node != root_ && color_of(node->parent) == Color::Red) {  // 父节点为红色时违反“红节点不能有红孩子”。
      Node* parent = node->parent;  // 保存父节点。
      Node* grand = parent->parent;  // 保存祖父节点，旋转和变色围绕它进行。
      if (parent == grand->left) {  // 父节点在祖父左侧时处理左侧情形。
        Node* uncle = grand->right;  // 叔叔节点是祖父的右孩子。
        if (color_of(uncle) == Color::Red) {  // 情形 1：叔叔为红，只需变色并向上继续。
          set_color(parent, Color::Black);  // 父节点变黑。
          set_color(uncle, Color::Black);  // 叔叔节点变黑。
          set_color(grand, Color::Red);  // 祖父节点变红，把冲突上移。
          node = grand;  // 继续从祖父位置检查。
        } else {  // 叔叔为黑，需要旋转修复。
          if (node == parent->right) {  // 情形 2：内侧插入，先左旋变成外侧情形。
            node = parent;  // 把父节点作为旋转支点。
            rotate_left(node);  // 左旋父节点。
            parent = node->parent;  // 旋转后刷新父节点。
            grand = parent->parent;  // 旋转后刷新祖父节点。
          }  // 内侧情形处理结束。
          set_color(parent, Color::Black);  // 情形 3：外侧父节点变黑。
          set_color(grand, Color::Red);  // 祖父节点变红。
          rotate_right(grand);  // 右旋祖父节点恢复平衡。
        }  // 叔叔颜色分支结束。
      } else {  // 父节点在祖父右侧时处理镜像情形。
        Node* uncle = grand->left;  // 叔叔节点是祖父的左孩子。
        if (color_of(uncle) == Color::Red) {  // 镜像情形 1：叔叔为红。
          set_color(parent, Color::Black);  // 父节点变黑。
          set_color(uncle, Color::Black);  // 叔叔节点变黑。
          set_color(grand, Color::Red);  // 祖父节点变红，把冲突上移。
          node = grand;  // 继续从祖父位置检查。
        } else {  // 叔叔为黑，需要镜像旋转修复。
          if (node == parent->left) {  // 镜像情形 2：内侧插入，先右旋变成外侧情形。
            node = parent;  // 把父节点作为旋转支点。
            rotate_right(node);  // 右旋父节点。
            parent = node->parent;  // 旋转后刷新父节点。
            grand = parent->parent;  // 旋转后刷新祖父节点。
          }  // 镜像内侧情形处理结束。
          set_color(parent, Color::Black);  // 镜像情形 3：父节点变黑。
          set_color(grand, Color::Red);  // 祖父节点变红。
          rotate_left(grand);  // 左旋祖父节点恢复平衡。
        }  // 镜像叔叔颜色分支结束。
      }  // 左右镜像分支结束。
    }  // 插入修复循环结束。
    set_color(root_, Color::Black);  // 根节点最终必须为黑色。
  }  // fix_after_insert 结束。

  Node* bound(const Key& key, bool upper) const {  // 统一实现 lower_bound 和 upper_bound。
    Node* current = root_;  // 从根节点开始搜索。
    Node* candidate = nullptr;  // 保存目前找到的最优边界候选。
    while (current) {  // 沿搜索路径向下查找。
      const bool goes_left = upper ? compare_(key, current->key) : !compare_(current->key, key);  // upper 查找 key < current，lower 查找 current >= key。
      if (goes_left) {  // 当前节点可作为边界候选时继续向左找更小候选。
        candidate = current;  // 更新候选节点。
        current = current->left;  // 进入左子树寻找更靠前的边界。
      } else {  // 当前节点太小，不可能作为边界。
        current = current->right;  // 进入右子树寻找更大节点。
      }  // 搜索方向分支结束。
    }  // 搜索结束。
    return candidate;  // 返回边界候选，找不到时为 nullptr。
  }  // bound 结束。

  Node* minimum(Node* node) const {  // 查找子树中的最小 key 节点。
    if (!node) return nullptr;  // 空子树没有最小节点。
    while (node->left) node = node->left;  // 一直向左走到最左节点。
    return node;  // 返回最小节点。
  }  // minimum 结束。

  Node* successor(Node* node) const {  // 查找中序后继节点。
    if (!node) return nullptr;  // end 位置没有后继。
    if (node->right) return minimum(node->right);  // 有右子树时后继是右子树最小节点。
    Node* parent = node->parent;  // 无右子树时向上寻找第一个把当前节点放在左侧的祖先。
    while (parent && node == parent->right) {  // 当前节点位于父节点右侧时继续上爬。
      node = parent;  // 当前节点上移到父节点。
      parent = parent->parent;  // 父节点继续上移。
    }  // 上爬结束。
    return parent;  // 返回后继祖先，找不到时为 nullptr。
  }  // successor 结束。

  int height(Node* node) const {  // 递归计算子树高度。
    if (!node) return 0;  // 空子树高度为 0。
    return 1 + std::max(height(node->left), height(node->right));  // 当前节点高度等于较高子树加 1。
  }  // height 结束。

  bool validate_node(Node* node, int black_count, int& expected, const Key* min_key, const Key* max_key) const {  // 递归验证红黑性质和 key 范围。
    if (!node) {  // 到达空叶子时检查黑高。
      if (expected == -1) expected = black_count;  // 第一条路径记录期望黑高。
      return black_count == expected;  // 其他路径黑高必须一致。
    }  // 空叶子处理结束。
    if (min_key && !compare_(*min_key, node->key)) return false;  // 节点 key 必须大于最小边界。
    if (max_key && !compare_(node->key, *max_key)) return false;  // 节点 key 必须小于最大边界。
    if (node->color == Color::Red && (color_of(node->left) == Color::Red || color_of(node->right) == Color::Red)) return false;  // 红节点不能有红孩子。
    if (node->color == Color::Black) ++black_count;  // 黑节点会增加当前路径黑高。
    return validate_node(node->left, black_count, expected, min_key, &node->key) &&  // 递归检查左子树，并收紧最大边界。
           validate_node(node->right, black_count, expected, &node->key, max_key);  // 递归检查右子树，并收紧最小边界。
  }  // validate_node 结束。

  void clear(Node* node) {  // 递归释放子树所有节点。
    if (!node) return;  // 空节点无需释放。
    clear(node->left);  // 先释放左子树。
    clear(node->right);  // 再释放右子树。
    delete node;  // 最后释放当前节点。
  }  // clear 结束。

  void collect_except(Node* node, const Key& key, std::vector<std::pair<Key, Value>>& entries, bool& removed) const {  // 中序收集除目标 key 外的所有节点。
    if (!node) return;  // 空节点无需处理。
    collect_except(node->left, key, entries, removed);  // 先收集左子树，保持 key 有序。
    if (!removed && !compare_(key, node->key) && !compare_(node->key, key)) {  // 第一次遇到等价 key 时跳过它。
      removed = true;  // 标记已经删除过一个匹配节点。
    } else {  // 非目标节点或目标已删除后都需要保留。
      entries.push_back({node->key, node->value});  // 保存当前节点 key/value。
    }  // 当前节点处理结束。
    collect_except(node->right, key, entries, removed);  // 再收集右子树。
  }  // collect_except 结束。

  Node* root_ = nullptr;  // 红黑树根节点。
  std::size_t size_ = 0;  // 当前节点数量。
  Compare compare_{};  // key 比较器。
};  // RBTree 定义结束。

template <typename Key, typename Compare = std::less<Key>>  // Key 是集合元素类型，Compare 是排序比较器。
class MySet {  // 基于 RBTree 封装的有序唯一集合。
 public:  // 暴露树类型、迭代器和集合接口。
  using Tree = RBTree<Key, Key, Compare>;  // set 中 key 和 value 相同。
  class iterator {  // MySet 迭代器，包装底层 RBTree 迭代器。
   public:  // 暴露解引用、递增和比较接口。
    explicit iterator(typename Tree::iterator iter = {}) : iter_(iter) {}  // 用底层树迭代器构造 set 迭代器。
    const Key& operator*() const { return iter_->key; }  // 解引用只暴露只读 key，符合 set 元素不可修改语义。
    iterator& operator++() { ++iter_; return *this; }  // 前置 ++ 转发到底层树迭代器。
    bool operator==(const iterator& other) const { return iter_ == other.iter_; }  // 相等比较底层迭代器。
    bool operator!=(const iterator& other) const { return !(*this == other); }  // 不等比较复用相等比较。
   private:  // 迭代器内部状态。
    typename Tree::iterator iter_;  // 底层红黑树迭代器。
  };  // MySet::iterator 定义结束。
  bool insert(const Key& key) { return tree_.insert(key, key).second; }  // 插入 key，并返回是否新增节点。
  bool erase(const Key& key) { return tree_.erase(key); }  // 删除指定 key。
  bool contains(const Key& key) const { return tree_.find_node(key) != nullptr; }  // 判断 key 是否存在。
  iterator begin() const { return iterator(tree_.begin()); }  // 返回有序首元素迭代器。
  iterator end() const { return iterator(tree_.end()); }  // 返回尾后迭代器。
  std::size_t size() const { return tree_.size(); }  // 返回集合大小。
 private:  // 保存底层红黑树。
  Tree tree_;  // 负责排序、查找和存储的红黑树。
};  // MySet 定义结束。

template <typename Key, typename Value, typename Compare = std::less<Key>>  // Key 是键类型，Value 是映射值类型，Compare 是排序比较器。
class MyMap {  // 基于 RBTree 封装的有序唯一键映射。
 public:  // 暴露树类型和 map 常用接口。
  using Tree = RBTree<Key, Value, Compare>;  // map 底层红黑树保存 key/value。
  Value& operator[](const Key& key) { return tree_.insert(key, Value{}).first->value; }  // 不存在时插入默认值，返回 value 引用。
  bool insert(const Key& key, const Value& value) { return tree_.insert(key, value).second; }  // 插入 key/value，返回是否新增。
  bool erase(const Key& key) { return tree_.erase(key); }  // 删除指定 key。
  Value* find(const Key& key) { auto* node = tree_.find_node(key); return node ? &node->value : nullptr; }  // 查找 key 并返回 value 指针。
  typename Tree::iterator lower_bound(const Key& key) const { return tree_.lower_bound(key); }  // 返回第一个不小于 key 的节点。
  typename Tree::iterator upper_bound(const Key& key) const { return tree_.upper_bound(key); }  // 返回第一个大于 key 的节点。
  std::pair<typename Tree::iterator, typename Tree::iterator> equal_range(const Key& key) const { return tree_.equal_range(key); }  // 返回 key 的等价范围。
  typename Tree::iterator begin() const { return tree_.begin(); }  // 返回有序首节点迭代器。
  typename Tree::iterator end() const { return tree_.end(); }  // 返回尾后迭代器。
  std::size_t size() const { return tree_.size(); }  // 返回映射大小。
 private:  // 保存底层红黑树。
  Tree tree_;  // 负责排序、查找和存储的红黑树。
};  // MyMap 定义结束。

template <typename Key, typename Compare = std::less<Key>>  // Key 是元素类型，Compare 是排序比较器。
class MyMultiSet {  // 教学版 multiset，用有序 vector 展示重复 key 语义。
 public:  // 暴露插入、计数和遍历接口。
  void insert(const Key& key) { values_.push_back(key); std::sort(values_.begin(), values_.end(), compare_); }  // 插入后重新排序，保持遍历有序。
  std::size_t count(const Key& key) const { auto range = std::equal_range(values_.begin(), values_.end(), key, compare_); return static_cast<std::size_t>(range.second - range.first); }  // 用 equal_range 统计重复 key 数量。
  auto begin() const { return values_.begin(); }  // 返回底层 vector 首迭代器。
  auto end() const { return values_.end(); }  // 返回底层 vector 尾后迭代器。
 private:  // 保存有序元素和比较器。
  std::vector<Key> values_;  // 允许重复 key 的有序数组。
  Compare compare_{};  // key 比较器。
};  // MyMultiSet 定义结束。

template <typename Key, typename Value, typename Compare = std::less<Key>>  // Key 是键类型，Value 是值类型，Compare 是排序比较器。
class MyMultiMap {  // 教学版 multimap，用 vector 展示重复 key 到多个 value 的语义。
 public:  // 暴露插入和计数接口。
  void insert(const Key& key, const Value& value) {  // 插入一个 key/value 条目。
    values_.push_back({key, value});  // 追加到数组末尾。
    std::stable_sort(values_.begin(), values_.end(), [this](const auto& lhs, const auto& rhs) { return compare_(lhs.first, rhs.first); });  // 稳定排序保持相同 key 的插入相对顺序。
  }  // insert 结束。
  std::size_t count(const Key& key) const {  // 统计指定 key 的条目数量。
    return static_cast<std::size_t>(std::count_if(values_.begin(), values_.end(), [&](const auto& entry) { return !compare_(key, entry.first) && !compare_(entry.first, key); }));  // 两边都不小于表示 key 等价。
  }  // count 结束。
 private:  // 保存条目和比较器。
  std::vector<std::pair<Key, Value>> values_;  // 允许重复 key 的有序 key/value 数组。
  Compare compare_{};  // key 比较器。
};  // MyMultiMap 定义结束。

template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>>  // Key/Value 是键值类型，Hash/Equal 是哈希与相等策略。
class MyUnorderedMap {  // 简化版 unordered_map，使用 vector<list<pair>> 开链哈希表。
 public:  // 暴露构造、访问、查找、删除和负载因子接口。
  explicit MyUnorderedMap(std::size_t buckets = 8) : buckets_(buckets) {}  // 构造指定桶数量的哈希表。
  Value& operator[](const Key& key) {  // 访问或插入 key 对应的 value。
    if ((size_ + 1.0) / buckets_.size() > max_load_factor_) rehash(buckets_.size() * 2);  // 插入前预测负载因子，超过阈值则扩桶。
    auto& bucket = buckets_[bucket_index(key)];  // 根据 key 哈希定位桶链表。
    for (auto& entry : bucket) if (equal_(entry.first, key)) return entry.second;  // 已存在 key 时返回现有 value。
    bucket.push_back({key, Value{}});  // 不存在 key 时插入默认 value。
    ++size_;  // 更新元素数量。
    return bucket.back().second;  // 返回新插入 value 的引用。
  }  // operator[] 结束。
  Value* find(const Key& key) {  // 查找 key 对应的 value。
    auto& bucket = buckets_[bucket_index(key)];  // 定位 key 对应桶。
    for (auto& entry : bucket) if (equal_(entry.first, key)) return &entry.second;  // 遍历链表查找等价 key。
    return nullptr;  // 未找到时返回空指针。
  }  // find 结束。
  bool erase(const Key& key) {  // 删除指定 key。
    auto& bucket = buckets_[bucket_index(key)];  // 定位 key 对应桶。
    for (auto iter = bucket.begin(); iter != bucket.end(); ++iter) {  // 遍历桶内链表。
      if (equal_(iter->first, key)) {  // 找到等价 key。
        bucket.erase(iter);  // 从链表中删除该条目。
        --size_;  // 更新元素数量。
        return true;  // 返回删除成功。
      }  // 当前条目处理结束。
    }  // 桶链表遍历结束。
    return false;  // 未找到 key，删除失败。
  }  // erase 结束。
  void rehash(std::size_t bucket_count) {  // 重新分配桶数组并迁移所有条目。
    std::vector<std::list<std::pair<Key, Value>>> next(bucket_count);  // 创建新的桶数组。
    for (auto& bucket : buckets_) {  // 遍历旧桶。
      for (auto& entry : bucket) next[hash_(entry.first) % bucket_count].push_back(std::move(entry));  // 重新计算桶下标并移动条目。
    }  // 迁移结束。
    buckets_ = std::move(next);  // 用新桶数组替换旧桶数组。
  }  // rehash 结束。
  double load_factor() const { return static_cast<double>(size_) / buckets_.size(); }  // 返回当前负载因子。
  double max_load_factor() const { return max_load_factor_; }  // 返回最大负载因子阈值。
  std::size_t bucket_count() const { return buckets_.size(); }  // 返回桶数量。
  std::size_t size() const { return size_; }  // 返回元素数量。
 private:  // 以下成员维护桶数组和哈希策略。
  std::size_t bucket_index(const Key& key) const { return hash_(key) % buckets_.size(); }  // 把 key 映射到桶下标。
  std::vector<std::list<std::pair<Key, Value>>> buckets_;  // 桶数组，每个桶是一条 key/value 链表。
  std::size_t size_ = 0;  // 当前元素数量。
  double max_load_factor_ = 0.75;  // 自动 rehash 的负载因子阈值。
  Hash hash_{};  // key 哈希函数对象。
  Equal equal_{};  // key 相等比较函数对象。
};  // MyUnorderedMap 定义结束。

template <typename Iterator>  // Iterator 是随机访问迭代器类型。
void heap_sort(Iterator first, Iterator last) {  // 用堆算法实现排序封装。
  std::make_heap(first, last);  // 先把范围调整成堆。
  std::sort_heap(first, last);  // 再反复弹出堆顶得到有序序列。
}  // heap_sort 结束。

template <typename Iterator, typename Compare = std::less<typename std::iterator_traits<Iterator>::value_type>>  // Iterator 是迭代器类型，Compare 是比较器。
void stable_sort(Iterator first, Iterator last, Compare compare = Compare{}) {  // 稳定排序封装。
  std::stable_sort(first, last, compare);  // 委托标准库稳定排序，保持等价元素相对顺序。
}  // stable_sort 结束。

template <typename Iterator, typename Compare = std::less<typename std::iterator_traits<Iterator>::value_type>>  // Iterator 是随机访问迭代器类型，Compare 是比较器。
void partial_sort(Iterator first, Iterator middle, Iterator last, Compare compare = Compare{}) {  // 部分排序封装。
  std::partial_sort(first, middle, last, compare);  // 委托标准库选出并排序 [first, middle) 前缀。
}  // partial_sort 结束。

template <typename Iterator, typename T>  // Iterator 是有序范围迭代器，T 是待查找值类型。
bool binary_search(Iterator first, Iterator last, const T& value) {  // 二分查找封装。
  return std::binary_search(first, last, value);  // 委托标准库在有序范围中查找 value。
}  // binary_search 结束。

template <typename T>  // T 是 allocator 分配的元素类型。
class PoolAllocator {  // 最小 allocator 示例，满足 allocator_traits 基础接口。
 public:  // 暴露标准 allocator 所需类型和分配接口。
  using value_type = T;  // 标准 allocator 必须提供 value_type。
  PoolAllocator() = default;  // 默认构造 allocator。
  template <typename U> PoolAllocator(const PoolAllocator<U>&) {}  // 允许从其他 value_type 的 allocator 转换构造。
  T* allocate(std::size_t count) { return static_cast<T*>(::operator new(sizeof(T) * count)); }  // 分配能容纳 count 个 T 的原始内存。
  void deallocate(T* pointer, std::size_t) noexcept { ::operator delete(pointer); }  // 释放 allocate 获得的原始内存。
};  // PoolAllocator 定义结束。

template <typename T, typename U>  // T/U 是两个 allocator 的 value_type。
bool operator==(const PoolAllocator<T>&, const PoolAllocator<U>&) { return true; }  // 所有 PoolAllocator 实例都视为可互相释放资源。
template <typename T, typename U>  // T/U 是两个 allocator 的 value_type。
bool operator!=(const PoolAllocator<T>&, const PoolAllocator<U>&) { return false; }  // 相等运算取反语义下始终不等为 false。

}  // namespace mini_stl  // 手写 STL 关联容器和算法命名空间结束。