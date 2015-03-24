#include <gtest/gtest.h>
#include "map.h"


class MapTest : public ::testing::Test {
};

TEST_F(MapTest, DefaultAllocatorTest) {
	Map<std::string, int> map;

    map.insert(std::pair<std::string ,int>("ten", 10));
    ASSERT_EQ(10, map["ten"]);
    ASSERT_EQ(0, map["nine"]);
    ASSERT_EQ(1, map.size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

