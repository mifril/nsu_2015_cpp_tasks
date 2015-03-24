#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <functional>
#include <memory>

template <class Key, class Value, class Comparator = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Value> > >
class SplayTree {
public:
    struct Node {
        Node* left;
        Node* right;
        Node* parent;
        Key key;
        Value value;

        Node(const Key & newKey = Key(), const Value & newValue = Value()) : left(nullptr), right(nullptr), parent(nullptr), key(newKey), value(newValue)
        {}
    };

    class TreeIterator : public std::iterator<std::forward_iterator_tag, Key> {
    public:
        TreeIterator(Node* node) : cur(node)
        {}
        TreeIterator(SplayTree* tree) : cur(tree->root)
        {}
        TreeIterator(const TreeIterator& other) : cur(other.cur)
        {}
        TreeIterator& operator++() {
            if (cur->left) {
                cur = cur->left;
                while (cur->right) {
                    cur = cur->right;
                }
            } else {
                Node* parent = cur->parent;
                while (cur == parent->right) {
                    cur = parent;
                    parent = parent->parent;
                }
                if (cur->left != parent) {
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
        Value& operator*() {
            return cur->value;
        }
    private:
        Node* cur;
    };

    typedef TreeIterator iterator;
    typedef typename Allocator::template rebind<Node>::other allocator_type;

    SplayTree() : root(nullptr), treeSize(0), allocator(), defaultValue()
    {}

    void insert(const Key& key, const Value& value) {
        if (!root) {
            root = allocator.allocate(1);
            allocator.construct(root, key, value);
            //root = new Node(key, value);
            return;
        }

        Node* cur = root;
        Node* prev = nullptr;
        while (cur) {
            prev = cur;
            if(comparator(cur->key, key)) {
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
        cur = allocator.allocate(1);
        allocator.construct(root, key, value);
    //    cur = new Node(key, value);

        if(nullptr == prev) {
            root = cur;
        } else if(comparator(prev->key, cur->key)) {
            prev->right = cur;
        } else {
            prev->left = cur;
        }
        ++treeSize;
    }

    Value& find(const Key &key) {
        if (!root) {
            return defaultValue;
        }
        if (root->key == key) {
            return root->value;
        }
        Node* oldRoot = root;
        root = splay(root, key);
        if (oldRoot == root) {
            return defaultValue;
        }
        return root->value;
    }

    size_t erase(const Key& key) {
        if (nullptr == root) {
            return 0;
        }
        if (root->key == key) {
            delete root;
            return 0;
        }

        root = splay(root, key);
        Node* erased = root;
        if (!root->right && !root->left) {
            delete root;
            return 0;
        }
        if (!root->left) {
            root = root->right;
        } else if (!root->right) {
            root = root->left;
        } else {
            root = splay(root->left, key);
            root->right = erased->right;
        }
        delete erased;
        --treeSize;
        return 1;
    }

    template <class... Args>
    iterator insert(Args&&... args) {
       Node* newNode = std::allocator_traits<allocator_type>::allocate(allocator, 1);
       std::allocator_traits<allocator_type>::construct(allocator, newNode, std::forward(args)...);
       return new TreeIterator(newNode);
   }

    inline void clear() {
        while (0 < erase(root->key))
        ;
    }
    inline bool empty() const {
        return root == 0;
    }
    inline unsigned long size() const {
        return treeSize;
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
        x->left = y->right;
        y->right = x;
        return y;
    }

    // left rotate subtree rooted with x
    Node* leftRotate(Node* x) {
        if (!x) {
            return nullptr;
        }
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // if key is in tree -> put key_Node at root
    // else -> put the last accessed item at root
    Node* splay(Node* root, Key key) {
        if (!root || root->key == key) {
            return root;
        }

        if (comparator(key, root->key)) {
            // key in left subtree

            if (!root->left) {
                return root;
            }

            if (comparator(key, root->left->key)) {
                // Zig-Zig (Left Left)
                root->left->left = splay(root->left->left, key);
                root = rightRotate(root);
            } else if (comparator(root->left->key, key)) {
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

            if (comparator(key, root->right->key)) {
                // Zag-Zig (Right Left)
                root->right->left = splay(root->right->left, key);
                if (root->right->left) {
                    root->right = rightRotate(root->right);
                }
            } else if (root->right->key < key) {
                // Zag-Zag (Right Right)
                root->right->right = splay(root->right->right, key);
                root = leftRotate(root);
            }
            return (!root->right) ? root : leftRotate(root);
        }
    }
};

#endif // SPLAYTREE_H
