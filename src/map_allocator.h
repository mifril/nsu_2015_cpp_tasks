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
                size = 8*1024-16
            }; //слегка меньше 8K, чтобы кусок памяти умещался в 8К
            char mem[size] ;
            Chunk* next;
        };
        Chunk* chunks;
        const unsigned int element_size;
        Link* head;

        Pool(Pool&);
        void operator=(Pool&);

        void grow() {
            Chunk* n = new Chunk();
            n->next = chunks;
            chunks = n;
            const int nelem = Chunk::size / element_size;
            char* start = n->mem;
            char* last = &start[ (nelem - 1) * element_size] ;
            for (char* p = start; p < last; p += element_size) {
                reinterpret_cast<Link*>(p)->next = reinterpret_cast<Link*>(p + element_size) ;
            }
            reinterpret_cast<Link*> (last) ->next = 0;
            head = reinterpret_cast<Link*> (start) ;
        }

    public:
        inline void* alloc() {
            if(!head) {
                grow();
            }
            Link* p = head;
            head = p->next;
            return p;
        }

        inline void free(void* b) {
            Link* p = static_cast<Link*> (b);
            p->next = head;
            head = p;
        }

        Pool(unsigned int sz) : element_size(sz < sizeof(Link) ? sizeof(Link) : sz), head (nullptr), chunks (nullptr)
        {}

        ~Pool() {
            Chunk* n = chunks;
            while (n) {
                Chunk* p = n;
                n = n->next;
//                delete p;
            }
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
        return static_cast<pointer>(pool.alloc());
    }

    inline size_t max_size() const throw() {
        return sizeof(T);
    }

    inline void deallocate(pointer p, size_t n) {
        if (1 == n) {
            pool.free(p);
            return;
        }
        //TODO
    }

    template<class U, class... Args >
    inline void construct(U* p, Args&&... args ) {
        new (static_cast<void*>(p)) value_type(args...);
    }

    template<class U>
    inline void destroy(U* p) {
        p->~U();
    }

private:
    static Pool pool;
    static size_t Used;
    static size_t size;
};

template<class T> typename MapAllocator<T>::Pool MapAllocator<T>::pool (sizeof(T)) ;

#endif // MAP_ALLOCATOR_H
