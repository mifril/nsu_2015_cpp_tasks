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

    Map() : tree (std::make_shared<SplayTree<Key, Value, Comparator, Allocator>>())
    {}
    Map(const Map& other) {
        insert(other.begin(), other.end());
    }
    Map(Map&& other) : tree(other.tree) {
        other.tree = nullptr;
    }
    Map& operator=(const Map& other) {
        if (*this != other) {
            if (tree) {
                tree->clear();
            }
            insert(other.begin(), other.end());
        }
        return *this;
    }
    Map& operator=(Map&& other) {
        if (*this != other) {
            std::swap(tree, other.tree);
        }
        return *this;
    }
    ~Map() {
        if (tree) {
            clear();
        }
    }

    inline Value& operator[](const Key& key) {
        std::pair<iterator, bool> insertResult = tree->insert(key);
        return (*(insertResult.first)).second;
    }
    inline Value& operator[](Key&& key) {
        Key keyCopy;
        std::swap(key, keyCopy);
        std::pair<iterator, bool> insertResult = tree->insert(keyCopy);
        return (*(insertResult.first)).second;
    }
    inline iterator begin() noexcept {
        return tree->begin();
    }
    inline iterator end() noexcept {
        return tree->end();
    }

    inline bool empty() const noexcept {
        return tree->empty();
    }
    inline int size() const noexcept {
        return tree->size();
    }

    inline void clear() noexcept {
        tree->clear();
    }
    inline std::pair<iterator, bool> insert(const value_type& pair) {
        return tree->insert(pair.first, pair.second);
    }
    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }
    template <class... Args>
    inline std::pair<iterator, bool> emplace(Args&&... args) {
        return tree->insert(std::forward<Args>(args)...);
    }
    inline size_t erase(const Key& key) {
        return tree->erase(key);
    }

private:
    std::shared_ptr<SplayTree<Key, Value, Comparator, Allocator>> tree;
};

#endif // MAP_H
