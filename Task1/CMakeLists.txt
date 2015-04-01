set(CMAKE_BUILD_TYPE Debug)
project(PromCPP_Task1_Map)
cmake_minimum_required(VERSION 2.8)

#enable_testing()
#find_package(GTest REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

set(CODE_COVERAGE_ENABLED ON)
if (CODE_COVERAGE_ENABLED)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -g -O0 -fprofile-arcs -ftest-coverage")
else()
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
endif()

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
add_executable(run_tests ${SRC_LIST})

target_link_libraries(run_tests ${LDADD})

if (UNIX AND CODE_COVERAGE_ENABLED)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    include(coverage)
    setup_target_for_coverage(coverage run_tests coverage)
    target_link_libraries(run_tests gcov)
endif()
