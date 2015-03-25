#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <functional>
#include <memory>

template <class Key, class Value, class Comparator = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Value> > >
class SplayTree {
public:
    typedef std::pair<const Key, Value> value_type;

private:
    struct Node {
        Node* left;
        Node* right;
        Node* parent;
        value_type value;

        Node(const Key & newKey = Key(), const Value & newValue = Value()) : left(nullptr), right(nullptr), parent(nullptr),
            value(newKey, newValue)
        {}
    };

public:
    class TreeIterator : public std::iterator<std::forward_iterator_tag, value_type> {
    public:
        TreeIterator() : cur(nullptr)
        {}
        TreeIterator(Node* node) : cur(node)
        {}
        TreeIterator(SplayTree* tree) : cur(tree->root)
        {}
        TreeIterator(const TreeIterator& other) : cur(other.cur)
        {}
        TreeIterator(TreeIterator&& other) : cur(other.cur) {
            other.cur = nullptr;
        }
        TreeIterator& operator=(const TreeIterator& other) {
            cur = other.cur;
            return *this;
        }
        TreeIterator& operator=(TreeIterator&& other) {
            std::swap(cur, other.cur);
            return *this;
        }
        TreeIterator& operator++() {
            if (!cur) {
                return *this;
            }
            if (cur->right) {
                cur = cur->right;
                while (cur && cur->left) {
                    cur = cur->left;
                }
            } else {
                Node* parent = cur->parent;
                while (parent && cur == parent->right) {
                    cur = parent;
                    parent = parent->parent;
                }
                if (cur && cur->right != parent) {
                    cur = parent;
                }
            }
            return *this;
        }
        TreeIterator operator++(int) {
            TreeIterator tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const TreeIterator& other) {
            return cur == other.cur;
        }
        bool operator!=(const TreeIterator& other) {
            return cur != other.cur;
        }
        value_type& operator*() {
            if (cur) {
                return cur->value;
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
        value_type* operator->() {
            if (cur) {
                return &(cur->value);
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
    private:
        Node* cur;
    };

    typedef TreeIterator iterator;
    typedef typename Allocator::template rebind<Node>::other allocator_type;

    SplayTree() : root(nullptr), treeSize(0), allocator(), defaultValue()
    {}

    std::pair<iterator, bool> insert(const Key& key, const Value& value = Value()) {
        iterator* findResult = find(key);
        if(findResult) {
            return std::make_pair(*findResult, false);
        }
        Node* cur = root;
        if (!root) {
            root = allocator.allocate(1);
            allocator.construct(root, key, value);
            ++treeSize;
            return std::make_pair(iterator(root), true);
        }
        Node* prev = nullptr;
        while (cur) {
            prev = cur;
            if(comparator(cur->value.first, key)) {
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
        cur = allocator.allocate(1);
        allocator.construct(cur, key, value);

        if(nullptr == prev) {
            root = cur;
        } else if(comparator(prev->value.first, cur->value.first)) {
            prev->right = cur;
        } else {
            prev->left = cur;
        }
        cur->parent = prev;

        ++treeSize;
        root = splay(root, key);
        return std::make_pair(iterator(cur), true);
    }

    iterator* find(const Key &key) {
        if (!root) {
            return nullptr;
        }
        root = splay(root, key);
        if (root->value.first == key) {
            return new iterator(root);
        }
        return nullptr;
    }

    size_t erase(const Key& key) {
        if (nullptr == root) {
            return 0;
        }

        root = splay(root, key);
        Node* erased = root;
        if (!root->right && !root->left) {
            allocator.destroy(root);
            allocator.deallocate(root, 1);
            root = nullptr;
            --treeSize;
            return 1;
        }
        if (!root->left) {
            root = root->right;
        } else if (!root->right) {
            root = root->left;
        } else {
            Node* leftMax = root->left;
            while(leftMax->right) {
                leftMax = leftMax->right;
            }
            root = splay(root->left, leftMax->value.first);
            root->right = erased->right;
            root->parent = nullptr;
            if (root->right) {
                root->right->parent = root;
            }
        }
        allocator.destroy(erased);
        allocator.deallocate(erased, 1);
        --treeSize;
        return 1;
    }

//    template <class... Args>
//    std::pair<iterator, bool> emplace(Args&&... args) {
//        //Node* newNode= allocator.allocate(allocator, 1);
//        //allocator.construct(newNode, std::forward<Args>(args)...);
//        return insert(std::forward<Args>(args)...);
//   }

    inline void clear() {
        while (0 < erase(root->value.first))
        ;
    }
    inline bool empty() const {
        return root == 0;
    }
    inline unsigned long size() const {
        return treeSize;
    }
    inline iterator begin() noexcept {
        Node* cur = root;
        while (cur->left) {
            cur = cur->left;
        }
        return iterator(cur);
    }
    inline iterator end() noexcept {
        return iterator();
    }

private:
    allocator_type allocator;
    Comparator comparator;
    unsigned long treeSize;
    Value defaultValue;
    Node* root;

    // right rotate subtree rooted with x
    Node* rightRotate(Node *x) {
        if (!x) {
            return nullptr;
        }
        Node *y = x->left;
        if (y) {
            x->left = y->right;
            if (y->right) {
                y->right->parent = y;
            }
            y->parent = x->parent;
        }
        if(!x->parent) {
            root = y;
        } else if(x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        if(y) {
            y->right = x;
        }
        x->parent = y;

        return y;
    }

    // left rotate subtree rooted with x
    Node* leftRotate(Node* x) {
        if (!x) {
            return nullptr;
        }
        Node* y = x->right;
        if(y) {
          x->right = y->left;
          if(y->left) {
              y->left->parent = x;
          }
          y->parent = x->parent;
        }

        if(!x->parent) {
            root = y;
        } else if(x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        if(y) {
            y->left = x;
        }
        x->parent = y;

        return y;
    }

    // if key is in tree -> put key_Node at root
    // else -> put the last accessed item at root
    Node* splay(Node* root, Key key) {
        if (!root || root->value.first == key) {
            return root;
        }

        if (comparator(key, root->value.first)) {
            // key in left subtree

            if (!root->left) {
                return root;
            }

            if (comparator(key, root->left->value.first)) {
                // Zig-Zig (Left Left)
                root->left->left = splay(root->left->left, key);
                root = rightRotate(root);
            } else if (comparator(root->left->value.first, key)) {
                // Zig-Zag (Left Right)
                root->left->right = splay(root->left->right, key);
                if (root->left->right) {
                    root->left = leftRotate(root->left);
                }
            }
            return (!root->left) ? root : rightRotate(root);
        } else {
            // key in right subtree

            if (!root->right) {
                return root;
            }

            if (comparator(key, root->right->value.first)) {
                // Zag-Zig (Right Left)
                root->right->left = splay(root->right->left, key);
                if (root->right->left) {
                    root->right = rightRotate(root->right);
                }
            } else if (root->right->value.first < key) {
                // Zag-Zag (Right Right)
                root->right->right = splay(root->right->right, key);
                root = leftRotate(root);
            }
            return (!root->right) ? root : leftRotate(root);
        }
    }
};

#endif // SPLAYTREE_H
