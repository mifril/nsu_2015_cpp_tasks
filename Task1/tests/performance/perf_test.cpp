#include <map>
#include "map.h"
#include "map_allocator.h"
#include "timer.h"

typedef Map<int, int, std::less<int>, MapAllocator<std::pair<int, int>> > MyAllocatorMap;
typedef Map<int, int> DefaultAllocatorMap;
typedef std::map<int, int, std::less<int>, MapAllocator<std::pair<int, int>> > MyAllocatorStdMap;
typedef std::map<int, int> DefaultAllocatorStdMap;

template<class Map>
void TestInsertMap(Map& map, int n, Timer& timer) {
    if (0 > n) {
        std::cout << "TestInsertMap: Bad number of iterations: " << n << std::endl;
        return;
    }
    timer.start();
    for (size_t i = 0; i < n; ++i)
    {
        map[rand()] = rand();
    }
    timer.stop();
}

template<class Map>
void TestFindMap(Map& map, int n, Timer& timer) {
    if (0 > n) {
        std::cout << "TestFindMap: Bad number of iterations: " << n << std::endl;
        return;
    }
    timer.start();
    for (size_t i = 0; i < n; ++i)
    {
        map.find(rand());
    }
    timer.stop();
}


int main() {
    std::map<int, int> StdMapDefaultAll;
    Map<int, int> MyMapDefaultAll;
    Map<int, int, std::less<int>, MapAllocator<std::pair<int, int>> > MyMapMyAll;

    int N = 100000;
    int experimentsNumber = 3;
    Timer timer;

    size_t time_insert = 0;
    size_t time_find = 0;
    for (int i = 0; i < experimentsNumber; ++i) {
        TestInsertMap(StdMapDefaultAll, N, timer);
        time_insert += timer.getDuration();
        TestFindMap(StdMapDefaultAll, N, timer);
        time_find += timer.getDuration();
        StdMapDefaultAll.clear();
    }
    time_insert /= experimentsNumber;
    time_find /= experimentsNumber;
    std::cout << "std::map with default allocator (avg insert time): " << time_insert << " ms" << std::endl;
    std::cout << "std::map with default allocator (avg find time): " << time_find << " ms" << std::endl;


    time_insert = 0;
    time_find = 0;
    for (int i = 0; i < experimentsNumber; ++i) {
        TestInsertMap(MyMapDefaultAll, N, timer);
        time_insert += timer.getDuration();
        TestFindMap(MyMapDefaultAll, N, timer);
        time_find += timer.getDuration();
        MyMapDefaultAll.clear();
    }
    time_insert /= experimentsNumber;
    time_find /= experimentsNumber;
    std::cout << "my Map with default allocator (avg insert time): " << time_insert << " ms" << std::endl;
    std::cout << "my Map with default allocator (avg find time): " << time_find << " ms" << std::endl;


    time_insert = 0;
    time_find = 0;
    for (int i = 0; i < experimentsNumber; ++i) {
        TestInsertMap(MyMapMyAll, N, timer);
        time_insert += timer.getDuration();
        TestFindMap(MyMapMyAll, N, timer);
        time_find += timer.getDuration();
        MyMapDefaultAll.clear();
    }
    time_insert /= experimentsNumber;
    time_find /= experimentsNumber;
    std::cout << "my Map with my allocator (avg insert time): " << time_insert << " ms" << std::endl;
    std::cout << "my Map with my allocator (avg find time): " << time_find << " ms" << std::endl;

    return 0;
}

