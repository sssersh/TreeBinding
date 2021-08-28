
cmake_minimum_required(VERSION 2.8)

project(one_header_generator)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_EXE generator.cpp)				

add_executable(generator ${SOURCE_EXE})

target_link_libraries(generator stdc++fs)		
