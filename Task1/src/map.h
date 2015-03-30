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
    typedef typename SplayTree<key_type, mapped_type, comparator_type, allocator_type>::const_iterator const_iterator;

    Map() : _tree (std::make_shared<SplayTree<Key, Value, Comparator, Allocator>>())
    {}
    Map(const Map& other) : _tree (std::make_shared<SplayTree<Key, Value, Comparator, Allocator>>()) {
        insert(other.begin(), other.end());
    }
    Map(Map&& other) : _tree(other._tree) {
        other._tree = nullptr;
    }
    Map& operator=(const Map& other) {
        if (*this != other) {
            if (_tree) {
                _tree->clear();
            }
            insert(other.begin(), other.end());
        }
        return *this;
    }
    Map& operator=(Map&& other) {
        if (*this != other) {
            std::swap(_tree, other._tree);
        }
        return *this;
    }
    ~Map() {
        if (_tree) {
            clear();
        }
    }

    inline Value& operator[](const Key& key) {
        std::pair<iterator, bool> insertResult = _tree->insert(key);
        return (*(insertResult.first)).second;
    }
    inline Value& operator[](Key&& key) {
        Key keyCopy;
        std::swap(key, keyCopy);
        std::pair<iterator, bool> insertResult = _tree->insert(keyCopy);
        return (*(insertResult.first)).second;
    }
    inline iterator begin() noexcept {
        return _tree->begin();
    }
    inline iterator end() noexcept {
        return _tree->end();
    }
    inline const_iterator begin() const noexcept {
        return _tree->cbegin();
    }
    inline const_iterator end() const noexcept {
        return _tree->cend();
    }
    inline const_iterator cbegin() const noexcept {
        return _tree->cbegin();
    }
    inline const_iterator cend() const noexcept {
        return _tree->cend();
    }
    inline bool empty() const noexcept {
        return _tree->empty();
    }
    inline size_t size() const noexcept {
        return _tree->size();
    }
    inline void clear() noexcept {
        _tree->clear();
    }
    inline std::pair<iterator, bool> insert(const value_type& pair) {
        return _tree->insert(pair.first, pair.second);
    }
    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }
    template <class... Args>
    inline std::pair<iterator, bool> emplace(Args&&... args) {
        return _tree->insert(std::forward<Args>(args)...);
    }
    inline size_t erase(const Key& key) {
        return _tree->erase(key);
    }
    inline iterator find(const key_type& key) {
        return _tree->find(key);
    }
    inline const_iterator find(const key_type& key) const {
        return _tree->find(key);
    }

private:
    std::shared_ptr<SplayTree<Key, Value, Comparator, Allocator>> _tree;
};

#endif // MAP_H
