#
# CMakeLists.txt
#
# Build file for the tests
#

cmake_minimum_required(VERSION 3.10.1)

set(PROJECT_NAME tests)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(TREE_BINDING_INC_DIR ../include)

set(SOURCE_DIR src)
set(SOURCES
		${SOURCE_DIR}/test1.cpp
		)

set(INCLUDES
		${TREE_BINDING_INC_DIR}/TreeBinding.h
		)

include_directories(${TREE_BINDING_INC_DIR})

add_executable(
	tests
    ${SOURCES}
		${INCLUDES}
)

#add_test(NAME check COMMAND tests)

enable_testing()

target_link_directories (
    ${PROJECT_NAME}
    PUBLIC
)

# Link against Google Test library
# try gtest_main
target_link_libraries(tests gtest gmock gtest_main pthread)
