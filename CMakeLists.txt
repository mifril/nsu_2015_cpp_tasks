project(PromCPP_Task1_Map)
cmake_minimum_required(VERSION 2.8)

#set(CMAKE_CXX_COMPILER=clang++)
aux_source_directory(. SRC_LIST)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(task1 ${SRC_LIST})
