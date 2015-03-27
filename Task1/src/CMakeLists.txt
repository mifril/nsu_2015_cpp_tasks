SET(CMAKE_BUILD_TYPE Debug)
project(PromCPP_Task1_Map)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(UNIX)
    set(LDADD ${LDADD} "-lpthread" "-Wall" "-Wextra")
endif()

add_library(map_splay_tree SHARED ${SRC_LIST})

