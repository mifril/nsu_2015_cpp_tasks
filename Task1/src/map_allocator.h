#ifndef MAP_ALLOCATOR_H
#define MAP_ALLOCATOR_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <utility>

template <typename T>
class MapAllocator {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U>
    struct rebind {
        typedef MapAllocator<U> other;
    };

    class Pool {
    private:
        struct Link {
            Link* next;
        };
        struct Chunk {
            enum {
                size = 8 * 1024 - 16
            };
            char mem[size] ;
            Chunk* next;
        };
        Chunk* _chunks;
        const size_t _element_size;
        Link* _head;

        Pool(Pool&) = delete;
        void operator=(Pool&) = delete;

        void grow() {
            Chunk* n = new Chunk();
            n->next = _chunks;
            _chunks = n;
            const int nelem = Chunk::size / _element_size;
            char* start = n->mem;
            char* last = &start[(nelem - 1) * _element_size] ;
            for (char* p = start; p < last; p += _element_size) {
                reinterpret_cast<Link*>(p)->next = reinterpret_cast<Link*>(p + _element_size) ;
            }
            reinterpret_cast<Link*> (last)->next = 0;
            _head = reinterpret_cast<Link*>(start) ;
        }

    public:
        Pool(size_t element_size) : _element_size(element_size < sizeof(Link) ? sizeof(Link) : element_size),
            _head (nullptr), _chunks (nullptr)
        {}
        ~Pool() {
            Chunk* n = _chunks;
            while (n) {
                Chunk* p = n;
                n = n->next;
                delete p;
            }
        }
        inline void* alloc() {
            if(!_head) {
                grow();
            }
            Link* p = _head;
            _head = p->next;
            return p;
        }
        inline void free(void* b) {
            Link* p = static_cast<Link*> (b);
            p->next = _head;
            _head = p;
        }
    };

    MapAllocator()
    {}
    MapAllocator(const MapAllocator&)
    {}
    template<typename U>
    MapAllocator(const MapAllocator<U> &)
    {}

    inline pointer address(reference x) const {
        return &x;
    }
    inline const_pointer address(const_reference x) const {
        return &x;
    }
    inline pointer allocate(size_t n, const void* = 0) {
        return static_cast<pointer>(_pool.alloc());
    }
    inline size_t max_size() const throw() {
        return sizeof(T);
    }
    inline void deallocate(pointer p, size_t n) {
        if (1 == n) {
            _pool.free(p);
            return;
        }
        //TODO
    }
    template<class U, class... Args >
    inline void construct(U* p, Args&&... args ) {
        new (static_cast<void*>(p)) value_type(std::forward<Args>(args)...);
    }
    template<class U>
    inline void destroy(U* p) {
        p->~U();
    }

private:
    static Pool _pool;
    static size_t _used;
    static size_t _size;
};

template<class T> typename MapAllocator<T>::Pool MapAllocator<T>::_pool (sizeof(T)) ;

#endif // MAP_ALLOCATOR_H
