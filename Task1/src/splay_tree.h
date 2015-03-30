#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <functional>
#include <memory>

template <class Key, class Value, class Comparator = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, Value> > >
class SplayTree {
public:
    typedef std::pair<const Key, Value> value_type;

private:
    struct Node {
        Node* _left;
        Node* _right;
        Node* _parent;
        value_type _value;

        Node(const Key & newKey = Key(), const Value & newValue = Value()) :
            _left(nullptr), _right(nullptr), _parent(nullptr), _value(newKey, newValue)
        {}
    };

public:
    template<class T>
    class TreeIterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
        TreeIterator() : _cur(nullptr)
        {}
        TreeIterator(Node* node) : _cur(node)
        {}
        TreeIterator(SplayTree* tree) : _cur(tree->_root)
        {}
        TreeIterator(const TreeIterator& other) : _cur(other._cur)
        {}
        TreeIterator(TreeIterator&& other) : _cur(other._cur) {
            other._cur = nullptr;
        }
        TreeIterator& operator=(const TreeIterator& other) {
            if (*this != other) {
                _cur = other._cur;
            }
            return *this;
        }
        TreeIterator& operator=(TreeIterator&& other) {
            if (*this != other) {
                std::swap(_cur, other._cur);
            }
            return *this;
        }
        TreeIterator& operator++() {
            if (!_cur) {
                return *this;
            }
            if (_cur->_right) {
                _cur = _cur->_right;
                while (_cur && _cur->_left) {
                    _cur = _cur->_left;
                }
            } else {
                Node* parent = _cur->_parent;
                if (nullptr == parent) {
                    _cur = nullptr;
                } else {
                    while (parent && _cur == parent->_right) {
                        _cur = parent;
                        parent = parent->_parent;
                    }
                    if (_cur && _cur->_right != parent) {
                        _cur = parent;
                    }
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
            return _cur == other._cur;
        }
        bool operator==(const TreeIterator& other) const {
            return _cur == other._cur;
        }
        bool operator!=(const TreeIterator& other) {
            return _cur != other._cur;
        }
        bool operator!=(const TreeIterator& other) const {
            return _cur != other._cur;
        }
        T& operator*() {
            if (_cur) {
                return _cur->_value;
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
        T* operator->() {
            if (_cur) {
                return &(_cur->_value);
            } else {
                throw new std::out_of_range("Iterator is out of range");
            }
        }
    private:
        Node* _cur;
    };

    typedef TreeIterator<value_type> iterator;
    typedef TreeIterator<const value_type> const_iterator;
    typedef typename Allocator::template rebind<Node>::other allocator_type;

    SplayTree() : _root(nullptr), _treeSize(0), _allocator(), _defaultValue()
    {}
    ~SplayTree() {
        if (_root) {
            clear();
        }
    }

    std::pair<iterator, bool> insert(const Key& key, const Value& value = Value()) {
        if (!_root) {
            _root = _allocator.allocate(1);
            _allocator.construct(_root, key, value);
            ++_treeSize;
            return std::make_pair(iterator(_root), true);
        }
        if (_root) {
            _root = splay(_root, key);
            if (_root->_value.first == key) {
                return std::make_pair(iterator(_root), false);
            }
        }

        Node* cur = _root;
        Node* prev = nullptr;
        while (cur) {
            prev = cur;
            if(_comparator(cur->_value.first, key)) {
                cur = cur->_right;
            } else {
                cur = cur->_left;
            }
        }
        cur = _allocator.allocate(1);
        _allocator.construct(cur, key, value);
        if(nullptr == prev) {
            _root = cur;
        } else if(_comparator(prev->_value.first, cur->_value.first)) {
            prev->_right = cur;
        } else {
            prev->_left = cur;
        }
        cur->_parent = prev;
        ++_treeSize;
        _root = splay(_root, key);
        return std::make_pair(iterator(cur), true);
    }
    size_t erase(const Key& key) {
        if (nullptr == _root) {
            return 0;
        }
        _root = splay(_root, key);
        Node* erased = _root;
        if (!_root->_right && !_root->_left) {
            _allocator.destroy(_root);
            _allocator.deallocate(_root, 1);
            _root = nullptr;
            --_treeSize;
            return 1;
        }
        if (!_root->_left) {
            _root = _root->_right;
        } else if (!_root->_right) {
            _root = _root->_left;
        } else {
            Node* leftMax = _root->_left;
            while(leftMax->_right) {
                leftMax = leftMax->_right;
            }
            _root = splay(_root->_left, leftMax->_value.first);
            _root->_right = erased->_right;

            if (_root->_right) {
                _root->_right->_parent = _root;
            }
        }
        _root->_parent = nullptr;
        _allocator.destroy(erased);
        _allocator.deallocate(erased, 1);
        --_treeSize;
        return 1;
    }
    inline iterator find(const Key& key) {
        if (!_root) {
            return end();
        }
        _root = splay(_root, key);
        if (_root->_value.first == key) {
            return iterator(_root);
        }
        return end();
    }
    inline void clear() {
        if (_root) {
            while (0 < erase(_root->_value.first))
            { ; }
        }
    }
    inline bool empty() const {
        return _root == 0;
    }
    inline size_t size() const {
        return _treeSize;
    }
    inline iterator begin() noexcept {
        Node* cur = _root;
        while (cur->_left) {
            cur = cur->_left;
        }
        return iterator(cur);
    }
    inline iterator end() noexcept {
        return iterator();
    }
    inline const_iterator cbegin() const noexcept {
        Node* cur = _root;
        while (cur->_left) {
            cur = cur->_left;
        }
        return const_iterator(cur);
    }
    inline const_iterator cend() const noexcept {
        return const_iterator();
    }

private:
    allocator_type _allocator;
    Comparator _comparator;
    size_t _treeSize;
    Value _defaultValue;
    Node* _root;

    // right rotate subtree rooted with x
    Node* rightRotate(Node *x) {
        if (!x) {
            return nullptr;
        }
        Node *y = x->_left;
        if (y) {
            x->_left = y->_right;
            if (y->_right) {
                y->_right->_parent = x;
            }
            y->_parent = x->_parent;
        }
        if(!x->_parent) {
            _root = y;
        } else if(x == x->_parent->_left) {
            x->_parent->_left = y;
        } else {
            x->_parent->_right = y;
        }
        if(y) {
            y->_right = x;
        }
        x->_parent = y;
        return y;
    }
    // left rotate subtree rooted with x
    Node* leftRotate(Node* x) {
        if (!x) {
            return nullptr;
        }
        Node* y = x->_right;
        if(y) {
          x->_right = y->_left;
          if(y->_left) {
              y->_left->_parent = x;
          }
          y->_parent = x->_parent;
        }
        if(!x->_parent) {
            _root = y;
        } else if(x == x->_parent->_left) {
            x->_parent->_left = y;
        } else {
            x->_parent->_right = y;
        }
        if(y) {
            y->_left = x;
        }
        x->_parent = y;
        return y;
    }
    // if key is in tree -> put key_Node at root
    // else -> put the last accessed item at root
    Node* splay(Node* root, Key key) {
        if (!root || root->_value.first == key) {
            return root;
        }
        // key in left subtree
        if (_comparator(key, root->_value.first)) {
            if (!root->_left) {
                return root;
            }
            if (_comparator(key, root->_left->_value.first)) {
                // Zig-Zig (Left Left)
                root->_left->_left = splay(root->_left->_left, key);
                root = rightRotate(root);
            } else if (_comparator(root->_left->_value.first, key)) {
                // Zig-Zag (Left Right)
                root->_left->_right = splay(root->_left->_right, key);
                if (root->_left->_right) {
                    root->_left = leftRotate(root->_left);
                }
            }
            return (!root->_left) ? root : rightRotate(root);
        } else /* key in right subtree */ {
            if (!root->_right) {
                return root;
            }
            if (_comparator(key, root->_right->_value.first)) {
                // Zag-Zig (Right Left)
                root->_right->_left = splay(root->_right->_left, key);
                if (root->_right->_left) {
                    root->_right = rightRotate(root->_right);
                }
            } else if (root->_right->_value.first < key) {
                // Zag-Zag (Right Right)
                root->_right->_right = splay(root->_right->_right, key);
                root = leftRotate(root);
            }
            return (!root->_right) ? root : leftRotate(root);
        }
    }
};

#endif // SPLAYTREE_H
