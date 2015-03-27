#include <gtest/gtest.h>
#include "map.h"
#include "map_allocator.h"

typedef Map<std::string, int, std::less<std::string>, MapAllocator<std::pair<std::string, int>> > MyAllocatorMap;

typedef std::pair<Map<std::string, int>::iterator, bool> def_all_insert_pair;
typedef std::pair<MyAllocatorMap::iterator, bool> my_all_insert_pair;

class MapTest : public ::testing::Test {
};

TEST_F(MapTest, DefaultAllocatorTest) {
    Map<std::string, int> map;
    ASSERT_TRUE(map.empty());

    def_all_insert_pair result = map.insert(std::make_pair("ten", 10));
    ASSERT_EQ(10, (*(result.first)).second);
    ASSERT_EQ(true, result.second);

    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(1, map.size());

    ASSERT_EQ(0, map["nine"]);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(map.empty());

    result = map.insert(std::make_pair("nine", 9));
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(0, map["nine"]);
    ASSERT_EQ(0, (*(result.first)).second);
    ASSERT_EQ(false, result.second);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(map.empty());

    map["nine"] = 9;
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(9, map["nine"]);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(map.empty());

    result = map.insert(std::make_pair("one", 1));
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(9, map["nine"]);
    ASSERT_EQ(1, map["one"]);
    ASSERT_EQ(1, (*(result.first)).second);
    ASSERT_EQ(true, result.second);
    ASSERT_EQ(3, map.size());
    ASSERT_FALSE(map.empty());

    std::string ten = "ten";
    std::string nine = "nine";
    std::string one = "one";
    ASSERT_EQ(10, map[ten]);
    ASSERT_EQ(9, map[nine]);
    ASSERT_EQ(1, map[one]);

    map["two"] = 2;
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(9, map["nine"]);
    ASSERT_EQ(1, map["one"]);
    ASSERT_EQ(2, map["two"]);
    ASSERT_EQ(4, map.size());
    ASSERT_FALSE(map.empty());

    result = map.emplace("three", 3);
    ASSERT_EQ(3, (*(result.first)).second);
    ASSERT_EQ(3, map["three"]);
    ASSERT_EQ(5, map.size());
    ASSERT_FALSE(map.empty());

    for (Map<std::string, int>::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
    }
    std::cout << "=============" << std::endl;

    size_t erased = map.erase("ten");
    ASSERT_EQ(1, erased);
    ASSERT_EQ(4, map.size());
    ASSERT_FALSE(map.empty());

    for (Map<std::string, int>::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
    }

    map.clear();
    ASSERT_EQ(0, map.size());
    ASSERT_TRUE(map.empty());
}




TEST_F(MapTest, MyAllocatorTest) {
    MyAllocatorMap map;
    ASSERT_TRUE(map.empty());

    my_all_insert_pair result = map.insert(std::make_pair("ten", 10));
    ASSERT_EQ(10, (*(result.first)).second);
    ASSERT_EQ(true, result.second);

    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(1, map.size());

    ASSERT_EQ(0, map["nine"]);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(map.empty());

    result = map.insert(std::make_pair("nine", 9));
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(0, map["nine"]);
    ASSERT_EQ(0, (*(result.first)).second);
    ASSERT_EQ(false, result.second);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(map.empty());

    map["nine"] = 9;
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(9, map["nine"]);
    ASSERT_EQ(2, map.size());
    ASSERT_FALSE(map.empty());

    result = map.insert(std::make_pair("one", 1));
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(9, map["nine"]);
    ASSERT_EQ(1, map["one"]);
    ASSERT_EQ(1, (*(result.first)).second);
    ASSERT_EQ(true, result.second);
    ASSERT_EQ(3, map.size());
    ASSERT_FALSE(map.empty());

    std::string ten = "ten";
    std::string nine = "nine";
    std::string one = "one";
    ASSERT_EQ(10, map[ten]);
    ASSERT_EQ(9, map[nine]);
    ASSERT_EQ(1, map[one]);

    map["two"] = 2;
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(9, map["nine"]);
    ASSERT_EQ(1, map["one"]);
    ASSERT_EQ(2, map["two"]);
    ASSERT_EQ(4, map.size());
    ASSERT_FALSE(map.empty());

    result = map.emplace("three", 3);
    ASSERT_EQ(3, (*(result.first)).second);
    ASSERT_EQ(3, map["three"]);
    ASSERT_EQ(5, map.size());
    ASSERT_FALSE(map.empty());

    for (MyAllocatorMap::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
    }
    std::cout << "=============" << std::endl;

    size_t erased = map.erase("ten");
    ASSERT_EQ(1, erased);
    ASSERT_EQ(4, map.size());
    ASSERT_FALSE(map.empty());

    for (MyAllocatorMap::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
    }

    map.clear();
    ASSERT_EQ(0, map.size());
    ASSERT_TRUE(map.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

