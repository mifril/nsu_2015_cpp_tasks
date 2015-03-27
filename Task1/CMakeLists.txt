SET(CMAKE_BUILD_TYPE Debug)
project(PromCPP_Task1_Map)
cmake_minimum_required(VERSION 2.8)

#enable_testing()
#find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gtest ${CMAKE_CURRENT_SOURCE_DIR}/build/gtest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/build/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gtest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(LDADD
    map_splay_tree
    gtest
    gtest_main
)
if(UNIX)
    set(LDADD ${LDADD} "-Wall" "-Wextra" "-lpthread")
endif()
aux_source_directory(tests SRC_LIST)

add_executable(task1 ${SRC_LIST})
target_link_libraries(task1 ${LDADD})
