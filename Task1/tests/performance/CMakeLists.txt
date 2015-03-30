SET(CMAKE_BUILD_TYPE Debug)
project(PromCPP_Task1_Map)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../src ${CMAKE_CURRENT_SOURCE_DIR}/build/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

set(LDADD
    map_splay_tree
)
if(UNIX)
    set(LDADD ${LDADD} "-Wall" "-Wextra" "-lpthread")
endif()
aux_source_directory(. SRC_LIST)

add_executable(performance ${SRC_LIST})
target_link_libraries(performance ${LDADD})
