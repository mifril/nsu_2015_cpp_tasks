#ifndef MAP_H
#define MAP_H

#include <utility>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>
#include "splay_tree.h"

template <class Key, class Value, class Comparator = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Value> > >
class Map {
public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef Allocator allocator_type;
    typedef Comparator comparator_type;
    typedef typename SplayTree<key_type, mapped_type, comparator_type, allocator_type>::iterator iterator;

    Map() {
        tree = std::make_shared<SplayTree<Key, Value, Comparator, Allocator>>();
    }

    Map(const Map& other) : tree(other.tree)
    {}

    Map(Map&& other)  {
        tree = other.tree;
        other.tree = nullptr;
    }

    Map& operator=(const Map& other) {
        if (tree) {
            tree->clear();
        }
        tree = other.tree;
        return *this;
    }

    Map& operator=(Map&& other) {
        std::swap(tree, other.tree);
        return *this;
    }

    ~Map() {
        if (tree) {
            clear();
        }
    }

    inline size_t erase(const Key& key) {
        return tree->erase(key);
    }
    //inline std::pair<iterator, bool> insert(const Key& key, const Value& value) {
    inline void insert(const value_type& pair) {
        return tree->insert(pair.first, pair.second);
    }
    inline void clear() noexcept {
        tree->clear();
    }
    inline bool empty() {
        return tree->empty();
    }
    inline int size() {
        return tree->size();
    }
    inline Value& operator[](const Key& key) {
        return tree->find(key);
    }
    inline Value& operator[](Key&& key) {
        return tree->find(std::move(key));
    }
    inline iterator begin() noexcept {
        return new iterator(tree);
    }
    inline iterator end() noexcept {
        return new iterator(nullptr);
    }
    inline Value& front() {
        return *(begin());
    }
    inline Value& back() {
        Value tmp;
        for (auto it = begin(); it < end(); ++it) {
            tmp = *it;
        }
        return tmp;
    }

    template <class... Args>
    inline std::pair<iterator, bool> emplace(Args&&... args) {
        iterator it = tree->insert(std::forward(args)...);
        return new std::pair<iterator, bool>(it, true);
    }

private:
    std::shared_ptr<SplayTree<Key, Value, Comparator, Allocator>> tree;
};

#endif // MAP_H
