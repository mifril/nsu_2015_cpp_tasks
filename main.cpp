#include <iostream>
#include "map_allocator.h"
#include "map.h"
#include "timer.h"

int main() {
    Map<int, int, std::less<int>, MapAllocator<std::pair<int, int> > > map;
    std::cout << "Hello World!" << std::endl;
    return 0;
}

