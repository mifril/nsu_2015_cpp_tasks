SET(CMAKE_BUILD_TYPE Debug)
project(PromCPP_Task1_Map)
cmake_minimum_required(VERSION 2.8)

#enable_testing()
#find_package(GTest REQUIRED)

aux_source_directory(. SRC_LIST)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../gtest ${CMAKE_CURRENT_SOURCE_DIR}/gtest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gtest/include)

set(LDADD
    gtest
    gtest_main
)

if(UNIX)
    set(LDADD ${LDADD} "-lpthread" "-Wall" "-Wextra")
endif()

add_executable(task1  ${SRC_LIST})
target_link_libraries(task1 ${LDADD})
