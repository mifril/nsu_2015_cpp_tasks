SET(CMAKE_BUILD_TYPE Debug)
project(PromCPP_Task1_Map)
cmake_minimum_required(VERSION 2.8)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_SOURCE_DIR}/src)

aux_source_directory(. SRC_LIST)
set(LDADD
	splay_tree_map
	gtest
	gtest_main
)

if(UNIX)
	set(LDADD ${LDADD} "-lpthread")
endif()

add_executable(run_tests ${SRC_LIST})
target_link_libraries(run_tests ${LDADD})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../src
)

